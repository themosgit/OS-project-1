#pragma once
typedef struct edge* Edge;

typedef struct graph* Graph;

typedef struct graph_node* GraphNode;

typedef int (*HashFunc)(int);

Graph graph_create();

void insert_node(Graph graph, int id);
void remove_node(Graph graph, int id);
void hash_table_size_increase(Graph graph);
void increment_rehash(Graph graph);
void graph_destroy(Graph graph);

GraphNode find_node(Graph graph, int id);

int test_hash(int Value);
