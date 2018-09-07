from distutils.core import setup, Extension
import platform
import os

path = os.path.realpath(__file__)
cur_dir = os.path.dirname(path)

module_testbuff = Extension('TestBuff',
        define_macros = [],
        extra_compile_args = ['-Wall', '-g3', '-O0'],
        include_dirs  = [],
        library_dirs = [], 
        libraries = [],
        sources = [
            os.path.join(cur_dir, 'c_source.c'),
            os.path.join(cur_dir, 'interface_to_py.c'),
            ]
)

setup (name = 'TestBuff',
        version = '1.0',
        description = 'This is test deamon',
        author = 'lcm',
        author_email = 'lichengman@163.com',
        ext_modules = [module_testbuff],
)
