//
// Created by Themos Papatheofanous on 14/10/24.
//
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


int commandPrompt(char* command) {
    char cmd[11], date[10], date1[10];
    int node, node1, sum, sum1, k, m;

    command[strlen(command) - 1] = '\0';
    sscanf(command, "%s", cmd);
    command += strlen(cmd) + 1;

    if (!strcmp(cmd,"i") || !strcmp(cmd, "insert")) {       //insert into the graph structure 1 or more nodes
        node = get_int(&command);                           // with specific INT ids.
        while (node != -1) {
            if (node) {
                //insert_node()
                printf("%d ", node);
            }
            node = get_int(&command);
        }
        printf("\n");

    } else if (!strcmp(cmd,"n") || !strcmp(cmd, "insert2")) {       //insert2
        node = get_int(&command);
        node1 = get_int(&command);
        sum = get_int(&command);
        sscanf(command, "%s", date);
        if ((node && node1 && sum) &&
            (date[4] == '-' && date[7] == '-')) {
            //insert_edge()
        } else {
            printf("Incorrect input\n");
        }

    } else if (!strcmp(cmd,"d") || !strcmp(cmd, "delete")) {        //delete
        node = get_int(&command);
        while (node != -1) {
            if (node) {
                //remove_node()
                printf("%d ", node);
            }
            node = get_int(&command);
        }
        printf("\n");

    } else if (!strcmp(cmd,"l") || !strcmp(cmd, "delete2")) {       //delete2
        node = get_int(&command);
        node1 = get_int(&command);
        if (node && node1) {
            //remove_edge()
        }
        printf("delete2\n");

    } else if (!strcmp(cmd,"m") || !strcmp(cmd, "modify")) {        //modify
        node = get_int(&command);
        node1 = get_int(&command);
        sum = get_int(&command);
        sum1 = get_int(&command);
        sscanf(command, "%s", date);
        sscanf(command, "%s", date1);
        if ((node && node1 && sum && sum1) &&
            (date[4] == '-' && date[7] == '-') &&
            (date1[4] =='-' && date[7] == '-')) {
            //change_edge()
        }
        printf("modify\n");

    } else if (!strcmp(cmd,"f") || !strcmp(cmd, "find")) {          //find
        node = get_int(&command);
        if (node){
            //find_outgoing_edges()
        }
        printf("find\n");

    } else if (!strcmp(cmd,"r") || !strcmp(cmd, "receiving")) {     //receiving
        node = get_int(&command);
        if (node){
            //find_incoming_edges()
        }
        printf("receiving\n");

    } else if (!strcmp(cmd,"c") || !strcmp(cmd, "circlefind")) {    //circlefind
        node = get_int(&command);
        if (node) {
            //find_circle()
        }
        printf("circleFind\n");

    } else if (!strcmp(cmd,"fi") || !strcmp(cmd, "findCircles")) {  //findCircles
        node = get_int(&command);
        k = get_int(&command);
        if (node && k){
            //find_circles()
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
        printf("exiting..");
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