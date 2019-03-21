# -*- coding:utf-8 -*-
""" Setup script for hamming code. """
from os import path
from setuptools import setup, find_packages, Extension
from codecs import open

# !/usr/bin/python
# Python:   3.6.5
# Platform: Windows/Linux
# Author:   Heyn (heyunhuan@gmail.com)
# Program:  Library for hamming code.
# History:  2019-03-21 Wheel Ver:0.2 [Heyn] Initialization

here = path.abspath(path.dirname(__file__))

with open(path.join(here, 'README.rst'), encoding='utf-8') as f:
    long_description = f.read()
    long_description = long_description.replace('\r', '')

setup(
    name='libhamming',
    version='0.2',

    description='Library for hamming code',
    long_description=long_description,

    url='https://github.com/hex-in/libhammingcode.git',
    author='Heyn',
    author_email='heyunhuan@gmail.com',

    license='GPLv3',
    platforms='any',

    classifiers=[
        'Development Status :: 3 - Alpha',
        'Intended Audience :: Developers',
        'Topic :: Software Development :: Build Tools',
        'License :: OSI Approved :: MIT License',
        'Programming Language :: Python :: 3',
        'Programming Language :: Python :: 3.6',
        'Programming Language :: Python :: Implementation :: CPython',
        'Programming Language :: Python :: Implementation :: PyPy',
    ],

    keywords=['hamming', 'hamming code'],
    packages=['libhamming'],
    package_dir  = { 'libhamming' : 'libhamming' },

    # install_requires=['pyserial >= 3.4'],
    ext_modules=[Extension( 'libhamming.hamming',
                            sources=['./src/hxhamming.c', './src/hamming.c'],),
                ],
)
