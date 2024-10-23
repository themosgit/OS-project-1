#pragma once
#include "graph.h"
int commandPrompt(char* command, Graph graph);
int get_int(char** command);
void write_graph_to_file(Graph graph, FILE* ptr);