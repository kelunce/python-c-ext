# python-c-ext
C扩展库开发例子

```text
.
├── c_header.h          C语言头文件，无py相关
├── c_source.c          C语言测试功能源码，无py相关
├── interface_to_py.c   把C语言功能接口开放给python. 
├── libbuild.sh         简单的shell脚本，调用setup.py
├── README.md           本文件
├── setup.py            通过python编译, python setup.py build
├── TestBuff.so         C开发的功能动态库，setup.py产出
└── test.py             测试动态库的使用。不使用ctypes，直接import
```
