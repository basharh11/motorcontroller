#include "queue.h"

void queueInit(queue *q) {
    q->front = 0;
    q->end = 0;
    q->size = 0;
}

bool isQueueFull(const queue *q) {
    return q->size == QUEUE_CAPACITY;
}

bool isQueueEmpty(const queue *q) {
    return q->size == 0;
}

bool enqueue(queue *q, uint8_t in) {
    if(isQueueFull(q)) 
        return false;
    q->buf[q->front] = in;
    q->front = (q->front + 1) % QUEUE_CAPACITY;
    q->size++;
    return true;
}

bool dequeue(queue *q, uint8_t *out) {
    if(isQueueEmpty(q)) 
        return false;
    *out = q->buf[q->end];
    q->end = (q->end + 1) % QUEUE_CAPACITY;
    q->size--;
    return true;
}

void clearQueue(queue *q) {
    q->front = 0;
    q->end = 0;
    q->size = 0;
}
