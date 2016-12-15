//
//  graphgen.c
//  linkedgraph
//
//  Created by Yunhe Song on 16/10/22.
//  Copyright © 2016 Yunhe Song. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "graphgen.h"

/*
 *  Function randomg:
 *  Randomly select a node k in probability P(k)
 */
int randomg(int i, int degree , int nodedegree[]){
    int rd = rand()%degree;
    int sum = 0;
    for (int k=0;k<i;k++){
        sum = sum + nodedegree[k];
        if(rd < sum){
            return k; /* k is the selected node with probability P(k) */
        }else{
            continue;
        }
    }
    return 0;
}

/*
 *  Function countedge:
 *  Count edges for every node.
 */
int countedge(Node **h,int i){
    Node *p = h[i];
    int c = 1;
    while (p->next != NULL) {
        p=p->next;
        c = c + 1;
    }
    return c;
}

/*
 *  Function : createnodes
 *  create n nodes scale free networks graph
 */
Node **createnodes(Node **graph, int n){
    /* initialize the first 3 node */
    Node *node = (Node *) malloc(sizeof(Node));
    node->id = 1;
    node->next = NULL;
    graph[0] = node;
    node = (Node *) malloc(sizeof(Node));
    node->id = 2;
    node->next = NULL;
    graph[0]->next = node;
    node = (Node *) malloc(sizeof(Node));
    node->id = 0;
    node->next = NULL;
    graph[1] = node;
    node = (Node *) malloc(sizeof(Node));
    node->id = 2;
    node->next = NULL;
    graph[1]->next = node;
    node = (Node *) malloc(sizeof(Node));
    node->id = 0;
    node->next = NULL;
    graph[2] = node;
    node = (Node *) malloc(sizeof(Node));
    node->id = 1;
    node->next = NULL;
    graph[2]->next = node;

    /* add new node */
    int *nodedegree; /* The list of node_degree for every node */
    nodedegree=(int*)malloc(n*sizeof(int));
    for(int i=0;i<n; i++)
    {
        nodedegree[i]=0;
    }
    int degree = 6;
    nodedegree[0] = nodedegree[1] = nodedegree[2] = 2;
    
    for (int i=3;i<n;i++){
        srand((unsigned)time(NULL));
        int g = randomg(i,degree,nodedegree);
        node = (Node *) malloc(sizeof(Node));
        node->id = g;
        node->next = NULL;
        graph[i] = node;/* For node i, it has an edge to node g */
        node = (Node *) malloc(sizeof(Node));
        node->id = i;
        node->next = NULL;
        Node *p = graph[g];
        while(p->next!=NULL)p = p->next;
        p->next = node;/* add an new edge to node g */
        
        nodedegree[g] = nodedegree[g] + 1; /* add a degree to the selected node g */
        nodedegree[i] = nodedegree[i] + 1; /* add a degree to this new node i */
        degree = degree + 2; /* total degree get 2 more degree from g and i */
    }

    free(nodedegree);
    nodedegree = NULL;

    return graph;
}

/*
 *  Function outputtopology:
 *  write the network into a file in the format agreed with my partner
 */
void outputtopology(Node **h,int n,FILE *graph_file){
    
    if(graph_file == NULL){
        fprintf(stderr,"Error in opening file\n"); /* Check if file opens successful  */
        exit(EXIT_FAILURE);
    }
    fprintf(graph_file,"%d\n",n);
    for (int i = 0;i<n;i++){
        Node *p=h[i];
        while(p->next!=NULL){
            if (p->id > i){
                fprintf(graph_file,"%d\t%d\n",i,p->id);
            }
            p = p->next;
        }
        if (p->id > i){
            fprintf(graph_file,"%d\t%d\n",i,p->id);
        }
    }
    fclose(graph_file);
}


/*
 *  Function outputhistogram:
 *  write the node degree distribution into a file called "histogram"
 */
void outputhistogram(FILE *output_file,int n,Node **h){
    if(output_file == NULL ){
        fprintf(stderr,"Error in opening file histogram\n"); /* Check if file opens successful  */
        exit(EXIT_FAILURE);
    }
    
    int *degreeh ; /* A list that record the count number for every degree h */
    degreeh=(int*)malloc(n*sizeof(int*)); /* If there is n nodes, the max degree is n-1 */
    
    for(int i=0;i<n; i++)
    {
        degreeh[i]=0; /* initialize the count number */
    }
    
    int *nodedegree; /* The list of node_degree for every node */
    nodedegree=(int*)malloc(n*sizeof(int*));
    for(int i=0;i<n; i++)
    {
        nodedegree[i]=countedge(h,i);
    }
    
    /* Count the number of nodes which have the same degree */
    for (int d=1;d<=n;d++){
        for(int i=0;i<n;i++){
            if(d == nodedegree[i]){
                degreeh[d-1] = degreeh[d-1] + 1;
            }
        }
        
        /* Print histogram */
        if(degreeh[d-1] != 0){
            fprintf(output_file,"degree i = %d,count num = %d, list = (",d,degreeh[d-1]);
            for(int i=0;i<n;i++){
                if(d == nodedegree[i]){
                    fprintf(output_file," %d ",i);
                }
            }
            fprintf(output_file,") \n");
        }
    }
    fclose(output_file);
    free(degreeh);
    free(nodedegree);
}

/*
 *  Function print:
 *  to print the sample graph in the console
 */
void print(Node **head,int n){
    int sum = 0;
    for (int i = 0;i<n;i++){
        printf("Node:%d\t edge:\t",i);
        Node *p = head[i];
        while(p->next!=NULL){
            printf("%d\t",p->id);
            p = p->next;
            sum = sum +1;
        }
        printf("%d\n",p->id);
        sum = sum + 1;
    }
    printf("degree %d\n",sum);
    
}

