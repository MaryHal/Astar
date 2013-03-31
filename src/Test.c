#include <stdio.h>

#include "PriorityQueue.h"

int minCompare(const void* a, const void* b)
{
    return *(int*)a - *(int*)b;
}

int testQueue(int argc, const char *argv[])
{
    PriorityQueue* q = CreateQueue(minCompare, 10);
    int array[10];
    array[9] = 20;
    QueueAdd(q, &array[9]);
    for (int i = 8; i >= 0; i--)
    {
        array[i] = i * 2;
        QueueAdd(q, &array[i]);
    }
    for (int i = 0; i < 9; i++)
    {
        array[i] /= 2;
        int index = QueueSearch(q, &array[i]);
        PercolateUp(q, index, &array[i]);
    }
    array[9] = -1;
    int index = QueueSearch(q, &array[9]);
    PercolateUp(q, index, &array[9]);
    printf("\n%lu\n", (unsigned long)GetQueueSize(q));
    while (GetQueueSize(q) > 0)
        printf("%d ", *(int*)QueueRemove(q));

    printf("\n%lu\n", (unsigned long)GetQueueSize(q));
    DeleteQueue(q);
    return 0;
}
