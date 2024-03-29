#include<stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include"gol.h"


int main(int argc, char *argv[]) {
    //Initialise needed variables
    struct universe v;

    FILE *inpfile;
    FILE *outfile;

    bool (*will_be_alive_func)(struct universe *u, int column, int row) = &will_be_alive;
    bool printstats = false;
    long gencount = 5;
    long gencounttemp = 5;
    bool gencountset = false;

    char *inputfilename = NULL;
    char *outputfilename = NULL;

    char *endptr = NULL;

    //Parse the input arguments
    for (int arg = 1; arg < argc; ++arg) {
        if (argv[arg][0] == '-') { //If the first part of an argument is "-", assume its a switch (negative numbers are not real and they cannot hurt me)
            switch (argv[arg][1]) { //The character after the "-" will be the switch character, see which case it matches
                case 'i': //INPUT CATE
                    ++arg;
                    if (arg >= argc) {
                        fprintf(stderr,
                                "No valid argument provided for \"-i\" option (note: arguments cannot start with \"-\")\n");
                        exit(7);
                    } else if (inputfilename == NULL || inputfilename == argv[arg]) {
                        //printf("Setting input file to %s...\n", argv[arg]);
                        inputfilename = argv[arg];
                        break;
                    } else {
                        fprintf(stderr,
                                "Conflicting input arguments given, please run with valid arguments.\n");
                        exit(10);
                    }
                case 'o': //OUTPUT CASE
                    ++arg;
                    if (arg >= argc) {
                        fprintf(stderr,
                                "No valid argument provided for switch -o \n");
                        exit(8);
                    } else if (outputfilename == NULL || outputfilename == argv[arg]) {
                        //printf("Setting output file to %s...\n", optarg);
                        outputfilename = argv[arg];
                        break;
                    } else {
                        fprintf(stderr,
                                "Conflicting output arguments given, please run with valid arguments.\n");
                        exit(10);
                    }
                case 'g': //SET GENERATIONS CASE
                    ++arg;
                    errno = 0;


                    if (arg >= argc || argv[arg][0] == '-') {
                        fprintf(stderr,
                                "No valid argument provided for switch -g (note: arguments cannot start with \"-\")\n");
                        exit(9);
                    }
                    gencounttemp = strtol(argv[arg], &endptr, 10);
                    if (errno != 0 || argv[arg] == endptr) { //Check for an error set by strtol
                        if (gencounttemp == LONG_MIN || gencounttemp == LONG_MAX) {
                            fprintf(stderr, "Failed to convert argument: %s into an integer for switch -g.\n"
                                            "Input number out of range, please input a number between  0 and %ld.\n",
                                    argv[arg], LONG_MAX);
                        } else {
                            fprintf(stderr, "Failed to convert argument: %s into an integer for switch -g.\n"
                                            "Please insert a valid argument for -g.\n", argv[arg]);
                        }
                        exit(15);
                    } else if (*endptr != 0) { //Check that there arent remaining characters in the string passed to strtol
                        fprintf(stderr, "Failed to convert argument: %s into an integer for switch -g.\n"
                                        "Please insert a valid integer.\n", argv[arg]);
                        exit(60);
                    }
                    if (gencountset == true && gencounttemp != gencount) {
                        fprintf(stderr,
                                "Conflicting \"g\" arguments given, please run with valid arguments.\n");
                        exit(10);
                    } else {
                        gencountset = true;
                        gencount = gencounttemp;
                    }
                    break;

                case 's': //STATISTICS CASE
                    //printf("Printing statistics at end of run.\n");
                    printstats = true;
                    break;
                case 't': //TORUS CASE
                    //printf("Using Torus configuration\n");
                    will_be_alive_func = &will_be_alive_torus; //Set the function to pass to the library to be the torus rather than the standard
                    break;
                default: //Oh No, something's cone wrong
                    fprintf(stderr,
                            "Unrecognised Switch \"%s\", please enter only valid options.\n", argv[arg]);
                    exit(10);
            }
        } else {
            fprintf(stderr, "%s is not a valid switch, switches must start with a \"-\" character and be two characters long \n", argv[arg]);
            exit(9);
        }
    }


    if (outputfilename == NULL) {
        //printf("No output file specified, using STDOUT, the result will be printed below\n");
        outfile = stdout;
    } else if ((outfile = fopen(outputfilename, "w")) == NULL) { //Opens file and returns NULL if failed
        perror("Error opening output file, please ensure the file you entered exists in same directory.\n");
        // Program exits if file pointer returns NULL.
        exit(5);
    }

    if (inputfilename == NULL) {
        //printf("No input file specified, using STDIN, type your input below and use Ctrl+D when done\n");
        inpfile = stdin;
    } else if ((inpfile = fopen(inputfilename, "r")) == NULL) { //Opens file and returns NULL if failed
        perror("Error opening input file, please ensure the file you entered exists in same directory.\n");
        // Program exits if file pointer returns NULL.
        exit(6);
    }


    read_in_file(inpfile, &v);

    //printf("Input successful, input universe is as follows:\n");
    //write_out_file(stdout, &v);


    for (int i = 0; i < gencount; ++i) {
        evolve(&v, will_be_alive_func);

    }

    //printf("Simulation Successful, final universe is as follows:\n");
    write_out_file(outfile, &v);

    if (printstats == true) {
        print_statistics(&v);
    }

    return 0;
}
