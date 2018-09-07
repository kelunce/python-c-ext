# -*- coding: utf-8 -*-  
import  TestBuff as mbuf
# 调用模块函数
print(mbuf.c_strlen("中文测试"))
# 创建TestBuff对象
c = "this is a test string to init buff at starting."
a = mbuf.TestBuff(c, 200)
# 测试对象类型
print(type(a))
# 调用对象函数
errno,b = a.GetTestBuff(0, 2)
print(errno, b)
# 以16进制dump出字符
print(mbuf.DumpTestBuff(b))
# 调用对象方法测试Append
errno = a.AppendTestBuff(-1, b)
print (errno, a.GetTestBuff(0, len(c) + len(b)))
