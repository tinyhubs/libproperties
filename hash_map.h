#ifndef __hash_map_H_
#define __hash_map_H_


struct hash_node
{
    struct hash_node*   next;
    struct hash_node*   prev;
};

struct hash_map;

struct hash_map*    hash_map_new(int buckets_size); 
void                hash_map_del(struct hash_map* h); 
void                hash_map_put(struct hash_map* h, struct hash_node* n); 
void                hash_map_put(struct hash_map* h, struct hash_node* n); 


#endif//__hash_map_H_
