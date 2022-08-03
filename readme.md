# buf
a very bad string buffer module for c applications. very dumb, use cautiously.

## usage
```bash
# generate a .so and copy .so -> /usr/lib, .h -> /usr/include
make lib
# or generate a static object
make object
# or compile for testing
make test
```

## structures

a malloc'd char buffer
```c
typedef struct buf {
  char* buf;       // the actual buffer
  size_t capacity; // size of buffer
  int len;         // length of contents
} buf;
```
## functions

allocate a new buffer. if allocation fails, return an empty buffer with .capacity=0 and set errno.
```c
buf new_buf(const char* string, size_t capacity);
```

reallocate an existing buffer, possibly truncating its contents.
```c
void realloc_buf(buf* b, size_t new_capacity);
```

free a buffers internal pointer. updates capacity and length of buffer, will not double free.
```c
void free_buf(buf* b);
```

clear a buffers contents and reset its length. returns an exit code and set errno on failure.
```c
int clear_buf(buf* bp);
```

append to a buffer. returns an exit code and set errno on failure.
```c
int append_buf(const char* string, buf* bp);
```


check if buffer is full. normally returns a boolean, returns -1 on error and sets errno.
```c
int is_full(buf b);
```

check if buffer is empty. normally returns a boolean, returns -1 on error and sets errno.
```c
int is_empty(buf b);
```

print the buffer contents.
```c
void print_buf(buf b);
```

safe accessors for buffer elements. `get_buf_element` returns a `'\0'` on errors, and set returns an exit code. both set errno on failure.
```c
char get_buf_element(buf b, int i);
int set_buf_element(buf* b, int i, char c);
```
