#include <stdlib.h>
#include "memory.h"

#include <stdio.h>

#define MALLOC_ATTEMPTS_COUNT 10

Stack static *allocatedMemoryStack = NULL;

void static *addPointerToAllocatedMemoryStack(void *pointer) {
    Stack *newAllocatedMemoryNode = NULL;
    for(int i = 0; i < MALLOC_ATTEMPTS_COUNT && !newAllocatedMemoryNode; i++) {
        newAllocatedMemoryNode = malloc(sizeof(Stack));
        printf("{%d}\n", i);
        printf("{%ld}\n", sizeof(Stack));
    }
    if (newAllocatedMemoryNode) {
        newAllocatedMemoryNode->payload = pointer;
        newAllocatedMemoryNode->next = allocatedMemoryStack;
        printf("{%p}\n", newAllocatedMemoryNode);
        printf("{%p}\n", newAllocatedMemoryNode->next);
        printf("{%p}\n", allocatedMemoryStack);
        // allocatedMemoryStack = newAllocatedMemoryNode;
    }
    return newAllocatedMemoryNode;
}

void free_(void **ptr) {
    free(*ptr);
    *ptr = NULL;
}

void *malloc_(size_t size) {
    void *result = NULL;
    for(int i = 0; i < MALLOC_ATTEMPTS_COUNT && !result; i++) {
        result = malloc(size);
    }
    if(result) {
        result = addPointerToAllocatedMemoryStack(result);
    }
    return result;
}

void freeAll() {
    Stack *temp = allocatedMemoryStack;
    while (allocatedMemoryStack) {
        allocatedMemoryStack = temp->next;
        free(temp->payload);
        free(temp);
    }
}