// 本文件演示增加python类型及类型的方法，并且增加模块函数
// 新类型中增加的函数是对象方法。和模块宣言的函数是两回事！
// 可以参考官方文档
// https://docs.python.org/2.7/extending/newtypes.html
#include <Python.h>
#include <structmember.h>
#include "c_header.h"



//---------------------------以下开始定义新类型-----------------

// 定义新类型的的内存结构。
typedef struct {
    PyObject_HEAD        // 这个字段是官方预定义必须的，包含引用计数器等。
    TestBuffStruct_t buf;// 扩展我们自己业务数据
    int id;
}MyTestBuff;

// 用于给每个buff增加id 
static int CustomId = 0;

// 新类型支持的方法
static PyObject *GetTestBuff(MyTestBuff *self, PyObject *args);
static PyObject *AppendTestBuff(MyTestBuff *self, PyObject *args);
static PyObject *PopTestBuff(MyTestBuff *self, PyObject *args);
// https://docs.python.org/2.7/c-api/structures.html#c.PyMethodDef
static PyMethodDef MyTestBuffMethods[] = {
    {"GetTestBuff", (PyCFunction)GetTestBuff, METH_VARARGS, "get buff with [start, start + len)" },
    {"AppendTestBuff", (PyCFunction)AppendTestBuff, METH_VARARGS, "append buff at [start, start + len) and source buff" },
    {"PopTestBuff", (PyCFunction)PopTestBuff, METH_VARARGS, "pop buff with [start len)" },
    {NULL}  /* Sentinel 用作哨兵*/
};



/*
 * 以下会定义各种新类型的函数，用来注册到python虚拟机。
 * 当python在适当的机时，会回调回来.
 * 是通过PyModule_AddObject注册进去
 * */

// 当self的引用计数为0时，python自动调用到此处销毁object
static void FreeMyTestBuff(MyTestBuff* self)
{
    fprintf(stderr, "FreeMyTestBuff. id=%d\n", self->id);
    // 销毁业务内存
    FreeBuff(&(self->buf));
    // 销毁python给我们的内存(必须)
    Py_TYPE(self)->tp_free((PyObject*)self);
}

// 当对象被创建时回调，之后回调下面的init 
// 可作为基类时一定要实现此函数
static PyObject *NewMyTestBuff(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    MyTestBuff *self = (MyTestBuff *)type->tp_alloc(type, 0);
    NewBuffer(&(self->buf));
    self->id = ++CustomId;
    fprintf(stderr, "NewMyTestBuff. id=%d\n", self->id);
    return (PyObject *)self;
}

// 初始化代码__init__
static int InitMyTestBuff(MyTestBuff *self, PyObject *args, PyObject *kwds)
{
    const char* origin_buff;
    int capacity; 
    // 解析python虚拟机参数
    //https://docs.python.org/2/c-api/arg.html
    if (!PyArg_ParseTuple(args, "si", &origin_buff, &capacity)) {
        return -1;
    }
    InitBuffer(&(self->buf), capacity, origin_buff); 
    fprintf(stderr, "InitMyTestBuff. id:%d,origin_buff:%s,capacity:%d\n", self->id, origin_buff, capacity);
    return 0;
}

// 支持Get出buf
static PyObject *GetTestBuff(MyTestBuff *self, PyObject *args)
{
    int start = 0;
    size_t len = 0;
    char * msg;
    
    if (!PyArg_ParseTuple(args, "ii", &start, &len)){ 
        msg = "invalid args.";
        return Py_BuildValue("is#", 1, msg, strlen(msg));
    }

    fprintf(stderr, "GetTestBuff. id:%d,start:%d,len:%d\n", self->id, start, len);
    const char *data = GetBuffer(&(self->buf), start, len);
    if(data == 0) {
        msg = "invalid args.";
        return Py_BuildValue("is#", 2, msg, strlen(msg));
    }

    return Py_BuildValue("is#", 0, data, len);
}

static PyObject *AppendTestBuff(MyTestBuff *self, PyObject *args)
{
    fprintf(stderr, "AppendTestBuff. id:%d\n", self->id);
    int start, len;
    char *input;
    if (!PyArg_ParseTuple(args, "is#", &start, &input, &len)){
        return Py_BuildValue("i", 1);
    }

    int ret = AppendBuffer(&(self->buf), start, len, input);
    return Py_BuildValue("i", ret > 0 ? 0 : ret);
}

static PyObject *PopTestBuff(MyTestBuff *self, PyObject *args)
{
    fprintf(stderr, "PopTestBuff. id:%d\n", self->id);
    int start, len;
    if (!PyArg_ParseTuple(args, "ii", &start, &len)){
        return Py_BuildValue("is#", 1, NULL, NULL);
    }

    char *out = malloc(len);
    int ok = PopBuffer(&(self->buf), start, len, out);
    if(ok != 0){
        // pop suc!
        PyObject * ret = Py_BuildValue("is#", 0, out, len);
        free(out);
        return ret;
    }else{
        return Py_BuildValue("is#", 1, NULL, NULL);
    }
}


// 声明新类型python信息,给py虚拟机用
// https://docs.python.org/2.7/c-api/typeobj.html
static PyTypeObject oTestBuff = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "TestBuff",                /* tp_name */
    sizeof(MyTestBuff),        /* tp_basicsize */
    0,                         /* tp_itemsize */
    (destructor)FreeMyTestBuff,/* tp_dealloc */
    0,                         /* tp_print */
    0,                         /* tp_getattr */
    0,                         /* tp_setattr */
    0,                         /* tp_compare */
    0,                         /* tp_repr */
    0,                         /* tp_as_number */
    0,                         /* tp_as_sequence */
    0,                         /* tp_as_mapping */
    0,                         /* tp_hash */
    0,                         /* tp_call */
    0,                         /* tp_str */
    0,                         /* tp_getattro */
    0,                         /* tp_setattro */
    0,                         /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT|Py_TPFLAGS_BASETYPE,        /* tp_flags 可作为基类*/
    "MyTestBuff v1",           /* tp_doc */
    0,                         /* tp_traverse */
    0,                         /* tp_clear */
    0,                         /* tp_richcompare */
    0,                         /* tp_weaklistoffset */
    0,                         /* tp_iter */
    0,                         /* tp_iternext */
    MyTestBuffMethods,         /* tp_methods */
    0,                         /* tp_members */
    0,                         /* tp_getset */
    0,                         /* tp_base */
    0,                         /* tp_dict */
    0,                         /* tp_descr_get */
    0,                         /* tp_descr_set */
    0,                         /* tp_dictoffset */
    (initproc)InitMyTestBuff,  /* tp_init */
    0,                         /* tp_alloc */
    NewMyTestBuff,             /* tp_new */
};


//---------------------------新类型相关定义结束, 下面开始增加模块函数-----------------

static PyObject *StrLen(PyObject *self, PyObject *args)
{
    const char *input;

    if (!PyArg_ParseTuple(args, "s", &input))
        return NULL;
    return Py_BuildValue("i", strlen(input));
}

static PyObject *DumpTestBuff(MyTestBuff *self, PyObject *args )
{
    char *input;
    int len, i;
    if (!PyArg_ParseTuple(args, "s#", &input, &len))
        return NULL;
    char *msg = malloc(2*len + 1);
    msg[2*len] = '\0';
    i = 0;
    while(i < len){
        sprintf(msg + 2*i, "%02x", input[i]);
        i++;
    }
    PyObject *ret = Py_BuildValue("s", msg);
    free(msg);
    return ret;
}

//python -c "import TestBuff as m; print(m.c_strlen('xxx'))"
static PyMethodDef ModuleMethods[] = {
    {"c_strlen", StrLen, METH_VARARGS, "get string len"},
    {"DumpTestBuff", DumpTestBuff, METH_VARARGS , "dump buff" },
    {NULL}        /* Sentinel */
};


// 定义模块初始化入口. import时自动执行
#ifndef PyMODINIT_FUNC  /* declarations for DLL import/export */
#define PyMODINIT_FUNC void
#endif

// 把新类型和相关模块函数注册进来
PyMODINIT_FUNC initTestBuff(void)
{
    PyObject* m;
    // 新类型操作模板header
    if (PyType_Ready(&oTestBuff) < 0) return;

    // 创建新模块
    m = Py_InitModule3("TestBuff", ModuleMethods, "It's buff manage util");

    if (m == NULL) return;

    Py_INCREF(&oTestBuff);
    // 把新类型加入模块中
    // import tbuff;ins = tbuff.TestBuff();
    PyModule_AddObject(m, "TestBuff", (PyObject *)&oTestBuff);
}
