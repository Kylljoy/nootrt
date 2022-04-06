#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ent_tag.h"

#define INTERNAL_TAG_LENGTH (12)
#define MAX_TEMPLATE_NUM_LENGTH (3)

/*
  Entity Tags
  This file *should* contain functions for the following:
    - Entity Tag Definition
    - Entity Tag Allocation
    - Entity Tag Management
    - Entity Tag Access
    - Entity Tag Deallocation
    - Entity Tag Construction from Template File
  God Help Me
*/

/*
    IN THIS FILE:
    - nrt_ent_tag:
      - nrt_ent_tag *child
      - nrt_ent_tag *next
      - void *value
      - 1 bit type (0 int, 1 string)
      - 7 bit size (0, 127) => (1, 128) [+null-terminator]
    - nrt_ent_archetype:
      - As above, but seperate type for storing the template of an entity
    - void nrt_free_ent_tag(nrt_ent_tag *tag)
    - nrt_ent_tag *nrt_ent_new_ent_from_archetype(nrt_ent_archetype *archetype)
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

/*
 * Definition of a single entity tag; Tags are linked into an entity tree
 */
typedef struct nrt_ent_tag_struct {
  struct nrt_ent_tag_struct *child; //Child node
  struct nrt_ent_tag_struct *next; //Adjacent node
  void *value; //Value of this tag
  unsigned char type : 1; //Type of the value (0 means int, 1 is string)
  unsigned char size : 7; //Size of the value, between (1, 128)
} nrt_ent_tag;

typedef nrt_ent_tag nrt_ent_archetype;

/*
 * Recursively frees the given entity tag
 */

void nrt_free_ent_tag(nrt_ent_tag *tag) {
  if ((tag->child) || (tag->next)) {
    //Free the children!
    //...or the siblings, but that's not a pun
    if (tag->child) {
      nrt_free_ent_tag(tag->child);
    }
    if (tag->next) {
      nrt_free_ent_tag(tag->next);
    }
  }
  free(tag->value);
  free(tag);
} /* nrt_free_ent_tag() */

/*
 * Constructs an empty entity based on the given stream
 * Okay, friend. Look here. I'm trusting you:
 * This function has very little in the way of syntax validation
 * for the lengths of the strings. Just don't be an idiot and we'll
 * be fine;
 */

nrt_ent_tag *nrt_ent_new_ent_from_template(FILE *stream) {
  /*
  How a template file is structured:
  INT
  INT
  STR 012
  INT:
    STR 045;
  */
  if (stream == NULL || feof(stream)) {
    return NULL;
  }
  char token[9] = {0};
  nrt_ent_tag *current = NULL;
  nrt_ent_tag *head = NULL;
  while (!feof(stream)) {

    //Get rid of preceeding whitespace
    fscanf(stream, "%*[ \t\n]");
    int matches = fscanf(stream, "%8[^\n]\n", token);
    if ((matches < 1) && !feof(stream)) {
      printf("ENT_TAG : MALFORMATTED TEMPLATE");
      return head;
    }
    if (feof(stream)) {
      return head;
    }
    // 'INT' code
    if ((token[0] == 'I') && (token[1] == 'N') && (token[2] == 'T')) {
      if (current) {
        current->next = nrt_ent_new_int_tag();
        current = current->next;
      } else {
        current = nrt_ent_new_int_tag();
        head = current;
      }
      if (token[3] == ':') {
        current->child = nrt_ent_new_ent_from_template(stream);
      } else if (token[3] == ';') {
        return head;
      }

    // 'STR' code
    } else if ((token[0] == 'S') && (token[1] == 'T') && (token[2] == 'R')) {
      int length = 0;
      for (int i = 4; i < 7; i++) {
        if (token[i] < 48 || token[i] > 57) {
          return head;
        }
        length = (length * 10) + (token[i] - 48);
      }
      if (current) {
        current->next = nrt_ent_new_str_tag(length);
        current = current->next;
      } else {
        current = nrt_ent_new_str_tag(length);
        head = current;
      }
      if (token[7] == ':') {
        current->child = nrt_ent_new_ent_from_template(stream);
      } else if (token[7] == ';') {
        return head;
      }
    // Single Semicolon
    } else if (token[0] == ';') {
      return head;
    } else {
      printf("ENT_TAG : MALFORMATTED TEMPLATE");
      return head;
    } 
  }
  return head;
} /* nrt_new_ent_from_template() */


/*
 * Takes a non-populated 'archetype' entity and creates a clone
 */
nrt_ent_tag *nrt_ent_new_ent_from_archetype(nrt_ent_archetype *archetype) {
  if (archetype == NULL) {
    return NULL;
  }
  //Make a new tag
  nrt_ent_tag *new_tag = malloc(sizeof(nrt_ent_tag));
  assert(new_tag);
  if (archetype->type) {
    new_tag->value = malloc(sizeof(char) * (archetype->size + 2));
    new_tag->size = archetype->size;
  } else {
    new_tag->value = malloc(sizeof(int));
  }
  assert(new_tag->value);
  new_tag->type = archetype->type;
  new_tag->next = nrt_ent_new_ent_from_archetype(archetype->next);
  new_tag->child = nrt_ent_new_ent_from_archetype(archetype->child);
  return new_tag;
} /* nrt_new_ent_from_archetype() */

/*
 * Returns the type of a tag
 */

unsigned char nrt_ent_tag_get_type(nrt_ent_tag *tag) {
  return tag->type;
} /* nrt_ent_tag_get_type() */

/*
 * Takes the integer value from a tag
 */

int nrt_ent_tag_get_int(nrt_ent_tag *tag) {
  if (tag->type) {
    return 0;
  }
  return * (int *) tag->value;
} /* nrt_ent_tag_get_int() */

/*
 * Takes the string value from a tag
 */
char *nrt_ent_tag_get_str(nrt_ent_tag *tag) {
  if (tag->type) {
    return (char *) tag->value;
  }
  return NULL;
} /* nrt_ent_tag_get_str() */

/*
 * Sets the value of the given tag to the given string
 * Does not do anything if the types are incompatible
 */

void nrt_ent_tag_set_str(nrt_ent_tag *tag, char *str) {
  if (tag->type) {
    strcpy((char *) tag->value, str);
    ((char *) tag->value)[tag->size + 1] = '\0'; //Null-terminating just in case
  }
} /* nrt_ent_tag_set_str() */
/*
 * Sets the value of the given tag to the given integer
 * Does not do anything if the types are incompatible
 */

void nrt_ent_tag_set_int(nrt_ent_tag *tag, int val) {
  if (!tag->type) {
    *((int *) tag->value) = val;
  }
} /* nrt_ent_tag_set_str() */

/*
 * Generates an unlinked tag with the given size
 */

nrt_ent_tag *nrt_ent_new_str_tag(unsigned char size) {
  if (size > 127) {
    return NULL;
  }
  nrt_ent_tag *new_tag = malloc(sizeof(nrt_ent_tag));
  assert(new_tag);
  new_tag->child = NULL;
  new_tag->next = NULL;
  new_tag->size = size;
  new_tag->type = 1;
  new_tag->value = malloc(sizeof(char) * (size+2));
  assert(new_tag->value);
  ((char *) new_tag->value)[0] = '\0';
  return new_tag;
} /* nrt_ent_new_str_tag() */


/*
 * Generates an unlinked tag with the given ID
 */

nrt_ent_tag *nrt_ent_new_int_tag() {
  nrt_ent_tag *new_tag = malloc(sizeof(nrt_ent_tag));
  assert(new_tag);
  new_tag->child = NULL;
  new_tag->next = NULL;
  new_tag->size = 2;
  new_tag->type = 0;
  new_tag->value = malloc(sizeof(int));
  assert(new_tag->value);
  *((int *) new_tag->value) = 0;
  return new_tag;
} /* nrt_ent_new_int_tag() */




/*
 * Accesses the entity tag at the given numerical address
 * i.e 0.1.3.4 for the fourth child of the third child of the first child of the root tag
 */

nrt_ent_tag *nrt_ent_get_tag_by_address(nrt_ent_tag *root, const char *address) {
  nrt_ent_tag *current = root;
  int index_value = 0;
  do { 
    if (*address == 46) {
      //This index is complete
        for (int i = 0; i < index_value; i++) {
        current = current->next;
        if (current == NULL) {
          //Specified address does not exist
          printf("ENT_TAG : ADDRESS EXCEEDS TREE BOUNDS\n");
          return NULL;
        }
      }
      current = current->child;
      if (current == NULL) {
        printf("ENT_TAG : CHILD DOES NOT EXIST\n");
        //Specified address does not exist
        return NULL;
      }
      index_value = 0;
    } else {
      if (*address < 48 || *address > 57) {
        printf("ENT_TAG : MALFORMATTED ADDRESS (CODE '%d')\n", *address);
        //Malformatted Address
        return NULL;
      }
      index_value = (index_value * 10) + (*address - 48);
    }
  } while (*(++address) != '\0'); 
  for (int i = 0; i < index_value; i++) {
    current = current->next;
    if (current == NULL) {
      //Specified address does not exist
      printf("ENT_TAG : ADDRESS EXCEEDS TREE BOUNDS\n");
      return NULL;
    }
  }
  return current;
} /* nrt_ent_get_tag_by_address() */


/*
 * Joins two tags together; the former becomes the parent of the latter
 * Disconnects nodes if in danger of creating an infinite circle
 */

void nrt_ent_tag_make_parent(nrt_ent_tag *parent, nrt_ent_tag *child) {
  if (child->child == parent) {
    child->child = NULL;
  }
  parent->child = child;
} /* nrt_ent_make_parent() */

/*
 * Joins two tags together; the former becomes the 'older' sibling of the latter
 * Disconnects nodes if in danger of creating an infinite circle
 */

void nrt_ent_tag_make_sibling(nrt_ent_tag *older, nrt_ent_tag *younger) {
  if (younger->next == older) {
    younger->next = NULL;
  }
  older->next = younger;
} /* nrt_ent_make_sibling() */

/*
 * Debugging function, displays an entity tree
 */

void nrt_ent_print_ent_tree(nrt_ent_tag *tree, int level) {
  nrt_ent_tag *current = tree;
  int index = 0;
  while (current) {
    for (int i = 0; i < level; i++) {
      printf("|");
    }
    if (current->type) {
      printf("%d : %s\n", index, nrt_ent_tag_get_str(current));
    } else {
      printf("%d : %d\n", index, nrt_ent_tag_get_int(current));
    }
    if (current->child != NULL) {
      nrt_ent_print_ent_tree(current->child, level+1);
    }
    index++;
    current = current->next;
  }
  for (int i = 0; i < (level - 1); i++) {
    printf("|");
  }
  printf("\n");
} /* nrt_ent_print_ent_tree() */
