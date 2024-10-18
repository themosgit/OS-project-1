cmake_minimum_required(VERSION 3.29)
project(k22_1 C)

set(CMAKE_C_STANDARD 11)

add_executable(k22_1
        miris/modules/main.c
        miris/include/helper.h
        miris/modules/prompt.c
        miris/modules/graph.c
        miris/include/graph.h
)
