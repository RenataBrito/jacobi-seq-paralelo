#include <stdio.h>
#include <stdlib.h>

#define EXPECTED_ARGS 3

void testArguments(int argumentCounter){
     if( argumentCounter == EXPECTED_ARGS ) {
      printf("The supplied number of arguments is correct.\n");
      //TODO checar se é um numero maior que 0
   }
   else if( argumentCounter > EXPECTED_ARGS ) {
      printf("Too many arguments supplied.\n");
      exit(EXIT_FAILURE);
   }
   else {
      printf("Two arguments expected.\n");
      exit(EXIT_FAILURE);
   } 
}

int **matrixGen(int N){
   int *matrix;
   int i;
   matrix=(int *)calloc(N ,sizeof(int));
   for(int i=0;i<N ;i++){
          matrix[i]= (int) (int *)calloc(N ,sizeof(int));
   }

   return matrix;
}
