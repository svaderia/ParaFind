#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "myRead.h"

int offset = 0;
bool eof = false;

FILE *getStream(FILE *fp, char* b, int k)
{	int count = -1;
	if (!eof)
        count = fread(b, 1, k, fp);
    //hack
    if(count < k) eof = true ;
    b[count] = '\0';
    return fp;
}

char* getWord(FILE* fp, char* b, int k){
    char* temp = (char*) malloc(MAX_WORD_SIZE * sizeof(char));
    int i = 0;
    memset(temp, 0, MAX_WORD_SIZE);
    while(1){
        if (offset == k || strlen(b) == 0 || b[offset] == '\0'){
			if( eof ){
				return temp;
			}
			memset(b, 0, k);
            fp = getStream(fp, b, k);
            offset = 0;
		}
        if('0' <= b[offset] && b[offset] <= '9'){
            temp[i++] = b[offset++];
        }else{
            offset++;
            if(temp[0] == '\0') continue;
            else return temp;
        }
    }
}

int** get_matrix(int** mat, char* path, int* dim){
    FILE* fp = fopen(path, "r");
    if(fp == NULL){
        printf("Fucked");
    }

    fscanf(fp, "%d", dim);
    mat = (int**) malloc((*dim) * sizeof(int*));

    char* b = (char*) malloc(4096 * sizeof(char));
    char* word_buffer;
    int k = 4096;
    offset = 0;
    eof = false;
    memset(b, 0, k);
    
    for(int i = 0; i < *dim; i++){
        mat[i] = (int*) malloc((*dim) * sizeof(int));
        for(int j = 0; j < *dim; j++){
            word_buffer = getWord(fp, b, k);
            mat[i][j] = atoi(word_buffer);
            free(word_buffer);
        }
    }
    return mat;
}

void print_matrix(int** mat, int dim){
    for(int i = 0; i < dim; i++){
        for(int j = 0; j < dim; j++){
            printf("%d\t", mat[i][j] );
        }
        printf("\n");
    }
}