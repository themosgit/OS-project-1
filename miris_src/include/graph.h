#pragma once

typedef struct graph* Graph;

typedef struct edge* Edge;

typedef struct graph_node* GraphNode;

typedef struct incoming_nodes* incomingNodes;

typedef int (*HashFunc)(int);

//node.c Funcs

Graph graph_create();

void graph_destroy(Graph graph);

int insert_node(Graph graph, int id);

int remove_node(Graph graph, int id);

GraphNode find_node(Graph graph, int id);

void hash_table_size_increase(Graph graph);

void increment_rehash(Graph graph);

int test_hash(int Value);

int node_exists(Graph graph, int id);

Edge returnEdge(GraphNode node);

void setEdge(Edge edge, GraphNode node);

int returnNodeID(GraphNode node);

incomingNodes returnInNode(GraphNode node);

void setInNode(incomingNodes inNode, GraphNode node);

void write_graph_to_file(Graph graph, FILE* ptr);

//edge.c funcs


void insert_edge(Graph graph, int src, int dest, int weight, char* date);

void remove_edge(Graph graph, int src, int dest);

Edge search_edge(Graph graph,  int src, int dest, int weight, char* date);

int modify(Graph graph, int src, int dest, int weight, int newWeight, char* date, char* newDate);

void printEdges(Graph graph, int id);

void destroy_edges(Edge edge);


int returnEdgeDest(Edge edge);

int returnEdgeWeight(Edge edge);

char* returnEdgeDate(Edge edge);

Edge returnNextEdge(Edge edge);


// inNodes.c Funcs


void addIncoming(GraphNode destNode, int src_of_edge);

void removeIncoming(GraphNode destNode, int src_of_edge);

incomingNodes incomingNodes_find(GraphNode node, int target_id);

void printIncomingEdges(Graph graph, int dest_id);

void destroy_incomingNodes(incomingNodes node);

int returnInNodeId(incomingNodes inNode);


