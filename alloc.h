/*****************************************************
 * ROCKER ALLOCATOR HEADER
 * MIT License
 * Copyright (c) 2024 Paul Passeron
 *****************************************************/

#ifndef ALLOC_H
#define ALLOC_H

#include <stdlib.h>

typedef struct alloc_elem_t {
  void* ptr;
  int scope;
} alloc_elem_t;

typedef struct alloc_stack_t {
  alloc_elem_t* data;
  void** persistents;
  size_t capacity;
  size_t capacity_p;
  int length;
  int length_p;
  size_t scope;
} alloc_stack_t;

void init_stack(alloc_stack_t* alloc);
void kill_stack(alloc_stack_t alloc);

void new_scope(alloc_stack_t* alloc);
void end_scope(alloc_stack_t* alloc);

void* allocate(alloc_stack_t* alloc, size_t size);
void* allocate_persistent(alloc_stack_t* alloc, size_t size);

void init_compiler_stack(void);

void kill_compiler_stack(void);

void new_compiler_scope(void);

void end_compiler_scope(void);

void* allocate_compiler(size_t size);

void* allocate_compiler_persistent(size_t size);

#endif  // ALLOC_H