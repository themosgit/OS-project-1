//
// Created by Themos Papatheofanous on 14/10/24.
//
#include <stdio.h>
#include "../include/helper.h"

int main(){

    char command[50];
    do{
        printf("miris $");
        fgets(command, sizeof(command), stdin);
    }while(commandPrompt(command)); // on exit the commandPrompt function returns 0



}