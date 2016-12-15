/*
 ============================================================================
 Name        : graph-analytics.c
 Author      : Hongyang Wang & YunHe Song
 Version     : 1.0
 Copyright   : © Fall 2016 CSE 6010 Hongyang Wang & YunHe Song
 Description : CSE 6010 assignment 3, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include "graph_utils.h"

int main(int argc, char *argv[]) {
	// parse input arguments
	// analysis topology –o output
	if (argc != 2 && argc != 4) {
		printf("usage: analysis graph_file [-o output_file]");
		return -1;
	}
	char *graph_filename = NULL, *output_filename = NULL;
	graph_filename = argv[1];
	if (argc == 4)
		output_filename = argv[3];

	// load graph
	int N = 0;
	Node **graph = load_graph(graph_filename, &N);
//	print_graph(graph, N);

	// get the diameter and max distance for each node
	int *max_dist[N];  // ARRAY OF POINTERS
	int diameter = 0;
	for (int i = 0; i < N; i++) {
		max_dist[i] = bfs(graph, N, i);
		if (max_dist[i][0] > diameter)  // max_dist, siz, node1, node2,...
			diameter = max_dist[i][0];
	}
	// print the result
	FILE *fp = NULL;
	if (output_filename != NULL) {
		fp = fopen(output_filename, "w");
	    if (fp == NULL) {
	    	printf("Can't open %s\n", output_filename);
			return -1;
	    }
	}
	if (fp == NULL)
		printf("%d\n", diameter);
	else
		fprintf(fp, "%d\n", diameter);
	int OFFSET = 2;
	for (int i = 0; i < N; i++) {
		if (fp == NULL)
			printf("%d: %d, [", i, max_dist[i][0]);
		else
			fprintf(fp, "%d: %d, [", i, max_dist[i][0]);
		for (int j = 0; j < max_dist[i][1]; j++) {
			if (fp == NULL)
				printf("%d", max_dist[i][j + OFFSET]);
			else
				fprintf(fp, "%d", max_dist[i][j + OFFSET]);
			if (j < max_dist[i][1] - 1) {
				if (fp == NULL)
					printf(", ");
				else
					fprintf(fp, ", ");
			}
		}
		if (fp == NULL)
			printf("]\n");
		else
			fprintf(fp, "]\n");
	}
	// ----
	if (fp != NULL) {  // print concise result to the console for analysis
		printf("%d\t%d\n", N, diameter);
	}
	// ----
	if (fp != NULL)
		fclose(fp);

	// clean
	// clean graph
	for (int i = 0; i < N; i++) {
		Node *p = graph[i];
		while (p) {
			Node *temp = p;
			p = p->next;
			free(temp);
			temp = NULL;
		}
	}
	free(graph);
	graph = NULL;
	// clean results
	for (int i = 0; i < N; i++) {
		free(max_dist[i]);
		max_dist[i] = NULL;
	}

}
