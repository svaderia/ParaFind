#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int special_bs(int* arr, int key, int low, int high);

int search_in_column(int** matrix, int key, int col_num, int low_row, int num_rows);

int search_in_row(int** matrix, int key, int row_num, int low_col, int num_cols);
