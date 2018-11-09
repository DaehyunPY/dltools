from sys import platform
from setuptools import setup, Extension
from Cython.Build import cythonize
try:
    import numpy
except ModuleNotFoundError:
    numpy = None
try:
    import pybind11
except ModuleNotFoundError:
    pybind11 = None


ext_modules = [
    Extension(
        'dltools.sacla.model',
        [
            'dltools/src/sacla_model_binder.cpp',
            'dltools/src/sacla_model_pickler.cpp',
            'dltools/src/sacla_model.cpp',
            'dltools/src/hittype.cpp',
        ],
        include_dirs=[
            *([pybind11.get_include(),
               pybind11.get_include(user=True)]
              if pybind11 else []),
            'include',
        ],
        extra_compile_args=[
            "-std=c++11",
            *(["-stdlib=libc++"] if platform == 'darwin' else []),
            "-static-libstdc++",
        ],
        language='c++',
    ),
    Extension(
        'dltools.hittype',
        [
            'dltools/src/hittype_binder.cpp',
            'dltools/src/hittype.cpp',
        ],
        include_dirs=[
            *([pybind11.get_include(),
               pybind11.get_include(user=True)]
              if pybind11 else []),
            'include',
        ],
        extra_compile_args=[
            "-std=c++11",
            *(["-stdlib=libc++"] if platform == 'darwin' else []),
            "-static-libstdc++",
        ],
        language='c++',
    ),
    *cythonize([
        Extension(
            'dltools.lmafmt',
            [
                'dltools/lmafmt.pyx',
            ],
            include_dirs=[
                *([numpy.get_include()]
                  if numpy else []),
                'include',
            ],
        ),
    ]),
]

setup(
    name='dltools',
    version='201811.3',
    author='Daehyun You',
    author_email='daehyun@dc.tohoku.ac.jp',
    url='https://github.com/DaehyunPY/dltools',
    # description='',
    # long_description='',
    license='MIT',
    ext_modules=ext_modules,
    packages=[
        'dltools',
        'dltools.sacla',
    ],
    install_requires=[
        'numpy',
        'pyspark',
    ],
    zip_safe=False,
)
