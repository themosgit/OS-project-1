#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "helper.h"


int prime_sizes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
                     786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

typedef enum {
    EMPTY, OCCUPIED, DELETED
} State;

typedef enum {
    UNVISITED, VISITING, VISITED
} DFS;
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
    DFS status;
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
                remove_node(graph, graph->hash_table[i].id);
            }
        }
        free(graph->old_hash_table);
    }

    for (int i = 0; i < graph->capacity; i++) {
        if (graph->size == 0) break;
        if (graph->hash_table[i].state == OCCUPIED){
            remove_node(graph, graph->hash_table[i].id);
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
            if (count == graph->old_capacity || graph->old_hash_table[old_pos].state == EMPTY) old_stop = 1;
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
    Edge edge = node->edges;
    while(edge != NULL) {
        remove_edge(graph, id, (int)edge->dest);
        edge = node->edges;
    }
    free(node->edges);
    incomingNodes inNode = node->nodes;
    while(inNode != NULL){
        remove_edge(graph, inNode->id, id);
        inNode = node->nodes;
    }
    free(node->edges);
    node->state = DELETED;
    graph->size--;
    return 1;
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

void write_graph_to_file(Graph graph, FILE* ptr){
    char str[50];
    Edge edge;
    if (graph->old_capacity != 0){
        for (int i = 0; i < graph->old_capacity; ++i) {
            if (graph->old_hash_table[i].state == OCCUPIED){
                edge = graph->old_hash_table[i].edges;
                while(edge != NULL) {
                    sprintf(str, "%d %d %d %s\n", graph->old_hash_table[i].id, edge->dest, edge->weight, edge->date);
                    fputs(str, ptr);
                    edge = edge->nextEdge;
                }
            }
        }
    }
    for (int i = 0; i < graph->capacity; ++i) {
        if (graph->hash_table[i].state == OCCUPIED){
            edge = graph->hash_table[i].edges;
            while(edge != NULL) {
                sprintf(str, "%d %d %d %s\n", graph->hash_table[i].id, edge->dest, edge->weight, edge->date);
                fputs(str, ptr);
                edge = edge->nextEdge;
            }
        }
    }
}

int dfs(Graph graph, GraphNode node, int* stack, int* path, int index, int startNode){
    if (node->status == VISITING) {
        printf("Cycle found: ");
        for (int i = index; i >= 0; ++i) {
            printf("%d ", path[i]);
            if (path[i] == startNode) {
                printf("\n");
                return 1;
            }
        }
    } else if (node->status == VISITED) return 0;

    node->status = VISITING;
    stack[node->id] = 1;
    path[index] = node->id;

    Edge edge = node->edges;
    while (edge != NULL){
        index++;
        GraphNode nextNode = find_node(graph, edge->dest);
        if (dfs(graph, nextNode, stack, path, index , startNode)) return 1;
        edge = edge->nextEdge;
    }

    node->status =  VISITED;
    stack[node->id] = 0;
    return 0;
}

void circleFind(Graph graph, int startNode){
    int stack[10000] = {0};
    int path[10000];
    memset(path, -1, sizeof(path));

    for(int i = 0; i < graph->capacity; i ++){
        graph->hash_table[i].status = UNVISITED;
    }

    dfs(graph, find_node(graph, startNode), stack, path, 0, startNode);
}
