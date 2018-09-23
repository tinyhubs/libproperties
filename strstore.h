#ifndef __strstore_H_
#define __strstore_H_


typedef unsigned int    uint32_t;
typedef int             int32_t;


struct str_t
{
    void*       owner;
    uint32_t    hash;
    int32_t     len;
    char        text[0];
};


struct strstore_t;


struct strstore_t*  strstore_new    (int32_t init_cap);
void                strstore_del    (struct strstore_t* s);
struct str_t*       strstore_save   (struct strstore_t* s, char* str, char* end);
struct str_t*       strstore_find   (struct strstore_t* s, char* str, char* end);



#endif//__strstore_H_
