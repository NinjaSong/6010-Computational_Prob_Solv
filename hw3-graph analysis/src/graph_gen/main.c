//
//  main.c
//  linkedgraph
//
//  Created by Yunhe Song on 16/10/25.
//  Copyright © 2016 Yunhe Song. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graphgen.h"

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]);
    Node **graph = (Node **) malloc(n * sizeof(Node *));
    for (int i = 0; i < n; i++) {
        graph[i] = NULL;  /* initialization */
    }
    Node **h = createnodes(graph,n); /* Create the graph in a linked list */
    
    //print(h,n);
    
    FILE *graph_file; /* The first "topology" file */
    graph_file = fopen(/*"output.txt"*/argv[2], "w+");
    outputtopology(h,n,graph_file);/* The graph file output "topology" */
    
    if(argc >= 5){
        FILE *output_file; /* The second "histogram" file */
        output_file = fopen(argv[4]/*"his1234.txt"*/, "w+");
        outputhistogram(output_file,n,h);/* The file output "histogram" */
    }
    
    // free graph
    for (int i = 0; i < n; i++) {
	Node *p = graph[i], *temp = NULL;
	while (p != NULL) {
	    temp = p->next;
	    free(p);
	    p = temp;
	}
    }
    free(graph);
    
    return 0;
}

