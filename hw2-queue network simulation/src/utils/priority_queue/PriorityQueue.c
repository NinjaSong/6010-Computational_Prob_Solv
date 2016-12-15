/*
 * PriorityQueue.c
 *
 *  Created on: Sep 22, 2016
 *      Author: YS
 */

#include <stdio.h>
#include <stdlib.h>
#include "PriorityQueue.h"

void pq_init(PriorityQueue *q, int (*cmp)(void *o1, void *o2)) {
	q->head = (PQNode *) malloc(sizeof(PQNode));  // dummy head
	q->head->data = NULL;
	q->head->next = NULL;
	q->size = 0;
	q->cmp = cmp;
}

int pq_size(PriorityQueue *q) {
	return q->size;
}

void pq_push(PriorityQueue *q, void *p) {
	PQNode *node = (PQNode *) malloc(sizeof(PQNode));
	node->data = p;
	PQNode *n = q->head;
	while (n->next != NULL && q->cmp(n->next->data, p) < 0) {
		n = n->next;
	}
	node->next = n->next;
	n->next = node;
	q->size++;
}

void *pq_pop(PriorityQueue *q) {
	void *ret = NULL;
	if (q->size == 0) {
		printf("Cannot pop from an empty PriorityQueue\n");
		return ret;
	}
	PQNode *temp = q->head->next;
	q->head->next = temp->next;
	ret = temp->data;
	free(temp);
	q->size--;
	return ret;
}

void *pq_peek(PriorityQueue *q) {
	void *ret = NULL;
	if (q->size == 0) {
		printf("The PriorityQueue is empty\n");
		return ret;
	}
	return q->head->next->data;
}

void pq_clear(PriorityQueue *q, void (*free_data)(void *)) {
	while (q->size > 0) {
		PQNode *temp = q->head->next;
		q->head->next = temp->next;
		free_data(temp->data);
		free(temp);
		q->size--;
	}
	free(q->head);  // free the dummy head
	q->head = NULL;
}


