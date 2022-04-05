#include <assert.h>
#include <stdio.h>

#include "ent_tag.h"

/*
 * Unit test for entity tags
 */



int main() {
  nrt_ent_tag *root, *num, *num2, *str, *str2 = NULL;
  root = nrt_ent_new_int_tag();
  num = nrt_ent_new_int_tag();
  num2 = nrt_ent_new_int_tag();
  str = nrt_ent_new_str_tag(4);
  str2 = nrt_ent_new_str_tag(5);
  nrt_ent_tag_set_str(str, "Hello");
  nrt_ent_tag_set_str(str2, "World!");
  nrt_ent_tag_set_int(num, 1);
  nrt_ent_tag_set_int(root, 0);
  nrt_ent_tag_set_int(num2, 2);
  nrt_ent_tag_make_parent(root, num);
  nrt_ent_tag_make_parent(num, num2);
  nrt_ent_tag_make_parent(str, str2);
  nrt_ent_tag_make_sibling(num, str);
  nrt_ent_print_ent_tree(root, 0);
  nrt_ent_tag *world = nrt_ent_get_tag_by_address(root, "0.1.");
  assert(world);
  printf("\n\nRetrieving 'World': %s\n", nrt_ent_tag_get_str(world));
}
