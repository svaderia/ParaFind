#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#ifndef _TOKEN
#define _TOKEN
#define MAX_WORD_SIZE 140

int** get_matrix(int** mat, char* path, int* dim);

char* getWord(FILE* fp, char* b, int k, int* offset, bool* eof);

void print_matrix(int** mat, int dim);

int* read_queries(int* queries, char* path, int* total_queries);

#endif