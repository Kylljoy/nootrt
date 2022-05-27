#include "fun.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>



/*
  NootRoot function library, contains definition for function PRORGAM OBJECTS
  !!NOT!! the actual functions used by the program
*/

typedef struct nrt_param_struct {
  void *value;
  unsigned char type : 2; //Type of the paramater. bit 0 { 0 = int, 1 = str}, bit 1 {0 = not function, 1 = function}
  // So 0 = int, 1 = str, 2 = int fun, 3 = str fun
  unsigned char size : 6; //Size of this parameter, if a string. Between (1-64)
  nrt_param *next; //Next parameter in this list
} nrt_param;



typedef struct nrt_fun_struct {
  nrt_param *params; //Points to the first input parameter of this function
  unsigned char type; //Defines the type of this function
} nrt_fun;

/*
 * Evaluates the function, returning a void value
 * It's up to YOU to remember how to decode it
 */

void *nrt_fun_evaluate(nrt_fun *function) { 
  // 1 : Add
  // 2 : Subtract
  // 3 : Multiply
  // 4 : Divide
  switch (function->type) {
    //Addition
    case 1:
      {
        int *out = malloc(sizeof(int));
        *out = 0;
        nrt_param *parameter = function->params;
        while (parameter) {
          if (parameter->type == 0) {
            //Add constant parameter
            *out += *((int *) parameter->value); 
          } else if (parameter->type == 2) {
            //Evaluate subfunctions
            int *function_value = (int *) nrt_fun_evaluate((nrt_fun *) parameter->value);
            *out += *function_value;
            free(function_value); 
          } else {
            //NOT AN INT!
            continue;
          }
          parameter = parameter->next;
        }
        return out;
     }  // End Addition
    //Subtraction
    case 2:
      {
        int *out = malloc(sizeof(int));
        *out = 0;
        nrt_param *parameter = function->params;
        // Set the first value
        if (parameter) {
          if (parameter->type == 0) {
            *out = *((int *) parameter->value);
          } else if (parameter->type == 2) { 
            int *function_value = (int *) nrt_fun_evaluate((nrt_fun *) parameter->value);
            *out = *function_value;
            free(function_value);
          }
        }
        parameter = parameter->next;
        //Now do the subtracting!
        while (parameter) {
          if (parameter->type == 0) {
            //Add constant parameter
            *out -= *((int *) parameter->value); 
          } else if (parameter->type == 2) {
            //Evaluate subfunctions
            int *function_value = (int *) nrt_fun_evaluate((nrt_fun *) parameter->value);
            *out -= *function_value;
            free(function_value); 
          } else {
            //NOT AN INT!
            continue;
          }
          parameter = parameter->next;
        }
        return out;
      } // End Subtraction
    default:
      return NULL;
  }
  return NULL;
} /* nrt_fun_evaluate() */

/*
 * Converts a given void ptr to an integer
 */

int ptr_to_int(void *ptr) {
  return *((int *) ptr);
} /* prt_to_int() */

/*
 * Converts a given void ptr to a string
 */

char *ptr_to_str(void *ptr) {
  return (char *) ptr;
} /* ptr_to_str() */
