all:
	gcc -c -O3 -I. -o example_DGELS_colmajor.o example_DGELS_colmajor.c
	gcc -c -O3 -I. -o lapacke_example_aux.o lapacke_example_aux.c
	gfortran example_DGELS_colmajor.o lapacke_example_aux.o -llapacke -llapack -lblas -o xexample_DGELS_colmajor
	#gcc -o xlapacke -O3 example_DGELS_colmajor.c lapacke_example_aux.c -I /home/cqy/local/include/ -llapacke -llapack -lblas
clean:
	rm -f *.o x*
