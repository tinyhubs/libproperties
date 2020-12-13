#ifndef __buf_H_
#define __buf_H_

#include <stddef.h>


#ifndef EXTERN
#ifdef __cplusplus
#define EXTERN  extern "C"
#else
#define EXTERN
#endif
#endif//EXTERN




#ifndef EXTERN
#ifdef __cplusplus
#define EXTERN  extern "C"
#else
#define EXTERN
#endif
#endif//EXTERN




struct buf_t
{
    int     error;
    size_t  cap;
    size_t  len;
    char    data[0];
};


EXTERN  struct buf_t*   buf_new             (size_t size);
EXTERN  void            buf_del             (struct buf_t* buf);
EXTERN  struct buf_t*   buf_append          (struct buf_t* buf, void* data, size_t len);
EXTERN  struct buf_t*   buf_append_string   (struct buf_t* buf, char* s, char*  end);
EXTERN  struct buf_t*   buf_append_char     (struct buf_t* buf, char            val);
EXTERN  struct buf_t*   buf_append_wchar    (struct buf_t* buf, wchar_t         val);
EXTERN  struct buf_t*   buf_append_int32    (struct buf_t* buf, int             val);
EXTERN  struct buf_t*   buf_append_uint32   (struct buf_t* buf, unsigned int    val);
EXTERN  struct buf_t*   buf_append_int16    (struct buf_t* buf, short           val);
EXTERN  struct buf_t*   buf_append_uint16   (struct buf_t* buf, unsigned short  val);
EXTERN  struct buf_t*   buf_append_int8     (struct buf_t* buf, char            val);
EXTERN  struct buf_t*   buf_append_uint8    (struct buf_t* buf, unsigned char   val);
#define buf_ok(buf)         (!((buf)->error))
#define buf_clrerr(buf)     (((buf)->error = 0),(buf))
#define buf_reset(buf)      (((buf)->error = 0),((buf)->len = 0),((buf)->data[0] = 0),(buf))

#endif//__buf_H_
