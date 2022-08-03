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
} buf;

// allocate a new buffer
buf new_buf(const char* string, size_t capacity);
// reallocate an existing buffer, possibly truncating its contents
int realloc_buf(buf* b, size_t new_capacity);
// free a buffers internal pointer
void free_buf(buf* b);

// clear a buffers contents and reset its length
int clear_buf(buf* bp);
// append to a buffer
int append_buf(const char* string, buf* bp);

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
