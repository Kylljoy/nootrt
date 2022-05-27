#include <assert.h>
#include <stdlib.h>

/*
  NootRoot function library, contains definition for function PRORGAM OBJECTS
  !!NOT!! the actual functions used by the program
*/

/*
  All Functions return type 'void *' and accept an array of size 'void *',
*/

/*
  PLEASE, FOR THE LOVE OF GOD, NULL-TERMINATE YOUR STRINGS!!!!
*/


/*
  Linked-List object for denoting entity tag addresses
*/
typedef nrt_fun_ent_address_struct {
  nrt_fun_ent_address_struct *next;
  unsigned short next_address;
} nrt_fun_ent_address;


// Two-bit type indicating memory type (0: int, 1: str, 2: function, 3: entity tag address)
typedef nrt_fun_metadata_bitfield { 
  unsigned char type : 2;
} nrt_fun_metadata;



/*
  - fun_ptr: the nested function. Args are 'argv', 'argc', 'argt';
      argv is argument values, argc is argument counts, argt is argument type
  - argv is the values to pass to the function
  - argt is the type of values in argc
*/
typedef nrt_fun_mem_struct {
  void (*fun_ptr)(void *, nrt_fun_metadata *argt);
  void *argv;
  nrt_fun_metadata *argt;
} nrt_fun;



