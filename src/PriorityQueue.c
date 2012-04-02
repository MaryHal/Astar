#include "PriorityQueue.h"

#include <stdlib.h>
#include <stdio.h>

#define LEFT(x) (2*(x) + 1)
#define RIGHT(x) (2*(x) + 2)
#define PARENT(x) ((x) / 2)

PriorityQueue* CreateQueue(int (*cmp)(const void* a, const void* b), size_t capacity)
{
    PriorityQueue* q = NULL;
    if (!cmp)
        return NULL;

    q = malloc(sizeof(*q));
    if (!q)
        return NULL;

    q->cmp = cmp;
    q->data = malloc(capacity * sizeof(*(q->data)));
    if (!q->data)
        return NULL;

    q->capacity = capacity;
    q->size = 0;

    return (q);
}

void DeleteQueue(PriorityQueue* q)
{
    if (!q)
        return;

    free(q->data);
    free(q);
}

size_t GetQueueSize(PriorityQueue* q)
{
    return q->size;
}

void QueueAdd(PriorityQueue* q, const void* data)
{
    if (!q)
        return;

    // Queue is full
    if (q->size >= q->capacity)
    {
        //printf("Queue is full\n");
        return;
    }

    // Percolate up
    int hole = ++(q->size);
    q->data[0] = (void*)data;

    for(; q->cmp((void*)data, q->data[hole/2]) < 0; hole /= 2)
    {
        q->data[hole] = q->data[hole / 2];
    }
    q->data[hole] = (void*)data;
}

void* QueueRemove(PriorityQueue* q)
{
    if (!q)
        return NULL;

    // Queue is empty
    if (q->size < 1)
    {
        printf("queue is empty");
        return NULL;
    }

    void* data = q->data[1];

    q->data[1] = q->data[q->size--];
    PercolateDown(q, 1);

    return (data);
}

void PercolateDown(PriorityQueue* q, int hole)
{
    int child;
    void* tmp = q->data[hole];

    for(; hole * 2 <= q->size; hole = child)
    {
        child = hole * 2;
        if(child != q->size &&
           q->cmp(q->data[ child + 1 ], q->data[ child ] ) < 0)
        {
           child++;
        }
        if(q->cmp( q->data[ child ], tmp ) < 0 )
        {
            q->data[ hole ] = q->data[ child ];
        }
        else
            break;
    }
    q->data[hole] = tmp;
}

void Heapify(PriorityQueue* q, int i)
{
    int l = LEFT(i);
    int r = RIGHT(i);
    int minimum;

    //if(l < q->size && q->data[l] < q->data[i])
    if(l < q->size && q->cmp(q->data[l], q->data[i]) < 0)
    {
        minimum = l;
    }
    else
    {
        minimum = i;
    }

    //if(r < q->size && q->data[r] < q->data[minimum])
    if(r < q->size && q->cmp(q->data[r], q->data[minimum]) < 0)
    {
        minimum = r;
    }

    if(minimum != i)
    {
        void* temp = q->data[i];
        q->data[i] = q->data[minimum];
        q->data[minimum] = temp;

        Heapify(q, minimum);
    }
}

int QueueSearch(PriorityQueue* q, const void* data)
{
    for (int i = 0; i < q->size; ++i)
    {
        if (q->data[i] == data)
            return i;
    }
    return q->size;
}

