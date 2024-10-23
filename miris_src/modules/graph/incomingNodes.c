
struct incoming_nodes{
    int id;
    int occurrences;
    incomingNodes next;
};


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