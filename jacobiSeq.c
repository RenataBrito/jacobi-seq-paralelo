#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_ITERACOES 50 // número máximo de iterações que serão efetuadas caso o valor da tolerância não seja superado

//Funções utlizadas ao longo do programa
double norma_infinito(double dim, double *v);
double erro_relativo(double x0, double x1);
int compara(double erro, double tolerancia);
void jacobi(int dim, double **matriz, double *x, double *b, double tolerancia);
int criterio_das_linhas(int n, double** matriz);
void seidel(int dim, double** matriz, double*x, double *b, double tolerancia);
int sassenfeld(int dim, double** matriz);

int main(){


	int i,j,dim;
	double tolerancia;

	printf("Insira a dimensão da matriz desejada: \n");
	scanf("%d", &dim);

	double **matriz, *x, *b;

	//Alocamos memória dinâmicamente para as matrizes e os vetores
	matriz = (double**) malloc(sizeof(double*) *dim);
	for(i = 0; i < dim; i++)
		matriz[i] = (double*) malloc(sizeof(double) *dim);

	x = (double*) malloc(sizeof(double) * dim);
	b = (double*) malloc(sizeof(double) * dim);

	// O usuário entra com a matriz, e em seguida , com os vetores
	printf("Insira a matriz (A): \n");
	for(i =0; i < dim; i++)
		for(j = 0; j < dim; j++)
			scanf("%lf", &matriz[i][j]);

	printf("Insira o vetor aproximação x (geralmente é zerado):\n");
	for(i =0; i < dim; i++)
		scanf("%lf", &x[i]);

	printf("Insira o vetor solução (b): \n");
	for(i = 0; i < dim; i++)
		scanf("%lf", &b[i]);

	printf("Tolerância: \n");
	scanf("%lf", &tolerancia);


	printf("MÉTODO DE JACOBI\n\n");

	//Primeiramente, efetuamos o critério das linhas.
	criterio_das_linhas(dim, matriz);
	//Chamamos a função jacobi
	jacobi(dim, matriz, x, b, tolerancia);

	printf("\n\nMÉTODO DE GAUSS - SEIDEL\n\n");

	//Efetuamos o critério de sassenfels
	sassenfeld(dim, matriz);
	//Chamamos a função seidel
	seidel(dim, matriz, x, b, tolerancia);
	
	free(matriz);
	free(x);
	free(b);

	return 0;
}


double norma_infinito(double dim, double *v){

	double maior = v[0];

	for(int i = 1; i < dim; i++)
		if(v[i] > maior)
			maior = v[i];
	return maior;
}


double erro_relativo(double x0, double x1){
	double erro =  fabs((fabs(x1) - fabs(x0) )/ fabs(x1) );
	return erro;
}

//erro > tolerância -> retorna 0, caso contrário, retornamos 1
int compara(double erro, double tolerancia){

	if(erro > tolerancia){
		printf("Erro %lf > Tolerância %lf \n\n", erro, tolerancia);
		return 0;
	}
	
	printf("Erro %lf < Tolerância %lf. Fim das iterações !\n", erro, tolerancia);
	return 1;
}

void jacobi(int dim, double **matriz, double *x, double *b, double tolerancia){

	int i,j,k;
	double erro, ** iteracoes;

	// Criamos uma matriz, que armazena as iterações em suas linhas, e o valor destas em suas colunas.
	iteracoes = (double**) malloc(sizeof(double*) * MAX_ITERACOES);
	for(i =0; i < MAX_ITERACOES; i++)
		iteracoes[i] = (double*) malloc(sizeof(double)*dim);

	//inicializamos a matriz de iterações com os valores do vetor x nas primeiras posições, e 0 nas seguintes
		for(k =0; k < MAX_ITERACOES; k++){
			for(i = 0; i < dim; i ++){
				iteracoes[k][i] = 0;
				if(k == 0){
					iteracoes[k][i] = x[i];
				}
			}
		}

	// Eftuamos as contas do sistema 
	for(k = 1; k < MAX_ITERACOES; k ++){
		printf("Iteração %d: ", k);
		for(i = 0; i < dim; i ++){
			for(j = 0; j < dim; j ++){

				if(i == j)
					iteracoes[k][i] += b[i];
				else
					iteracoes[k][i] -= iteracoes[k-1][j] * matriz[i][j];
				
			}

				iteracoes[k][i] /= matriz[i][i];
			printf("%lf ", iteracoes[k][i]);
		}

		printf("\n");
		//Calculamos o erro relativo dessa iteração
		erro = erro_relativo( norma_infinito(dim, iteracoes[k-1]), norma_infinito(dim, iteracoes[k])) ;
		// se o erro for menor que a tolerância, terminamos as iterações , saindo do loop
		if(compara(erro, tolerancia) == 1)
			break;		
	}

	free(iteracoes);

}

void seidel(int dim, double** matriz, double*x, double *b, double tolerancia){

	int i, j, k = 0;

	double erro;
	// Criamos uma matriz, que armazena as iterações em suas linhas, e o valor destas em suas colunas.
	double **iteracoes = (double**) malloc(sizeof(double*) * MAX_ITERACOES);
	for(i = 0; i < MAX_ITERACOES; i++)
		iteracoes[i] = (double*) malloc(sizeof(double)* dim);
	
		for(k =0; k < MAX_ITERACOES; k++){
			for(i = 0; i < dim; i ++){
				iteracoes[k][i] = 0;
				if(k == 0){
					iteracoes[k][i] = x[i];
				}
			}
		}
	
	for(k = 1; k < MAX_ITERACOES; k ++){
		printf("Iteração %d: ", k);
		for(i = 0; i < dim; i ++){
			for(j = 0; j < dim; j ++){
				if(i == j)
					iteracoes[k][i] += b[i];
				
				else
				{
					if(j <= i)
						iteracoes[k][i] -= iteracoes[k][j] * matriz[i][j];
					else
						iteracoes[k][i] -= iteracoes[k-1][j] * matriz[i][j];
				}
			}
			iteracoes[k][i] /= matriz[i][i];
			printf("%lf ", iteracoes[k][i]);
		}


		printf("\n");
		erro = erro_relativo( norma_infinito(dim, iteracoes[k-1]), norma_infinito(dim, iteracoes[k])) ;
		if(compara(erro, tolerancia) == 1)
			break;		
	}
	
	free(iteracoes);
}

// satisfeito -> retorna 1. Não satisfeito -> retorna 0
int criterio_das_linhas(int dim, double** matriz){

	printf("Verificando o critério das linhas...\n\n");

	int i,j,k, diagonal,satisfaz = 1;
	double alfas[dim], somatorio = 0, maior;

	for(i = 0; i < dim; i++){
		for(j = 0; j < dim; j++){

			if(i == j)
				diagonal = matriz[i][j]; //armazenamos o valor da diagonal
			else 
				somatorio += matriz[i][j]; //caso o elemento não esteja na diagonal principal, efetuamos o somatório
		}
		alfas[i] = somatorio/diagonal; // adicionamos o alfa no vetor
		if(alfas[i] >= 1)
		{
			// Caso algum alfa for maior ou igual a 1, saímos do loop. O critério das linhas não foi satisfeito
			satisfaz = 0;
			break;
		}
		somatorio = 0;
	} 

	maior = alfas[0];
	//selecionamos o maior alfa 
	for(i =1; i < dim; i++){
		if(alfas[i] > maior)
			maior = alfas[i];
	}

	if(satisfaz == 1){
		printf("Satisifaz o critério das linhas\n Maior alfa: %lf < 1\n\n", maior);
		return 1;
	}

	printf("Não satisfaz o critério das linhas.\n\n");
	return 0;
}

// satisfeito -> retorna 1. Não satisfeito -> retorna 0
int sassenfeld(int dim, double** matriz){

	printf("Verififcando o critério de Sassenfeld ...\n\n\n");

    int i, j, satisfaz = 1;
	double betas[dim],diagonal = 0, outros = 0;

	for(i = 0; i < dim; i++){
		for(j = 0; j < dim; j++){
			if(i == j) // caso o elemento esteja na diagonal principal, seu módulo será guardado
				diagonal = abs(matriz[i][j]);
			else{
				if(j >= i)
					outros += abs(matriz[i][j]);
				else
					outros += abs(matriz[i][j]) * betas[j];
			}
		}

		betas[i] = outros / diagonal;
		if(betas[i] >= 1){ // Caso algum alfa for maior ou igual a 1, saímos do loop. O critério das sassenfeld não foi satisfeito
			satisfaz = 0;
			break;
		}
	}

	if(satisfaz == 1){
		printf("Satisfaz o critério de Sassenfeld! ");
		int maxIndex = 0;
		// selecionamos o índice do maior beta
		for(int i = 0; i < dim; i++){
			if(betas[i] > betas[maxIndex])
				maxIndex = i;
		}

		printf("O beta de maior valor é: %lf\n\n", betas[maxIndex]);
		return 1;
	}
	
		printf("Não satisfaz o critério de Sassenfeld\n\n");
		return 0;
}