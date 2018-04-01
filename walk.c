#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[]){

    int n;
    int threads;
    char *filename = NULL;
    if(argc >= 1)
    {
        filename = argv[1];
    }
    else
    {
        printf( "Error: No file provided!\n");
        exit(0);
    }
    if(filename == NULL)
    {
        printf( "Error: No file provided!\n");
        exit(0);
    }
    printf("Matrix file name : %s\n" , filename);
    if(argc >= 2)
    {
       threads  = atoi(argv[2]);
    }
    if(threads == 0)
    {       
        printf( "Num of threads not given. Working on 4 threads.\n");
    }
    printf("Num of Threads : %d\n" ,threads );
    if(argc >= 3)
    {
       n  = atoi(argv[3]);
    }
    if(threads == 0)
    {   printf( "Matrix size not given assuming 10^3 by 10^3\n");
        n = 1000;
    }
    printf("Size of Matrix : %d by %d\n" ,n,n );

    

    omp_set_num_threads(threads);
    int** mat;
    char queryString[12];
    int query;
    double wtime;
    //mat  = read_matrix(filename)
    while(scanf("%s", queryString))
    {   query = atoi(queryString);
        if (query == -1)
            break;
        wtime = omp_get_wtime();

        printf("query :%4d  ",query);
        printf("time taken %14f \n",(omp_get_wtime() - wtime));
    }
    
    



}