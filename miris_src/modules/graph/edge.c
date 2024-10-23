#include "graph.h"

struct edge{
    Edge nextEdge;
    int dest;
    int weight;
    char date[10];
};

void insert_edge(Graph graph, int src, int dest, int weight, char* date){
    if (!node_exists(graph, src)){
        insert_node(graph, src);
    }
    if (!node_exists(graph, dest)){
        insert_node(graph, dest);
    }
    Edge edge = malloc(sizeof(struct edge));
    strcpy(edge->date, date);
    edge->weight = weight;
    edge->dest = dest;
    GraphNode node = find_node(graph, src);
    edge->nextEdge = node->edges;
    node->edges = edge;
    node = find_node(graph,dest);
    addIncoming(node, src);

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

    Edge currentEdge = find_node(graph, src)->edges;
    if(currentEdge == NULL) return;

    if(currentEdge->nextEdge == NULL) {
        find_node(graph, src)->edges = NULL;
        removeIncoming(find_node(graph, dest), src);
        free(currentEdge);
        return;
    }


    if (currentEdge->dest == dest){
        find_node(graph, src)->edges = currentEdge->nextEdge;
        removeIncoming(find_node(graph, dest), src);
        free(currentEdge);
        return;
    }

    Edge previousEdge = currentEdge;
    currentEdge = currentEdge->nextEdge;
    while(currentEdge != NULL){
        if (currentEdge->dest == dest){
            previousEdge->nextEdge = currentEdge->nextEdge;
            removeIncoming(find_node(graph, dest), src);
            free(currentEdge);
            return;
        } else {
            previousEdge = currentEdge;
            currentEdge = currentEdge->nextEdge;
        }
    }
}

Edge search_edge(Graph graph,  int src, int dest, int weight, char* date){
    if (!node_exists(graph, src) || !node_exists(graph, dest)) return NULL;
    Edge index = find_node(graph, src)->edges;

    while(index != NULL){
        if (index->dest == dest && index->weight == weight
            && strcmp(index->date, date) == 0)  {
            return index;
        }
        index = index->nextEdge;
    }
    return NULL;
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
    strncpy(edge->date, newDate, 10);
    return 1;
}

void printEdges(Graph graph, int id){
    if (!node_exists(graph, id)){
        printf("Node: %d does not exist\n", id);
        return;
    }
    Edge edge = find_node(graph, id)->edges;
    while (edge != NULL){
        printf("%d %d %d %s\n", id, edge->dest, edge->weight, edge->date);
        edge = edge->nextEdge;
    }
}
