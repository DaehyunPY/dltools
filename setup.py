from sys import platform
from setuptools import setup, Extension
from setuptools.command.build_ext import build_ext
from distutils.errors import CompileError


ext_modules = [
    Extension(
        'dltools.sacla.model',
        [
            'dltools/src/sacla_model_binder.cpp',
            'dltools/src/sacla_model_pickler.cpp',
            'dltools/src/sacla_model.cpp',
            'dltools/src/hittype.cpp',
        ],
        include_dirs=['include'],
        language='c++'
    ),
    Extension(
        'dltools.hittype',
        [
            'dltools/src/hittype_binder.cpp',
            'dltools/src/hittype.cpp',
        ],
        include_dirs=['include'],
        language='c++'
    ),
]


# As of Python 3.6, CCompiler has a `has_flag` method.
# cf http://bugs.python.org/issue26689
def has_flag(compiler, flagname):
    """
    Return a boolean indicating whether a flag name is supported on
    the specified compiler.
    """
    import tempfile
    with tempfile.NamedTemporaryFile('w', suffix='.cpp') as f:
        f.write('int main (int argc, char **argv) { return 0; }')
        try:
            compiler.compile([f.name], extra_postargs=[flagname])
        except CompileError:
            return False
    return True


def cpp_flag(compiler):
    """
    Return the -std=c++[11/14] compiler flag.
    The c++14 is prefered over c++11 (when it is available).
    """
    if has_flag(compiler, '-std=c++14'):
        return '-std=c++14'
    elif has_flag(compiler, '-std=c++11'):
        return '-std=c++11'
    else:
        raise RuntimeError('Unsupported compiler -- at least C++11 support '
                           'is needed!')


class BuildExt(build_ext):
    """
    A custom build extension for adding compiler-specific options.
    """
    c_opts = {
        'msvc': ['/EHsc'],
        'unix': [],
    }

    if platform == 'darwin':
        c_opts['unix'] += ['-stdlib=libc++', '-mmacosx-version-min=10.7']

    def build_extensions(self):
        ct = self.compiler.compiler_type
        opts = self.c_opts.get(ct, [])
        if ct == 'unix':
            opts.append('-DVERSION_INFO="%s"' % self.distribution.get_version())
            opts.append(cpp_flag(self.compiler))
            if has_flag(self.compiler, '-fvisibility=hidden'):
                opts.append('-fvisibility=hidden')
        elif ct == 'msvc':
            opts.append('/DVERSION_INFO=\\"%s\\"' % self.distribution.get_version())
        for ext in self.extensions:
            ext.extra_compile_args = opts
        build_ext.build_extensions(self)


setup(
    name='dltools',
    version='201811.1',
    author='Daehyun You',
    author_email='daehyun@dc.tohoku.ac.jp',
    url='https://github.com/DaehyunPY/dltools',
    # description='',
    # long_description='',
    license='MIT',
    ext_modules=ext_modules,
    packages=['dltools', 'dltools.sacla'],
    install_requires=['pyspark'],
    cmdclass={'build_ext': BuildExt},
    zip_safe=False,
)
