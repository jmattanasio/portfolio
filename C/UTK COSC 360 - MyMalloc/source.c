#include "mymalloc.h"
#include <unistd.h> // Include unistd.h for sbrk

// Define a structure to represent a free memory block.
typedef struct flist {
    int blockSize;          // Size of the free block.
    struct flist *forwardLink; // Pointer to the next free block.
    struct flist *backwardLink; // Pointer to the previous free block.
} *FreeBlock;

// Initialize the head of the free list to NULL.
FreeBlock freeListHead = NULL;

// Returns the head of the free list.
void *free_list_begin() {
    return freeListHead;
}

// Returns the next free block in the list.
void *free_list_next(void *node) {
    return ((FreeBlock)node)->forwardLink;
}

// Inserts a free block into the free list, maintaining address order.
void insertFreeBlock(FreeBlock node) {
    if (freeListHead == NULL) {
        freeListHead = node;
        return;
    }
    FreeBlock currentBlock;
    for (currentBlock = free_list_begin(); currentBlock->forwardLink != NULL && node > currentBlock->forwardLink; currentBlock = currentBlock->forwardLink);
    if (currentBlock->forwardLink == NULL) {
        currentBlock->forwardLink = node;
        node->backwardLink = currentBlock;
        return;
    }
    node->forwardLink = currentBlock->forwardLink;
    currentBlock->forwardLink->backwardLink = node;
    currentBlock->forwardLink = node;
    node->backwardLink = currentBlock;
}

// Deletes a free block from the free list.
void deleteFreeBlock(FreeBlock node) {
    if (node == freeListHead)
        freeListHead = node->forwardLink;
    if (node->forwardLink)
        node->forwardLink->backwardLink = node->backwardLink;
    if (node->backwardLink)
        node->backwardLink->forwardLink = node->forwardLink;
}

// Creates a new free block by allocating memory from the system using sbrk.
FreeBlock createNewFreeBlock(size_t size) {
    size_t allocSize = 8192;
    if (size > 8184)
        allocSize = (size + 7 + 8) & -8;
    FreeBlock newBlock = (FreeBlock)sbrk(allocSize);
    if (newBlock == (FreeBlock)-1) {
        return NULL; // sbrk failed
    }
    newBlock->blockSize = allocSize;
    newBlock->backwardLink = NULL;
    newBlock->forwardLink = NULL;
    insertFreeBlock(newBlock);
    return newBlock;
}

// Coalesces adjacent free blocks in the free list.
void coalesce_free_list() {
    FreeBlock currentBlock = free_list_begin();
    while (currentBlock != NULL) {
        FreeBlock nextBlock = currentBlock->forwardLink;
        if ((void *)currentBlock + currentBlock->blockSize == nextBlock) {
            currentBlock->blockSize += nextBlock->blockSize;
            deleteFreeBlock(nextBlock);
        } else {
            currentBlock = currentBlock->forwardLink;
        }
    }
}

// Allocates memory from the free list or by creating a new free block.
void *my_malloc(size_t size) {
    FreeBlock foundBlock;
    size_t requiredSize = (size + 7 + 8) & -8;
    for (foundBlock = free_list_begin(); foundBlock != NULL; foundBlock = free_list_next(foundBlock))
        if (foundBlock->blockSize >= requiredSize)
            break;
    if (foundBlock == NULL)
        foundBlock = createNewFreeBlock(size);
    if (foundBlock == NULL) return NULL; // sbrk failed in createNewFreeBlock
    foundBlock->blockSize -= requiredSize;
    FreeBlock allocatedBlock = (void *)foundBlock + foundBlock->blockSize;
    if (foundBlock->blockSize <= 16) {
        requiredSize += foundBlock->blockSize;
        deleteFreeBlock(foundBlock);
    }
    allocatedBlock->blockSize = requiredSize;
    return (void *)allocatedBlock + 8;
}

// Frees a previously allocated memory block.
void my_free(void *ptr) {
    insertFreeBlock((FreeBlock)(ptr - 8));
}