#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"
struct incoming_nodes{
    int id;
    int occurrences;
    incomingNodes next;
};


void addIncoming(GraphNode destNode, int src_of_edge){
    incomingNodes inNode = incomingNodes_find(destNode, src_of_edge);
    if (inNode == NULL){
        inNode = malloc(sizeof(struct incoming_nodes));
        inNode->next = returnInNode(destNode);
        inNode->id = src_of_edge;
        inNode->occurrences = 1;
        setInNode(inNode, destNode);
    } else{
        inNode->occurrences++;
    }
}

void removeIncoming(GraphNode destNode, int src_of_edge){
    incomingNodes inNode =  incomingNodes_find(destNode, src_of_edge);
    inNode->occurrences--;
    if (inNode->occurrences > 0) return;
    if (returnInNode(destNode) == inNode) {
        setInNode(inNode->next, destNode);
        free(inNode);
        return;
    }
    incomingNodes inNode_prev = returnInNode(destNode);
    while(inNode_prev->next != inNode){
        inNode_prev = inNode_prev->next;
    }
    inNode_prev->next = inNode->next;
    free(inNode);
}

incomingNodes incomingNodes_find(GraphNode node, int target_id){
    incomingNodes index = returnInNode(node);
    while(index != NULL){
        if (index->id == target_id) return index;
        index = index->next;
    }
    return NULL;
}

void printIncomingEdges(Graph graph, int dest_id){
    GraphNode node = find_node(graph, dest_id);
    if (node == NULL){
        printf("Node: %d does not exist\n", dest_id);
        return;
    }
    incomingNodes index = returnInNode(node);
    Edge incomingEdge;
    while (index != NULL) {
        int i = index->occurrences;
        incomingEdge = returnEdge(find_node(graph, index->id));
        while(i > 0){
            if (incomingEdge == NULL) exit(66);
            if (returnEdgeDest(incomingEdge) == returnNodeID(node)){
                printf("%d %d %d %s\n", index->id, returnEdgeDest(incomingEdge), returnEdgeWeight(incomingEdge), returnEdgeDate(incomingEdge));
                i--;
            }
            incomingEdge = returnNextEdge(incomingEdge);
        }
        index = index->next;
    }
}

void destroy_incomingNodes(incomingNodes node){
    incomingNodes index;
    while(node != NULL){
        free(index);
        index = node;
        node = node->next;

    }
}

int returnInNodeId(incomingNodes inNode){
    return inNode->id;
}