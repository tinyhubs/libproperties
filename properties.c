

struct properties_t
{

};


struct p_cache_t
{
    int     len;
    char    cache[1024];
};

#define P_NEWLINE   (0x01)  //  \n  \0
#define P_VISIBLE   (0x02)  //  
#define P_SPLITER   (0x04)  //  : = \b
#define P_ESCAPE    (0x08)  //  \   
#define P_SPACE     (0x10)  //  \r  \t \b   注意不包含换行，因为换行需要特殊处理行号问题
#define P_COMMENT   (0x20)  //  #   !
unsigned char cmap[256] = 
{

};


struct p_cache_t
{
    int     lino;
    char*   line;   //  行的起始位置
    char*   pos;    //  当前已经识别到什么位置
    char*   cache;  //  缓冲区大小
    char*   end;    //  cache 的结尾
};

inline static void p_skip_space(struct p_cache_t* cache)
{
    register char* pos = cache->pos;

retry:
    //  跳过所有的空白
    while (P_SPACE&cmap[*pos])
    {
        pos++;
    }

    //  如果不是换行标记，那么可以提前结束了，这里有利于提高性能
    if (!(P_NEWLINE&cmap[*pos]))
    {
        return;
    }

    //  如果遇到的是真换行，那么需要重置行的起始位置和行号，然后重新扫描和跳过空白
    while ('\n' == *pos)
    {
        pos++;
        cache->pos = pos;
        cache->lino++;
        cache->line = pos;
        goto    retry;
    }

    //  如果遇到的是 \0 符号，那么我们可以认为可能遇到了缓冲区不够或者输入流截止的问题
    if ('\0' == *pos)
    {
        //  如果是缓冲区不够
        if (pos == cache->end)
        {
            cache->pos = pos
            p_cache_read_more();    //  载入更多数据
            pos = cache->pos;
            if (pos == cache->end)  //  如果载入数据之后仍然不行
            {
                return;
            }

            //  如果缓冲区已经被新数据填充了，那么继续前面的处理
            goto    retry;
        }

        //  如果输入字符串就是 \0，那么就当作正常符号处理，该符号统一在key的识别中会当作错误处理
        return;
    }

    //  程序不应该走到这里来
    return;
}


inline static void p_accept_comment(struct p_cache_t* cache)
{
    register char* pos = cache->pos;

    //  只要不是换行或者cache结尾符号
    while (!(P_NEWLINE&cmap[*pos]))
    {
        pos++;
    }

    //  如果遇到真换行,这里不处理，而是交给换行处理机制处理
    if ('\n' == *pos)
    {
        cache->pos = pos;
    }

    return;
}


char* p_accept_key(char* s)
{
    char* pos = s;

retry:
    //  跳过所有的空白
    while (P_SPACE&cmap[*pos])
    {
        pos++;
        continue;
    }

    //  如果第一个非空白字符是注释（将这两类折合在一起计算，提高性能）
    if ((P_COMMENT|P_EOS)&cmap[*pos])
    {
        if (P_COMMENT&cmap[*pos])
        {
            //  跳过空白区域直到\n符号
            p_accept_comment()
            goto    retry;
        }

        
        //  检查一下是否真的到\0了，还是说只是缓冲区慢了
        goto    retry;
    }

    register char c = pos[0];
    
    read_key();

    
}

struct properties_t*    properties_new          ()
{

}


void                    properties_del          (struct properties_t* p)
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


