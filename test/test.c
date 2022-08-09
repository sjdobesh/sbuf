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
    print_sbuf(b);

  dbug("appending...\n");
  if (append_sbuf(" world!\n", &b)) {
    printf("appending failed\n");
  }
  if (VERBOSE)
    print_sbuf(b);

  dbug("is_full?...\n");
  if(!is_full(b)) {
    dbug("error, should be full\n");
    return 1;
  }
  dbug("is_full :)\n");

  dbug("reallocing...\n");
  realloc_sbuf(&b, 12);
  dbug("shouldn't be full anymore...\n");
  if (VERBOSE)
    print_sbuf(b);
  if (is_full(b)) {
    dbug("failed to realloc\n");
  }
  dbug("isn't full :)\n");

  dbug("poking...\n");
  if (set_sbuf_index(&b, 1, 'i')) {
    dbug("poking failed\n");
  }
  if (VERBOSE)
    print_sbuf(b);

  dbug("getting...\n");
  element = get_sbuf_index(b, 1);
  if (element == '\0') {
    dbug("getting failed\n");
  }
  if (VERBOSE)
    printf("b[1] : %c\n", element);

  dbug("clearing...\n");
  if (clear_sbuf(&b)) {
    dbug("failed to clear\n");
  }
  if (VERBOSE)
    print_sbuf(b);

  dbug("is_empty?...\n");
  if(!is_empty(b)) {
    dbug("error, should be empty\n");
    return 1;
  }
  dbug("is_empty :)\n");

  dbug("freeing...\n");
  free_sbuf(&b);
  if (VERBOSE)
    print_sbuf(b);

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
    print_sbuf(a);
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
    print_sbuf(b);
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
    print_sbuf(a);

  dbug("testing basic index set\n");
  set_sbuf_index(&a, 0, 'a');
  if (VERBOSE)
    print_sbuf(a);
  if (a.len != 1) {
    dbug("[ERROR] len\n");
    return 1;
  }
  if (a.buf[1] != '\0') {
    dbug("[ERROR] terminator\n");
    return 1;
  }

  dbug("setting a bit further in...\n");
  set_sbuf_index(&a, 2, 'c');
  if (VERBOSE)
    print_sbuf(a);

  dbug("filling in ...\n");
  set_sbuf_index(&a, 1, 'b');
  if (VERBOSE)
    print_sbuf(a);
  return 0;
}

/* test append */
int test_append() {
  sbuf a;
  dbug("\n\n**** APPEND TESTS ****\n");
  a = new_sbuf_size("", 64);
  while (!is_full(a)) {
    append_char_sbuf('a', &a);
  }
  if (VERBOSE)
    print_sbuf(a);
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
  return 0;
}
