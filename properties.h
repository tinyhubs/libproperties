#ifndef __properties_H_
#define __properties_H_




#ifndef EXTERN
#ifdef __cplusplus
#define EXTERN  extern "C"
#else
#define EXTERN
#endif
#endif//EXTERN




typedef int     (*PARSE_READ)(struct parse_source_t* source, char* buf, int size);
typedef void    (*PARSE_FREE)(struct parse_source_t* source);
struct parse_source_t
{
    PARSE_READ  read;
    PARSE_FREE  free;
};




EXTERN  struct parse_source_t*  parse_source_new_from_file      (char* filename);
EXTERN  struct parse_source_t*  parse_source_new_from_string    (char* str, char* end);
EXTERN  void                    parse_source_del                (struct parse_source_t* source);




typedef int     (*HANDLE_PROPERTY)(void* context, char* key, int key_len, char* val, int val_len);
EXTERN  int     properties_load(struct parse_source_t* source, HANDLE_PROPERTY handle, void* context);




#endif//__properties_H_
