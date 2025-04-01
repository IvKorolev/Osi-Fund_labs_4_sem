cmake_minimum_required(VERSION 3.29)
project(OsiLabs C)

set(CMAKE_C_STANDARD 11)

add_executable(OsiLabs lab1/task1/main.c
        lab1/task7/main.c
        lab1/task2/main.c
        Avito/binaryTree.h
        Avito/binaryTree.c
        lab1/task1/test.c
        lab1/task7/test.c
)
