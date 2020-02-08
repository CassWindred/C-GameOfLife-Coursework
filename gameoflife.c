#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include"gol.h"


int main(int argc, char *argv[]) {
    struct universe v;

    FILE *inpfile;
    FILE *outfile;

    bool usetorus = false;
    bool printstats = false;
    int gencount = 5;

    char *inputfilename = "";
    char *outputfilename = "";


    int opt;
    while ((opt = getopt(argc, argv, ":i:o:g:st")) != -1)   {
        switch(opt) {
            case '?':
                printf("-%c is not a recognised option.\n", optopt);
                break;
            case ':':
                printf("Missing argument for option -%c\n", opt);
            case 'i':
                if (optarg[0]=='-') {
                    printf("Missing valid argument for option -i, please try again with valid argument...\n");
                    printf("Ending Program...");
                    exit(1);
                }
                printf("Setting input file to %s...\n",optarg);
                inputfilename = optarg;
                break;
            case 'o':
                if (optarg[0]=='-') {
                    printf("Missing valid argument for option -o, please try again with valid argument...\n");
                    printf("Ending Program...");
                    exit(1);
                }
                printf("Setting output file to %s...\n", optarg);
                outputfilename = optarg;
                break;
            case 'g':
                if (optarg[0]=='-') {
                    printf("Missing valid argument for option -g, please try again with valid argument...\n");
                    printf("Ending Program...");
                    exit(1);
                }
                printf("Setting generation count to %s\n", optarg);
                gencount = strtol(optarg, NULL, 10);
                break;
            case 's':
                printf("Printing statistics at end of run.\n");
                printstats = true;
                break;
            case 't':
                printf("Using Torus configuration\n");
                usetorus = true;
                break;
            default:
                printf("-%c is not a recognised option.", opt);
        }
    }

    if ((inpfile = fopen("glider.txt", "r")) == NULL) {
        printf("Error opening file, please ensure the file you entered exists in same directory.");
        // Program exits if file pointer returns NULL.
        exit(1);
    }

    printf("Owo whats this\n");
    read_in_file(inpfile, &v);

/*
  evolve(&v,will_be_alive);
  evolve(&v,will_be_alive);
  evolve(&v,will_be_alive);
  evolve(&v,will_be_alive);
  evolve(&v,will_be_alive);
  write_out_file(stdout,&v);*/
    return 0;
}
