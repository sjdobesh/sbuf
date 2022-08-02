// samantha jane
// test.c
// test file for buf module

#include <stdio.h>
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
  // check that we cant overflow upon creation
  buf b = new_buf("hiiiiiiiii", 2);
  if (VERBOSE)
    print_buf(b);
  // only 2 should've been copied
  if (b.len != 2) {
    return 1;
  }
  if ('\0' != get_buf_element(b, 100)) {
    dbug("error, should've failed invalid buffer access\n");
    return 1;
  }
  if ('\0' != get_buf_element(b, -100)) {
    dbug("error, should've failed invalid buffer access\n");
    return 1;
  }
  return 0;
}

int main() {
  // intended usage, just uses each function once
  printf("Basic tests: %s\n\n", (test_basic() ? "FAIL": "PASS"));
  printf("Error tests: %s\n", (test_errors() ? "FAIL": "PASS"));
}
