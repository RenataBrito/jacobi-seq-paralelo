# jacobi-seq-paralelo

Para a execução gerar os binarios
* make

Executar os binarios
* make run-seq
* make run-par

Vale ressaltar que a ordem da matriz e o numero de threads devera ser passado como argumento da execucao. 
Como padrao estamos enviando a ordem da matriz 1000 e 2 threads para o paralelo. 
Caso queira diferente deverá editar o makefile ou passar os parametros por exemplo:
* ./execSeq 100 1
* ./execPar 100 4
