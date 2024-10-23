#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/helper.h"


int prime_sizes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
                     786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

typedef enum {
    EMPTY, OCCUPIED, DELETED
} State;

#define MAX_LOAD_FACTOR 0.5

struct graph{
    GraphNode hash_table;
    int capacity;
    int capacity_index;
    int size;
    HashFunc hash;

    GraphNode old_hash_table;
    int old_capacity;
    int old_size;
    int rehashing_index;
};

struct graph_node{
    int id;
    Edge edges;
    incomingNodes nodes;
    State state;
};

struct edge{
    Edge nextEdge;
    int dest;
    int weight;
    char date[10];
};

struct incoming_nodes{
    int id;
    int occurrences;
    incomingNodes next;
};



Graph graph_create(){
    Graph graph = malloc(sizeof(*graph));
    graph->capacity_index = 0;
    graph->capacity = prime_sizes[0];
    graph->hash_table = malloc(graph->capacity * sizeof(struct graph_node));

    for (int i = 0; i < graph->capacity; i++) {
        graph->hash_table[i].state = EMPTY;
    }
    graph->hash = test_hash;
    graph->size = 0;
    graph->old_hash_table = malloc(sizeof(struct graph_node));
    graph->old_capacity = 0;
    graph->old_size = 0;
    graph->rehashing_index = 0;

    return graph;
}

void hash_table_size_increase(Graph graph){
    if (graph->old_capacity != 0){
        for (int i = 0; i < graph->old_capacity; i++) {
            if (graph->old_size == 0) break;
            if (graph->old_hash_table[i].state == OCCUPIED){
                destroy_edges(graph->old_hash_table[i].edges);
                destroy_incomingNodes(graph->old_hash_table[i].nodes);
            }
        }
        free(graph->old_hash_table);
    }
    graph->old_size = graph->size;
    graph->old_capacity = graph->capacity;
    graph->old_hash_table = graph->hash_table;

    if (graph->capacity_index == sizeof(prime_sizes)){
        graph->capacity *= 2;
    } else {
        graph->capacity_index++;
        graph->capacity = prime_sizes[graph->capacity_index];
    }
    GraphNode new_hash_table = malloc(graph->capacity * sizeof(struct graph_node));
    graph->hash_table = new_hash_table;
    for (int i = 0; i < graph->capacity; i++) {
        graph->hash_table[i].state = EMPTY;
    }
    graph->size = 0;
    graph->rehashing_index = 0;
}

void increment_rehash(Graph graph){
    if (graph->old_size == 0) return;

    for (int currentIndex = graph->rehashing_index; currentIndex < graph->old_capacity; currentIndex++) {
        if (currentIndex == graph->rehashing_index + 2) break;
        if (graph->old_hash_table[currentIndex].state == OCCUPIED){
            int pos;
            for (pos = graph->hash(graph->old_hash_table[currentIndex].id) % graph->capacity;
                 graph->hash_table[pos].state != EMPTY;
                 pos = (pos + 1) % graph->capacity);
            graph->hash_table[pos].id = graph->old_hash_table[currentIndex].id;
            graph->hash_table[pos].edges = graph->old_hash_table[currentIndex].edges;
            graph->hash_table[pos].nodes = graph->old_hash_table[currentIndex].nodes;
            graph->hash_table[pos].state = OCCUPIED;
            graph->old_hash_table[currentIndex].state = DELETED;
            graph->size++;
            graph->old_size--;
        }

    }
    graph->rehashing_index += 2;
}

void graph_destroy(Graph graph) {
    if (graph->old_capacity != 0) {
        for (int i = 0; i < graph->old_capacity; i++) {
            if (graph->old_size == 0) break;
            if (graph->old_hash_table[i].state == OCCUPIED) {
                destroy_edges(graph->old_hash_table[i].edges);
                destroy_incomingNodes(graph->old_hash_table[i].nodes);
            }
        }
        free(graph->old_hash_table);
    }
    for (int i = 0; i < graph->capacity; i++) {
        if (graph->size == 0) break;
        if (graph->hash_table[i].state == OCCUPIED){
            destroy_edges(graph->hash_table[i].edges);
            destroy_incomingNodes(graph->hash_table[i].nodes);

        }
    }
    free(graph->hash_table);
    free(graph);
}

GraphNode find_node(Graph graph, int id){
    int old_pos, old_stop;
    if (graph->old_capacity != 0){
        old_pos = graph->hash(id) % graph->old_capacity;
        old_stop = 0;
    } else{
        old_stop = 1;
    }
    int pos = graph->hash(id) % graph->capacity;
    int stop = 0;
    int count = 0;
    while (!stop || !old_stop){
        if (!old_stop) {
            if (graph->old_hash_table[old_pos].state == OCCUPIED
                && graph->old_hash_table[old_pos].id == id) {
                return &graph->old_hash_table[old_pos];
            }
            old_pos = (old_pos + 1) % graph->old_capacity;
            if (count >= graph->old_capacity || graph->old_hash_table[old_pos].state == EMPTY) old_stop = 1;
        }
        if (!stop) {
            if (graph->hash_table[pos].state == OCCUPIED
                && graph->hash_table[pos].id == id) {
                return &graph->hash_table[pos];
            }
            pos = (pos + 1) % graph->capacity;
            if (count == graph->capacity || graph->hash_table[pos].state == EMPTY) stop = 1;
        }
        count++;
    }
    return NULL;
}

int insert_node(Graph graph, int id){

    GraphNode node = find_node(graph, id);
    if (node != NULL) return 0;

    increment_rehash(graph);
    int pos;
    for (pos = graph->hash(id) % graph->capacity;
        graph->hash_table[pos].state != EMPTY && graph->hash_table[pos].state != DELETED;
        pos = (pos + 1) % graph->capacity);
    node = &graph->hash_table[pos];
    node->state = OCCUPIED;
    node->id = id;
    node->edges = NULL;
    node->nodes = NULL;
    graph->size++;
    float load_factor = (float)(graph->size / graph->capacity);
    if (load_factor > MAX_LOAD_FACTOR){
        hash_table_size_increase(graph);
        //ισως χρειαζεται και εδω ενα increment rehash
    }
    return 1;
}

int remove_node(Graph graph, int id){
    GraphNode node = find_node(graph, id);
    if (node == NULL) return 0;
   // free(node->edges);
    //destroy_edges(node->edges);
    //destroy_incomingNodes(node->nodes);
    node->state = DELETED;
    graph->size--;
    return 1;
    //needs to remove incoming edgesa
}

void insert_edge(Graph graph, int src, int dest, int weight, char* date){
    if (!node_exists(graph, src)){
        insert_node(graph, src);
    }
    if (!node_exists(graph, dest)){
        insert_node(graph, dest);
    }
    printf("date give: %s\n", date);
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
    if(currentEdge == find_node(graph, src)->edges){
        find_node(graph, src)->edges = currentEdge->nextEdge;
        removeIncoming(find_node(graph, dest), src);
        free(currentEdge);
        return;
    }
    Edge previousEdge;
    while(currentEdge != NULL){
        previousEdge = currentEdge;
        if (currentEdge->dest == dest){
            previousEdge = currentEdge->nextEdge;
            removeIncoming(find_node(graph, dest), src);
            free(currentEdge);
            return;

        } else {
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
    printf("date give: %s\n", newDate);
    Edge edge = search_edge(graph, src, dest, weight, date);
    if (edge == NULL){
        printf("Edge does not exist\n");
        return 0;
    }
    edge->weight = newWeight;
    strcpy(edge->date, newDate);
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

void addIncoming(GraphNode destNode, int src_of_edge){
    incomingNodes inNode = incomingNodes_find(destNode, src_of_edge);
    if (inNode == NULL){
        inNode = malloc(sizeof(struct incoming_nodes));
        inNode->next = destNode->nodes;
        inNode->id = src_of_edge;
        inNode->occurrences = 1;
        destNode->nodes = inNode;
    } else{
        inNode->occurrences++;
    }
}

void removeIncoming(GraphNode destNode, int src_of_edge){
    incomingNodes inNode =  incomingNodes_find(destNode, src_of_edge);
    inNode->occurrences--;
    if (inNode->occurrences > 0) return;
    if (destNode->nodes == inNode) {
        destNode->nodes = inNode->next;
        free(inNode);
        return;
    }
    incomingNodes inNode_prev = destNode->nodes;
    while(inNode_prev->next != inNode){
        inNode_prev = inNode_prev->next;
    }
    inNode_prev->next = inNode->next;
    free(inNode);
}

incomingNodes incomingNodes_find(GraphNode node, int target_id){
    incomingNodes index = node->nodes;
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
    incomingNodes index = node->nodes;
    Edge incomingEdge;
    while (index != NULL) {
        int i = index->occurrences;
        incomingEdge = find_node(graph, index->id)->edges;
        while(i > 0){
            if (incomingEdge == NULL) exit(66);
            if (incomingEdge->dest == node->id){
                printf("%d %d %d %s\n", index->id,incomingEdge->dest, incomingEdge->weight, incomingEdge->date);
                i--;
            }
            incomingEdge = incomingEdge->nextEdge;
        }
        index = index->next;
    }
}

int test_hash(int value){
    return value;
}

int node_exists(Graph graph, int id){
    GraphNode node = find_node(graph, id);
    if (node == NULL){
        return 0;
    } else{
        return 1;
    }
}

void destroy_edges(Edge edge){
    Edge index;
    while(edge != NULL){
        index = edge;
        edge = edge->nextEdge;
        free(index);
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