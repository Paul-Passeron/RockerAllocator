#include "alloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define INIT_CAP_ALLOC_STACK 1024
void init_stack(alloc_stack_t* alloc) {
  alloc->capacity = INIT_CAP_ALLOC_STACK;
  alloc->capacity_p = INIT_CAP_ALLOC_STACK;
  alloc->data = malloc(sizeof(alloc_elem_t) * INIT_CAP_ALLOC_STACK);
  alloc->persistents = malloc(sizeof(void*) * INIT_CAP_ALLOC_STACK);
  alloc->length = 0;
  alloc->length_p = 0;
}

void kill_stack(alloc_stack_t alloc) {
  for (int i = 0; i < alloc.length; i++)
    free(alloc.data[i].ptr);
  for (int i = 0; i < alloc.length_p; i++)
    free(alloc.persistents[i]);
  free(alloc.data);
  free(alloc.persistents);
}

void push_stack(alloc_stack_t* alloc, void* ptr) {
  if (alloc->length >= (int)alloc->capacity) {
    alloc->capacity = alloc->capacity * 2;
    alloc->data = realloc(alloc->data, alloc->capacity * sizeof(alloc_elem_t));
  }
  alloc_elem_t pushed;
  pushed.ptr = ptr;
  pushed.scope = alloc->scope;
  alloc->data[alloc->length++] = pushed;
}

void push_persistent(alloc_stack_t* alloc, void* ptr) {
  if (alloc->length_p >= (int)alloc->capacity_p) {
    alloc->capacity_p = alloc->capacity_p * 2;
    alloc->persistents =
        realloc(alloc->persistents, alloc->capacity_p * sizeof(void*));
  }
  alloc->persistents[alloc->length_p++] = ptr;
}

void* pop_stack(alloc_stack_t* alloc) {
  if (alloc->length == 0)
    return NULL;
  return alloc->data[--alloc->length].ptr;
}

size_t get_top_scope(alloc_stack_t alloc) {
  if (alloc.length <= 0)
    return -1;
  return alloc.data[alloc.length - 1].scope;
}

void new_scope(alloc_stack_t* alloc) {
  alloc->scope++;
}
void end_scope(alloc_stack_t* alloc) {
  if (alloc->length == 0)
    return;
  while (get_top_scope(*alloc) >= alloc->scope && alloc->length > 0)
    free(pop_stack(alloc));
  if (alloc->scope >= 1)
    alloc->scope--;
}

void* allocate(alloc_stack_t* alloc, size_t size) {
  void* ptr = malloc(size);
  if (ptr == NULL) {
    printf("Could not allocate\n");
    exit(1);
  }
  push_stack(alloc, ptr);
  return ptr;
}

void* allocate_persistent(alloc_stack_t* alloc, size_t size) {
  void* ptr = malloc(size);
  if (ptr == NULL) {
    printf("Could not allocate persistent\n");
    exit(1);
  }
  memset(ptr, 0, size);
  push_persistent(alloc, ptr);
  return ptr;
}

void* reallocate(alloc_stack_t* alloc, void* ptr, size_t size) {
  void* res = realloc(ptr, size);
  if (res == NULL) {
    printf("Could not reallocate data !\n");
    exit(1);
  }
  for (int i = 0; i < alloc->length; i++) {
    void* test = alloc->data[i].ptr;
    if (test == ptr) {
      alloc->data[i].ptr = res;
      break;
    }
  }
  return res;
}

void* reallocate_persistent(alloc_stack_t* alloc, void* ptr, size_t size) {
  void* res = realloc(ptr, size);
  if (res == NULL) {
    printf("Could not reallocate data !\n");
    exit(1);
  }
  for (int i = 0; i < alloc->length_p; i++) {
    void* test = alloc->persistents[i];
    if (test == ptr) {
      alloc->persistents[i] = res;
      break;
    }
  }
  return res;
}

alloc_stack_t global;

void init_compiler_stack(void) {
  init_stack(&global);
}

void kill_compiler_stack(void) {
  kill_stack(global);
}

void new_compiler_scope(void) {
  new_scope(&global);
}

void end_compiler_scope(void) {
  end_scope(&global);
}

void* allocate_compiler(size_t size) {
  return allocate(&global, size);
}

void* allocate_compiler_persistent(size_t size) {
  return allocate_persistent(&global, size);
}

void* reallocate_compiler(void* ptr, size_t size) {
  return reallocate(&global, ptr, size);
}

void* reallocate_compiler_persistent(void* ptr, size_t size) {
  return reallocate_persistent(&global, ptr, size);
}