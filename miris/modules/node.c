#include <stdlib.h>
#include <stdio.h>
#include "../include/graph.h"


int prime_sizes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
                     786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};

typedef enum {
    EMPTY, OCCUPIED, DELETED
} State;

#define MAX_LOAD_FACTOR 0.5

struct graph_node{
    int id;
    Edge incomingEdges;
    Edge outgoingEdges;
    State state;
};

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

void insert_node(Graph graph, int id){
    GraphNode node = find_node(graph, id);
    if (node != NULL) {
        printf("Node: %d already in graph...\n", id);
        return;
    }
    increment_rehash(graph);
    int pos;
    for (pos = graph->hash(id) % graph->capacity;
        graph->hash_table[pos].state != EMPTY;
        pos = (pos + 1) % graph->capacity);
    node = &graph->hash_table[pos];
    node->state = OCCUPIED;
    node->id = id;
    graph->size++;

    float load_factor = (float)(graph->size / graph->capacity);
    if (load_factor > MAX_LOAD_FACTOR){
        hash_table_size_increase(graph);
        //ισως χρειαζεται και εδω ενα increment rehash
    }
}

void hash_table_size_increase(Graph graph){
    if (graph->old_capacity != 0){
        for (int i = 0; i < graph->old_capacity; i++) {
            if (graph->old_size == 0) break;
            if (graph->old_hash_table[i].state == OCCUPIED){
                free(graph->old_hash_table[i].incomingEdges);
                free(graph->old_hash_table[i].outgoingEdges);
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
            graph->hash_table[pos].incomingEdges = graph->old_hash_table[currentIndex].incomingEdges;
            graph->hash_table[pos].outgoingEdges = graph->old_hash_table[currentIndex].outgoingEdges;
            graph->hash_table[pos].state = OCCUPIED;
            graph->old_hash_table[currentIndex].state = EMPTY;
            graph->size++;
            graph->old_size--;
        }

    }
    graph->rehashing_index += 2;
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
            if (count == graph->old_capacity || graph->old_hash_table[old_pos].state == EMPTY) stop = 1;
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


void remove_node(Graph graph, int id){
    GraphNode node = find_node(graph, id);
    if (node == NULL){
        fprintf(stderr, "\nNode: %d does not exist..\n", id);
        return;
    }
    free(node->incomingEdges);
    free(node->outgoingEdges);
    node->state = EMPTY;
    graph->size--;
    //needs to remove incoming edgesa
}


void graph_destroy(Graph graph) {
    if (graph->old_capacity != 0) {
        for (int i = 0; i < graph->old_capacity; i++) {
            if (graph->old_size == 0) break;
            if (graph->old_hash_table[i].state == OCCUPIED) {
                free(graph->old_hash_table[i].incomingEdges);
                free(graph->old_hash_table[i].outgoingEdges);
            }
        }
        free(graph->old_hash_table);
    }
    for (int i = 0; i < graph->capacity; i++) {
        if (graph->size == 0) break;
        if (graph->hash_table[i].state == OCCUPIED){
            free(graph->hash_table[i].incomingEdges);
            free(graph->hash_table[i].outgoingEdges);
        }
    }
    free(graph->hash_table);
    free(graph);
}


int test_hash(int value){
    return value;
}