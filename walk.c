#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "myRead.h"

int main(int argc, char* argv[]){

    int num_threads;
    char *filename = NULL;

    if(argc > 1) num_threads  = atoi(argv[1]);
    else num_threads = 4;

    if(argc > 2) filename = argv[2];
    else filename = "./test/test.csv";

    // omp_set_num_threads(num_threads);

    int** mat;
    int dim;
    char queryString[12];
    int query;
    double wtime;

    // reading a the matrix
    mat = get_matrix(mat, filename, &dim);
    print_matrix(mat, dim);

    // while(scanf("%s", queryString))
    // {   query = atoi(queryString);
    //     if (query == -1)
    //         break;
    //     wtime = omp_get_wtime();

    //     printf("query :%4d  ",query);
    //     printf("time taken %14f \n",(omp_get_wtime() - wtime));
    // }
}