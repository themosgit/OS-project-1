cmake_minimum_required(VERSION 3.29)
project(k22_1 C)

set(CMAKE_C_STANDARD 11)

add_executable(k22_1
        miris/modules/main.c
        miris/modules/prompt.c
        miris/include/helper.h
        miris/include/graph.h
        miris/modules/node.c
        miris/modules/edge.c

)
