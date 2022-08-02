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

// allocate a buffer
buf new_buf(const char* string, size_t capacity) {
  buf b = {
    .buf = malloc(capacity),
    .len = 0,
    .capacity = capacity,
    .freed = 0
  };
  int len = strnlen(string,capacity);
  strncpy(b.buf, string, len);
  b.len = len;
  return b;
}

// reallocate a buffer size
void realloc_buf(buf* b, size_t new_capacity) {
  b->buf = realloc(b->buf, new_capacity);
  b->capacity = new_capacity;
  // contents possibly truncated
  if (new_capacity < b->len) {
    b->len = new_capacity;
  }

}

// try to free the buffer. sets errno on failure
void free_buf(buf* b) {
  if (b->buf != NULL) {
    free(b->buf);
  }
  b->buf = NULL;
  b->freed = 1;
}

// clear out a buffers contents
int clear_buf(buf* b){
  if (b->freed || b->buf == NULL) {
    return 1;
  }
  memset(b->buf, '\0', b->capacity);
  b->len = 0;
  return 0;
}

// append to a buffer
int append_buf(const char* string, buf* bp) {
  // cant write to a freed buffer
  if (bp->freed) {
    errno = 1;
    return 1;
  }
  // cant write to a full buffer
  if (is_full(*bp)) {
    errno = 12;
    return 1;
  }
  // check length requirements and copy maximum allowed
  int space = bp->capacity - bp->len;
  int len = strnlen(string, space);
  strncat(bp->buf, string, len);
  bp->len += len;
  return 0;
}

// check if buffer is full
int is_full(buf b) {
  return (b.len == b.capacity);
}

// check if buffer is empty
int is_empty(buf b) {
  return (b.len == 0);
}

// print the buffer contents
void print_buf(buf b) {
  printf("BUF [\n");
  if (b.freed) {
    printf("  freed buffer\n");
  } else if (b.buf == NULL) {
    printf("  null buffer\n");
  } else {
    printf("  %d / %lu\n", b.len, b.capacity);
    printf("  buf: %s\n", b.buf);
  }
  printf("]\n");
}

// get a buffer element but check bounds
// fails silently, returning '\0'
char get_buf_element(buf b, int i) {
  if (b.freed || b.buf == NULL || i >= b.capacity || i < 0) {
    return '\0';
  } else {
    return b.buf[i];
  }
}

// returns an exit code
int set_buf_element(buf* b, int i, char c) {
  if (b->freed || b->buf == NULL || i >= b->capacity || i < 0) {
    return 1;
  } else {
    b->buf[i] = c;
    return 0;
  }
}
