#ifndef __properties_H_
#define __properties_H_


#define SOURCE_FORMAT_PROPERTIES    "properties"
#define SOURCE_FORMAT_YAML          "yaml"
#define SOURCE_FORMAT_XML           "xml"
#define SOURCE_FORMAT_INI           "ini"


//  Fountion:   P_READ
//      从 source 读取数据
//  Parameters:
//      source  -   输入源
//      buf     -   用于存放读取数据的缓冲区的起始地址
//      size    -   缓冲区的总大小
//  Returns:
//      -1  -   表示读取失败
//      0   -   表示输入流结束，已经没有数据可以读取
//      >0  -   表示成功读取出多少字节的数据
typedef int     (*P_READ)(struct source_t* source, char* buf, int size);
typedef void    (*P_FREE)(struct source_t* source);
struct source_t
{
    void*   context;    //  输入流的上下文
    P_READ  read;       //  加载数据
    P_FREE  free;       //  释放输入源
    char*   cache;      //  缓冲区
    size_t  size;       //  缓冲区大小
};


int     source_init_by_file     (struct source_t* source, char* filename);
int     source_init_by_string   (struct source_t* source, char* str, char* end);
int     source_init_by_envs     (struct source_t* source);
void    source_exit             (struct source_t* source);


struct properties_t;
struct properties_t*    properties_new          ();
void                    properties_del          (struct properties_t* p);
int                     properties_load         (struct properties_t* p, struct source_t* source, char* format);
char*                   properties_get          (struct properties_t* p, char* key, char* def);
void                    properties_set          (struct properties_t* p, char* key, char* val);
void                    properties_clear        (struct properties_t* p, char* key);
char*                   properties_find         (struct properties_t* p, char* key);
void                    properties_foreach      (struct properties_t* p, int (*callback)(void* context, char* key, char* val), void* context);

#endif//__properties_H_
