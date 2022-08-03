// samantha jane
// test.c
// test file for buf module

#include <stdio.h>
#include <string.h>
#include "buf.h"

#define VERBOSE 0

void dbug(const char* string) {
  if (VERBOSE) {
    printf("%s", string);
  }
}

// behave
int test_basic() {

  dbug("\nStarting Basic Tests...\n");

  dbug("\n    ****    \n\n");
  dbug("allocating...\n");
  buf b = new_buf("hello", 11);
  if (VERBOSE)
    print_buf(b);

  dbug("\n    ****    \n\n");
  dbug("appending...\n");
  if (append_buf(" world!\n", &b)) {
    printf("appending failed\n");
  }
  if (VERBOSE)
    print_buf(b);

  dbug("\n    ****    \n\n");
  dbug("is_full?...\n");
  if(!is_full(b)) {
    dbug("error, should be full\n");
    return 1;
  }
  dbug("is_full :)\n");

  dbug("\n    ****    \n\n");
  dbug("reallocing...\n");
  realloc_buf(&b, 12);
  dbug("shouldn't be full anymore...\n");
  if (VERBOSE)
    print_buf(b);
  if (is_full(b)) {
    dbug("failed to realloc\n");
  }
  dbug("isn't full :)\n");

  dbug("\n    ****    \n\n");
  dbug("poking...\n");
  if (set_buf_element(&b, 1, 'i')) {
    dbug("poking failed\n");
  }
  if (VERBOSE)
    print_buf(b);

  dbug("\n    ****    \n\n");
  dbug("getting...\n");
  char element = get_buf_element(b, 1);
  if (element == '\0') {
    dbug("getting failed\n");
  }
  if (VERBOSE)
    printf("b[1] : %c\n", element);

  dbug("\n    ****    \n\n");
  dbug("clearing...\n");
  if (clear_buf(&b)) {
    dbug("failed to clear\n");
  }
  if (VERBOSE)
    print_buf(b);

  dbug("\n    ****    \n\n");
  dbug("is_empty?...\n");
  if(!is_empty(b)) {
    dbug("error, should be empty\n");
    return 1;
  }
  dbug("is_empty :)\n");

  dbug("\n    ****    \n\n");
  dbug("freeing...\n");
  free_buf(&b);
  if (VERBOSE)
    print_buf(b);

  return 0;
}

// try to do stuff you shouldnt
int test_errors() {
  dbug("\nStarting Error Tests...\n");

  dbug("\n    ****    \n\n");
  dbug("allocating with too little...\n");
  // check that we cant overflow upon creation
  int allocate = 3;
  buf b = new_buf("hiiiiiiiii", allocate);
  if (VERBOSE)
    print_buf(b);
  // only 2 should've been copied
  if (b.len != allocate || strlen(b.buf) != allocate) {
    dbug("error allocating\n");
    return 1;
  }
  dbug("pass\n");

  dbug("\n    ****    \n\n");
  dbug("reallocate to truncate...\n");
  // realloc to truncate
  int reallocate = 2;
  realloc_buf(&b, reallocate);
  if (VERBOSE)
    print_buf(b);
  if (b.len != reallocate || strlen(b.buf) != reallocate) {
    dbug("reallocate error\n");
    return 1;
  }
  dbug("pass\n");

  // try appending two bufs
  dbug("\n    ****    \n\n");
  dbug("append buf to buf: C->A\n");
  buf a = new_buf("hello ", 12);
  buf c = new_buf("world!", 12);
  dbug("A:\n");
  if (VERBOSE)
    print_buf(a);
  dbug("C:\n");
  if (VERBOSE)
    print_buf(c);
  append_buf(c.buf, &a);
  int sum = 12;
  if (VERBOSE)
    print_buf(a);
  if (a.len != sum || strlen(a.buf) != sum) {
    dbug("append error\n");
    return 1;
  }
  dbug("pass\n");


  dbug("\n    ****    \n\n");
  dbug("bad indicies\n");
  if ('\0' != get_buf_element(b, 100)) {
    dbug("error, should've failed invalid buffer access\n");
    return 1;
  }
  if ('\0' != get_buf_element(b, -100)) {
    dbug("error, should've failed invalid buffer access\n");
    return 1;
  }
  return 0;
  dbug("pass\n");
}

int main() {
  // intended usage, just uses each function once
  printf("Basic tests:");
  printf("%s\n", (test_basic() ? "FAIL": "PASS"));
  printf("Error tests:");
  printf("%s\n", (test_errors() ? "FAIL": "PASS"));
}
