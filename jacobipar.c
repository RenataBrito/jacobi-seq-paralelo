#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "utils.h"

#define RAND_UPPER_BOUND 10
#define RAND_LOWER_BOUND 0

int main(int argc, char *argv[])
{

   srand(64591);
   testArguments(argc);
   double wtime;
   int orderOfMatrix = atoi(argv[1]);
   int numberOfThreads = atoi(argv[2]);
   int i, j;
   int* lineSum = malloc(orderOfMatrix*sizeof(long int));
   int bVector[orderOfMatrix];
   printf("Matrix Order: %d, Number of Threads:  %d \n", orderOfMatrix, numberOfThreads);
   int** matrix = malloc(orderOfMatrix*sizeof(*matrix));
   for(i=0;i<orderOfMatrix;i++){
      matrix[i]=malloc(orderOfMatrix*sizeof(*matrix[i]));
   }
   
  // printf(" acaba de criar a matriz\n ");
   // teste do prof
   /* int matrix[3][3] = {
       {4, 2, 1},
       {1, 3, 1},
       {2, 3, 6}};
   int bVector[3] = {7, -8, 6}; */

   // TODO tirar os prints
   //printf("\n");
   //printf("Matriz\n");
   int nthreads,thread_num; //USO NO OPENMP
   # pragma omp parallel for private(i) num_threads(numberOfThreads)
      for (i = 0; i < orderOfMatrix; i++){
         lineSum[i] = 0;
      }
   // Geração de matriz ordem (orderOfMatrix) que sempre passa no critério das linhas.
   for (i = 0; i < orderOfMatrix; i++)
   {
      for (j = 0; j < orderOfMatrix; j++)
      {
         if (i != j){
            matrix[i][j] = (rand() % (RAND_UPPER_BOUND - RAND_LOWER_BOUND + 1)) + RAND_LOWER_BOUND;
            lineSum[i] = lineSum[i] + matrix[i][j];
         }
         matrix[i][i] = (rand() % ((RAND_UPPER_BOUND + lineSum[i]) - RAND_LOWER_BOUND + 1)) + (lineSum[i] + 1); // posicao diagonal principal > soma dos outros elementos.
         bVector[i] = (rand() % (RAND_UPPER_BOUND - RAND_LOWER_BOUND + 1)) + RAND_LOWER_BOUND;                  // vetor B do enunciado do problema.
      }
   }

   wtime = omp_get_wtime();
   /*********************** Criterios de Convergencia ***************************** */
   /************************** Criterio das Linhas ******************************** */
   int lineCriteria = 1;
   #pragma omp parallel for private(i) num_threads(numberOfThreads) 
   for (i = 0; i < orderOfMatrix; i++)
   { 
/*       thread_num = omp_get_thread_num();    
      nthreads = omp_get_num_threads( );
      printf(" \nHello-world da thread %d na região paralela, Num_threads aqui: %d, iteracao %d\n", thread_num, nthreads,i);
 */      lineSum[i] = 0;
   }
   #pragma omp parallel for private(i,j) num_threads(numberOfThreads) //grão do tamanho de cada linha!!!
   for (i = 0; i < orderOfMatrix; i++)
   {
      if (lineCriteria)
      {
         for (j = 0; j < orderOfMatrix; j++)
         {
/*             thread_num = omp_get_thread_num();    
            nthreads = omp_get_num_threads( );
            #pragma omp critical //é mesmo necessário?
            printf(" \nHello-world da thread %d na região paralela, Num_threads aqui: %d, i: %d, j: %d\n", thread_num, nthreads,i,j);
 */            if (i != j)
            {
               lineSum[i] = lineSum[i] + matrix[i][j];
            }
            if (lineSum[i] > matrix[i][i])
            {
               #pragma omp critical (lineCriteriaWrite) //é mesmo necessário?
               {
               lineCriteria = 0;
               }
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
      /*********************** Critério das Colunas ***************************** */
      int colunmCriteria = 1;
      int* colunmSum = malloc(orderOfMatrix*sizeof(long int));

      #pragma omp parallel for private(i) num_threads(numberOfThreads)
      for (i = 0; i < orderOfMatrix; i++)
      {
         colunmSum[i] = 0;
      }
      #pragma omp parallel for private(i,j) num_threads(numberOfThreads)
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
                  #pragma omp critical (colunmCriteriaWrite) // é mesmo necessário?
                  {
                  colunmCriteria = 0;
                  }
                  printf("\nNao converge pelo criterio das colunas e nem das linhas\n");
                  break;
               }
            }
         }
      }
      if (colunmCriteria)
         printf("Converge pelo metodo das colunas");
   }
   /*********************** -------------------- ***************************** */
   /*********************** Calculos dos x_i^k+1 ***************************** */
   /*********************** -------------------- ***************************** */
   float* lastResults = malloc(orderOfMatrix*sizeof(float));
   float  maximoDiff, maximoValor, diferenca;
   float* currentResults = malloc(orderOfMatrix*sizeof(float));
   int k = 0;
   /******************** Definicao do vetor inicial ************************** */
   #pragma omp parallel for private(i) num_threads(numberOfThreads)
   for (i = 0; i < orderOfMatrix; i++)
   {
      lastResults[i] = (float)bVector[i] / (float)matrix[i][i];
   }
   /******************** Iteracoes do metodo de Jacobi *********************** */
   printf("\n\nComecando as iteracoes\n");
   omp_set_nested(1);
   do
   {
      maximoValor = 0;
      maximoDiff = 0;
      #pragma omp parallel for shared(k) private(i,j) schedule(static) \
       num_threads(numberOfThreads) reduction(max: maximoValor) reduction(max: maximoDiff)
      for (i = 0; i < orderOfMatrix; i++)
      {
         currentResults[i] = 0;

         for (j = 0; j < orderOfMatrix; j++)
         {
            if (i == j)
               currentResults[i] += ((float)bVector[i]/(float)matrix[i][i]);
            else
               currentResults[i] -=  ((float) matrix[i][j] * lastResults[j] / (float) matrix[i][i]);
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
/*          thread_num = omp_get_thread_num();    
         nthreads = omp_get_num_threads( );
         #pragma omp critical //é mesmo necessário?
         {
         printf(" \nHello-world da thread %d na região paralela, Num_threads aqui: %d, i: %d, j: %d\n", thread_num, nthreads,i,j);
         }
 */      }
      // passa os valores pro vetor "velho" pra ser usado na proxima iteração
      #pragma omp parallel for private(i) num_threads(orderOfMatrix)
      for (i = 0; i < orderOfMatrix; i++)
      {
         lastResults[i] = currentResults[i];
      }
      #pragma omp critical //Impacto disso no desempenho???
      {
      k++;
      }
      //printf(" \n ---------- Separador --------- \n");
   } while (maximoDiff / maximoValor >= 0.0015);

   wtime = omp_get_wtime() - wtime;
   printf("Tempo Paralelo: %.5f\n", wtime );
   printf("Total de iteracoes: %d\n", k);

   /* printf("\nResposta: \n");
   for (i = 0; i < orderOfMatrix; i++)
   {
      printf("%.3f ", currentResults[i]);
   }
   printf("\n"); */
   return 0;
}