
#include <stdio.h>

int mat[4][4] = { {10, 20, 30, 40},
                  {15, 25, 35, 45},
                  {27, 29, 37, 48},
                  {32, 33, 39, 50},
                };

int getVal(int i, int j){
    return mat[i][j];
}

int search(int n, int x)
{
   int i = 0, j = n-1;  //set indexes for top right element
   while ( i < n && j >= 0 )
   {  int val = getVal(i,j);
      printf("Element: %d , index: %d,%d \n",val, i,j);
      if ( val == x )
      {
         printf("n Found at %d, %d", i, j);
         return 1;
      }
      if ( val > x )
        j--;
      else //  if val < x
        i++;
   }

   printf("n Element not found");
   return 0;  // if ( i==n || j== -1 )
}

// driver program to test above function
int main()
{

  search(4, 29);
  return 0;
}