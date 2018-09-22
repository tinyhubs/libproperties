#include "properties.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


struct properties_t
{
    int a;
};


enum
{
    P_NEWLINE   =   (0x01), //  \n  \0
    P_SPLITER   =   (0x04), //  : = \b
    P_ESCAPE    =   (0x08), //  \   
    P_SPACE     =   (0x10), //  \r  \t \b   注意不包含换行，因为换行需要特殊处理行号问题
    P_COMMENT   =   (0x20), //  #   !
    P_EOS       =   (0x40), //  \0 
};


unsigned char cmap[256] = 
{
    0,
};

typedef int (*P_READ)(void* context, char* buf, int size);

struct p_cache_t
{
    int     lino;
    char*   line;       //  行的起始位置
    char*   pos;        //  当前已经识别到的位置
    char*   end;        //  cache 的结尾


    char*   cache;      //  缓冲器的起始位置
    char*   tail;       //  缓冲器的结束位置
    
    void*   context;
    P_READ  read;
};


//  文件已经结束或者文件读取已经出错了
int p_read_from_file(void* context, char* buf, int size)
{
    FILE* file = (FILE*)context;
    if (feof(file))
    {
        return  0;
    }

    if (ferror(file))
    {
        return -1;
    }

    //  开始读取
    size_t read_size   = fread(buf, 1, size, file);
    if (read_size > 0)
    {
        return read_size;
    }

    return read_size;
}

int p_read_from_string(void* context, char* buf, int size)
{
    return  0;
}


inline static char* p_cache_read_more(struct p_cache_t* cache, char* pos)
{
    //  设置终止位置
    cache->pos = pos;

    //  如果缓冲区里面还有数据存在，那么需要搬移数据，以便腾出更多的空间
    int offset    = cache->pos - cache->cache;  //  需要移动多远
    if (offset > 0)
    {
        int move_size = cache->end - cache->pos;    //  需要移动多少数据
        if (move_size > 0)
        {
            memmove(cache->cache, pos, move_size);
        }
        cache->line -= offset;
        cache->pos  -= offset;
        cache->end  -= offset;
    }
    
    //  读取新数据填充缓冲区
    int remain_size = cache->tail - cache->end;
    int ret = cache->read(cache->context, cache->end, remain_size);

    //  文件已经结束或者文件读取已经出错了
    if (ret <= 0)
    {
        return cache->pos;
    }

    cache->end += ret;
    return cache->pos;
}




inline static void p_skip_space(struct p_cache_t* cache)
{
    register char* pos = cache->pos;

retry:
    //  跳过所有的空白
    while (P_SPACE&cmap[*pos])
    {
        pos++;
    }

    //  如果遇到的是 \0 符号，那么我们可以认为可能遇到了缓冲区不够或者输入流截止的问题
    if ('\0' == *pos)
    {
        //  如果是缓冲区结尾标记
        if (pos == cache->end)
        {
            //  载入更多数据
            pos = p_cache_read_more(cache, pos);
            
            //  如果载入数据之后，仍然是缓冲区结尾，说明已经数据全部处理完毕了
            if (pos == cache->end)
            {
                return;
            }

            //  如果缓冲区已经被新数据填充了，那么继续前面的处理
            goto    retry;
        }
    }

    //  到这里可以确信已经 cache->pos 一定不是空白了  
    return;
}


inline static void p_accept_comment(struct p_cache_t* cache)
{
    register char* pos = cache->pos;

retry:

    //  注释可能会因为输入流结束或者读到文件尾而结束
    while (!((P_NEWLINE|P_EOS)&cmap[*(++pos)]))
    {
    }

    //  如果遇到换行,那么注释就结束了
    if (P_NEWLINE&cm[*pos])
    {
        cache->pos = pos;
        return;
    }

    if (P_EOS&cm[*pos])
    {
        //  载入更多数据
        pos = p_cache_read_more(cache, pos);

        //  如果载入数据之后，仍然是缓冲区结尾，说明已经数据全部处理完毕了
        if (pos == cache->end)
        {
            return;
        }

        //  如果缓冲区已经被新数据填充了，那么继续前面的处理
        goto    retry;
    }

    return;
}


void p_accept_key(struct p_cache_t* cache)
{
    char* pos = s;

retry:
    
    while (!(P_SPACE|P_NEWLINE|P_ESCAPE|P_EOS|P_SPLITER)&cmap[*pos])
    {
        pos++;
        continue;
    }

    switch (cm[*pos])
    {
    case P_SPACE:
    case P_SPLITER:
        //  生成key
        cache->pos = pos;
        return;
    case P_NEWLINE:
        //  生成key，并向props添加key
        cache->pos = pos;
        return;
    case P_ESCAPE:
        p_accept_escape(cache);
        goto    retry;
    case P_EOS:
        //  如果实际是缓冲区结束标志
        if (pos == cache->end)
        {
            //  TODO 这里需要将字符串存到cache里面去
            p_cache_read_more(cache);
            if (pos == cache->end)
            {
                //  生成key，并向props添加key
                cache->pos = pos;
                return;
            }

            goto    retry;
        }

        //  真遇到 \0 实际已经无法继续下去了
        goto    retry;
    default:
        //  不应该走到这里来
        ASSERT(0)
    }

    //  不应该走到这里来
    ASSERT(0)
}

inline static void p_accept_unicode_escape(struct p_cache_t* cache)
{
    //  \uxxxx
    cache->pos += 2;

    //  如果提前结束
    if ((cache->pos + 4) > cache->end)
    {
        p_cache_read_more(cache, cache->pos);    //  确保至少有4个字符
        if ((cache->pos + 4) > cache->end)
        {
            //  要报错吗？？？
            return;
        }
    }

    //  对数据进行格式转换
}

inline static void p_accept_escape(struct p_cache_t* cache)
{
    cache->pos++;

retry:

    switch (*pos)
    {
    case '\0':
        //  如果实际是缓冲区结束标志
        if (pos == cache->end)
        {
            //  TODO    此时要将字符串存起来
            pos = p_cache_read_more(cache, pos);
            if (pos == cache->end)
            {
                return;
            }

            goto    retry;
        }

        //  真遇到 \0 实际已经无法继续下去了
        goto    retry;
    case 'n':
        //  TODO \n
        return;
    case 't':
        //  TODO \t
        return;
    case 'r':
        //  TODO \t
        return;
    case 'u':
        register char* pos = s;
        //unicode转义
        cache->pos = pos;
        p_accept_unicode_escape(cache);
        return;
    default:
        // *pos
        return;
    }
}




inline static void p_accept_value(struct p_cache_t* cache)
{
    register char* pos = s;

retry:

    while (!(P_NEWLINE|P_ESCAPE|P_EOS)&cmap[*pos])
    {
        pos++;
        continue;
    }

    switch (cm[*pos])
    {
    case P_NEWLINE:
        //  生成key，并向props添加key
        cache->pos = pos;
        return;
    case P_ESCAPE:
        p_accept_escape(cache);
        goto    retry;
    case P_EOS:
        //  如果实际是缓冲区结束标志
        if (pos == cache->end)
        {
            //  TODO    此时要将字符串存起来
            pos = p_cache_read_more(cache, pos);
            if (pos == cache->end)
            {
                //  生成key，并向props添加key
                cache->pos = pos;
                return;
            }

            goto    retry;
        }

        //  真遇到 \0 实际已经无法继续下去了
        goto    retry;
    default:
        //  不应该走到这里来
        ASSERT(0)
    }

    //  不应该走到这里来
    ASSERT(0)
}

void p_accept_spliter(struct p_cache_t* cache)
{
    register char* pos = cache->pos;

retry:

    while (!((P_SPACE|P_SPLITER|P_EOS|P_NEWLINE)&cmp[*pos]))
    {
        pos++;
    }
    cache->pos = pos;

    if (P_EOS&cmp[*pos])
    {
        //  如果实际是缓冲区结束标志
        if (pos == cache->end)
        {
            p_cache_read_more(cache);
            if (pos == cache->end)
            {
                return;
            }

            goto    retry;
        }

        //  真遇到 \0 实际已经无法继续下去了
        return;
    }

    if ((P_SPACE|P_SPLITER)&cmp[pos])
    {
        p_skip_space(cache);
        return;
    }

    if (P_NEWLINE&cmp[pos])
    {
        return;
    }
}

int properties_parse(struct p_cache_t* cache, void* context)
{
    register char* pos = cache->pos;

retry:

    p_skip_space(cache);
    pos = cache->pos;

    //  如果是注释行，跳过
    if (P_COMMENT&cmp[*pos])
    {
        p_accept_comment(cache);
        goto    retry;
    }

    //  如果是空行，修改行号和行起始
    if (P_NEWLINE&cmp[*pos])
    {
        cache->pos++;
        cache->line = cache->pos;
        cache->lino++;
        goto    retry;
    }

    //  如果遇到\0符号，如果是输入流结尾，那么识别结束
    if (P_EOS&cmp[*pos])
    {
        //  如果是行尾，说明已经无法再载入更多数据，识别结束
        if (pos == cache->end)
        {
            return 0;
        }
    }

    p_accept_key(cache);
}






struct properties_t*    properties_new         void                    properties_del          (struct properties_t* p)
{

}


int                     properties_load_file    (struct properties_t* p, char* file)
{

}


int                     properties_load_string  (struct properties_t* p, char* s)
{
    if (NULL == s)
    {
        return -1;
    }

    struct p_cache_t cache;
    cache
    p_accept_key()
    
}


int                     properties_load_envs    (struct properties_t* p, char* file)
{

}


char*                   properties_get          (struct properties_t* p, char* key, char* def)
{

}


void                    properties_set          (struct properties_t* p, char* key, char* val)
{

}


void                    properties_clear        (struct properties_t* p, char* key)
{

}


char*                   properties_find         (struct properties_t* p, char* key)
{

}


void                    properties_foreach      (struct properties_t* p, int (*callback)(void* context, char* key, char* val), void* context)
{

}


