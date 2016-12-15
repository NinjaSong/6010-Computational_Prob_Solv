//
//  grephgen.h
//  linkedgraph
//
//  Created by Yunhe Song on 16/10/23.
//  Copyright © 2016 Yunhe Song. All rights reserved.
//

#ifndef grephgen_h
#define grephgen_h

#include <stdio.h>

/*
 *  Structure
 *  define structure of node
 */
typedef struct _node {
    int id;
    struct _node *next;
    
} Node;

/*
 *  Function : createnodes
 *  create n nodes scale free networks graph
 */
Node **createnodes(Node **graph, int n);

/*
 *  Function randomg:
 *  Randomly select a node k in probability P(k)
 */
int randomg(int i, int degree , int nodedegree[]);

/*
 *  Function countedge:
 *  Count edges (degree) for every node.
 */
int countedge(Node **h,int i);

/*
 *  Function outputtopology:
 *  write the network into a file in the format agreed with my partner
 */
void outputtopology(Node **h,int n,FILE *graph_file);

/*
 *  Function outputhistogram:
 *  write the node degree distribution into a file called "histogram"
 */
void outputhistogram(FILE *output_file,int n,Node **h);

/*
 *  Function print:
 *  to print the sample graph in the console
 */
void print(Node **head,int n);





#endif /* grephgen_h */
