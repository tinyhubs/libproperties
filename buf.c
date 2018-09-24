#include "buf.h"


#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>


#ifndef ASSERT
#define ASSERT(e)   assert(e)
#endif//ASSERT




inline static size_t  buf_calc_size(size_t need_cap)
{
    if (need_cap < 512)
    {
        if (need_cap < 128)
        {
            if (need_cap < 64)
            {
                return 64;
            }

            return 128;
        }

        if (need_cap < 256)
        {
            return 256;
        }

        return 512;
    }

    return ((need_cap / 512) + 1) * 512;
}

EXTERN struct buf_t*   buf_new             (size_t cap)
{
    size_t size = buf_calc_size(sizeof(struct buf_t) + cap);
    struct buf_t* buf = (struct buf_t*)malloc(size);
    if (NULL == buf)
    {
        return NULL;
    }

    buf->error   = 0;
    buf->cap     = size - sizeof(struct buf_t);
    buf->len     = 0;
    buf->data[0] = 0;

    return buf;
}


EXTERN void            buf_del             (struct buf_t* buf)
{
    if (NULL != buf)
    {
        free(buf);
    }
}

EXTERN struct buf_t*   buf_append          (struct buf_t* buf, void* data, size_t len)
{
    ASSERT(NULL != buf);
    ASSERT(NULL != data);
    ASSERT(len >= 0);

    if (0 == len)
    {
        return buf;
    }

    if (buf->error)
    {
        return buf;
    }

    size_t need_cap = (buf->len + len);
    if (need_cap > buf->cap)    //  »º³åÇø²»¹»
    {
        size_t new_size = buf_calc_size(sizeof(struct buf_t) + need_cap);
        struct buf_t* new_buf = (struct buf_t*)malloc(new_size);
        if (NULL == new_buf)
        {
            buf->error = 1;
            return  buf;
        }

        memcpy(new_buf, buf, sizeof(struct buf_t) + buf->len);
        new_buf->cap = new_size - sizeof(struct buf_t);
        free(buf);
        buf = new_buf;
    }

    memcpy(buf->data + buf->len, data, len);
    buf->len += len;
    return buf;
}

EXTERN struct buf_t*   buf_append_string   (struct buf_t* buf, char* s, char*  end)
{
    ASSERT(NULL != buf);
    ASSERT(NULL != s);

    size_t len = 0;
    if (NULL == end)
    {
        len = strlen(s);
    }
    else
    {
        len = end - s;
    }

    if (0 == len)
    {
        return buf;
    }

    return buf_append(buf, s, len);
}

EXTERN struct buf_t*   buf_append_char     (struct buf_t* buf, char            val)
{
    return buf_append(buf, &val, sizeof(char));
}

EXTERN struct buf_t*   buf_append_wchar    (struct buf_t* buf, wchar_t         val)
{
    return buf_append(buf, &val, sizeof(wchar_t));
}

EXTERN struct buf_t*   buf_append_int32    (struct buf_t* buf, int             val)
{
    return buf_append(buf, &val, sizeof(int));
}


EXTERN struct buf_t*   buf_append_uint32   (struct buf_t* buf, unsigned int    val)
{
    return buf_append(buf, &val, sizeof(unsigned int));
}


EXTERN struct buf_t*   buf_append_int16    (struct buf_t* buf, short           val)
{
    return buf_append(buf, &val, sizeof(short));
}


EXTERN struct buf_t*   buf_append_uint16   (struct buf_t* buf, unsigned short  val)
{
    return buf_append(buf, &val, sizeof(unsigned short));
}


EXTERN EXTERN struct buf_t*   buf_append_int8     (struct buf_t* buf, char            val)
{
    return buf_append(buf, &val, sizeof(char));
}


EXTERN struct buf_t*   buf_append_uint8    (struct buf_t* buf, unsigned char   val)
{
    return buf_append(buf, &val, sizeof(unsigned char));
}


