#include "properties.h"
#include "buf.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#ifdef  _MSC_VER
#include <malloc.h>
#else
#include <alloca.h>
#endif//_MSC_VER


#ifndef ASSERT
#define ASSERT(e)   assert(e)
#endif//ASSERT



//  缺省 cache 的大小
#define CACHE_SIZE_DEF  (4*1024)


//      解析的数据源
struct parse_source_file_t
{
    PARSE_READ  read;       //  加载数据
    PARSE_FREE  free;       //  释放输入源

    FILE*       file;    //  输入流的上下文
};


//      解析的数据源
struct parse_source_string_t
{
    PARSE_READ  read;       //  加载数据
    PARSE_FREE  free;       //  释放输入源

    char*       str;        //  字符串起始地址
    char*       end;        //  字符串结束的位置
    char*       pos;        //  缓冲区大小
};


struct cache_t
{
    struct parse_source_t*    source;
    char*               cache;      //  缓冲器的起始位置
    char*               tail;       //  缓冲器的结束位置

    int                 lino;       //  行号
    char*               line;       //  行的起始位置
    char*               pos;        //  当前已经识别到的位置
    char*               limit;      //  cache 的结尾

    struct buf_t*       key;
    struct buf_t*       val; 
};


enum
{
    P_NEWLINE   =   (0x01), //  \n  \r
    P_SPLITER   =   (0x02), //  :   = 
    P_ESCAPE    =   (0x04), //  \\  (反斜杠转义字符)
    P_SPACE     =   (0x08), //  0x20(space) 0x09(tab \t) 0x0C(换页 \f)
    P_COMMENT   =   (0x10), //  #   !
    P_EOS       =   (0x20), //  \0 
};


static unsigned char cm[256] = 
{
    /*  00  NUL     */  P_EOS,
    /*  01  SOH     */  0,
    /*  02  STX     */  0,
    /*  03  ETX     */  0,
    /*  04  EOT     */  0,
    /*  05  ENQ     */  0,
    /*  06  ACK     */  0,
    /*  07  BEL     */  0,
    /*  08  BS      */  0,
    /*  09  HT      */  P_SPACE,
    /*  0A  LF      */  P_NEWLINE,
    /*  0B  VT      */  0,
    /*  0C  FF      */  P_SPACE,
    /*  0D  CR      */  0,
    /*  0E  SO      */  0,
    /*  0F  SI      */  0,
    /*  10  DLE     */  0,
    /*  11  DC1     */  0,
    /*  12  DC2     */  0,
    /*  13  DC3     */  0,
    /*  14  DC4     */  0,
    /*  15  NAK     */  0,
    /*  16  SYN     */  0,
    /*  17  ETB     */  0,
    /*  18  CAN     */  0,
    /*  19  EM      */  0,
    /*  1A  SUB     */  0,
    /*  1B  ESC     */  0,
    /*  1C  FS      */  0,
    /*  1D  GS      */  0,
    /*  1E  RS      */  0,
    /*  1F  US      */  0,
    /*  20  (space) */  P_SPACE,
    /*  21  !       */  P_COMMENT,
    /*  22  "       */  0,
    /*  23  #       */  P_COMMENT,
    /*  24  $       */  0,
    /*  25  %       */  0,
    /*  26  &       */  0,
    /*  27  '       */  0,
    /*  28  (       */  0,
    /*  29  )       */  0,
    /*  2A  *       */  0,
    /*  2B  +       */  0,
    /*  2C  ,       */  0,
    /*  2D  -       */  0,
    /*  2E  .       */  0,
    /*  2F  /       */  0,
    /*  30  0       */  0,
    /*  31  1       */  0,
    /*  32  2       */  0,
    /*  33  3       */  0,
    /*  34  4       */  0,
    /*  35  5       */  0,
    /*  36  6       */  0,
    /*  37  7       */  0,
    /*  38  8       */  0,
    /*  39  9       */  0,
    /*  3A  :       */  P_SPLITER,
    /*  3B  ;       */  0,
    /*  3C  <       */  0,
    /*  3D  =       */  P_SPLITER,
    /*  3E  >       */  0,
    /*  3F  ?       */  0,
    /*  40  @       */  0,
    /*  41  A       */  0,
    /*  42  B       */  0,
    /*  43  C       */  0,
    /*  44  D       */  0,
    /*  45  E       */  0,
    /*  46  F       */  0,
    /*  47  G       */  0,
    /*  48  H       */  0,
    /*  49  I       */  0,
    /*  4A  J       */  0,
    /*  4B  K       */  0,
    /*  4C  L       */  0,
    /*  4D  M       */  0,
    /*  4E  N       */  0,
    /*  4F  O       */  0,
    /*  50  P       */  0,
    /*  51  Q       */  0,
    /*  52  R       */  0,
    /*  53  S       */  0,
    /*  54  T       */  0,
    /*  55  U       */  0,
    /*  56  V       */  0,
    /*  57  W       */  0,
    /*  58  X       */  0,
    /*  59  Y       */  0,
    /*  5A  Z       */  0,
    /*  5B  [       */  0,
    /*  5C  \       */  P_ESCAPE,
    /*  5D  ]       */  0,
    /*  5E  ^       */  0,
    /*  5F  _       */  0,
    /*  60  `       */  0,
    /*  61  a       */  0,
    /*  62  b       */  0,
    /*  63  c       */  0,
    /*  64  d       */  0,
    /*  65  e       */  0,
    /*  66  f       */  0,
    /*  67  g       */  0,
    /*  68  h       */  0,
    /*  69  i       */  0,
    /*  6A  j       */  0,
    /*  6B  k       */  0,
    /*  6C  l       */  0,
    /*  6D  m       */  0,
    /*  6E  n       */  0,
    /*  6F  o       */  0,
    /*  70  p       */  0,
    /*  71  q       */  0,
    /*  72  r       */  0,
    /*  73  s       */  0,
    /*  74  t       */  0,
    /*  75  u       */  0,
    /*  76  v       */  0,
    /*  77  w       */  0,
    /*  78  x       */  0,
    /*  79  y       */  0,
    /*  7A  z       */  0,
    /*  7B  {       */  0,
    /*  7C  |       */  0,
    /*  7D  }       */  0,
    /*  7E  ~       */  0,
    /*  7F  DEL     */  0,
};




static void p_source_file_free(struct parse_source_t* psource)
{
    if (NULL == psource)
    {
        return;
    }

    struct parse_source_file_t* source = (struct parse_source_file_t*)psource;
    if (NULL != source->file)
    {
        fclose((FILE*)(source->file));
    }

    free(source);
}




//  文件已经结束或者文件读取已经出错了
static int p_source_file_read(struct parse_source_t* psource, char* buf, int* size)
{
    ASSERT(NULL != psource);

    struct parse_source_file_t* source = (struct parse_source_file_t*)psource;
    FILE* file = source->file;
    if (feof(file))
    {
        size = 0;
        return  1;
    }

    if (ferror(file))
    {
        size = 0;
        return -1;
    }

    //  开始读取
    size_t read_size   = fread(buf, 1, *size, file);
    *size = read_size;
    return 0;
}




EXTERN  struct parse_source_t*  parse_source_new_from_file     (char* filename)
{
    ASSERT(NULL != filename);

    FILE* file = fopen(filename, "r");
    if (NULL == file)
    {
        return  NULL;
    }

    struct parse_source_file_t* source = (struct parse_source_file_t*)malloc(sizeof(struct parse_source_file_t));
    if (NULL == source)
    {
        return  NULL;
    }

    memset(source, 0, sizeof(struct parse_source_file_t));
    source->read    =   p_source_file_read;   
    source->free    =   p_source_file_free;
    source->file    =   file;

    return  (struct parse_source_t*)source;
}

static void     p_source_string_free(struct parse_source_t* source)
{
    if (NULL == source)
    {
        return;
    }

    free(source);
    return;
}

static int      p_source_string_read(struct parse_source_t* psource, char* buf, int* size)
{
    ASSERT(NULL != psource);

    struct parse_source_string_t* source = (struct parse_source_string_t*)psource;

    if (source->end == source->pos)
    {
        *size = 0;
        return 1;
    }

    int copy_len = (source->end - source->pos);
    if (copy_len > *size)
    {
        copy_len = *size;
    }

    if (0 == copy_len)
    {
        *size = copy_len;
        return 0;
    }

    memcpy(buf, source->pos, copy_len);
    source->pos += copy_len;

    *size = copy_len;
    return 0;
}

EXTERN  struct parse_source_t*    parse_source_new_from_string   (char* str, char* end)
{
    ASSERT(NULL != str);
    if (NULL == end)
    {
        end = str + strlen(str);
    }

    struct parse_source_string_t* source = (struct parse_source_string_t*)malloc(sizeof(struct parse_source_string_t));
    if (NULL == source)
    {
        return  NULL;
    }

    memset(source, 0, sizeof(struct parse_source_t));
    source->read    =   p_source_string_read;   
    source->free    =   p_source_string_free;
    source->str     =   str;
    source->end     =   end;
    source->pos     =   str;

    return  (struct parse_source_t*)source;
}



EXTERN  void    parse_source_del(struct parse_source_t* source)
{
    if (NULL == source)
    {
        return;
    }

    if (NULL != source->free)
    {
        source->free(source);
    }
}


static inline char* p_cache_read_more(struct cache_t* cache, char* pos)
{
    //  设置终止位置
    cache->pos = pos;

    //  如果缓冲区里面还有数据存在，那么需要搬移数据，以便腾出更多的空间
    int offset    = cache->pos - cache->cache;  //  需要移动多远
    if (offset > 0)
    {
        int move_size = cache->limit - cache->pos;    //  需要移动多少数据
        if (move_size > 0)
        {
            memmove(cache->cache, pos, move_size);
        }
        cache->line     -= offset;
        cache->pos      -= offset;
        cache->limit    -= offset;
        cache->limit[0] = '\0';
    }

    //  读取新数据填充缓冲区
    int remain_size = cache->tail - cache->limit;
    int ret = cache->source->read(cache->source, cache->limit, &remain_size);

    //  文件已经结束或者文件读取已经出错了
    if (ret != 0)
    {
        return cache->pos;
    }

    cache->limit += remain_size;
    cache->limit[0] = '\0';
    return cache->pos;
}




static inline char* p_skip_space(struct cache_t* cache, char* pos)
{
    cache->pos = pos;

retry:
    //  跳过所有的空白
    while (P_SPACE&cm[*pos])
    {
        pos++;
    }

    //  如果遇到的是 \0 符号，那么我们可以认为可能遇到了缓冲区不够或者输入流截止的问题
    if ('\0' == *pos)
    {
        //  如果是缓冲区结尾标记
        if (pos == cache->limit)
        {
            //  载入更多数据
            pos = p_cache_read_more(cache, pos);

            //  如果载入数据之后，仍然是缓冲区结尾，说明已经数据全部处理完毕了
            if (pos == cache->limit)
            {
                return pos;
            }

            //  如果缓冲区已经被新数据填充了，那么继续前面的处理
            goto retry;
        }
    }

    //  到这里可以确信已经 cache->pos 一定不是空白了  
    return cache->pos = pos;
}


static inline char* p_accept_comment(struct cache_t* cache, char* pos)
{
    cache->pos = pos + 1;

retry:
    //  注释可能会因为输入流结束或者读到文件尾而结束
    while (!((P_NEWLINE|P_EOS)&cm[*pos]))
    {
        pos++;
    }

    //  如果遇到换行,那么注释就结束了
    if (P_NEWLINE&cm[*pos])
    {
        return cache->pos = pos;
    }

    if (P_EOS&cm[*pos])
    {
        //  载入更多数据
        pos = p_cache_read_more(cache, pos);

        //  如果载入数据之后，仍然是缓冲区结尾，说明已经数据全部处理完毕了
        if (pos == cache->limit)
        {
            return cache->pos;
        }

        //  如果缓冲区已经被新数据填充了，那么继续前面的处理
        goto retry;
    }

    return cache->pos = pos;
}

static inline char* p_accept_unicode_escape(struct cache_t* cache, char* pos, struct buf_t* buf)
{
    //  uxxxx
    cache->pos = pos;

    pos++;  //  吃掉字符 u

    register char* end = cache->pos + 4;

    //  如果提前结束
    if ((cache->pos + 4) > cache->limit)
    {
        p_cache_read_more(cache, cache->pos);    //  确保至少有4个字符
        if ((cache->pos + 4) > cache->limit)
        {
            //  要报错吗? 先保证尽量读取
            end = cache->limit;
        }
    }


    //  对数据进行格式转换
    wchar_t c = 0;
    for (register char* p = cache->pos; p != end; p++)
    {
        switch (*p)
        {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            c = (c << 4) | (*p - '0');
            continue;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
            c = (c << 4) | (*p - 'a' + 10);
            continue;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'E':
        case 'F':
            c = (c << 4) | (*p - 'A' + 10);
            continue;
        default:
            //  遇到非预期的字符，提前终止
            buf_append_wchar(buf, c);
            cache->pos = p;
            break;
        }
    }

    //  处理完毕
    buf_append_wchar(buf, c);
    return cache->pos = end;
}



static inline char* p_accept_escape(struct cache_t* cache, char* pos, struct buf_t* buf)
{
    cache->pos = pos;

    pos++;  //  吃掉 `\\` 

retry:
    switch (*pos)
    {
    case '\0':
        //  如果实际是缓冲区结束标志
        if (pos == cache->limit)
        {
            pos = p_cache_read_more(cache, pos);
            if (pos == cache->limit)
            {
                return pos;
            }

            goto retry;
        }

        //  斜杠后面跟着一个有效的 \0，这种情况其实不应该存在，但 Java 规范并没有说明这种情况应该怎么处理
        buf_append_char(buf, '\0');
        pos++;
        goto retry;
    case 'n':   //  \n
        buf_append_char(buf, '\n');
        return cache->pos = pos + 1;
        ;
    case 't':   //  \t
        buf_append_char(buf, '\t');
        return cache->pos = pos + 1;
    case 'r':   //  \t
        buf_append_char(buf, '\t');
        return cache->pos = pos + 1;
    case 'u':   //  unicode转义
        return p_accept_unicode_escape(cache, pos, buf);
    case '\n':
        return p_skip_space(cache, pos + 1);
    default:    // *pos
        buf_append_char(buf, *pos);
        return cache->pos = pos + 1;
    }
}


char* p_accept_key(struct cache_t* cache)
{
    register char* pos = cache->pos;

    while (1)
    {
        while (!((P_SPACE|P_NEWLINE|P_ESCAPE|P_EOS|P_SPLITER)&cm[*pos]))
        {
            pos++;
        }

        buf_append_string(cache->key, cache->pos, pos);
        switch (cm[*pos])
        {
        case P_SPACE:
        case P_SPLITER:
            return cache->pos = pos;
        case P_NEWLINE:
            return cache->pos = pos;
        case P_ESCAPE:
            pos = p_accept_escape(cache, pos, cache->key);
            continue;
        case P_EOS:
            //  如果实际是缓冲区结束标志
            if (pos == cache->limit)
            {
                //  这里需要将字符串存到cache里面去
                pos = p_cache_read_more(cache, pos);
                if (pos == cache->limit)
                {
                    //  生成key，并向props添加key
                    return cache->pos = pos;
                }

                continue;
            }

            //  真遇到 \0 实际已经无法继续下去了
            buf_append_char(cache->key, '\0');
            pos++;
            continue;
        default:
            //  不应该走到这里来
            ASSERT(0);
        }

        //  不应该走到这里来
        ASSERT(0);
    }
}


static inline char* p_accept_value(struct cache_t* cache, struct buf_t* buf)
{
    register char* pos = cache->pos;

    while (1)
    {
        while (!((P_NEWLINE|P_ESCAPE|P_EOS)&cm[*pos]))
        {
            pos++;
        }

        buf_append_string(buf, cache->pos, pos);

        switch (cm[*pos])
        {
        case P_NEWLINE:
            //  生成val，并向props添加key
            return cache->pos = pos;
        case P_ESCAPE:
            pos = p_accept_escape(cache, pos, cache->val);
            continue;
        case P_EOS:
            //  如果实际是缓冲区结束标志
            if (pos == cache->limit)
            {
                //  TODO    此时要将字符串存起来
                pos = p_cache_read_more(cache, pos);
                if (pos == cache->limit)
                {
                    //  生成key，并向props添加key
                    return pos;
                }

                continue;
            }

            //  真遇到 \0 实际已经无法继续下去了
            buf_append_char(buf, '\0');
            pos++;
            continue;
        default:
            //  不应该走到这里来
            ASSERT(0);
        }

        //  不应该走到这里来
        ASSERT(0);
    }
}

static inline char* p_accept_spliter(struct cache_t* cache)
{
    register char* pos = cache->pos;

retry:
    pos = p_skip_space(cache, pos + 1);

    if (P_EOS&cm[*pos])
    {
        //  如果实际是缓冲区结束标志
        if (pos == cache->limit)
        {
            pos = p_cache_read_more(cache, pos);
            if (pos == cache->limit)
            {
                return pos;
            }

            goto retry;
        }

        //  真遇到 \0 实际已经无法继续下去了
        return cache->pos = pos;
    }

    if (P_SPLITER&cm[*pos])
    {
        return p_skip_space(cache, pos + 1);
    }

    if (P_NEWLINE&cm[*pos])
    {
        return cache->pos = pos;
    }

    return cache->pos = pos;
}



// int properties_load         (struct parse_source_t* source)
// {
//     ASSERT(NULL != source);
// 
//     struct cache_t cache;
//     cache.source    =   source;
//     if (NULL != source->cache)
//     {
//         cache.cache     =   source->cache;
//         cache.tail      =   source->cache + source->size;
//         cache.outsize   =   1;
//     }
//     else
//     {
//         cache.cache     =   (char*)malloc(CACHE_SIZE_DEF);
//         cache.outsize   =   0;
//     }
//     cache.lino      =   0;
//     cache.line      =   cache.cache;
//     cache.pos       =   cache.cache;
//     cache.limit       =   cache.cache;
// 
//     switch (format[0])
//     {
//     case 'p':
//         if (0 == strcmp(format, SOURCE_FORMAT_PROPERTIES))
//         {
//             return p_parse_format_properties(p, &cache);
//         }
//         return -1;
//     case 'y':
//         if (0 == strcmp(format, SOURCE_FORMAT_YAML))
//         {
//             return p_parse_format_yaml(p, &cache);
//         }
//         return -1;
//     case 'x':
//         if (0 == strcmp(format, SOURCE_FORMAT_XML))
//         {
//             return p_parse_format_xml(p, &cache);
//         }
//         return -1;
//     case 'i':
//         if (0 == strcmp(format, SOURCE_FORMAT_INI))
//         {
//             return p_parse_format_ini(p, &cache);
//         }
//         return -1;
//     default:
//         return -1;
//     }
// }

static inline int   properties_load_impl(struct cache_t* cache, HANDLE_PROPERTY handle, void* context)
{
    register char*  pos   = p_cache_read_more(cache, cache->pos);
    while (1)
    {
        if (P_SPACE&cm[*pos])
        {
            pos = p_skip_space(cache, pos);
        }

        //  如果是注释行，跳过
        if (P_COMMENT&cm[*pos])
        {
            pos = p_accept_comment(cache, pos);
            continue;
        }

        //  如果是空行，修改行号和行起始
        if (P_NEWLINE&cm[*pos])
        {
            pos++;
            cache->pos = pos;
            cache->line = cache->pos;
            cache->lino++;
            pos = p_skip_space(cache, pos);
            continue;
        }

        //  如果遇到\0符号，如果是输入流结尾，那么识别结束
        if (P_EOS&cm[*pos])
        {
            //  如果是行尾，说明已经无法再载入更多数据，识别结束
            if (pos == cache->limit)
            {
                return 0;
            }
        }

        //  重置 key 和 val
        buf_reset(cache->key);
        buf_reset(cache->val);

        //  收取一个 key
        pos = p_accept_key(cache);

        //  收取分隔符号
        pos = p_accept_spliter(cache);

        //  如果没有后续内容，那么判定该项只有 key
        if ((P_NEWLINE|P_EOS)&cm[*pos])
        {
            cache->key = buf_append_char(cache->key, '\0');
            cache->val = buf_append_char(cache->val, '\0');
            if (0 != handle(context, cache->key->data, cache->key->len, cache->val->data, cache->val->len))
            {
                return 0;
            }

            continue;
        }

        //  收取 val
        pos = p_accept_value(cache, cache->val);

        cache->key = buf_append_char(cache->key, '\0');
        cache->val = buf_append_char(cache->val, '\0');
        if (0 != handle(context, cache->key->data, cache->key->len - 1, cache->val->data, cache->val->len - 1))
        {
            return 0;
        }
    }
}


int     properties_load(struct parse_source_t* source, HANDLE_PROPERTY handle, void* context)
{
    struct cache_t cache;
    cache.source    =   source;
    cache.cache     =   (char*)alloca(CACHE_SIZE_DEF + 1);
    cache.tail      =   cache.cache + CACHE_SIZE_DEF;
    cache.limit     =   cache.cache;
    cache.lino      =   0;
    cache.line      =   cache.cache;
    cache.pos       =   cache.limit;
    cache.limit[0]  =   '\0';

    cache.key   = buf_new(0);
    if (NULL == cache.key)
    {
        return -1;
    }

    cache.val   = buf_new(0);
    if (NULL == cache.val)
    {
        buf_del(cache.key);
        return -1;
    }

    return properties_load_impl(&cache, handle, context);
}
