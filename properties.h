#ifndef __properties_H_
#define __properties_H_


struct properties_t;
struct properties_t*    properties_new          ();
void                    properties_del          (struct properties_t* p);
int                     properties_load_file    (struct properties_t* p, char* file);
int                     properties_load_string  (struct properties_t* p, char* s);
int                     properties_load_envs    (struct properties_t* p, char* file);
char*                   properties_get          (struct properties_t* p, char* key, char* def);
void                    properties_set          (struct properties_t* p, char* key, char* val);
void                    properties_clear        (struct properties_t* p, char* key);
char*                   properties_find         (struct properties_t* p, char* key);
void                    properties_foreach      (struct properties_t* p, int (*callback)(void* context, char* key, char* val), void* context);

#endif//__properties_H_
