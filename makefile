all:
	gcc -fopenmp jacobi-seq.c -o execSeq

run:
	./execSeq 10000 2

