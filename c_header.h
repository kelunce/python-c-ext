#ifndef _MY_TEST_TYPE_H_
#define _MY_TEST_TYPE_H_

typedef struct cTestBuffStruct{
    char *buff;
    int capacity;
    int curIdx;
}TestBuffStruct_t;

int NewBuffer(TestBuffStruct_t *buf);
int InitBuffer(TestBuffStruct_t *buf, int capacity, const char * input );
int FreeBuff(TestBuffStruct_t *buf);
const char *GetBuffer(TestBuffStruct_t *buf, int start, int len);
int AppendBuffer(TestBuffStruct_t *buf, int start, int len, const char *buff);
int PopBuffer(TestBuffStruct_t *buf, int start, int len, char *out);
#endif
