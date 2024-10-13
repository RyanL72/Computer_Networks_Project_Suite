#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define HEAP_SIZE 5000
#define MAX_REQUEST 9
#define EPOCHS 1000
#define FREE_PROB 0.3

typedef struct FreeBlock {
    int start;
    int size;
    struct FreeBlock *next;
} FreeBlock;

int heap[HEAP_SIZE];

FreeBlock *freelist = NULL;

void initHeap() {
    FreeBlock *initialBlock = (FreeBlock *)malloc(sizeof(FreeBlock));
    initialBlock->start = 0;
    initialBlock->size = HEAP_SIZE;
    initialBlock->next = NULL;
    freelist = initialBlock;
}

void printHeap() {
    for (int i = 0; i < HEAP_SIZE; i++) {
        printf("%d", heap[i]);
    }
    printf("\n");
}

void printFreeList() {
    FreeBlock *current = freelist;
    printf("Free list: ");
    while (current != NULL) {
        printf("[%d, %d] ", current->start, current->size);
        current = current->next;
    }
    printf("\n");
}

void allocate(int size) {
    FreeBlock *prev = NULL;
    FreeBlock *current = freelist;

    while (current != NULL) {
        if (current->size >= size) {
            for (int i = current->start; i < current->start + size; i++) {
                heap[i] = 1;
            }

            if (current->size == size) {
                if (prev == NULL) {
                    freelist = current->next;
                } else {
                    prev->next = current->next;
                }
                free(current);
            } else {
                current->start += size;
                current->size -= size;
            }
            return;
        }
        prev = current;
        current = current->next;
    }

    printf("No free block large enough to allocate %d units\n", size);
}

void freeBlock(int start, int size) {
    for (int i = start; i < start + size; i++) {
        heap[i] = 0;
    }

    FreeBlock *newBlock = (FreeBlock *)malloc(sizeof(FreeBlock));
    newBlock->start = start;
    newBlock->size = size;
    newBlock->next = freelist;
    freelist = newBlock;
}

void simulate(int useSeed) {
    if (useSeed) {
        srand(time(NULL));
    }

    int allocations = 0;
    int frees = 0;

    for (int i = 0; i < EPOCHS; i++) {
        if ((double)rand() / RAND_MAX < FREE_PROB) {
            int start = rand() % HEAP_SIZE;
            int size = rand() % MAX_REQUEST + 1;
            printf("Freeing %d units starting at %d\n", size, start);
            freeBlock(start, size);
            frees++;
        } else {
            int size = rand() % MAX_REQUEST + 1;
            printf("Allocating %d units\n", size);
            allocate(size);
            allocations++;
        }

        printHeap();
        printFreeList();
    }

    printf("Total allocations: %d\n", allocations);
    printf("Total frees: %d\n", frees);
}

int main(int argc, char *argv[]) {
    int useSeed = 0;
    if (argc > 1 && argv[1][0] == '1') {
        useSeed = 1;
    }

    initHeap();
    simulate(useSeed);

    return 0;
}
