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
    
    position* loc1 = NULL;
    position* loc2 = NULL;
    
    if(query == mat[pt1->row][pt1->coll])
    {
        loc1 = (position*) malloc(sizeof(position));
        loc1-> row = pt1->row;
        loc1->coll = pt1->coll;
        // printf("Element %d found at row: %d, coll:%d\n", mat[pt1->row][pt1->coll], loc1->row, loc1->coll);
        return loc1;

    }
    else if(query == mat[pt2->row][pt2->coll])
    {
        loc1 = (position*) malloc(sizeof(position));
        loc1-> row = pt2->row;
        loc1->coll = pt2->coll;
        // printf("Element %d found at row: %d, coll:%d\n",mat[pt2->row][pt2->coll],loc1->row,loc1->coll);
        return loc1;
    }

    int num_rows = pt1->row < pt2->row ? (pt2->row - pt1->row):(pt1->row - pt2->row);
    
    if(num_rows > 1)
    {
        position* mid = (position*) malloc(sizeof(position));
        mid->row = (pt1->row + pt2->row)/2;
        int low_coll = pt1->coll < pt2->coll ? pt1->coll : pt2->coll;
        int num_coll = pt1->coll < pt2->coll ? (pt2->coll - pt1->coll + 1):(pt1->coll - pt2->coll + 1);
        mid->coll = search_in_row(mat, query,mid->row ,low_coll , num_coll );

        // printf("Point 1 row: %d, coll:%d val:%d \n",pt1->row,pt1->coll,mat[pt1->row][pt1->coll]);
        // printf("Point 2 row: %d, coll:%d  val:%d \n",pt2->row,pt2->coll,mat[pt2->row][pt2->coll]);
        // printf("sent to search:row:%d start coll:%d, num_of_coll:%d ",mid->row ,low_coll , num_coll);
        // printf("depth: %d \n", depth);
        // printf("mid elem %d  row: %d, coll:%d\n\n",mat[mid->row][mid->coll],mid->row,mid->coll);

        // #pragma omp single 
        // {
            #pragma omp task shared(loc1)
                loc1 = splitWalk(mat, query,pt1,mid,depth+1);         //left recursion

            #pragma omp task shared(loc2)
                loc2 = splitWalk(mat, query,mid,pt2,depth+1);       //right recursion
        // }
        
        #pragma omp taskwait
        
        free(mid);
        if(loc1 == NULL)
            return loc2;
        else
            return loc1;
        
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
                // printf("Element %d found at row: %d, coll:%d\n",mat[i-1][j],i-1,j);
                // printf("1\n");
                loc1 = (position*) malloc(sizeof(position));
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
    omp_set_num_threads(num_threads);
    omp_set_nested(1);

    if(argc > 2) filename = argv[2];
    else{
        filename = (char*) malloc(24 * sizeof(char));
        memset(filename, '\0', 24);
        strcpy(filename , "test/test.csv");
    }
    
    int** mat;
    int n;
    double wtime;
    
    mat = get_matrix(mat, filename, &n);

    // Reading Queries
    int* queries, total_queries;
    char* query_path = "test/query.in";
    queries = read_queries(queries, query_path, &total_queries);

    position* point1 = (position*) malloc(sizeof(position));
    position* point2 = (position*) malloc(sizeof(position));
    position* val = (position*) malloc(sizeof(position));

    double starttime = omp_get_wtime();

    for(int i = 0; i < total_queries; i++){   
        wtime = omp_get_wtime();
        int query = queries[i] ;

        if(query >= mat[0][0] && query <= mat[n-1][n-1]){
            if(query >= mat[n-1][0]){
                if(query >= mat[0][n-1]){
                    
                    point2->coll = n-1;
                    point1->row = n-1;

                    #pragma omp parallel
                    {
                        #pragma omp sections
                        {
                            #pragma omp section
                                point2->row = search_in_column(mat, query, n-1, 0, n);
                            #pragma omp section                           
                                point1->coll = search_in_row(mat, query, n-1, 0, n);
                            //pt2 btm row, pt1 right coll 
                        }
                        #pragma omp single nowait
                        {
                            #pragma omp task shared(val)
                                val = splitWalk(mat, query,point1,point2,1);
                        }
                    }
                    
                }
                else{   
                    point2->row = 0;
                    point1->row = n-1;
                    #pragma omp parallel
                    {
                        #pragma omp sections
                        {
                            #pragma omp section                            
                                point1->coll =  search_in_row(mat, query, n-1, 0, n); // search in row
                            #pragma omp section                                
                                point2->coll = search_in_row(mat, query, 0, 0,n);
                        }

                        #pragma omp single nowait
                        {
                            #pragma omp task shared(val)
                                val = splitWalk(mat, query,point1,point2,1);
                        }
                    }

                }
            }
            else{   
                if(query >= mat[0][n-1]){
                    point1->coll = 0;
                    point2->coll = n-1;

                    #pragma omp parallel
                    {   
                        #pragma omp sections
                        {
                            #pragma omp section
                                point2->row = search_in_column(mat, query, n-1, 0,n);
                            #pragma omp section    
                                point1->row = search_in_column(mat, query, 0 ,0,n); // search in column
                        }
                        #pragma omp single nowait
                        {
                            #pragma omp task shared(val)
                                val = splitWalk(mat, query,point1,point2,1);
                        }
                    }
                }
                else{
                    point1->coll = 0;
                    point2->row = 0;
                    #pragma omp parallel
                    {
                        #pragma omp sections
                        {
                            #pragma omp section
                                point1->row = search_in_column(mat, query,0, 0, n);
                            #pragma omp section
                                point2->coll = search_in_row(mat, query, 0, 0,n);
                        }
                        #pragma omp single nowait
                        {
                            #pragma omp task shared(val)
                                val = splitWalk(mat, query,point1,point2,1);
                        }
                    }
                }
            }
        }
        else{
            printf("0\n" );
            continue;
        }
        
        // #pragma omp taskwait
        if(val == NULL){
            printf("0\n" );
        }else{
            printf("1\n");
        }
    }
    double timeTotal = (omp_get_wtime() - starttime);
    printf("time taken %14.7f \t Avg time %14.5f \n",timeTotal , timeTotal/total_queries);
}