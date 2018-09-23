#include "hash_map.h"

struct hash_bucket
{
    struct hash_node*   first;
};

struct hash_map
{
    struct hash_bucket* items[];
};


struct hash_map*    hash_map_new(int buckets_size); 
void                hash_map_del(struct hash_map* h); 
void                hash_map_put(struct hash_map* h, struct hash_node* n); 
void                hash_map_put(struct hash_map* h, struct hash_node* n); 
