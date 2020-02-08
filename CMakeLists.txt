cmake_minimum_required(VERSION 3.15)
project(C_GameOfLife_Coursework C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(C_GameOfLife_Coursework
        gameoflife.c
        gol.c
        gol.h)
