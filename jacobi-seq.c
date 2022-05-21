#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"

#define RAND_UPPER_BOUND 10
#define RAND_LOWER_BOUND 0

int main(int argc, char *argv[])
{

   srand(64591);
   testArguments(argc);
   int orderOfMatrix = atoi(argv[1]);
   int numberOfThreads = atoi(argv[2]);
   int i, j;
  // long long int lineSum[orderOfMatrix];
   int* lineSum = malloc(orderOfMatrix*sizeof(long int));
   int bVector[orderOfMatrix];
   printf("Matrix Order: %d, Number of Threads:  %d \n", orderOfMatrix, numberOfThreads);
   //long long int matrix[orderOfMatrix][orderOfMatrix];
   int** matrix = malloc(orderOfMatrix*sizeof(*matrix));
   for(i=0;i<orderOfMatrix;i++){
      matrix[i]=malloc(orderOfMatrix*sizeof(*matrix[i]));
   }
   
  // printf(" acaba de criar a matriz\n ");
   // teste do prof
   /*int matrix[3][3] = {
       {4, 2, 1},
       {1, 3, 1},
       {2, 3, 6}};
   int bVector[3] = {7, -8, 6};*/

   // TODO tirar os prints
   //printf("\n");
   //printf("Matriz\n");

   for (i = 0; i < orderOfMatrix; i++)
   {
      //printf(" entra 1 for geracao da matriz\n ");
      lineSum[i] = 0;
   }
   // Geração de matriz ordem (orderOfMatrix) que sempre passa no critério das linhas.
  //printf(" entra for geracao da matriz\n ");
   for (i = 0; i < orderOfMatrix; i++)
   {
      for (j = 0; j < orderOfMatrix; j++)

     // printf(" entra for geracao da matriz 3 \n ");
      {
         if (i != j)
         {
            matrix[i][j] = (rand() % (RAND_UPPER_BOUND - RAND_LOWER_BOUND + 1)) + RAND_LOWER_BOUND;
            lineSum[i] = lineSum[i] + matrix[i][j];
         }
         matrix[i][i] = (rand() % ((RAND_UPPER_BOUND + lineSum[i]) - RAND_LOWER_BOUND + 1)) + (lineSum[i] + 1); // posicao diagonal principal > soma dos outros elementos.
         bVector[i] = (rand() % (RAND_UPPER_BOUND - RAND_LOWER_BOUND + 1)) + RAND_LOWER_BOUND;                  // vetor B do enunciado do problema.
      }
   }

   //print da matriz e vetor B
   /*
   for (i = 0; i < orderOfMatrix; i++)
   {
      for (j = 0; j < orderOfMatrix; j++)
      {
         printf("%d ", matrix[i][j]);
      }
      printf("\n");
   }
   printf("\nVetor B: ");
   for (i = 0; i < orderOfMatrix; i++)
   {
      printf("%d ", bVector[i]);
   }
   */

   // Critérios de convergência *********************
   int lineCriteria = 1;

   for (i = 0; i < orderOfMatrix; i++)
   {
      //printf(" entra criterio linha\n "); 
      lineSum[i] = 0;
   }
   for (i = 0; i < orderOfMatrix; i++)
   {
      if (lineCriteria)
      {
         for (j = 0; j < orderOfMatrix; j++)
         {
            if (i != j)
            {
               lineSum[i] = lineSum[i] + matrix[i][j];
            }
            if (lineSum[i] > matrix[i][i])
            {
               lineCriteria = 0;
               printf("\nNao converge pelo criterio das linhas\n");
               break;
            }
         }
      }
   }
   if (lineCriteria)
   {
      printf("\n\nA matriz converge pelo criterio das linhas\n");
   }
   else
   {
      // Critério da coluna
      int colunmCriteria = 1;
      //long int colunmSum[orderOfMatrix];
      int* colunmSum = malloc(orderOfMatrix*sizeof(long int));

      for (i = 0; i < orderOfMatrix; i++)
      {
         colunmSum[i] = 0;
      }

      for (i = 0; i < orderOfMatrix; i++)
      {
         //printf(" entra criterio linha\n "); 
         if (colunmCriteria)
         {
            for (j = 0; j < orderOfMatrix; j++)
            {
               if (i != j)
               {
                  colunmSum[i] = colunmSum[i] + matrix[j][i];
               }
               if (colunmSum[i] > matrix[i][i])
               {
                  colunmCriteria = 0;
                  printf("\nNao converge pelo criterio das colunas e nem das linhas\n");
                  break;
               }
            }
         }
      }
      if (colunmCriteria)
         printf("Converge pelo metodo das colunas");
   }
   // Calculos dos x_i^k+1 *****************************
   //float lastResults[orderOfMatrix];
   float* lastResults = malloc(orderOfMatrix*sizeof(float));
   float accumulator, maximoDiff, maximoValor, diferenca;
   float* currentResults = malloc(orderOfMatrix*sizeof(float));
   int k = 0;
   //printf("\nVetor inicial: ");
   for (i = 0; i < orderOfMatrix; i++)
   { // lastVector = b(i)/a(ii) (valores iniciais.)
      lastResults[i] = (float)bVector[i] / (float)matrix[i][i];
      currentResults[i] = 0;
      //printf("%.3f ", lastResults[i]);
   }

   printf("\n\nComecando as iteracoes\n");
   /*for (k = 0; k < 15; k++)*/
   do
   {
      maximoValor = 0;
      maximoDiff = 0;
      for (i = 0; i < orderOfMatrix; i++)
      {
         accumulator = 0;

         for (j = 0; j < orderOfMatrix; j++)
         {
            if (i == j)
               accumulator = accumulator + (float)bVector[i];
            else
               accumulator = accumulator - (matrix[i][j] * lastResults[j]);
         }
         accumulator = accumulator / matrix[i][i];
         currentResults[i] = accumulator;

         if (fabs(currentResults[i]) > maximoValor)
         {
            maximoValor = fabs(currentResults[i]);
         }

         diferenca = fabs(currentResults[i] - lastResults[i]);
         if (diferenca > maximoDiff)
         {
            maximoDiff = diferenca;
         }
      }
      /*
      if (maximoDiff / maximoValor <= 0.0015) // 0.0015 está sendo utilizado por conta do arredondamento das casas decimais
      {
         break;
      }*/

      for (i = 0; i < orderOfMatrix; i++)
      {
         lastResults[i] = currentResults[i];
      }
      k++;
   } while (maximoDiff / maximoValor >= 0.0015);

   for (i = 0; i < orderOfMatrix; i++)
   {
      lastResults[i] = currentResults[i];
   }

   printf("Total de iteracoes: %d\n", k);

   /*printf("\nResposta: ");
   for (i = 0; i < orderOfMatrix; i++)
   {
      printf("%.3f ", currentResults[i]);
   }*/
   return 0;
}