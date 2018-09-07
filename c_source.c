#include "c_header.h"

int NewBuffer(TestBuffStruct_t *buf)
{
    buf->buff = 0;
    buf->capacity = 0;
    buf->curIdx = -1;
    return 0;
}

int InitBuffer(TestBuffStruct_t *buf, int capacity, const char * input )
{
    if(capacity < 0) return 1;
    int inputLen = strlen(input);
    int len = inputLen > capacity ? capacity : inputLen;
    buf->buff = (char *)malloc(capacity);

    strncpy(buf->buff, input, len);
    buf->capacity = capacity;
    buf->curIdx = len - 1;
    return 0;
}

int FreeBuff(TestBuffStruct_t *buf)
{
    free(buf->buff);
    return 0;
}

const char *GetBuffer(TestBuffStruct_t *buf, int start, int len)
{
    if(start > buf->curIdx || start < 0) return 0;
    if(start + len - 1 > buf->curIdx) return 0;
    return buf->buff + start;
}

int AppendBuffer(TestBuffStruct_t *buf, int start, int len, const char *buff)
{
    start = start < 0 ? (buf->curIdx + 1) : start;
    if(len < 1 || start + len - 1 >= buf->capacity) return 0;
    
    int i = 0;
    char *begin = buf->buff + start; 
    while(i < len){
        begin[i] = buff[i];
        ++i;
    }
    buf->curIdx += len; 
    return 1;
}

int PopBuffer(TestBuffStruct_t *buf, int start, int len, char *out)
{
    start = start < 0 ? (buf->curIdx - len + 1) : start;
    if(start > buf->curIdx || start < 0) return 0;
    if(start + len - 1 > buf->curIdx) return 0;
    
    int i = 0;
    char *begin = buf->buff + start; 
    while(i < len){
        out[i] = begin[i];
    }
    buf->curIdx -= len; 
    return 1;
}
