/*
 * PriorityQueue.h
 *
 *  Created on: Sep 22, 2016
 *      Author: YS
 */

#ifndef UTILS_PRIORITY_QUEUE_PRIORITYQUEUE_H_
#define UTILS_PRIORITY_QUEUE_PRIORITYQUEUE_H_

typedef struct _pq_node {
	void *data;  // implement a generic priority queue by using void pointer
	struct _pq_node *next;
} PQNode;

typedef struct _min_priority_queue {
	PQNode *head;
	int size;
	int (*cmp)();  // this function pointer tells the priority queue how to compare your generic data
} PriorityQueue;

void pq_init(PriorityQueue *q, int (*cmp)(void *o1, void *o2));  // initialize with a comparator passing in
int pq_size(PriorityQueue *q);  // get the number of elements in the pq
void pq_push(PriorityQueue *q, void *p);  // add an element into the pq
void *pq_pop(PriorityQueue *q);  // remove and get the smallest element in the pq
void *pq_peek(PriorityQueue *q);  // get the smallest element
void pq_clear(PriorityQueue *q, void (*free_data)(void *));  // free all the remaining elements; the second arg tells how to free your data

#endif /* UTILS_PRIORITY_QUEUE_PRIORITYQUEUE_H_ */
