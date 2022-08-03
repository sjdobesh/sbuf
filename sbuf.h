// samantha jane 2022
// sbuf.h
// dynamic string buffer
//------------------------------------------------------------------------------

#ifndef _SBUF_H_
#define _SBUF_H_

#include <stddef.h>

// a malloc'd char buffer
typedef struct sbuf {
  char* buf;       // the actual buffer
  size_t capacity; // size of buffer
  size_t len;      // length of contents
} sbuf;

//* ALLOCATION *//--------------------------------------------------------------

// allocate a new buffer
sbuf new_sbuf(char* string, size_t capacity);
// reallocate an existing buffer, possibly truncating its contents
int realloc_sbuf(sbuf* b, size_t new_capacity);
// free a buffers internal pointer
void free_sbuf(sbuf* b);

//* ACCESSORS *//---------------------------------------------------------------

// string accessors
char* get_sbuf(sbuf s);
int set_sbuf(sbuf* s, char* string);
// index accessors
char get_sbuf_index(sbuf s, size_t i);
int set_sbuf_index(sbuf* s, size_t i, char c);

//* FUNCTIONS *//---------------------------------------------------------------

// append to a buffer
int append_sbuf(char* string, sbuf* s);
// clear a buffers contents and reset its length
int clear_sbuf(sbuf* s);
// malloc an exact copy
sbuf copy_sbuf(sbuf);
// adjusts size to current contents
int shrink_sbuf(sbuf* s);

//* UTIL *//--------------------------------------------------------------------

// check if buffer is full
int is_full(sbuf s);
// check if buffer is empty
int is_empty(sbuf s);
// print the buffer contents
void print_sbuf(sbuf s);

#endif
