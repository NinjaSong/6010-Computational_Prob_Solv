/*
 * read_graph.c
 *
 *  Created on: Oct 15, 2016
 *      Author: Frank
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "graph_utils.h"
#include "../queue/Queue.h"  // the generic queue from assignment 2

Node **load_graph(const char *filename, int *siz) {
	FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
    	printf("Can't open %s\n", filename);
		return NULL;
    }
    // first line is the total number of nodes
    fscanf(fp, "%d", siz);
    Node **graph = (Node **) malloc(*siz * sizeof(Node *));
    for (int i = 0; i < *siz; i++) {
    	graph[i] = NULL;  // HAS TO INITIALIZE
    }
    // construct the graph
    int id1 = 0, id2 = 0;
    while (fscanf(fp, "%d\t%d", &id1, &id2) != EOF) {
    	// add to id1's list
    	Node *node = (Node *) malloc(sizeof(Node));
    	node->id = id2;
    	node->next = NULL;  // HAS TO INITIALIZE
    	if (graph[id1] == NULL)
    		graph[id1] = node;
    	else {
			node->next = graph[id1]->next;
			graph[id1]->next = node;
    	}
    	// add to id2's list
    	node = (Node *) malloc(sizeof(Node));
    	node->id = id1;
    	node->next = NULL;
    	if (graph[id2] == NULL)
    		graph[id2] = node;
    	else {
			node->next = graph[id2]->next;
			graph[id2]->next = node;
    	}
    }
    fclose(fp);

    return graph;  // CANNOT PASS AS A PARAMETER (PASS BY VALUE)
}

void print_graph(Node **graph, int N) {
	printf("%d\n", N);
	if (graph == NULL) {
		printf("NULL\n");
		return;
	}
	for (int i = 0; i < N; i++) {
		printf("%d: ", i);
		Node *p = graph[i];
		while (p != NULL) {
			printf("%d ", p->id);
			p = p->next;
		}
		printf("\n");
	}
}

int *bfs(Node **graph, int N, int node) {
	Queue q;
	q_init(&q);
	int dist = -1;
	q_push(&q, (void *) node);
	int *ret = NULL;
	bool visited[N];
	for (int i = 0; i < N; i++) {
		visited[i] = false;
	}
	visited[node] = true;
	while (q_size(&q) > 0) {
		dist++;
		int siz = q_size(&q);
		int nodes[siz];  // nodes at this level
		for (int i = 0; i < siz; i++) {
			nodes[i] = (int) q_pop(&q);
			Node *p = graph[nodes[i]];
			while (p != NULL) {
				if (!visited[p->id]) {
					q_push(&q, (void *) p->id);
					visited[p->id] = true;
				}
				p = p->next;
			}
		}
		if (q_size(&q) == 0) {  // most distant nodes
			ret = (int *) malloc((siz + 2) * sizeof(int));
			ret[0] = dist;
			ret[1] = siz;
			int OFFSET = 2;
			for (int i = 0; i < siz; i++) {
				ret[i + OFFSET] = nodes[i];
			}
		}
	}

	return ret;
}




