/*
 * Queue.h
 *
 *  Created on: Sep 16, 2016
 *      Author: YS
 */

#ifndef UTILS_QUEUE_QUEUE_H_
#define UTILS_QUEUE_QUEUE_H_

typedef struct _q_node {
	void *data;  // implement a generic queue by using void pointer
	struct _q_node *next;
} QNode;

typedef struct {
	QNode *head, *tail;
	int size;
} Queue;

void q_init(Queue *q);  // initialize
int q_size(Queue *q);  // get the number of elements in the queue
void q_push(Queue *q, void *p);  // add an element to the queue; void *p is the pointer pointing to your data
void *q_peek(Queue *q);  // get the first element in the queue
void *q_pop(Queue *q);  // remove and get the first element
void q_clear(Queue *q, void (*free_data)(void *));  // free all the remaining elements in the queue; the second arg tells how to free your data

#endif /* UTILS_QUEUE_QUEUE_H_ */
