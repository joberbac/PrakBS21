cmake_minimum_required(VERSION 3.7.2)
project(PrakBS21 C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS -pthread)

add_executable(PrakBS21 main.c sub.c keyValStore.c)
