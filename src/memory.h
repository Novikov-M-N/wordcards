#ifndef MEMORY
#define MEMORY

typedef struct stack {
    void *payload;
    struct stack *next;
} Stack;

void free_(void **ptr);
void *malloc_(size_t size);
void freeAll();

#endif  // MEMORY