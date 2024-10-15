cmake_minimum_required(VERSION 3.29)
project(k22_1 C)

set(CMAKE_C_STANDARD 11)

add_executable(k22_1
        miris/main.c
        miris/helper.h
        miris/prompt.c
        miris/helper.c)
