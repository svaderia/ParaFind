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

int** make_matrix(int size){
    int** matrix = (int**) malloc(size * sizeof(int*));
    for(int i = 0; i < size; i ++){
        matrix[i] = (int*) malloc(size * sizeof(int));
    }

    int i = 0, j = 0, k = 0;
    int num = 1;

    for(k = 0; k < size; k ++){
        i = 0;
        for(j = k; j >= 0; j --){
            matrix[i][j] = num++;
            i ++;
        }
    }

    for(k = 1; k < size; k ++){
        j = size - 1;
        for(i = k; i < size; i++){
            matrix[i][j] = num++;
            j --;
        }
    }

    return matrix;
}

void print_matrix(int** matrix, int size){
    for(int i = 0; i < size; i ++){
        for(int j = 0; j < size; j ++){
            printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

position* splitWalk(int** mat, int query, position* pt1, position* pt2){
    
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
        printf("Element found at row: %d, coll:%d",loc1->row,loc1->coll);
        return loc1;

    }
    if(query == mat[pt2->row][pt2->coll])
    {
        loc1-> row = pt2->row;
        loc1->coll = pt2->coll;
        printf("Element found at row: %d, coll:%d",loc1->row,loc1->coll);
        return loc1;
    }
    int num_rows = pt1->row < pt2->row ? (pt2->row - pt1->row):(pt1->row - pt2->row);
    if(num_rows > 100)
    {
        position* mid = (position*)malloc(sizeof(position));
        mid->row = (pt1->row + pt2->row)/2;
        int low_coll = pt1->coll < pt2->coll ? pt1->coll : pt2->coll;
        int num_coll = pt1->coll < pt2->coll ? (pt2->coll - pt1->coll):(pt1->coll - pt2->coll);
        mid->coll = search_in_row(mat, query,(pt1->row + pt2->row)/2 ,low_coll , num_coll );
        
        #pragma omp task shared(mat,query,loc1)  
         loc1 = splitWalk(mat, query,pt1,mid);           //left recursion
        
        #pragma omp task shared(mat,query,loc2)
         loc2 = splitWalk(mat, query,mid,pt2);       //right recursion
        
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

        while(i > pt2->row && j < pt2-> coll){

            if(query == mat[i-1][j]){
                loc1->coll = j;
                loc1->row = i; 
                break;
            }
            else if(query < mat[i-1][j])
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
    else filename = "./test/test.csv";

    omp_set_num_threads(num_threads);

    int** mat;
    int dim;
    char queryString[12];
    int query;
    double wtime;

    // reading a the matrix
    mat = get_matrix(mat, filename, &dim);
    int n = dim;
    // print_matrix(mat, dim);

    //toy data
    //int mat[5][5] =  {{ 3,	47,	488,	635,	1088},
    //        {  9,	78,	504,	860,	1187},
      //      { 11,	179,	591,	864,	1230},
        //    { 16,	193,	608,	886,	1261},
          //  {18,	196,	648,	916,	1509}};
    ///int n = 5;

    // while(scanf("%s", queryString))
    // {   query = atoi(queryString);
    //     if (query == -1)
    //         break;
    //     wtime = omp_get_wtime();

    //     printf("query :%4d  ",query);
    //     printf("time taken %14f \n",(omp_get_wtime() - wtime));
    // }

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
                            #pragma omp single
                             val = splitWalk(mat, query,point2,point1);
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
                            #pragma omp single 
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
                            #pragma omp single 
                             val = splitWalk(mat, query,point2,point1);
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
                             val = splitWalk(mat, query,point1,point2);
                            }
                    }
                }    
           }
        
        if(flag == -1){
            printf("query :%4d  ",query);
            printf("Not found! :(\n");
        }
        else 
        
        printf("time taken %14f \n",(omp_get_wtime() - wtime));
    }

    
}
