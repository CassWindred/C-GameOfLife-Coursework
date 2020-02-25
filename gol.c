//
// Created by TomCass on 07/02/2020.
//
#include<stdio.h>
#include <stdlib.h>
#include"gol.h"

typedef struct coordinate {
    int column, row;
} coordinate;

double percent_cells_alive(struct universe *u) {
    double alivecells = 0;
    double totalcells = (u->columns * u->rows);
    for (int i = 0; i < (totalcells); ++i) {
        if (u->cells[i] == true) {
            alivecells++;
        }
    }
    return (alivecells / totalcells) * 100;
}

void read_in_file(FILE *infile, struct universe *u) {
    //printf("READING IN FILE!\n");
    bool widthfound = false;
    int width = 0;
    int height = 0;
    int cellsize = 1024; //The initial size of the cell array
    bool *cells = malloc(sizeof(bool) * cellsize); //Define an array with room for cellsize bools
    if (cells == NULL) {
        perror("Fatal Malloc Error, Failed to allocate memory for cells\n");
        exit(20);
    }
    char currentchar = ' ';
    int currline = 1;
    int currentindex = 0;
    int linewidth = 0;
    bool linestarted = true;
    //printf("Preparing to scan file\n");

    rewind(infile);
    while ((currentchar = getc(infile)) != EOF) {
        if (currentchar == '\n') {
            //printf("Maybe? New line?");
            if (linestarted == true) {
                if (currentindex == 0) {
                    fprintf(stderr, "Empty First Line Error: \n"
                                    "The first line cannot be empty.\n");
                    exit(31);
                }
                fprintf(stderr, "Double Empty Line Error: \n"
                                "Line %d is the second empty line in a row, this is not a valid input, "
                                "only the final line in a file may be empty.\n", currline);
                exit(32);
            }

            if (widthfound == false) {
                if (width>512) {
                    fprintf(stderr, "Too Many Columns Error: \n"
                                    "There cannot be more than 512 collumns in input.\n");
                    exit(50);
                }
                width = linewidth;
                widthfound = true;
            } else if (linewidth != width) {
                    fprintf(stderr, "Incorrect Line Width in Input: \n"
                                    "Line %d is %d characters long when width has been established to be %d characters long\n", currline,
                            linewidth, width);
                    exit(30);
                }

            linestarted = true;
            currline++;
            linewidth = 0;

        } else {
            linestarted = false;
            linewidth++;
            if (widthfound == false) {
                //printf("Increasing width!");
                width++;
            }
            if (currentindex >= cellsize) {
                cellsize = cellsize * 2;
                cells = realloc(cells, sizeof(bool) * cellsize);
                if (cells == NULL) {
                    perror("Fatal Realloc Error, Failed to re-allocate memory for cells\n");
                    exit(22);
                }

            }
            if (currentchar == '.') {
                cells[currentindex] = false;
            } else if (currentchar == '*') {
                cells[currentindex] = true;
            } else {
                fprintf(stderr, "Illegal character \"%c\" in input file, please input a correctly formatted file.\n",
                        currentchar);
                exit(10);
            }
            currentindex++;
        }
    }
    if (currentindex == 0) {
        fprintf(stderr, "Empty Input Error: \n"
                        "Input cannot be empty, please enter a valid input.\n");
        exit(31);
    }
    else if (linewidth == 0) { //If line is empty it should not be included in the total height
        height = currline - 1;
    } else if (linewidth == width) { //If line is the correct width it should be included in the total height
        height = currline;
    } else { //Line is neither empty nor the correct width, error!
        fprintf(stderr, "Incorrect Line Width in Input: \n"
                        "Final line is %d characters long when it should be %d characters long or empty\n", linewidth,
                width);
        exit(31);
    }
    //printf("Width is %d, Height is %d", width, height);
    (*u).cells = cells;
    (*u).columns = width;
    (*u).rows = height;
    (*u).generations = 1;
    (*u).averagealive = percent_cells_alive(u);

}


void write_out_file(FILE *outfile, struct universe *u) {
    //printf("Writing to file...\n");
    for (int row = 0; row < u->rows; ++row) {
        for (int column = 0; column < u->columns; ++column) {
            int pos = (row * u->columns) + column;
            char currchar;
            if (u->cells[pos] == true) {
                currchar = '*';
            } else {
                currchar = '.';
            }
            if (fputc(currchar, outfile) == EOF) {
                fprintf(stderr, "Failed to write \"%c\" to file, check permissions are correct\n", currchar);
                exit(11);
            }
        }
        if (fputc('\n', outfile) == EOF) {
            fprintf(stderr, "Failed to write newline to file, check permissions are correct\n");
            exit(11);
        }
    }
}

//Return a pointer to a block of 8 coordinates that each represent a neighbour to the input column and row location
coordinate *getneighbours(int column, int row) {
    coordinate *coordinates = malloc(sizeof(coordinate) * 8);
    if (coordinates == NULL) {
        perror("Fatal Malloc Error, Failed to allocate memory for neighbour coordinates\n");
        exit(21);
    }

    coordinates[0].column = column + 1;
    coordinates[0].row = row + 0;

    coordinates[1].column = column + 1;
    coordinates[1].row = row + 1;

    coordinates[2].column = column + 1;
    coordinates[2].row = row + -1;

    coordinates[3].column = column + 0;
    coordinates[3].row = row + 1;

    coordinates[4].column = column + 0;
    coordinates[4].row = row + -1;

    coordinates[5].column = column + -1;
    coordinates[5].row = row + 0;

    coordinates[6].column = column + -1;
    coordinates[6].row = row + -1;

    coordinates[7].column = column + -1;
    coordinates[7].row = row + 1;

    return coordinates;
}

int is_alive(struct universe *u, int column, int row) {

    if (column >= u->columns || column < 0 || row >= u->rows || row < 0) {
        //printf("Nonexistent cell position (%d, %d) requested, returning false", column, row);
        return false;
    }

    int position = (column + (row * u->columns));

    //printf("Checking position %i for column %i and row %i\n", position, column, row);

    return u->cells[position];


    //if (u->cells[position] == true)
}

int will_be_alive(struct universe *u, int column, int row) {
    coordinate *neighbourcoordinates = getneighbours(column, row);
    bool currentstate = is_alive(u, column, row);

    int aliveneighbours = 0;
    //Count how many neighbour cells are alive
    for (int i = 0; i < 8; ++i) {
        if (is_alive(u, neighbourcoordinates[i].column, neighbourcoordinates[i].row) == true) {
            aliveneighbours++;
        }
    }
    free(neighbourcoordinates);

    //Match against rules
    if ((currentstate == true && (aliveneighbours == 2 || aliveneighbours == 3))
        || (currentstate == false && aliveneighbours == 3)) {
        return true;
    } else {
        return false;
    }
}

int will_be_alive_torus(struct universe *u, int column, int row) {
    coordinate *neighbourcoordinates = getneighbours(column, row);
    bool currentstate = is_alive(u, column, row);

    int neighbourcolumn = 0;
    int neighbourrow = 0;

    //Count how many neighbour cells are alive
    int aliveneighbours = 0;
    for (int i = 0; i < 8; ++i) {
        neighbourcolumn = neighbourcoordinates[i].column % u->columns;
        neighbourrow = neighbourcoordinates[i].row % u->rows;
        if (neighbourcolumn < 0) {
            neighbourcolumn = u->columns + neighbourcolumn;
        }
        if (neighbourrow < 0) {
            neighbourrow = u->rows + neighbourrow;
        }
        //printf("COLUMN %d, ROW %d  -  ", neighbourcolumn, neighbourrow);
        if (is_alive(u, neighbourcolumn, neighbourrow) == true) {
            aliveneighbours++;
        }
    }
    free(neighbourcoordinates);


    //Match against rules
    if ((currentstate == true && (aliveneighbours == 2 || aliveneighbours == 3))
        || (currentstate == false && aliveneighbours == 3)) {
        return true;
    } else {
        return false;
    }

}

void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row)) {
    int positioncount = u->columns * u->rows;
    bool newpositions[positioncount];
    for (int column = 0; column < u->columns; ++column) {
        for (int row = 0; row < u->rows; ++row) {
            newpositions[(row * u->columns) + column] = (*rule)(u, column, row);
        }
    }

    for (int i = 0; i < positioncount; ++i) {
        u->cells[i] = newpositions[i];
    }

    u->generations++;
    u->averagealive = (((u->generations - 1.0) / u->generations) * u->averagealive) +
                      ((1.0 / u->generations) * percent_cells_alive(u));

    //printf("Evolution Complete, freeing newposition memory\n");
    //free(newpositions);
    //printf("Newposition freeing successful");
}

void print_statistics(struct universe *u) {
    printf("%.3f%% of cells currently alive\n", percent_cells_alive(u));
    printf("%.3f%% of cells alive on average\n", u->averagealive);
}

