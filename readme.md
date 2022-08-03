# sbuf
a very bad string buffer module for c applications. very dumb, use cautiously.

## usage
### compile
```bash
# generate a .so and copy .so -> /usr/lib, .h -> /usr/include
make lib
# or generate a static object
make object
# or compile for testing
make test
```
### include
```c
#include <sbuf.h>
```

## structures

a malloc'd string buffer

```c
typedef struct sbuf {
  char* buf;       // the actual buffer
  size_t capacity; // size of buffer
  size_t len;      // length of contents
} sbuf;
```

## functions

### allocation

```c
sbuf new_sbuf(const char* string, size_t capacity);
```
allocate a new buffer. if allocation fails, return an empty buffer with .capacity=0 and set errno.

```c
sbuf new_sbuf_small(char* string);
```
allocate the smallest possible buffer. returns equivalent to `new_sbuf()`

```c
void realloc_sbuf(sbuf* s, size_t new_capacity);
```
reallocate an existing buffer, possibly truncating its contents.

```c
void free_sbuf(sbuf* s);
```
free a buffers internal pointer. updates capacity and length of buffer, will not double free.

### accessors

```c
char get_sbuf_index(sbuf s, int i);
```
returns a '\0' and sets errno on errors.

```c
int set_sbuf_index(sbuf* s, int i, char c);
```
returns an exit code and sets errno on failure.

```c
char* get_sbuf(sbuf s);
```
returns direct pointer to char buffer.

```c
int set_sbuf(sbuf*s, char* string);
```
returns an exit code and sets errno on failure.

### functions

```c
int append_sbuf(const char* string, sbuf* s);
```
append to a buffer. returns an exit code and set errno on failure.

```c
int clear_buf(buf* s);
```
clear a buffers contents and reset its length. returns an exit code and set errno on failure.

```c
const char* copy_sbuf(sbuf s);
```
makes a static copy of the buffer contents.

```c
int shrink_sbuf(sbuf* s);
```
shrink the buffer to the size of its contents. returns an exit code.

### util
```c
int is_full(sbuf s);
```
check if buffer is full. normally returns a boolean, returns -1 on error and sets errno.

```c
int is_empty(sbuf s);
```
check if buffer is empty. normally returns a boolean, returns -1 on error and sets errno.

```c
void print_sbuf(sbuf s);
```
print the buffer contents.

