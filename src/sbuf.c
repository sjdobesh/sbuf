/*
 * samantha jane 2022
 * sbuf.c
 * dynamic string buffer
 *----------------------------------------------------------------------------*/

/**
 * define which strncpy to use
 **/
#include <asm-generic/errno-base.h>
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
int sbuf_is_full(sbuf s) {
  errno = 0;
  if (!s.buf) {
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
int sbuf_is_empty(sbuf s) {
  errno = 0;
  if (!s.buf) {
    errno = EPERM;
    return -1;
  }
  return (s.len == 0);
}

/**
 * search for a char
 *
 * @param s the sbuf to search
 * @param c the char to search for
 * @return index where char was found. -1 if error.
 */
int sbuf_search_char(sbuf s, char c) {
  unsigned int i;
  errno = 0;
  for (i = 0; i < s.len; i++) {
    if (s.buf[i] == c) {
      return i;
    }
  }
  errno = ENODATA;
  return -1;
}

/**
 * search for all instances of char
 *
 * @param s the sbuf to search
 * @param c the char to search for
 * @param rv return vector of indices
 * @param rc return vector count
 * @return an exit code, 0: found results, 1: no results, 2: memory error
 */
int sbuf_search_char_all(sbuf s, char c, int** rv, int* rc) {
  unsigned int i;
  *rc = 0;
  errno = 0;
  for (i = 0; i < s.len; i++) {
    if (s.buf[i] == c) {
      (*rc)++;
      *rv = realloc(*rv, *rc);
      if (!*rc) {
        errno = ENOMEM;
        return 2;
      }
      (*rv)[(*rc)-1] = i;
    }
  }
  /* getting nothing will also be an error */
  if (*rc == 0) {
    errno = ENODATA;
    return 1;
  }
  return 0;
}

/**
 * search for a substring
 *
 * @param s the sbuf to search
 * @param search the substring to search for
 * @return index where substring is located
 */
int sbuf_search_str(sbuf s, char* search) {
  char* result;
  errno = 0;
  result = strstr(s.buf, search);
  if (!result) {
    errno = ENODATA;
    return -1;
  }
  /* difference is the index */
  return (result - s.buf);
}

/**
 * search for all occurances of a substring
 *
 * @param s the sbuf to search
 * @param search the substring to search for
 * @param rv return vector of indices
 * @param rc return vector count
 * @return an exit code, 0: found results, 1: no results, 2: memory error
 */
int sbuf_search_str_all(sbuf s, char* search, int** rv, int* rc) {
  char* result;
  unsigned int index;
  unsigned int offset;
  *rc = 0;
  offset = 0;
  errno = 0;
  do {
    result = strstr(s.buf+offset, search);
    if (!result) {
      errno = ENODATA;
      return 1;
    }
    index = (result - s.buf);
    offset += index;
    (*rc)++;
    *rv = realloc(*rv, *rc);
    if (!*rc) {
      errno = ENOMEM;
      return 2;
    }
    (*rv)[(*rc)-1] = index;

  } while (offset < s.len);
  return 0;
}

/**
 * prints the buffer members.
 *
 * @param s the sbuf to print
 * @return void
 */
void sbuf_print(sbuf s) {
  printf("SBUF [\n");
  if (!s.buf) {
    printf("  null buffer\n");
  } else {
    printf("  %lu / %lu\n", s.len, s.capacity);
    printf("  buf: %s\n", s.buf);
  }
  printf("]\n");
}


/* ALLOCATION *---------------------------------------------------------------*/

/**
 * allocates a new buffer of a specific size.
 * note that allocation is actually 1 larger than specified.
 * buf[capacity + 1] is always string terminator to prevent strfunction errors.
 *
 * @param string the data to store in the buffer
 * @param capacity the max length of string the buffer can store
 * @return a newly allocated sbuf struct, empty on failure.
 */
sbuf sbuf_alloc_size(char* string, size_t capacity) {
  sbuf s;
  int len;
  errno = 0;
  s.buf = calloc(capacity + 1, 1); /* add room for '\0' */
  if (!s.buf) {
    errno = ENOMEM;
  } else {
    s.capacity = capacity;
    len = strnlen(string, capacity);
    strncpy(s.buf, string, len);
    s.len = len;
  }
  return s;
}

/**
 * allocates the smallest buffer possible.
 * note potentially unsafe due to strlen use.
 *
 * @param string the data to store in the buffer
 * @return a newly allocated sbuf struct, empty on failure.
 */
sbuf sbuf_alloc(char* string) {
  return sbuf_alloc_size(string, strlen(string));
}

/**
 * allocates the smallest buffer possible but
 * adds a dynamically expanding flag.
 *
 * @param string the data to store in the buffer
 * @return a newly allocated sbuf struct, empty on failure.
 */
sbuf sbuf_alloc_dynamic(char* string) {
  sbuf s = sbuf_alloc_size(string, strlen(string));
  s.dynamic = 1;
  return s;
}

/**
 * reallocates an existing buffer, possibly truncating its contents.
 *
 * @param s the sbuf to reallocate
 * @param new_capacity the new size to allocate the sbuf to
 * @return an exit code, setting errno on failure.
 */
int sbuf_realloc(sbuf* s, size_t new_capacity) {
  char* buf;
  errno = 0;
  if (!s) {
    errno = EPERM;
    return 1;
  }
  buf = realloc(s->buf, new_capacity + 1);
  if (!buf) {
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
 * frees a sbuf's internal pointer if it needs to be freed and sets it to `NULL`.
 * zeros out `.capacity`, `.len`, and unsets `.dyanmic`.
 * will not double free.
 *
 * @param s the sbuf to free
 * @return void
 */
void sbuf_free(sbuf* s) {
  if (s && s->buf) {
    free(s->buf);
    s->buf = NULL;
  }
  /* zero out a sbuf that was being allocated but failed */
  s->capacity = 0;
  s->len = 0;
  s->dynamic = 0;
}


/* FUNCTIONS *----------------------------------------------------------------*/

/**
 * appends a string to a sbuf.
 *
 * @param s the sbuf to append to
 * @param string the content to append
 * @return an exit code, setting errno on failure
 */
int sbuf_append_str(sbuf* s, char* string) {
  int space, len;
  errno = 0;
  if (!s || !s->buf) {
    errno = EPERM;
    return 1;
  }
  /* check length requirements. if dynamic check beyond limit */
  space = s->capacity - s->len;
  len = s->dynamic
    ? strlen(string)
    : strnlen(string, space);
  if (len > space) {
    /* if we are out of space, check if we can resize */
    if (s->dynamic) {
      sbuf_realloc(s, s->len + len - space);
    } else {
      errno = ENOMEM;
      return 1;
    }
  }
  strncat(s->buf, string, len);
  s->len += len;
  return 0;
}

/**
 * append a single char to the end of a buffer.
 * may expand a sbuf marked dynamic.
 *
 * @param s the sbuf to append to
 * @param c the char to append
 * @return an exit code, setting errno on failure
 */
int sbuf_append_char(sbuf* s, char c) {
  errno = 0;
  if (!s || !s->buf) {
    errno = EPERM;
    return 1;
  }
  if (sbuf_is_full(*s)) {
    if (! s->dynamic) {
      errno = ENOMEM;
      return 1;
    } else {
      sbuf_realloc(s, s->len + 1);
    }
  }
  s->buf[s->len] = c;
  s->len++;
  return 0;
}

/**
 * clear a sbuf's contents and reset its length.
 *
 * @param s the sbuf to clear
 * @return an exit code, setting errno on failure
 */
int sbuf_clear(sbuf* s){
  errno = 0;
  if (!s || !s->buf) {
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
sbuf sbuf_copy(sbuf s) {
  return sbuf_alloc_size(s.buf, s.capacity);
}

/**
 * shrink a sbuf to the size of its current contents.
 *
 * @param s the sbuf ptr to shrink
 * @return an exit code, setting errno on failure
 */
int sbuf_shrink(sbuf* s) {
  errno = 0;
  /* check if already at minimum size for contents */
  if (!s || s->len == s->capacity) {
    errno = EPERM;
    return 1;
  }
  return sbuf_realloc(s, s->len);
}


/* ACCESSORS *----------------------------------------------------------------*/

/**
 * returns the internal buffer ptr of a sbuf.
 *
 * @param s the sbuf to get string from
 * @return the char ptr to the sbuf's buffer (not a copy)
 */
char* sbuf_get_str(sbuf s) {
  return s.buf;
}

/**
 * clear and set the contents of the sbuf.
 *
 * @param s the sbuf ptr to clear and set
 * @param string the c string to store in s
 * @return an exit code, setting errno on failure
 */
int sbuf_set_str(sbuf* s, char* string) {
  errno = 0;
  if (!s || sbuf_clear(s)) {
    errno = EPERM;
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
char sbuf_get_index(sbuf s, size_t i) {
  if (!s.buf || i >= s.capacity) {
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
int sbuf_set_index(sbuf* s, size_t i, char c) {
  errno = 0;
  if (!s || !s->buf || i >= s->capacity) {
    errno = EPERM;
    return 1;
  } else {
    s->buf[i] = c;
    /* if its not a terminator and index exceeds length, update length */
    if (c != '\0' && (i + 1) > s->len) {
      s->len = i + 1;
    }
    return 0;
  }
}
