#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

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