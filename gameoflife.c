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
    while ((opt = getopt(argc, argv, ":if:lrx")) != -1)   {
        switch(opt) {
            case 'i':
                inputfilename = optarg;
                break;
            case 'o':
                outputfilename = optarg;
                break;
            case 'g':
                gencount = strtol(optarg, NULL, 10);
                break;
            case 's':
                printstats = true;
                break;
            case 't':
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
