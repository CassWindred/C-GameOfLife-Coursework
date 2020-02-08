typedef int bool;
#define true 1
#define false 0


struct universe {
/*Put some appropriate things here*/
    int columns, rows;
    int *cells;

};

typedef struct coordinate {
    int column, row;
}coordinate;

/*Do not modify the next seven lines*/
void read_in_file(FILE *infile, struct universe *u);
void write_out_file(FILE *outfile, struct universe *u);
int is_alive(struct universe *u, int column, int row);
int will_be_alive(struct universe *u, int column, int row);
int will_be_alive_torus(struct universe *u,  int column, int row);
void evolve(struct universe *u, int (*rule)(struct universe *u, int column, int row));
void print_statistics(struct universe *u);
/*You can modify after this line again*/

coordinate* getneighbours(int column, int row);


