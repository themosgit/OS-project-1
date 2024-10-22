/*#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/helper.h"

struct edge{
    Edge nextEdge;
    int dest;
    int weight;
    char* date;
};


int node_exists(Graph graph, int id){
    GraphNode node = find_node(graph, id);
    if (node == NULL){
        return 0;
    } else{
        return 1;
    }
}



void remove_edge(Graph graph, int src, int dest){
    if (!node_exists(graph,src)){
        printf("Node: %d does not exist please try again\n", src);
        return;
    }
    if (!node_exists(graph, dest)){
        printf("Node: %d does not exist please try again\n", dest);
        return;
    }
    Edge currentEdge = headEdge(find_node(graph, src));
    Edge previousEdge = currentEdge;
    while(currentEdge != NULL){
        if (currentEdge->dest == dest){
            previousEdge->nextEdge = currentEdge->nextEdge;
            free(currentEdge);
            return;

        } else {
            previousEdge = currentEdge;
            currentEdge = currentEdge->nextEdge;
        }
    }
}

int modify(Graph graph, int src, int dest, int weight, int newWeight, char* date, char* newDate){
    if (!node_exists(graph, src)){
        printf("Node: %d does not exist\n", src);
        return 0;
    }
    if (!node_exists(graph, dest)){
        printf("Node: %d does not exist\n", dest);
        return 0;
    }
    Edge edge = search_edge(graph, src, dest, weight, date);
    if (edge == NULL){
        printf("Edge does not exist\n");
        return 0;
    }
    edge->weight = newWeight;
    edge->date = newDate;
    return 1;
}



Edge search_edge(Graph graph,  int src, int dest, int weight, char* date){
    if (!node_exists(graph, src) || !node_exists(graph, dest)) return NULL;
    Edge index = headEdge(find_node(graph, src));
    while(index != NULL){
        if (index->dest == dest && index->weight == weight
            && strcmp(index->date, date) == 0)  {
            return index;
        }
        index = index->nextEdge;
    }
    return NULL;
}

void printEdges(Graph graph, int id){
    if (!node_exists(graph, id)){
        printf("Node: %d does not exist\n", id);
        return;
    }
    Edge edge = headEdge(find_node(graph, id));
    while (edge != NULL){
        printf("%d %d %d %s\n", id, edge->dest, edge->weight, edge->date);
        edge = edge->nextEdge;
    }
}

*/