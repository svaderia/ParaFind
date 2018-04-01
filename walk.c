#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "myRead.h"

struct position {
    int row;
    int coll;
};
typedef struct position position;


position* splitWalk(int** mat, int query, int pt1, int pt2){
    
    int num_rows = pt1->row < pt2->row ? (pt2->row - pt1->row):(pt1->row - pt2->row);
    if(num_rows > 100)
    {
        position* mid = (position*)malloc(sizeof(position));
        mid->row = (pt1-row + pt2->row)/2;
        int low_coll = pt1->coll < pt2->coll ? pt1->coll : pt2->coll;
        int num_colls = pt1->coll < pt2->coll ? (pt2->coll - pt1->coll):(pt1->coll - pt2->coll);
        mid->coll = search_in_row(mat, query,(pt1-row + pt2->row)/2 ,low_coll , num_coll );
        position* loc1 = (position*)malloc(sizeof(position));
        position* loc2 = (position*)malloc(sizeof(position));
        loc2->row = -1;
        loc1->row = -1;
        loc2->coll = -1;
        loc1->coll = -1;


        #pragma omp task shared(mat,query)  
         loc1 = splitWalk(mat, query,point1,mid);           //left recursion
        
        #pragma omp task shared(mat,query)
         loc2 = splitWalk(mat, query,point2,mid);       //right recursion
        
        #pragma omp taskwait
        {   
             //send back the tuple that has non -1 values
            
        }

    }
    //Write walk algo here @jbnerd
}



int main(int argc, char* argv[]){

    int num_threads;
    char *filename = NULL;

    if(argc > 1) num_threads  = atoi(argv[1]);
    else num_threads = 4;

    if(argc > 2) filename = argv[2];
    else filename = "./test/test.csv";

    omp_set_num_threads(num_threads);

    int** mat;
    int dim;
    char queryString[12];
    int query;
    double wtime;

    // reading a the matrix
    mat = get_matrix(mat, filename, &dim);
    // print_matrix(mat, dim);

    while(scanf("%s", queryString))
    {   query = atoi(queryString);
        if (query == -1)
            break;
        wtime = omp_get_wtime();

        printf("query :%4d  ",query);
        printf("time taken %14f \n",(omp_get_wtime() - wtime));
    }

    int flag = -1;
    position* point1 = (position*)malloc(sizeof(position));
    position* point2 = (position*)malloc(sizeof(position));
    position* val = (position*)malloc(sizeof(position));
    //mat  = read_matrix(filename)
    while(scanf("%s", queryString))
    {   query = atoi(queryString);
        if (query == -1)
            break;
        wtime = omp_get_wtime();
        // add magic here!
        if(query >= mat[0][0] && query <= mat[n-1][n-1])
           {   
               if(query >= mat[n-1][0])
                {   
                    if(query >= mat[0][n-1]){
                        point1->row = search_in_column(mat, query, n-1, 0, n);
                        point2->coll = search_in_row(mat, query, n-1, 0, n);
                        point1->coll = 0;
                        point2->row = 0;    
                        //pt2 btm row, pt1 right coll 

                           #pragma omp parallel
                            {
                            #pragma omp single shared(position)
                             val = splitWalk(mat, query,point1,point2);
                            }
                        
                    }
                    else
                    {   
                        point2->row =  search_in_row(mat, query, n-1, 0, n); // search in row
                        point1->row = search_in_row(mat, query, 0, 0,n);
                        point1->coll = 0;
                        point2->coll = 0;
                        //pt1 left coll, pt2 right coll
                            #pragma omp parallel
                            {
                            #pragma omp single shared(position)
                             val = splitWalk(mat, query,point1,point2);
                            }

                    }
                }
                else
                {   
                    if(query >= mat[0][n-1]){
                        point2->row = search_in_column(mat, query, n-1, 0,n);
                        point1->coll = search_in_column(mat, query, 0 ,0,n); // search in column
                        point1->row = 0;
                        point2->coll = 0;
                        //pt1 top row, pt2 btm col
                        #pragma omp parallel
                            {
                            #pragma omp single shared(position)
                             val = splitWalk(mat, query,point1,point2);
                            }

                    }
                    else
                    {
                        point1->row = search_in_column(mat, query,0, 0, n);
                        point2->coll = search_in_row(mat, query, 0, 0,n);
                        point1->coll = 0;
                        point2->row = 0;
                        //pt1 left coll and pt2 top
                        #pragma omp parallel
                            {
                            #pragma omp single shared(position)
                             val = splitWalk(mat, query,point1,point2);
                            }
                    }
                }    
           }
        
        if(flag == -1){
            printf("query :%4d  ",query);
            printf("Not found! :(\n")
        }
        else 
        
        printf("time taken %14f \n",(omp_get_wtime() - wtime));
    }

    
}