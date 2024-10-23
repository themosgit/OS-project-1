#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/helper.h"

int main(int argc, char* argv[]){
    if(argc != 5 || strcmp(argv[1], "-i") || strcmp(argv[3], "-o")){
        printf("Incorrect arguments\nEXPECTED: ./miris -i inputfile -o outfile\n");
        exit(1);
    }
    FILE *input = fopen(argv[2], "r");
    if (input == NULL){
        printf("Input file does not exist\n");
        exit(1);
    }
    FILE *output = fopen(argv[4], "w");
    if (output == NULL){
        printf("Ouput file could not be opened\n");
        exit(1);
    }

    Graph graph = graph_create();
    char str[40], date[10];
    char* stri;
    while (fgets(str, 40, input) != NULL){
        stri = str;
        int src = get_int(&stri);
        int dest = get_int(&stri);
        int sum = get_int(&stri);
        sscanf(stri, "%s", date);
        insert_edge(graph, src, dest, sum, date);
    }

    fclose(input);
    char command[50];
    do{
        printf("miris $");
        fgets(command, sizeof(command), stdin);
    }while(commandPrompt(command, graph)); // on exit the commandPrompt function returns 0

    write_graph_to_file(graph, output);
    fclose(output);
    return 0;




}

