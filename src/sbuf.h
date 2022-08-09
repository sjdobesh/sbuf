/*
 * samantha jane 2022
 * sbuf.h
 * dynamic string buffer
 *----------------------------------------------------------------------------*/

#ifndef _SBUF_H_
#define _SBUF_H_

#include <stddef.h>

/**
 * a malloc'd string buffer. tracks capacity and length of a dynamic ptr.
 **/
typedef struct sbuf {
  /** the actual char buffer. */
  char* buf;
  /** malloc size of buffer. */
  size_t capacity;
  /** length of contents. */
  size_t len;
} sbuf;

/* UTIL *---------------------------------------------------------------------*/

/* check if buffer is full */
int sbuf_is_full(sbuf s);
/* check if buffer is empty */
int sbuf_is_empty(sbuf s);
/* print the buffer contents */
void sbuf_print(sbuf s);

/* ALLOCATION *---------------------------------------------------------------*/

/* allocate a buffer of the minimum required space for the string */
sbuf new_sbuf(char* string);
/* allocate a new buffer with a user defined caacity*/
sbuf new_sbuf_size(char* string, size_t capacity);
/* reallocate an existing buffer, possibly truncating its contents */
int sbuf_realloc(sbuf* b, size_t new_capacity);
/* free a buffers internal pointer */
void sbuf_free(sbuf* b);

/* FUNCTIONS *----------------------------------------------------------------*/

/* append a string to a buffer */
int sbuf_append_str(sbuf* s, char* string);
/* append a single char to a buffer */
int sbuf_append_char(sbuf* s, char c);
/* clear a buffers contents and reset its length */
int sbuf_clear(sbuf* s);
/* malloc an exact copy */
sbuf sbuf_copy(sbuf);
/* adjusts size to current contents */
int sbuf_shrink(sbuf* s);

/* ACCESSORS *----------------------------------------------------------------*/

/* string accessors */
char* sbuf_get_str(sbuf s);
int sbuf_set_str(sbuf* s, char* string);
/* index accessors */
char sbuf_get_index(sbuf s, size_t i);
int sbuf_set_index(sbuf* s, size_t i, char c);

#endif