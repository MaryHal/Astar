#ifndef _PriorityQueue_h_
#define _PriorityQueue_h_

#include <string.h>

typedef struct
{
    size_t size;
    size_t capacity;

    void** data;
    int (*cmp)(const void* a, const void* b);
} PriorityQueue;

typedef PriorityQueue BinHeap;

PriorityQueue* CreateQueue(int (*cmp)(const void* a, const void* b), size_t capacity);
void DeleteQueue(PriorityQueue* q);

size_t GetQueueSize(PriorityQueue* q);
void QueueAdd(PriorityQueue* q, const void* data);
void* QueueRemove(PriorityQueue* q);

void PercolateDown(PriorityQueue* q, int hole);
void PercolateUp(PriorityQueue* q, int hole, const void* data);

void SwapElements(PriorityQueue* q, int first, int second);
int QueueSearch(PriorityQueue* q, const void* data);

#endif

