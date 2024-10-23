#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/helper.h"

int get_int(char** command) {
    char* n = strtok(*command, " ");
    if (n == NULL){
        return -1;
    }
    *command += strlen(n) + 1;
    return (int)strtol(n, NULL, 10);
}



int commandPrompt(char* command, Graph graph) {
    char cmd[11], date1[10], date2[10];
    int node, node1, sum, sum1, k, m;

    command[strlen(command) - 1] = '\0';
    sscanf(command, "%s", cmd);
    command += strlen(cmd) + 1;

    if (!strcmp(cmd,"i") || !strcmp(cmd, "insert")) {       //insert into the graph structure 1 or more nodes
        node = get_int(&command);// with specific INT ids.
        if (node == -1){
            printf("EXPECTED: insert Ni [Nj Nk ...] with nodes of type int\n");
        } else {
            while (node != -1) {
                if(insert_node(graph, node)) printf("%d inserted\n", node);
                else printf("%d already in graph\n", node);
                node = get_int(&command);
            }
        }

    } else if (!strcmp(cmd,"n") || !strcmp(cmd, "insert2")) {       //insert2
        node = get_int(&command);
        node1 = get_int(&command);
        sum = get_int(&command);
        sscanf(command, "%s", date1);
        if ((node && node1 && sum) &&
            (date1[4] == '-' && date1[7] == '-')) {
            insert_edge(graph, node, node1, sum, date1);
            printf("Successfully inserted edge\n");
        } else {
            printf("EXPECTED: insert2 Ni Nj sum date\n with nodes and sum of type int and date YYYY-MM-DD\n");
        }

    } else if (!strcmp(cmd,"d") || !strcmp(cmd, "delete")) {        //delete
        node = get_int(&command);
        if (node == -1){
            printf("EXPECTED: delete Ni [Nj Nk ...] with nodes of type int\n");
        } else {
            while (node != -1) {
                if (remove_node(graph, node)) printf("%d deleted\n", node);
                else printf("%d not in graph\n", node);
                node = get_int(&command);
            }
        }

    } else if (!strcmp(cmd,"l") || !strcmp(cmd, "delete2")) {       //delete2
        node = get_int(&command);
        node1 = get_int(&command);
        if (node != -1 && node1 != -1){
            remove_edge(graph, node, node1);
            printf("Successfully deleted edge\n");
        } else{
            printf("EXPECTED: delete2 Ni Nj\n With Ni Nj of type int\n");
        }

    } else if (!strcmp(cmd,"m") || !strcmp(cmd, "modify")) {        //modify
        node = get_int(&command);
        node1 = get_int(&command);
        sum = get_int(&command);
        sum1 = get_int(&command);
        sscanf(command, "%s %s", date1, date2);
        date1[0] = '2';
        if ((node && node1 && sum && sum1) &&
            (date1[4] == '-' && date1[7] == '-') &&
            (date2[4] =='-' && date2[7] == '-')) {
            if (modify(graph, node, node1, sum, sum1, date1, date2)){
                printf("Successfully modified edge\n");
            }
        } else {
            printf("EXPECTED: modify Ni Nj sum sum1 date date1\n with nodes and sums of type int and dates YYYY-MM-DD\n");
        }

    } else if (!strcmp(cmd,"f") || !strcmp(cmd, "find")) {          //find
        node = get_int(&command);
        if (node != -1){
            printEdges(graph, node);
        }

    } else if (!strcmp(cmd,"r") || !strcmp(cmd, "receiving")) {     //receiving
        node = get_int(&command);
        if (node){
            printIncomingEdges(graph, node);
        }

    } else if (!strcmp(cmd,"c") || !strcmp(cmd, "circlefind")) {    //circlefind
        node = get_int(&command);
        if (node) {
          //  circleFind(graph, node);
        }
        printf("circleFind\n");

    } else if (!strcmp(cmd,"fi") || !strcmp(cmd, "findCircles")) {  //findCircles
        node = get_int(&command);
        k = get_int(&command);
        if (node && k){
           //circleFind(graph,)
        }
        printf("findCircles\n");

    } else if (!strcmp(cmd,"ti1uh") || !strcmp(cmd, "traceflow")) { //traceflow
        node = get_int(&command);
        m = get_int(&command);
        if (node && m){
            //traceflow()
        }
        printf("traceFlow\n");
    } else if (!strcmp(cmd,"c") || !strcmp(cmd, "connected")) {     //connected
        node = get_int(&command);
        node1 = get_int(&command);
        if (node && node1) {
            //find_path()
        }
        printf("connected\n");

    } else if(!strcmp(cmd,"e") || !strcmp(cmd, "exit")) {            //exit
        printf("exiting...\n");
        return 0;
    } else {
        printf("Unrecognized command\n");
    }
    return 1;
}




/* else if (!strcmp(cmd,"h")) {
        char str[10];
        FILE *fp;
        fp = fopen("help.txt","r");
        while (fgets(str,10,fp)!=NULL){
            printf("%s",str);
        }
        fclose(fp);*/