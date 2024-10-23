#pragma once


typedef struct graph* Graph;

typedef struct graph_node* GraphNode;

typedef struct edge* Edge;

typedef struct incoming_nodes* incomingNodes;


typedef int (*HashFunc)(int);

Graph graph_create();

void hash_table_size_increase(Graph graph);

void increment_rehash(Graph graph);

GraphNode find_node(Graph graph, int id);

void graph_destroy(Graph graph);

void addEdge(GraphNode node, Edge edge);

Edge headEdge(GraphNode node);



int test_hash(int Value);

int node_exists(Graph graph, int id);

Edge search_edge(Graph graph,  int src, int dest, int weight, char* date);

void addIncoming(GraphNode destNode, int src_of_edge);

void removeIncoming(GraphNode destNode, int src_of_edge);

incomingNodes incomingNodes_find(GraphNode node, int target_id);

void destroy_incomingNodes(incomingNodes node);

void destroy_edges(Edge edge);


int insert_node(Graph graph, int id);

void insert_edge(Graph graph, int src, int dest, int weight, char* date);

int remove_node(Graph graph, int id);

int remove_edge(Graph, int src, int dest);

int modify(Graph graph, int src, int dest, int weight, int newWeight, char* date, char* newDate);

void printEdges(Graph graph, int id);

void printIncomingEdges(Graph graph, int dest_id);



