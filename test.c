#define ROCKER_ALLOC_IMPLEMENTATION
#include <stdio.h>
#include "alloc.h"

int main(void) {
  alloc_stack_t stack = {0};
  init_stack(&stack);
  for (int i = 0; i < 20; i++) {
    new_scope(&stack);
    for (int j = 0; j < 10; j++) {
      int* p = allocate(&stack, j * sizeof(int));
      for (int k = 0; k < j; k++) {
        p[k] = k;
      }
    }
    end_scope(&stack);
  }
  kill_stack(stack);
  printf("Hello, World !\n");

  return 0;
}