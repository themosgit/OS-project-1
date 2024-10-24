#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.h"

//πίνακας με τα μεγέθη που παίρνει το hash table καθώς μεγαλώνει
int prime_sizes[] = {53, 97, 193, 389, 769, 1543, 3079, 6151, 12289, 24593, 49157, 98317, 196613, 393241,
                     786433, 1572869, 3145739, 6291469, 12582917, 25165843, 50331653, 100663319, 201326611, 402653189, 805306457, 1610612741};
//κατάσταση των nodes
typedef enum {
    EMPTY, OCCUPIED, DELETED
} State;

typedef enum {
    UNVISITED, VISITING, VISITED
} DFS;

#define MAX_LOAD_FACTOR 0.5


struct graph{  //Η δομή του πίνακα κατακερματισμού στον οποίο αποθηκεύεται ο γράφος
    GraphNode hash_table;
    int capacity;
    int capacity_index; //Δείκτης για τη θέση στον πίνακα prime sizes
    int size;
    HashFunc hash;

    GraphNode old_hash_table; // παλιός πίνακας κατακερματισμού
    int old_capacity;
    int old_size;
    int rehashing_index;
};

struct graph_node{ // δομή node
    int id;
    Edge edges;
    incomingNodes nodes; // βοηθητική δομή για τις εισερχόμενες ακμές
    State state;
    DFS dfs;
};

Graph graph_create(){ // δημιουργία πίνακα κατακερματισμού
    Graph graph = malloc(sizeof(*graph));
    graph->capacity_index = 0;
    graph->capacity = prime_sizes[0];
    graph->hash_table = malloc(graph->capacity * sizeof(struct graph_node));

    for (int i = 0; i < graph->capacity; i++) { // αρχικά δεν υπάρχουν nodes
        graph->hash_table[i].state = EMPTY;
    }
    graph->hash = test_hash;
    graph->size = 0;
    graph->old_hash_table = NULL;
    graph->old_capacity = 0;
    graph->old_size = 0;
    graph->rehashing_index = 0;

    return graph;
}


void graph_destroy(Graph graph) { // αποδέσμευση μνήμης και καταστροφή γράφου
    if (graph->old_capacity != 0) { // έαν υπάρχει παλιός πίνακας πρέπει να καταστραφεί
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

int insert_node(Graph graph, int id){ // εισαγωγή node

    GraphNode node = find_node(graph, id);
    if (node != NULL) return 0;

    increment_rehash(graph);// κάνουμε increment rehashing για κάθε εισαγωγή νέου node
    int pos;
    for (pos = graph->hash(id) % graph->capacity; //αναζητούμε την πρώτη διαθέσιμη θέση στο hash_table
        graph->hash_table[pos].state != EMPTY && graph->hash_table[pos].state != DELETED;
        pos = (pos + 1) % graph->capacity);
    node = &graph->hash_table[pos];
    node->state = OCCUPIED;
    node->id = id;
    node->edges = NULL;
    node->nodes = NULL;
    graph->size++;
    float load_factor = (float)(graph->size / graph->capacity);
    if (load_factor > MAX_LOAD_FACTOR){ // έαν ξεπεραστεί το load factor μεγαλώνουμε το μέγεθος του πίνακα
        hash_table_size_increase(graph);
    }
    return 1;
}

int remove_node(Graph graph, int id){ // διαγραφή node και αποδέσμευση μνήμης επιμέρους δομών
    GraphNode node = find_node(graph, id);
    if (node == NULL) return 0;
    Edge edge = node->edges;
    while(edge != NULL) { //διαγραφή ακμών το node αρχικά εξερχόμενων
        remove_edge(graph, id, returnEdgeDest(edge));
        edge = node->edges;
    }
    destroy_edges(node->edges);
    free(node->edges); //στη συνέχεια εισερχόμενων
    incomingNodes inNode = node->nodes;
    while(inNode != NULL){
        remove_edge(graph, returnInNodeId(inNode), id);
        inNode = node->nodes;
    }
    destroy_incomingNodes(node->nodes);
    free(node->nodes);
    node->state = DELETED; // θέτουμε state deleted για να αποφύγουμε λάθη κατά την αναζήτηση
    graph->size--;         //στη περίπτωση που έχει υπάρξει κάποιο collision
    return 1;
}


GraphNode find_node(Graph graph, int id){ // αναζήτηση node με linear probing
    int old_pos, old_stop;
    if (graph->old_capacity != 0){ // έαν υπάρχει παλιός πίνακας πρέπει να κοιτάξουμε και εκεί
        old_pos = graph->hash(id) % graph->old_capacity;
        old_stop = 0;
    } else{
        old_stop = 1;
    }
    int pos = graph->hash(id) % graph->capacity; //πηγαίνουμε κατευθείαν στη θέση που κάνει hash το node κοιτάμε και τους δύο πίνακες
    int stop = 0;
    int count = 0;
    while (!stop || !old_stop){  //μεταβλητές που σταματούν την αναζήτηση αν ξεπεραστεί το μέγεθος του πίνακα
        if (!old_stop) {          // ή βρεθεί κενή θέση
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

void hash_table_size_increase(Graph graph){ //λειτουργία επέκτασης hash table
    if (graph->old_capacity != 0){ // καταστρέφεται ο παλιός
        for (int i = 0; i < graph->old_capacity; i++) {
            if (graph->old_size == 0) break;
            if (graph->old_hash_table[i].state == OCCUPIED){
                remove_node(graph, graph->old_hash_table[i].id);
            }
        }
        free(graph->old_hash_table);
    }
    graph->old_size = graph->size; //αρχικοποίηση του παλιού με τα στοιχεία του τρέχον
    graph->old_capacity = graph->capacity;
    graph->old_hash_table = graph->hash_table;

    if (graph->capacity_index == sizeof(prime_sizes)){ //έαν έχει υπερβεί το μέγεθος της prime_sizes διπλασιάζουμε
        graph->capacity *= 2;
    } else {
        graph->capacity_index++;
        graph->capacity = prime_sizes[graph->capacity_index];
    }
    GraphNode new_hash_table = malloc(graph->capacity * sizeof(struct graph_node));
    graph->hash_table = new_hash_table;     //αρχικοποίηση νέου πίνακα
    for (int i = 0; i < graph->capacity; i++) {
        graph->hash_table[i].state = EMPTY;
    }
    graph->size = 0;
    graph->rehashing_index = 0;
}

void increment_rehash(Graph graph){ //rehash δύο θέσης του παλιού πίνακα βάση του rehashing_index
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

int node_exists(Graph graph, int id){
    GraphNode node = find_node(graph, id);
    if (node == NULL){
        return 0;
    } else{
        return 1;
    }
}


int test_hash(int value){
    return value;
}

int returnNodeID(GraphNode node){
    return node->id;
}

Edge returnEdge(GraphNode node){
    return node->edges;
}

void setEdge(Edge edge, GraphNode node){
    node->edges = edge;
}

incomingNodes returnInNode(GraphNode node){
    return node->nodes;
}

void setInNode(incomingNodes inNode, GraphNode node){
    node->nodes = inNode;
}



//συνάρτηση που γράφει στο output αρχείο τον γράφο
void write_graph_to_file(Graph graph, FILE* ptr){
    char str[50];
    Edge edge;
    if (graph->old_capacity != 0){ // ξεκινώντας απο το παλιό πίνακα εάν υπάρχει
        for (int i = 0; i < graph->old_capacity; ++i) {
            if (graph->old_hash_table[i].state == OCCUPIED){
                edge = graph->old_hash_table[i].edges;
                while(edge != NULL) {
                    sprintf(str, "%d %d %d %s\n", graph->old_hash_table[i].id, returnEdgeDest(edge), returnEdgeWeight(edge), returnEdgeDate(edge));
                    fputs(str, ptr);
                    edge = returnNextEdge(edge);
                }
            }
        }
    } // συνεχίζοντας στον τρέχον
    for (int i = 0; i < graph->capacity; ++i) {
        if (graph->hash_table[i].state == OCCUPIED){
            edge = graph->hash_table[i].edges;
            while(edge != NULL) {
                sprintf(str, "%d %d %d %s\n", graph->hash_table[i].id, returnEdgeDest(edge), returnEdgeWeight(edge), returnEdgeDate(edge));
                fputs(str, ptr);
                edge = returnNextEdge(edge);
            }
        }
    }
}

void circleFind_traceflow_connected(Graph graph, int startNode, int var, int mode) {
    int *stack;
    if (mode == 0) stack = malloc(graph->capacity * sizeof(int));
    int* path = malloc(graph->capacity * sizeof(int));
    for (int i = 0; i < graph->capacity; i++) {
        if (mode == 0) stack[i] = 0;
        path[i] = -1;
        if (i < graph->old_capacity) {
            graph->old_hash_table[i].dfs = UNVISITED;
        }
        graph->hash_table[i].dfs = UNVISITED;
    }
    if (mode == 0) dfs_circleFind(graph, find_node(graph, startNode), stack, path, 0, startNode, var);
    else if (mode == 1) dfs_traceFlow(graph, find_node(graph, startNode), path, 0, var);
    else if (mode == 2) dfs_connected(graph, find_node(graph, startNode), find_node(graph, var), path, 0);
    free(stack);
    free(path);
}

int dfs_circleFind(Graph graph, GraphNode node, int* stack, int* path, int index, int startNode, int minsum) {
    if (node->dfs == VISITING) {
        printf("Cycle found: ");
        for (int i = index - 1; i >= 0; --i) {
            printf("%d ", path[i]);
            if (path[i] == startNode) {
                printf("\n");
                return 1;
            }
        }
    } else if (node->dfs == VISITED)  return 0;

    node->dfs = VISITING;
    stack[node->id] = 1;
    path[index] = node->id;
    Edge edge = node->edges;
    while (edge != NULL) {
        GraphNode nextNode = find_node(graph, returnEdgeDest(edge));
        if (returnEdgeWeight(edge) >= minsum) {
            if (dfs_circleFind(graph, nextNode, stack, path, index + 1, startNode, minsum)) return 1;
        }
        edge = returnNextEdge(edge);
    }

    node->dfs = VISITED;
    stack[node->id] = 0;
    return 0;
}


void dfs_traceFlow(Graph graph, GraphNode node, int* path, int index, int depth) {
    if (index > depth) return;
    path[index] = node->id;
    if (index == depth) {
        printf("Path: ");
        for (int i = 0; i <= index; i++) {
            printf("%d ", path[i]);
        }
        printf("\n");
    }
    Edge edge = node->edges;
    while (edge != NULL) {
        GraphNode nextNode = find_node(graph, returnEdgeDest(edge));
        dfs_traceFlow(graph, nextNode, path, index + 1, depth);
        edge = returnNextEdge(edge);
    }
}

int dfs_connected(Graph graph, GraphNode node, GraphNode destNode, int* path, int index){
    path[index] = node->id;
    if (node == destNode){
        printf("Connected path: ");
        for (int i = 0; i <= index; i++) {
            printf("%d ", path[i]);
        }
        printf("\n");
        return 1;
    }
    Edge edge = node->edges;
    while (edge != NULL) {
        GraphNode node = find_node(graph, returnEdgeDest(edge));
        if (dfs_connected(graph, node, destNode, path, index + 1)) return 1;
        edge = returnNextEdge(edge);
    }
    return 0;
}
