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


    //printf("Width is %d, Height is %d", width, height);
    (*u).cells = cells;
    (*u).columns = width;
    (*u).rows = height;

}

coordinate *getneighbours(int column, int row) {
    coordinate *coordinates = malloc(sizeof(coordinate) * 8);

    coordinates[0].column = row + 1;
    coordinates[0].row = column + 0;

    coordinates[1].column = row + 1;
    coordinates[1].row = column + 1;

    coordinates[2].column = row + 1;
    coordinates[2].row = column + -1;

    coordinates[3].column = row + 0;
    coordinates[3].row = column + 1;

    coordinates[4].column = row + 0;
    coordinates[4].row = column + -1;

    coordinates[5].column = row + -1;
    coordinates[5].row = column + 0;

    coordinates[6].column = row + -1;
    coordinates[6].row = column + -1;

    coordinates[7].column = row + -1;
    coordinates[7].row = column + 1;

    return coordinates;
}

int is_alive(struct universe *u, int column, int row) {

    if (column >= u->columns || row >= u->rows) {
        printf("Nonexistent cell position requested, returning false");
        return false;
    }

    int position = (column + (row * u->columns));
    return u->cells[position];


    //if (u->cells[position] == true)
}

int will_be_alive(struct universe *u, int column, int row) {
    coordinate *neighbourcoordinates = getneighbours(column, row);
    bool currentstate = is_alive(u, column, row);

    int aliveneighbours = 0;

    for (int i = 0; i < 8; ++i) {
        if (is_alive(u, neighbourcoordinates[i].column, neighbourcoordinates[i].row) == true) {
            aliveneighbours++;
        }
    }
    if ((currentstate == true && (aliveneighbours == 2 || aliveneighbours ==3))
                            || (currentstate == false && aliveneighbours == 3)) {
        return true;
    } else {
        return false;
    }
}

int will_be_alive_torus(struct universe *u,  int column, int row) {
    coordinate *neighbourcoordinates = getneighbours(column, row);
    bool currentstate = is_alive(u, column, row);

    column = column % u->columns;
    row = row % u->rows;

    int aliveneighbours = 0;

    for (int i = 0; i < 8; ++i) {
        if (is_alive(u, neighbourcoordinates[i].column, neighbourcoordinates[i].row) == true) {
            aliveneighbours++;
        }
    }
    if ((currentstate == true && (aliveneighbours == 2 || aliveneighbours ==3))
        || (currentstate == false && aliveneighbours == 3)) {
        return true;
    } else {
        return false;
    }

}