#include "strstore.h"


struct strblock_t
{

};

struct strstore_t
{
    int                 blocks_cap;
    int                 blocks_len;
    struct strblock_t*  blocks[];
};


struct strstore_t*  strstore_new(int32_t init_cap)
{
    strstore_t
}

void                strstore_del(struct strstore_t* s)
{

}

struct str_t*       strstore_save(struct strstore_t* s, char* str, char* end)
{

}

struct str_t*       strstore_find(struct strstore_t* s, char* str, char* end)
{

}
