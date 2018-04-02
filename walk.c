#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "binary.h"
#include "myRead.h"


struct position {
    int row;
    int coll;
};
typedef struct position position;


position* splitWalk(int** mat, int query, position* pt1, position* pt2, int depth){
    
    position* loc1 = (position*)malloc(sizeof(position));
    position* loc2 = (position*)malloc(sizeof(position));
    loc2->row = -1;
    loc1->row = -1;
    loc2->coll = -1;
    loc1->coll = -1;
    
    if(query == mat[pt1->row][pt1->coll])
    {
        loc1-> row = pt1->row;
        loc1->coll = pt1->coll;
        printf("Element %d found at row: %d, coll:%d\n",mat[pt1->row][pt1->coll],loc1->row,loc1->coll);
        return loc1;

    }
    if(query == mat[pt2->row][pt2->coll])
    {
        loc1-> row = pt2->row;
        loc1->coll = pt2->coll;
        printf("Element %d found at row: %d, coll:%d\n",mat[pt2->row][pt2->coll],loc1->row,loc1->coll);
        return loc1;
    }
    int num_rows = pt1->row < pt2->row ? (pt2->row - pt1->row):(pt1->row - pt2->row);
    if(num_rows >= 50)
    {
        position* mid = (position*)malloc(sizeof(position));
        mid->row = (pt1->row + pt2->row)/2;
        int low_coll = pt1->coll < pt2->coll ? pt1->coll : pt2->coll;
        int num_coll = pt1->coll < pt2->coll ? (pt2->coll - pt1->coll):(pt1->coll - pt2->coll);
        mid->coll = search_in_row(mat, query,mid->row ,low_coll , num_coll );

        // printf("Point 1 row: %d, coll:%d val:%d \n",pt1->row,pt1->coll,mat[pt1->row][pt1->coll]);
        // printf("Point 2 row: %d, coll:%d  val:%d \n",pt2->row,pt2->coll,mat[pt2->row][pt2->coll]);
        // printf("sent to search:row:%d start coll:%d, num_of_coll:%d ",mid->row ,low_coll , num_coll);
        // printf("depth: %d \n", depth);
        // printf("mid elem %d  row: %d, coll:%d\n\n",mat[mid->row][mid->coll],mid->row,mid->coll);

        #pragma omp task shared(mat,query,loc1)  
         loc1 = splitWalk(mat, query,pt1,mid,depth+1);           //left recursion
        
        #pragma omp task shared(mat,query,loc2)
         loc2 = splitWalk(mat, query,mid,pt2,depth+1);       //right recursion
        
        #pragma omp taskwait
        {   
             if(loc1->row == -1)
                return loc2;
            else
                return loc1;            
        }

    }
    else
    {
        int i = pt1->row, j = pt1-> coll;

        //printf("Point 1 row: %d, coll:%d val:%d \n",pt1->row,pt1->coll,mat[pt1->row][pt1->coll]);
        //printf("Point 2 row: %d, coll:%d  val:%d \n",pt2->row,pt2->coll,mat[pt2->row][pt2->coll]);
        //printf("depth: %d \n\n", depth);

        while(i > pt2->row && j < pt2-> coll){
            //printf("Element %d  at row: %d, coll:%d, traversed during walk\n",mat[i][j],i,j);
            if(query == mat[i-1][j]){
                printf("Element %d found at row: %d, coll:%d\n",mat[i-1][j],i-1,j);
                loc1->coll = j;
                loc1->row = i; 
                break;
            }
            else if(query > mat[i-1][j])
                j++;
            else
                i--;
        }
        return loc1;
    }    
}

int main(int argc, char* argv[]){
    int num_threads;
    char *filename = NULL;
    if(argc > 1) num_threads  = atoi(argv[1]);
    else num_threads = 4;
    if(argc > 2) filename = argv[2];
    else filename = "./test/test4.csv";
    omp_set_num_threads(num_threads);
    int** mat;
    int dim;
    char queryString[12];
    int query;
    double wtime;
    printf("pls wait...   \nLoading Matrix file.\n");
    // reading a the matrix
    mat = get_matrix(mat, filename, &dim);

    // int x = search_in_row(mat, 700003, 9, 8, 6);
    // printf("%d\n", mat[9][x]);

    int n = dim;
    printf("File loaded, Accepting Queries.\n");
    int flag = -1;
    position* point1 = (position*)malloc(sizeof(position));
    position* point2 = (position*)malloc(sizeof(position));
    position* val = (position*)malloc(sizeof(position));

    int queryNum = 0;
    double starttime = omp_get_wtime();
    while(scanf("%s", queryString))
    {   query = atoi(queryString);
        if (query == -1)
            break;
        wtime = omp_get_wtime();
        queryNum++;
        // add magic here!
        if(query >= mat[0][0] && query <= mat[n-1][n-1])
           {   
               if(query >= mat[n-1][0])
                {   
                    if(query >= mat[0][n-1]){
                        point2->row = search_in_column(mat, query, n-1, 0, n);
                        point1->coll = search_in_row(mat, query, n-1, 0, n);
                        point2->coll = n-1;
                        point1->row = n-1;    
                        //pt2 btm row, pt1 right coll 

                           #pragma omp parallel
                            {
                            #pragma omp single
                             val = splitWalk(mat, query,point1,point2,1);
                            }
                        
                    }
                    else
                    {   
                        point1->coll =  search_in_row(mat, query, n-1, 0, n); // search in row
                        point2->coll = search_in_row(mat, query, 0, 0,n);
                        point2->row = 0;
                        point1->row = n-1;
                        //pt1 left coll, pt2 right coll
                            #pragma omp parallel
                            {
                            #pragma omp single 
                             val = splitWalk(mat, query,point1,point2,1);
                            }

                    }
                }
                else
                {   
                    if(query >= mat[0][n-1]){
                        point2->row = search_in_column(mat, query, n-1, 0,n);
                        point1->row = search_in_column(mat, query, 0 ,0,n); // search in column
                        point1->coll = 0;
                        point2->coll = n-1;
                        //pt1 top row, pt2 btm col
                        #pragma omp parallel
                            {
                            #pragma omp single 
                             val = splitWalk(mat, query,point1,point2, 1);
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
                            #pragma omp single
                             val = splitWalk(mat, query,point1,point2,1);
                            }
                    }
                }    
           }
           else
           {
            printf("query :%4d  ",query);
            printf("Not found! :( Not in range\n");
            }
        
        if(val->row == -1 && val->coll == -1){
            printf("query :%4d  ",query);
            printf("Not found! :(\n");
        }        
        printf("time taken %14f \n",(omp_get_wtime() - wtime));
    }
    double timeTotal = (omp_get_wtime() - starttime);
    printf("time taken %14.7f \n Avg time %14.5f ",timeTotal , timeTotal/queryNum);
    
}
