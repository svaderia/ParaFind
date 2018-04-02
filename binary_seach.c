#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "binary.h"

int special_bs(int* arr, int key, int low, int high){
	if(arr[high] > key){
		return -1;
	}

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
	if(matrix[high] > key){
		return -1;
	}

	int low = low_row, high = low_row + num_rows;
	int intermediate_result;
	while(low < high){
		int mid = low + (high - low)/2;
		if(matrix[mid][col_num] < key){
			low = mid + 1;
		}else if(matrix[mid][col_num] > key){
			high = mid;
		}else{
			intermediate_result = mid;
		}
	}
	intermediate_result = low;
	return intermediate_result + low_row;
}

int search_in_row(int** matrix, int key, int row_num, int low_col, int num_cols){
	int intermediate_result = special_bs(matrix[row_num], key, low_col, low_col + num_cols);
	if(intermediate_result != -1){
		return low_col + intermediate_result;
	}else{
		return -1;
	}	
}

/*
int main(){

	int* arr = (int*) malloc(10 * sizeof(int));
	for(int i = 0; i < 10; i++){
		arr[i] = 2 * i;
	}
	int result = special_bs(arr, 18294, 0, 19);
	printf("%d\n", arr[result]);

	return 0;
}*/
