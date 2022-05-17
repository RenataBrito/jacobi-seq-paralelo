#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"


int main(int argc, char *argv[]){
    
    testArguments(argc);
    int orderOfMatrix = atoi(argv[1]);
    int numberOfThreads = atoi(argv[2]);
    
    printf("Matrix Order: %d, Number of Threads:  %d \n", orderOfMatrix, numberOfThreads);
    int **matrix;
    matrix=matrixGen(orderOfMatrix);
    // TODO tirar os prints
    printf("\n");
    printf("Matriz\n");
    int i,j;
    for(i=0;i<orderOfMatrix;i++){
      for(j=0;j<orderOfMatrix;j++){
         printf("%d ", matrix[i][j]);
      }
      printf("\n");
   }
    return 0;
}