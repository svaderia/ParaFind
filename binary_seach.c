#include <stdio.h>
#include <stdlib.h>

int special_bs(int* arr, int key, int low, int high){
	while(low < high){
		int mid = low + (high - low)/2;
		if(arr[mid] < key){
			low = mid + 1;
		}else if(arr[mid] > key){
			high = mid;
		}else{
			return mid;
		}
	}
	return low;
}

int search_in_column(int** matrix, int key, int col_num, int low_row, int num_rows){
	int* column = (int*) malloc(num_rows * sizeof(int));
	for(int i = 0; i < num_rows; i++){
		column[i] = matrix[i + low_row][col_num];
	}

	int intermediate_result = special_bs(column, key, 0, num_rows - 1);
	return intermediate_result + low_row;
}

int search_in_row(int** matrix, int key, int row_num, int low_col, int num_cols){
	int intermediate_result = special_bs(matrix[row_num], key, low_col, low_col + num_cols);
	return low_col + intermediate_result;
}

int main(){

	int* arr = (int*) malloc(10 * sizeof(int));
	for(int i = 0; i < 10; i++){
		arr[i] = 2 * i;
	}
	int result = special_bs(arr, 9, 0, 19);
	printf("%d\n", arr[result]);

	return 0;
}