/*
 * samantha jane 2022
 * sbuf.c
 * dynamic string buffer
 *----------------------------------------------------------------------------*/

/**
 * define which strncpy to use
 **/
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "sbuf.h"


/* UTIL *---------------------------------------------------------------------*/

/**
 * checks if a buffer is full.
 *
 * @param  s the sbuf to check
 * @return a boolean, or a -1 on error and sets errno
 */
int is_full(sbuf s) {
  errno = 0;
  if (s.buf == NULL) {
    errno = EPERM;
    return -1;
  }
  return (s.len == s.capacity);
}

/**
 * checks if a buffer is empty.
 *
 * @param s the sbuf to check
 * @return a boolean, or a -1 on error and sets errno
 */
int is_empty(sbuf s) {
  errno = 0;
  if (s.buf == NULL) {
    errno = EPERM;
    return -1;
  }
  return (s.len == 0);
}

/**
 * prints the buffer members.
 *
 * @param s the sbuf to print
 * @return void
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
 * allocates a new buffer.
 * note that allocation is actually 1 larger than specified.
 * buf[capacity + 1] is always string terminator to prevent strfunction errors.
 *
 * @param string the data to store in the buffer
 * @param capacity the max length of string the buffer can store
 * @return a newly allocated sbuf struct, empty on failure.
 */
sbuf new_sbuf(char* string, size_t capacity) {
  sbuf s;
  int len;
  errno = 0;
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
 * allocates the smallest buffer possible.
 * note potentially unssge due to strlen use.
 *
 * @param string the data to store in the buffer
 * @return a newly allocated sbuf struct, empty on failure.
 */
sbuf new_sbuf_small(char* string) {
  int len;
  len = strlen(string);
  return new_sbuf(string, len);
}

/**
 * reallocates an existing buffer, possibly truncating its contents.
 *
 * @param s the sbuf to reallocate
 * @param new_capacity the new size to allocate the sbuf to
 * @return an exit code, setting errno on failure.
 */
int realloc_sbuf(sbuf* s, size_t new_capacity) {
  char* buf;
  errno = 0;
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
 * frees a sbuf's internal pointer.
 * updates `.capacity` and `len`
 * and will not double free.
 *
 * @param s the sbuf to free
 * @return void
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
 * appends a string to a sbuf.
 *
 * @param string the content to append
 * @param s the sbuf to append to
 * @return an exit code, setting errno on failure
 */
int append_sbuf(char* string, sbuf* s) {
  int space, len;
  errno = 0;
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
 * clear a sbuf's contents and reset its length.
 *
 * @param s the sbuf to clear
 * @return an exit code, setting errno on failure
 */
int clear_sbuf(sbuf* s){
  errno = 0;
  if (s->buf == NULL) {
    errno = EPERM;
    return 1;
  }
  memset(s->buf, '\0', s->capacity);
  s->len = 0;
  return 0;
}

/**
 * mallocs a duplicate sbuf.
 *
 * @param s the sbuf data to duplicate
 * @return a newly malloc'd sbuf
 */
sbuf copy_sbuf(sbuf s) {
  return new_sbuf(s.buf, s.capacity);
}

/**
 * shrink a sbuf to the size of its current contents.
 *
 * @param s the sbuf ptr to shrink
 * @return an exit code, setting errno on failure
 */
int shrink_sbuf(sbuf* s) {
  errno = 0;
  /* check if already at minimum size for contents */
  if (s->len == s->capacity) {
    errno = EPERM;
    return 1;
  }
  return realloc_sbuf(s, s->len);
}


/* ACCESSORS *----------------------------------------------------------------*/

/**
 * returns the internal buffer ptr of a sbuf.
 *
 * @param s the sbuf to get string from
 * @return the char ptr to the sbuf's buffer (not a copy)
 */
char* get_sbuf(sbuf s) {
  return s.buf;
}

/**
 * clear and set the contents of the sbuf.
 *
 * @param s the sbuf ptr to clear and set
 * @param string the c string to store in s
 * @return an exit code, setting errno on failure
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
 * get a sbuf element by index within bounds.
 *
 * @param s sbuf to get data from
 * @param i index to access
 * @return the char in s at index i. returns '\0' and sets errno on failure
 */
char get_sbuf_index(sbuf s, size_t i) {
  if (s.buf == NULL || i >= s.capacity) {
    return '\0';
  } else {
    return s.buf[i];
  }
}

/**
 * set a sbuffer element by index within bounds.
 *
 * @param s the sbuf ptr to set
 * @param i the index to set
 * @param c the char to set i to
 * @return an exit code, setting errno on failure
 **/
int set_sbuf_index(sbuf* s, size_t i, char c) {
  if (s->buf == NULL || i >= s->capacity) {
    return 1;
  } else {
    s->buf[i] = c;
    return 0;
  }
}
