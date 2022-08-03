/**
 * samantha jane 2022
 * sbuf.c
 * dynamic string buffer
 *----------------------------------------------------------------------------*/

/* define which strncpy to use */
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "sbuf.h"


/* UTIL *---------------------------------------------------------------------*/

/**
 * check if a buffer is full
 * returns a -1 on error and sets errno
 */
int is_full(sbuf s) {
  if (s.buf == NULL) {
    errno = EPERM;
    return -1;
  }
  return (s.len == s.capacity);
}

/**
 * check if a buffer is empty
 * returns a -1 on error and sets errno
 */
int is_empty(sbuf s) {
  if (s.buf == NULL) {
    errno = EPERM;
    return -1;
  }
  return (s.len == 0);
}

/**
 * print the buffer contents
 */
void print_sbuf(sbuf s) {
  printf("SBUF [\n");
  if (s.buf == NULL) {
    printf("  null buffer\n");
  } else {
    printf("  %lu / %lu\n", s.len, s.capacity);
    printf("  buf: %s\n", s.buf);
  }
  printf("]\n");
}


/* ALLOCATION *---------------------------------------------------------------*/

/**
 * allocate a new buffer
 * if allocation fails, return an empty buffer with .capacity=0 and set errno
 * note that allocation is actually 1 larger than specified
 * buf[capacity + 1] is always string terminator to prevent strfunction errors
 */
sbuf new_sbuf(char* string, size_t capacity) {
  sbuf s;
  int len;
  s.buf = malloc(capacity + 1); /* add room for '\0' */
  if (s.buf == NULL) {
    errno = ENOMEM;
  } else {
    s.capacity = capacity;
    len = strnlen(string, capacity);
    strncpy(s.buf, string, len);
    s.buf[capacity] = '\0';
    s.len = len;
  }
  return s;
}

/**
 * allocate the smallest buffer possible
 */
sbuf new_sbuf_small(char* string) {
  int len;
  len = strlen(string);
  return new_sbuf(string, len);
}

/**
 * reallocate an existing buffer, possibly truncating its contents
 * returns an exit code and sets errno on failure
 */
int realloc_sbuf(sbuf* s, size_t new_capacity) {
  char* buf;
  buf = realloc(s->buf, new_capacity + 1);
  if (buf == NULL) {
    errno = ENOMEM;
    return 1;
  }
  s->buf = buf;
  s->capacity = new_capacity;
  s->buf[s->capacity] = '\0';
  if (new_capacity <= s->len) {
    s->len = new_capacity;
  }
  return 0;
}

/**
 * free a buffers internal pointer
 * updates capacity and length of buffer
 * will not double free
 */
void free_sbuf(sbuf* s) {
  if (s->buf != NULL) {
    free(s->buf);
    s->buf = NULL;
  }
  s->capacity = 0;
  s->len = 0;
}


/* FUNCTIONS *----------------------------------------------------------------*/

/**
 * append to a buffer
 * returns an exit code and set errno on failure
 */
int append_sbuf(char* string, sbuf* s) {
  int space, len;
  if (s->buf == NULL) {
    errno = EPERM;
    return 1;
  }
  if (is_full(*s)) {
    errno = ENOMEM;
    return 1;
  }
  /* check length requirements and copy maximum allowed */
  space = s->capacity - s->len;
  len = strnlen(string, space);
  strncat(s->buf, string, len);
  s->len += len;
  return 0;
}

/**
 * clear a buffers contents and reset its length
 * returns an exit code and set errno on failure
 */
int clear_sbuf(sbuf* s){
  if (s->buf == NULL) {
    errno = EPERM;
    return 1;
  }
  memset(s->buf, '\0', s->capacity);
  s->len = 0;
  return 0;
}

/**
 * mallocs a duplicate sbuf
 */
sbuf copy_sbuf(sbuf s) {
  return new_sbuf(s.buf, s.capacity);
}

/**
 * shrink a buffer to the size of its current contents
 */
int shrink_sbuf(sbuf* s) {
  /* check if already at minimum size for contents */
  if (s->len == s->capacity) {
    errno = EPERM;
    return 1;
  }
  return realloc_sbuf(s, s->len);
}


/* ACCESSORS *----------------------------------------------------------------*/

/**
 * return the buffer pointer (not a copy)
 */
char* get_sbuf(sbuf s) {
  return s.buf;
}

/**
 * clear and set the contents of the buffer
 * returns an exit code
 */
int set_sbuf(sbuf* s, char* string) {
  /* clear handles error checking */
  if (clear_sbuf(s)) {
    return 1;
  }
  s->len = strnlen(string, s->capacity);
  strncpy(s->buf, string, s->len);
  return 0;
}

/**
 * get a buffer element within bounds
 * returns '\0' on failure and sets errno
 */
char get_sbuf_index(sbuf s, size_t i) {
  if (s.buf == NULL || i >= s.capacity) {
    return '\0';
  } else {
    return s.buf[i];
  }
}

/**
 * set a buffer element within bounds
 * returns an exit code and sets errno
 */
int set_sbuf_index(sbuf* s, size_t i, char c) {
  if (s->buf == NULL || i >= s->capacity) {
    return 1;
  } else {
    s->buf[i] = c;
    return 0;
  }
}
