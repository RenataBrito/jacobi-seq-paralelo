#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include <omp.h>

#define RAND_UPPER_BOUND 10
#define RAND_LOWER_BOUND 0

int main(int argc, char *argv[])
{
   double wtime;

   srand(64591);
   testArguments(argc);
   int orderOfMatrix = atoi(argv[1]);
   int numberOfThreads = atoi(argv[2]);
   int i, j;
   int* lineSum = malloc(orderOfMatrix*sizeof(long int));
   int bVector[orderOfMatrix];
   printf("\nMatrix Order: %d, Number of Threads:  %d \n", orderOfMatrix, numberOfThreads);
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
      lineSum[i] = 0;
   }
   // Geração de matriz ordem (orderOfMatrix) que sempre passa no critério das linhas.
   for (i = 0; i < orderOfMatrix; i++)
   {
      for (j = 0; j < orderOfMatrix; j++)
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

   /*********************** Criterios de Convergencia ***************************** */
   /************************** Criterio das Linhas ******************************** */
   wtime = omp_get_wtime();

   int lineCriteria = 1; 
   for (i = 0; i < orderOfMatrix; i++)
   {
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
               printf("\nNao converge pelo criterio das linhas!\n");
               break;
            }
         }
      }
   }
   if (lineCriteria)
   {
      printf("\nA matriz converge pelo criterio das linhas!\n");
   }
   else
   {
      /*********************** Critério das Colunas ***************************** */
      int colunmCriteria = 1;
      int* colunmSum = malloc(orderOfMatrix*sizeof(long int));

      for (i = 0; i < orderOfMatrix; i++)
      {
         colunmSum[i] = 0;
      }

      for (i = 0; i < orderOfMatrix; i++)
      {
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
                  printf("\nNao converge pelo criterio das colunas e nem das linhas!\n");
                  break;
               }
            }
         }
      }
      if (colunmCriteria)
         printf("Converge pelo metodo das colunas!");
   }
   /*********************** -------------------- ***************************** */
   /*********************** Calculos dos x_i^k+1 ***************************** */
   /*********************** -------------------- ***************************** */
   float* lastResults = malloc(orderOfMatrix*sizeof(float));
   float  maximoDiff, maximoValor, diferenca;
   float* currentResults = malloc(orderOfMatrix*sizeof(float));
   int k = 0;
   /******************** Definicao do vetor inicial ************************** */
   for (i = 0; i < orderOfMatrix; i++)
   {
      lastResults[i] = (float)bVector[i] / (float)matrix[i][i];
      currentResults[i] = 0; //redundant?
   }
   /******************** Iteracoes do metodo de Jacobi *********************** */
   printf("\nComecando as iteracoes\n");
   do
   {
      maximoValor = 0;
      maximoDiff = 0;
      for (i = 0; i < orderOfMatrix; i++)
      {
         currentResults[i] = 0;

         for (j = 0; j < orderOfMatrix; j++)
         {
            if (i == j)
               currentResults[i] = currentResults[i] + ((float)bVector[i]/(float)matrix[i][i]);
            else
               currentResults[i] = currentResults[i] - ((float) matrix[i][j] * lastResults[j] / (float) matrix[i][i]);
         }
         //Define o max(vetorAtual) para usar no criterio de parada
         if (fabs(currentResults[i]) > maximoValor)
         {
            maximoValor = fabs(currentResults[i]);
         }
         //Define a maior diferença para ser usada no criterio de parada
         if (fabs(currentResults[i] - lastResults[i]) > maximoDiff)
         {
            maximoDiff = fabs(currentResults[i] - lastResults[i]);
         }
      }
      // passa os valores pro vetor "velho" pra ser usado na proxima iteração
      for (i = 0; i < orderOfMatrix; i++)
      {
         lastResults[i] = currentResults[i];
      }
      k++;
   } while (maximoDiff / maximoValor >= 0.0015);

   printf("Total de iteracoes: %d\n", k);
   wtime = omp_get_wtime() - wtime;

    
    printf ("Tempo sequencial: %.5f\n", wtime );

   /*printf("\nResposta: ");
   for (i = 0; i < orderOfMatrix; i++)
   {
      printf("%.3f ", currentResults[i]);
   }*/
   return 0;
}