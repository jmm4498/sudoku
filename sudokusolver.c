#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>


int GRIDSIZE = -1;
int sol_count = 0;
int **grid;

void printGrid(){
    for(int i = 0; i < GRIDSIZE; i++){
        fprintf(stdout, "||");
        for(int j = 0; j < GRIDSIZE; j++){
            fprintf(stdout, " %d ", grid[i][j]);
            if(j < GRIDSIZE - 1)
                fprintf(stdout, "|");
        } 
        fprintf(stdout, "||\n");
    }
}

int check_grid(int row, int col, int to_add){
    for(int i = 0; i < GRIDSIZE; i++){
        if(grid[row][i] == to_add)
             return 1;
    }

    for(int i = 0; i < GRIDSIZE; i++){
        if(grid[i][col] == to_add)
             return 1;
    }

    int square = (int)sqrt(GRIDSIZE);

    int rowStart = row - row % square;
    int colStart = col - col % square;

    for(int i = rowStart; i < rowStart + square; i++){
        for(int j = colStart; j < colStart + square; j++){
            if(grid[i][j] == to_add)
                return 1;
        }
    }
    return 0;
}

int solve(){

    int i, j;
    int isFull = 1;
    i = -1;
    j = -1;

    for(i = 0; i < GRIDSIZE; i++){
        for(j = 0; j < GRIDSIZE; j++){
            if(grid[i][j] == 0){
                isFull = 0;
                break;
            }
        }
        if(isFull == 0)
            break;
    }

    if(isFull == 1){
        sol_count += 1;
        return 0;
    }

    for(int to_add = 1; to_add <= GRIDSIZE; to_add++){
       if(check_grid(i, j, to_add) == 0){
           grid[i][j] = to_add;
           if(solve() == 0){
               return 0;
           } else {
               grid[i][j] = 0;
           }
       }
    }
    return 1;
}

void readGrid(char* fileName){
    int i, j;
    unsigned char in;
    FILE *fp = fopen(fileName, "r");
    if(!fp){
        fprintf(stderr, "File not found\n");
        exit(EXIT_FAILURE);
    }
    fscanf(fp, " %c", &in);

    if(!isdigit((unsigned char)in)){
        fprintf(stderr, "Invalid input file\n");
        exit(EXIT_FAILURE);
    }

    GRIDSIZE = in - '0';

    grid = malloc(sizeof(int *) * GRIDSIZE);

    for(i = 0; i < GRIDSIZE; i++)
        grid[i] = malloc(sizeof(int) * GRIDSIZE);

    for(i = 0; i < GRIDSIZE; i++){
        for(j = 0; j < GRIDSIZE; j++){
            if(fscanf(fp, " %c", &in) != 1){
                fprintf(stderr, "Error reading file\n");
                exit(EXIT_FAILURE);
            }
            else if(isdigit((unsigned char)in))
                grid[i][j] = in - '0';
        }
    }
    fclose(fp);
}

void writeGrid(char* fileName){
    int i, j;

    FILE *fp = fopen(fileName, "w");
    if(!fp){
        fprintf(stderr, "Error creating output file\n");
        exit(EXIT_FAILURE);
    }

    for(i = 0; i < GRIDSIZE; i++){
        for(j = 0; j < GRIDSIZE; j++){
            fprintf(fp, "%d ", grid[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);
}

int main(int argc, char *argv[]){

    if(argc < 2 || argc > 3){
        fprintf(stderr, "Usage: ./solver <input file> <output file>\n");
        return 1;
    }
    if(strcmp("help", argv[1]) == 0){
        fprintf(stdout, 
        "Usage: <input file> <output file>\nThe input file must only contain digits 0-9, seperate by spaces, and optionally newline characters.\nThe first number in the file must be the dimension of the square grid. ie 9 = 9x9 grid\nAn optional output file name can be provided if you want to output the solved grid (if a solution is possible) to a file.\n");
        return 0;
    }

    int to_ret = 1;

    char infile[256];

    strncpy(infile, argv[1], sizeof(infile));

    readGrid(infile);
    fprintf(stdout, "Grid from input file\n");
    printGrid();

    fprintf(stdout, "Attempting to solve puzzle...\n");
    to_ret = solve();
    if(to_ret == 0){
        fprintf(stdout, "Solved the puzzle\n");
        fprintf(stdout, "%d Solution(s) found\n", sol_count);
        printGrid();
    } else {
        fprintf(stdout, "No solution!\n");
    }

    if(argc == 3 && to_ret == 0){
        char outfile[256];
        strncpy(outfile, argv[2], sizeof(outfile));

        fprintf(stdout, "Writing grid to file: %s\n", outfile);
        writeGrid(outfile);
    }
    free(grid);    

    return to_ret;
}