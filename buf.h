// samantha jane
// buf.h
// string buffer header

#ifndef _BUF_H_
#define _BUF_H_

#include "stddef.h"

// a malloc'd char buffer
typedef struct buf {
  char* buf;       // the actual buffer
  size_t capacity; // size of buffer
  int len;         // length of contents
  int freed;       // if using dynamic memory, freed flag
} buf;

// malloc and fill a buffer
buf new_buf(const char* string, size_t capacity);
// realloc buffer space and handle truncation
void realloc_buf(buf* b, size_t new_capacity);
// try to free the buffer
void free_buf(buf* b);

// append to the buffer contents
int append_buf(const char* string, buf* bp);
// clear out a buffers contents
int clear_buf(buf* bp);

// check if buffer is full
int is_full(buf b);
// check if buffer is empty
int is_empty(buf b);

// print the buffer contents
void print_buf(buf b);

// safe accessors
char get_buf_element(buf b, int i);
int set_buf_element(buf* b, int i, char c);

#endif
