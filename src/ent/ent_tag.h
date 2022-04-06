#ifndef NRT_MEM_MANAGER
#define NRT_MEM_MANAGER

/*
    IN THIS FILE:
    - nrt_ent_tag:
      - nrt_ent_tag *child
      - nrt_ent_tag *next
      - void *value
      - 1 bit type (0 int, 1 string)
      - 7 bit size
    - nrt_ent_archetype:
      - As above, but seperate type for storing the template of an entity
    - void nrt_free_ent_tag(nrt_ent_tag *tag)
    - nrt_ent_tag *nrt_new_ent_from_archetype(nrt_ent_archetype *archetype)
    - nrt_ent_tag *nrt_new_ent_from_template(FILE *stream)
    - int nrt_ent_tag_get_int(nrt_ent_tag *tag)
    - char *nrt_ent_tag_get_str(nrt_ent_tag *tag) 
    - void nrt_ent_tag_set_int(nrt_ent_tag *tag, int val)
    - void nrt_ent_tag_set_str(nrt_ent_tag *tag, char *str)
    - nrt_ent_tag *nrt_ent_new_int_tag() 
    - nrt_ent_tag *nrt_ent_new_str_tag(unsigned char size)
    - nrt_ent_tag *nrt_ent_get_tag_by_address(nrt_ent_tag *root, const char *address)
    - void nrt_ent_tag_make_parent(nrt_ent_tag *parent, nrt_ent_tag *child)
    - void nrt_ent_tag_make_sibling(nrt_ent_tag *older, nrt_ent_tag *younger)
    - void nrt_ent_print_ent_tree(nrt_ent_tag *tree, int level)
*/


struct nrt_ent_tag_struct;
typedef struct nrt_ent_tag_struct nrt_ent_tag;
typedef nrt_ent_tag nrt_ent_archetype;

void nrt_free_ent_tag(nrt_ent_tag *tag);
nrt_ent_tag *nrt_ent_new_ent_from_archetype(nrt_ent_archetype *archetype);
nrt_ent_tag *nrt_ent_new_ent_from_template(FILE *stream);
int nrt_ent_tag_get_int(nrt_ent_tag *tag);
char *nrt_ent_tag_get_str(nrt_ent_tag *tag);
void nrt_ent_tag_set_int(nrt_ent_tag *tag, int val);
void nrt_ent_tag_set_str(nrt_ent_tag *tag, char *str);
nrt_ent_tag *nrt_ent_new_int_tag();
nrt_ent_tag *nrt_ent_new_str_tag(unsigned char size);
nrt_ent_tag *nrt_ent_get_tag_by_address(nrt_ent_tag *root, const char *address);
void nrt_ent_tag_make_parent(nrt_ent_tag *parent, nrt_ent_tag *child);
void nrt_ent_tag_make_sibling(nrt_ent_tag *older, nrt_ent_tag *younger);
void nrt_ent_print_ent_tree(nrt_ent_tag *tree, int level);


#endif
