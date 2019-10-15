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


VERSION = '201910.0'


ext_modules = [
    Extension(
        'dltools.sacla.model',
        [
            'dltools/sacla/model_binder.cpp',
            'dltools/sacla/model_pickler.cpp',
            'dltools/sacla/model.cpp',
            'dltools/hittype.cpp',
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
            'dltools/hittype_binder.cpp',
            'dltools/hittype.cpp',
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

with open('dltools/version.py', 'w') as fp:
    fp.write('# This file is generated from setup.py.\n'
             'version = "{}"\n'.format(VERSION))

setup(
    name='dltools',
    version=VERSION,
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
        'dltools.cov',
    ],
    install_requires=[
        'numba',
        'numpy',
        'pyspark',
        'cytoolz',
    ],
)
