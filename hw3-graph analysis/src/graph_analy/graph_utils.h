/*
 * graph_utils.h
 *
 *  Created on: Oct 15, 2016
 *      Author: Frank
 */

#ifndef SRC_GRAPH_UTILS_H_
#define SRC_GRAPH_UTILS_H_

// linkedlist node to hold the graph
typedef struct _node {
	int id;
	struct _node *next;

} Node;

/* load the graph from a file, and use the linkedlist strategy to store it
 * parameter: filename: the path and name to the file storing the graph
 * 		      siz: will be filled with the total number of nodes in the graph
 * return: an array of pointers, each of which points to a linkedlist
 */
Node **load_graph(const char *filename, int *siz);

/* print the graph, first line is the total number of nodes, and then each edge
 */
void print_graph(Node **graph, int N);

/* do breadth-first search from a node through out the entire graph
 * parameter: graph: the graph
 * 			  N: total number of nodes in the graph
 * 			  node: id of the starting node
 * return: a pointer to an int array that holds the nodes with max distance from the source node
 * 		   format: ret[0] = max_dist, ret[1] = number of nodes with this max distance, ret[...] = node ids
 */
int *bfs(Node **graph, int N, int node);

#endif /* SRC_GRAPH_UTILS_H_ */
