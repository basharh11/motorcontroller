#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdint.h>

#define QUEUE_CAPACITY 16

typedef struct {
    uint8_t buf[QUEUE_CAPACITY];
    uint8_t front;
    uint8_t end;
    uint8_t size;
} queue;

void queueInit(queue *q); // initialize the queue
bool isQueueFull(const queue *q); // check if queue is full
bool isQueueEmpty(const queue *q); // check if queue is empty
bool enqueue(queue *q, uint8_t in); // enqueue one byte
bool dequeue(queue *q, uint8_t *out); // dequeue one byte
void clear(queue *q); // clear queue

#endif