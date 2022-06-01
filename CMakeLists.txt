cmake_minimum_required(VERSION 3.20)
project(Calculator C)

set(CMAKE_C_STANDARD 11)

add_executable(Calculator main.c functions.c functions.h parsing.c parsing.h var.h)
