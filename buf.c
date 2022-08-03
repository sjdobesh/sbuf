// samantha jane
// buf.c
// dynamic string buffer

// define which strncpy to use
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include "buf.h"

// allocate a new buffer
// if allocation fails, return an empty buffer with .capacity=0 and set errno
buf new_buf(char* string, size_t capacity) {
  buf b = {
    .buf = malloc(capacity + 1), // add room for '\0'
  };
  if (b.buf == NULL) {
    errno = ENOMEM;
  } else {
    b.capacity = capacity;
    int len = strnlen(string, capacity);
    strncpy(b.buf, string, len);
    b.buf[capacity] = '\0';
    b.len = len;
  }
  return b;
}

// reallocate an existing buffer, possibly truncating its contents
// returns an exit code and sets errno on failure
int realloc_buf(buf* b, size_t new_capacity) {
  char* buf = realloc(b->buf, new_capacity + 1);
  if (buf == NULL) {
    errno = ENOMEM;
    return 1;
  }
  b->buf = buf;
  b->capacity = new_capacity;
  b->buf[b->capacity] = '\0';
  if (new_capacity <= b->len) {
    b->len = new_capacity;
  }
  return 0;
}

// free a buffers internal pointer
// updates capacity and length of buffer
// will not double free
void free_buf(buf* b) {
  if (b->buf != NULL) {
    free(b->buf);
    b->buf = NULL;
  }
  b->capacity = b->len = 0;
}

// clear a buffers contents and reset its length
// returns an exit code and set errno on failure
int clear_buf(buf* b){
  if (b->buf == NULL) {
    errno = EPERM;
    return 1;
  }
  memset(b->buf, '\0', b->capacity);
  b->len = 0;
  return 0;
}

// append to a buffer
// returns an exit code and set errno on failure
int append_buf(char* string, buf* b) {
  // can't append to a null buffer
  if (b->buf == NULL) {
    errno = EPERM;
    return 1;
  }
  // cant append to a full buffer either
  if (is_full(*b)) {
    errno = ENOMEM;
    return 1;
  }
  // check length requirements and copy maximum allowed
  int space = b->capacity - b->len;
  int len = strnlen(string, space);
  strncat(b->buf, string, len);
  b->len += len;
  return 0;
}

// check if a buffer is full
// returns a -1 on error and sets errno
int is_full(buf b) {
  if (b.buf == NULL) {
    errno = EPERM;
    return -1;
  }
  return (b.len == b.capacity);
}

// check if a buffer is empty
// returns a -1 on error and sets errno
int is_empty(buf b) {
  if (b.buf == NULL) {
    errno = EPERM;
    return -1;
  }
  return (b.len == 0);
}

// print the buffer contents
void print_buf(buf b) {
  printf("BUF [\n");
  if (b.buf == NULL) {
    printf("  null buffer\n");
  } else {
    printf("  %lu / %lu\n", b.len, b.capacity);
    printf("  buf: %s\n", b.buf);
  }
  printf("]\n");
}

// get a buffer element within bounds
// returns '\0' on failure and sets errno
char get_buf_element(buf b, size_t i) {
  if (b.buf == NULL || i >= b.capacity || i < 0) {
    return '\0';
  } else {
    return b.buf[i];
  }
}

// set a buffer element within bounds
// returns an exit code and sets errno
int set_buf_element(buf* b, size_t i, char c) {
  if (b->buf == NULL || i >= b->capacity || i < 0) {
    return 1;
  } else {
    b->buf[i] = c;
    return 0;
  }
}
