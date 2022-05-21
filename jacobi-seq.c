#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

#define RAND_UPPER_BOUND 10
#define RAND_LOWER_BOUND 0


int main(int argc, char *argv[]){
    
   srand(64591);
    testArguments(argc);
    int orderOfMatrix = atoi(argv[1]);
    int numberOfThreads = atoi(argv[2]);
    int i,j;
    int lineSum[orderOfMatrix];
    int bVector[orderOfMatrix];
    printf("Matrix Order: %d, Number of Threads:  %d \n", orderOfMatrix, numberOfThreads);
    int matrix[orderOfMatrix][orderOfMatrix];
    // TODO tirar os prints
    printf("\n");
    printf("Matriz\n");
    
    for(i=0;i<orderOfMatrix;i++){
        lineSum[i]=0;
    }
    //Geração de matriz ordem (orderOfMatrix) que sempre passa no critério das linhas.
    for(i=0;i<orderOfMatrix;i++){
      for(j=0;j<orderOfMatrix;j++){
          if(i!=j){
          matrix[i][j]= (rand() % (RAND_UPPER_BOUND - RAND_LOWER_BOUND + 1)) + RAND_LOWER_BOUND;
          lineSum[i]= lineSum[i] + matrix[i][j];
         }
        matrix[i][i]= (rand() %((RAND_UPPER_BOUND + lineSum[i]) - RAND_LOWER_BOUND + 1)) + (lineSum[i]+1); //posicao diagonal principal > soma dos outros elementos.
        bVector[i]= (rand() % (RAND_UPPER_BOUND - RAND_LOWER_BOUND + 1)) + RAND_LOWER_BOUND; //vetor B do enunciado do problema.
      }
   }
   
   for(i=0;i<orderOfMatrix;i++){
      for(j=0;j<orderOfMatrix;j++){
          printf("%d ", matrix[i][j]);
      }
      printf("\n");
   }
    printf("Vetor B\n");
   for(i=0;i<orderOfMatrix;i++){
       printf("%d ", bVector[i]);
   }
   // Critérios de convergência *********************
   int lineCriteria=1;

   for(i=0;i<orderOfMatrix;i++){
        lineSum[i]=0;
    }
   for(i=0;i<orderOfMatrix;i++){
      if(lineCriteria){
         for(j=0;j<orderOfMatrix;j++){
            if(i!=j){
               lineSum[i]= lineSum[i] + matrix[i][j];
            }
            if(lineSum[i]>matrix[i][i]){
               lineCriteria=0;
               printf("\nNao converge pelo criterio das linhas\n");
               break;
            }
         }
      }
   }
   if(lineCriteria){
      printf("\nA matriz converge pelo criterio das linhas\n");
   }else{
      //Critério da coluna
      int colunmCriteria=1;
      int colunmSum[orderOfMatrix];

      for(i=0;i<orderOfMatrix;i++){
         colunmSum[i]=0;
      }

      for(i=0;i<orderOfMatrix;i++){
         if(colunmCriteria){
            for(j=0;j<orderOfMatrix;j++){
               if(i!=j){
                  colunmSum[i]=colunmSum[i]+matrix[j][i];
               }
               if(colunmSum[i]>matrix[i][i]){
                  colunmCriteria=0;
                  printf("\nNao converge pelo criterio das colunas e nem das linhas\n");
                  break;
               }
            }
         }
      }
      if(colunmCriteria)
         printf("Converge pelo metodo das colunas");
   }
   //Calculos dos x_i^k+1 *****************************
   float lastResults[orderOfMatrix];
   float currentResults[orderOfMatrix], accumulator;
   int k;
   for(i=0;i<orderOfMatrix;i++){  // lastVector = -b(i)/a(ii) (valores iniciais.)
      lastResults[i]=  (float) bVector[i]/ (float) matrix[i][i];
      currentResults[i] = 0;
      printf("%.3f ",lastResults[i]);
   }

   printf("\nComecando as iteracoes:\n");
   for(k=0;k<orderOfMatrix;k++){
      for(i=0;i<orderOfMatrix;i++){
         accumulator=0;
         for(j=0;j<orderOfMatrix;j++){
            if(i==j)
               accumulator = accumulator + ((float) bVector[k]/ (float) matrix[k][k]);
            else
               accumulator = accumulator - (matrix[i][j]*lastResults[j]);
         }
         currentResults[i] = accumulator;

         printf("%.3f ", currentResults[i]);
      }
      for(i=0;i<orderOfMatrix;i++){
      lastResults[i] = currentResults[i];
      }
      //printf("%.3f ", currentResults[k]);
      //linha 1  current[1] = b1 + a12.last2 + a13 last3 + ... a1n last n  / a11 
      //linha 2  current[2] = a21.last1 + b2 + a23 last3 + ... a1n last n / a22
   }

   for(i=0;i<orderOfMatrix;i++){
      lastResults[i] = currentResults[i];
   }

   //Teste do critério de parada **********************
   // Se parada - Imprime resultado *******************
    return 0;
}