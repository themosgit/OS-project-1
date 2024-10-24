#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"

struct edge{ //δομή ακμών linked list
    Edge nextEdge;
    int dest;
    int weight;
    char date[10];
};

void insert_edge(Graph graph, int src, int dest, int weight, char* date) {
    if (!node_exists(graph, src)){ //έλεγχος για ύπαρξη nodes
        insert_node(graph, src);    //και εισαγωγή τους αν είναι απαραίτητο
    }
    if (!node_exists(graph, dest)){
        insert_node(graph, dest);
    }
    Edge edge = malloc(sizeof(struct edge));
    strcpy(edge->date, date);
    edge->weight = weight;
    edge->dest = dest;
    GraphNode node = find_node(graph, src);
    edge->nextEdge = returnEdge(node);
    setEdge(edge, node);
    node = find_node(graph,dest);
    addIncoming(node, src); //ανανέωση βοηθητικής δομής εισερχόμενων ακμών
}

void remove_edge(Graph graph, int src, int dest){ // αφαίρεση ακμής

    if (!node_exists(graph,src)){
        printf("Node: %d does not exist please try again\n", src);
        return;
    }
    if (!node_exists(graph, dest)){
        printf("Node: %d does not exist please try again\n", dest);
        return;
    }

    Edge currentEdge = returnEdge(find_node(graph, src));
    if(currentEdge == NULL) return; //εάν δεν υπάρχει ακμή

    if(currentEdge->nextEdge == NULL) { // έαν είναι η μόνη ακμή στο node
        setEdge(NULL, find_node(graph, src));
        removeIncoming(find_node(graph, dest), src); //ανανέωση βοηθητικής δομής εισερχόμενων ακμών
        free(currentEdge);
        return;
    }


    if (currentEdge->dest == dest){ //κυρίως χρήσιμο στη καταστροφή του γράφου η περίπτωση που είναι η στη θέση head
        setEdge(currentEdge->nextEdge, find_node(graph, src));
        removeIncoming(find_node(graph, dest), src); //ανανέωση βοηθητικής δομής εισερχόμενων ακμών
        free(currentEdge);
        return;
    }

    Edge previousEdge = currentEdge; //για τις υπόλοιπες περιπτώσεις
    currentEdge = currentEdge->nextEdge;
    while(currentEdge != NULL){
        if (currentEdge->dest == dest){
            previousEdge->nextEdge = currentEdge->nextEdge;
            removeIncoming(find_node(graph, dest), src); //ανανέωση βοηθητικής δομής εισερχόμενων ακμών
            free(currentEdge);
            return;
        } else {
            previousEdge = currentEdge;
            currentEdge = currentEdge->nextEdge;
        }
    }
}

Edge search_edge(Graph graph,  int src, int dest, int weight, char* date){ // αναζήτηση
    if (!node_exists(graph, src) || !node_exists(graph, dest)) return NULL;
    Edge index = returnEdge(find_node(graph, src));

    while(index != NULL){
        if (index->dest == dest && index->weight == weight
            && strcmp(index->date, date) == 0)  {
            return index;
        }
        index = index->nextEdge;
    }
    return NULL;
}
//modify command miris
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
    strncpy(edge->date, newDate, 10);
    return 1;
}
//find command miris
void printEdges(Graph graph, int id){
    if (!node_exists(graph, id)){
        printf("Node: %d does not exist\n", id);
        return;
    }
    Edge edge = returnEdge(find_node(graph, id));
    while (edge != NULL){
        printf("%d %d %d %s\n", id, edge->dest, edge->weight, edge->date);
        edge = edge->nextEdge;
    }
}
//αποδέσμευση linked list
void destroy_edges(Edge edge){
    Edge index;
    while(edge != NULL){
        index = edge;
        edge = edge->nextEdge;
        free(index);
    }
}

int returnEdgeDest(Edge edge){
    return edge->dest;
}

int returnEdgeWeight(Edge edge){
    return edge->weight;
}

char* returnEdgeDate(Edge edge){
    return edge->date;
}

Edge returnNextEdge(Edge edge){
    return edge->nextEdge;
}