

#include <stdio.h>
#include "walk.h"

int rowSearchLessThanEq(int** matrix, int rowNum, int left, int right, int X){
    int i = 0;

     if(X < matrix[rowNum][right])
        return right;
}