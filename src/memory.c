#include <stdlib.h>
#include "memory.h"

#include <stdio.h>

#define MALLOC_ATTEMPTS_COUNT 10

static Stack *allocatedMemoryStack = NULL;

static void printAllocatedMemoryStack() {
    Stack *currentNode = allocatedMemoryStack;
    printf("> allocated memory stack:\n");
    printf("**********\n");
    while (currentNode) {
        printf("    node:       %p\n", currentNode);
        printf("    payload:    %p\n", currentNode->payload);
        printf("    next:       %p\n", currentNode->next);
        printf("    ----------\n");
        currentNode = currentNode->next;
    }
    printf("**********\n");
}

static void *addPointerToAllocatedMemoryStack(void *pointer) {
        #ifdef DEBUG
        printf("ADD POINTER {%p} to allocated memory stack\n", pointer);
        printAllocatedMemoryStack();
        #endif
    Stack *newAllocatedMemoryNode = NULL;
    for(int i = 0; i < MALLOC_ATTEMPTS_COUNT && !newAllocatedMemoryNode; i++) {
        newAllocatedMemoryNode = malloc(sizeof(Stack));
            #ifdef DEBUG
            printf("allocate memory for new allocated memory stack node attempt № %d\n", i);
            #endif
    }
    if (newAllocatedMemoryNode) {
        newAllocatedMemoryNode->payload = pointer;
        newAllocatedMemoryNode->next = allocatedMemoryStack;
        allocatedMemoryStack = newAllocatedMemoryNode;
    }
        #ifdef DEBUG
        printAllocatedMemoryStack();
        #endif
    return newAllocatedMemoryNode;
}

void free_(void **ptr) {
    free(*ptr);
    *ptr = NULL;
}

void *malloc_(size_t size) {
        #ifdef DEBUG
        printf("MALLOC_:\n");
        #endif
    void *result = NULL;
    for(int i = 0; i < MALLOC_ATTEMPTS_COUNT && !result; i++) {
        result = malloc(size);
            #ifdef DEBUG
            printf("malloc attempt № %d: result = %p\n", i, result);
            #endif
    }
    if(!addPointerToAllocatedMemoryStack(result)) {
        result = NULL;
    }
    return result;
}

void freeAll() {
        #ifdef DEBUG
        printf("FREE ALL:\n");
        printAllocatedMemoryStack();
        #endif
    while (allocatedMemoryStack) {
        Stack *currentNode = allocatedMemoryStack;
            #ifdef DEBUG
            printf("current node = %p\n", currentNode);
            #endif
        allocatedMemoryStack = allocatedMemoryStack->next;
            #ifdef DEBUG
            printf("allocatd memory stack = %p\n", allocatedMemoryStack);
            printf("free(%p)\n", currentNode->payload);
            #endif
        free(currentNode->payload);
            #ifdef DEBUG
            printf("free(%p)\n", currentNode);
            #endif
        free(currentNode);
            #ifdef DEBUG
            printAllocatedMemoryStack();
            #endif
    }
}