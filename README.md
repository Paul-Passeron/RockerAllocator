# Rocker Allocator in C

Simple stb-style single header libc malloc wrapper for transpiled langage 'Rocker'.

Keeps track of allocated pointers and has a scope system which handles the freeing of memory

If you only care about the interface, you can just include the file as is but if you want to include the function definitions as well, you must define ```ROCKER_ALLOC_IMPLEMENTATION``` before including the header.


example:
```C
#define ROCKER_ALLOC_IMPLEMENTATION
#include "alloc.h"
int main(void){
  alloc_stack_t stack = {0};
  init_stack(&stack);
  new_scope(&stack);
  int *array1 = allocate(&stack, 15 * sizeof(int));
  int *array2 = allocate(&stack, 13 * sizeof(int));
  new_scope(&stack);
  int *array3 = allocate(&stack, 24 * sizeof(int));
  int *array4 = allocate(&stack, 20 * sizeof(int));
  int *array5 = allocate_persistent(&stack, 5 * sizeof(int));
  end_scope(&stack);// frees array3 and array4
  end_scope(&stack);// frees array1 and array2
  kill_stack(stack); // frees array5
}
```

The user cannot manually free the memory.

Memory is freed only when killing the stack and ending a scope.

You still can allocate global memory with the function ```allocate_persistent```.
This memory will only be freed when the allocator stack is killed

If only this system is used for dynamic memory management, memory leaks and double frees are not possible (and the stack is not modified by anything else but the available functions in the header)
