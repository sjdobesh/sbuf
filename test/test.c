/**
 * samantha jane
 * test.c
 * test file for buf module
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "sbuf.h"

#define VERBOSE 1

/* toggleable dbug print */
void dbug(const char* string) {
  if (VERBOSE) {
    printf("%s", string);
  }
}

/* behave */
int test_basic() {
  sbuf b;
  char element;

  dbug("\n\n**** BASIC TESTS ****\n");
  dbug("allocating...\n");
  b = new_sbuf_size("hello", 11);
  if (VERBOSE)
    sbuf_print(b);

  dbug("appending...\n");
  if (sbuf_append_str(&b, " world!\n")) {
    printf("appending failed\n");
  }
  if (VERBOSE)
    sbuf_print(b);

  dbug("is_full?...\n");
  if(!sbuf_is_full(b)) {
    dbug("error, should be full\n");
    return 1;
  }
  dbug("is_full :)\n");

  dbug("reallocing...\n");
  sbuf_realloc(&b, 12);
  dbug("shouldn't be full anymore...\n");
  if (VERBOSE)
    sbuf_print(b);
  if (sbuf_is_full(b)) {
    dbug("failed to realloc\n");
  }
  dbug("isn't full :)\n");

  dbug("poking...\n");
  if (sbuf_set_index(&b, 1, 'i')) {
    dbug("poking failed\n");
  }
  if (VERBOSE)
    sbuf_print(b);

  dbug("getting...\n");
  element = sbuf_get_index(b, 1);
  if (element == '\0') {
    dbug("getting failed\n");
  }
  if (VERBOSE)
    printf("b[1] : %c\n", element);

  dbug("clearing...\n");
  if (sbuf_clear(&b)) {
    dbug("failed to clear\n");
  }
  if (VERBOSE)
    sbuf_print(b);

  dbug("is_empty?...\n");
  if(!sbuf_is_empty(b)) {
    dbug("error, should be empty\n");
    return 1;
  }
  dbug("is_empty :)\n");

  dbug("freeing...\n");
  sbuf_free(&b);
  if (VERBOSE)
    sbuf_print(b);

  dbug("\n**** BASIC PASS ****\n");

  return 0;
}

/* test allocation */
int test_allocation() {

  int exit_code = 0;
  sbuf a, b;

  dbug("\n\n**** ALLOCATION TESTS ****\n");

  /* check that a standard allocation works as expected */
  dbug("test 1 - new_sbuf_size:\n");
  a = new_sbuf_size("Test test test!", 32);
  if (VERBOSE)
    sbuf_print(a);
  if (a.capacity != 32) {
    dbug("[ERROR] capacity\n");
    exit_code = 1;
  } else if (a.len != 15) {
    dbug("[ERROR] length\n");
    exit_code = 1;
  } else if (a.buf == NULL) {
    dbug("[ERROR] pointer\n");
    exit_code = 1;
  } else {
    dbug("test 1 pass\n");
  }

  /* check the new_sbuf method */
  dbug("test 2 - new_sbuf:\n");
  b = new_sbuf("minimum");
  if (VERBOSE)
    sbuf_print(b);
  if (b.capacity != 7) {
    dbug("[ERROR] capacity\n");
    exit_code = 1;
  } else if (b.len != 7) {
    dbug("[ERROR] length\n");
    exit_code = 1;
  }
  if (b.buf == NULL) {
    dbug("[ERROR] pointer\n");
    exit_code = 1;
  } else {
    dbug("test 2 pass\n");
  }

  return exit_code;
}

/* test index access */
int test_index_access() {
  sbuf a;
  dbug("\n\n**** INDEX TESTS ****\n");
  a = new_sbuf_size("", 1024);
  if (VERBOSE)
    sbuf_print(a);

  dbug("testing basic index set\n");
  sbuf_set_index(&a, 0, 'a');
  if (VERBOSE)
    sbuf_print(a);
  if (a.len != 1) {
    dbug("[ERROR] len\n");
    return 1;
  }
  if (a.buf[1] != '\0') {
    dbug("[ERROR] terminator\n");
    return 1;
  }

  dbug("setting a bit further in...\n");
  sbuf_set_index(&a, 2, 'c');
  if (VERBOSE)
    sbuf_print(a);

  dbug("filling in ...\n");
  sbuf_set_index(&a, 1, 'b');
  if (VERBOSE)
    sbuf_print(a);
  return 0;
}

/* test append */
int test_append() {
  sbuf a;
  dbug("\n\n**** APPEND TESTS ****\n");
  a = new_sbuf_size("", 64);
  while (!sbuf_is_full(a)) {
    sbuf_append_char(&a, 'a');
  }
  if (VERBOSE)
    sbuf_print(a);
  return 0;
}

/* test dynamic sbuf */
int test_dsbuf() {
  sbuf a;
  dbug("\n\n**** DYNAMIC TESTS ****\n");
  a= new_dsbuf("dynamic baby");
  sbuf_print(a);
  sbuf_append_str(&a, " B)\n");
  sbuf_print(a);

  return 0;
}

/* test copy */
int test_copy() {
  dbug("\n\n**** COPY TESTS ****\n");
  return 0;
}

/* test copy */
int test_free() {
  dbug("\n\n**** FREE TESTS ****\n");
  return 0;
}

int main() {
  /*
  printf("Basic:");
  printf("  - %s\n", (test_basic() ? "FAIL": "PASS"));
  */
  printf("Allocation:");
  printf("  - %s\n", (test_allocation() ? "FAIL": "PASS"));
  printf("Index:");
  printf("  - %s\n", (test_index_access() ? "FAIL": "PASS"));
  printf("Append:");
  printf("  - %s\n", (test_append() ? "FAIL": "PASS"));
  printf("Dynamic:");
  printf("  - %s\n", (test_dsbuf() ? "FAIL": "PASS"));
  return 0;
}
