# buf
a very bad string buffer module for c applications. very dumb, use cautiously.

## structures

a malloc'd char buffer
```c
typedef struct buf {
  char* buf;       // the actual buffer
  size_t capacity; // size of buffer
  int len;         // length of contents
  int freed;       // if using dynamic memory, freed flag
} buf;
```
## functions

malloc and fill a buffer
```c
buf new_buf(const char* string, size_t capacity);
```

realloc buffer space and handle truncation
```c
void realloc_buf(buf* b, size_t new_capacity);
```

try to free the buffer
```c
void free_buf(buf* b);
```

append to the buffer contents
```c
int append_buf(const char* string, buf* bp);
```

clear out a buffers contents
```c
int clear_buf(buf* bp);
```

check if buffer is full
```c
int is_full(buf b);
```

check if buffer is empty
```c
int is_empty(buf b);
```

print the buffer contents
```c
void print_buf(buf b);
```

### safe accessors
```c
char get_buf_element(buf b, int i);
int set_buf_element(buf* b, int i, char c);
```
