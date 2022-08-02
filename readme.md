# buf
a very bad string buffer module for c applications. very dumb, use cautiously.

## usage
generate and link a `.so` with the make file
```bash
git clone origin
cd buf
make object
# or
make lib
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

malloc and fill a buffer.
```c
buf new_buf(const char* string, size_t capacity);
```

realloc buffer space and handle truncation.
```c
void realloc_buf(buf* b, size_t new_capacity);
```

free the buffer and set it to NULL.
```c
void free_buf(buf* b);
```

append to the buffer contents. returns an exit code.
```c
int append_buf(const char* string, buf* bp);
```

clear out a buffers contents. returns an exit code.
```c
int clear_buf(buf* bp);
```

check if buffer is full. returns a boolean.
```c
int is_full(buf b);
```

check if buffer is empty. returns a boolean.
```c
int is_empty(buf b);
```

print the buffer contents.
```c
void print_buf(buf b);
```

safe accessors for buffer elements. `get_buf_element` returns a `'\0'` on errors, and set returns an exit code.
```c
char get_buf_element(buf b, int i);
int set_buf_element(buf* b, int i, char c);
```
