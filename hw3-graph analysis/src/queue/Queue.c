/*
 * Queue.c
 *
 *  Created on: Sep 16, 2016
 *      Author: YS
 */

#include <stdlib.h>
#include <stdio.h>
#include "Queue.h"

void q_init(Queue *q) {
	q->head = q->tail = NULL;
	q->size = 0;
}

int q_size(Queue *q) {
	return q->size;
}

void q_push(Queue *q, void *p) {
	QNode *new_node = (QNode *) malloc(sizeof(QNode));
	new_node->data = p;
	if (q->tail == NULL)
		q->head = q->tail = new_node;
	else {
		q->tail->next = new_node;
		q->tail = new_node;
	}
	q->size++;
}

void *q_peek(Queue *q) {
	void *ret = NULL;
	if (q->size == 0) {
		printf("The Queue is empty\n");
		return ret;
	}
	return q->head->data;
}

void *q_pop(Queue *q) {
	void *ret = NULL;
	if (q->size == 0) {
		printf("Cannot pop from an empty Queue\n");
		return ret;
	}
	QNode *temp = q->head;
	ret = temp->data;
	q->head = temp->next;
	if (temp == q->tail)  // important!!
		q->tail = NULL;
	free(temp);
	q->size--;
	return ret;
}

void q_clear(Queue *q, void (*free_data)(void *)) {
	while (q->size > 0) {
		QNode *temp = q->head;
		q->head = temp->next;
		free_data(temp->data);
		free(temp);
		q->size--;
	}
	q->head = q->tail = NULL;
}
