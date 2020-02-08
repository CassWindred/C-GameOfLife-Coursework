//
// Created by TomCass on 07/02/2020.
//
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include"gol.h"


//TODO: Properly handle inccorectly formatted input files and files without a newline at the end
void read_in_file(FILE *infile, struct universe *u) {
    printf("READING IN FILE!\n");
    bool widthfound = false;
    int width = 0;
    int height = 0;
    int cellsize = 1024; //The initial size of the cell array
    bool *cells = malloc(sizeof(bool) * cellsize); //Define an array with room for cellsize bools
    char currentchar = ' ';
    int currentindex = 0;
    printf("Preparing to scan file\n");

    rewind(infile);
    while ((currentchar = getc(infile)) != EOF) {
        if (currentchar == '\n') {
            //printf("Maybe? New line?");
            height++;
            widthfound = true;
        } else {
            if (widthfound == false) {
                //printf("Increasing width!");
                width++;
            }
            if (currentindex >= cellsize) {
                cellsize = cellsize * 2;
                realloc(cells, sizeof(bool) * cellsize);
            }
            if (currentchar == '.') {
                cells[currentindex] = false;
            } else if (currentchar == '*') {
                cells[currentindex] = true;
            } else {
                printf("Illegal character %c in input file, please input a correctly formatted file", currentchar);
                printf("Closing Program");
                exit(1);
            }
            currentindex++;
        }
    }


    printf("Width is %d, Height is %d", width, height);
    (*u).cells = cells;
    (*u).sizex = width;
    (*u).sizey = height;

}

