#include "dense.h"
#include <sys/time.h>
#include <stdio.h>

void get_random(dense::RealMatrix &A) {
	srand((unsigned int)time(NULL));	
	for (UINT i = 0; i < A.row; i++) {
		for (UINT j = 0; j < A.col; j++) {		
			REAL r = rand()%10;
			A.M[i*A.col + j] = r;
		}
	}
}

int main() {
	for (UINT i = 0; i < 1; i++)	{
		const UINT d = 400;
    struct timeval start, end;
		int timeuse;
		dense::RealMatrix *A = new dense::RealSquare(d);
		dense::RealMatrix *B = new dense::RealSquare(d);
		dense::RealMatrix *C = new dense::RealSquare(d);
		get_random(*A);
		get_random(*B);
		//A->print();
		//B->print();
    gettimeofday(&start, NULL);
		dense::mul(*C, *A, *B);
    gettimeofday(&end, NULL);
		timeuse = 1000000 * ( end.tv_sec - start.tv_sec ) + end.tv_usec - start.tv_usec;
		std::cout << timeuse << std::endl;

		delete A; A = NULL;
		delete B; B = NULL;
		delete C; C = NULL;
	}
	return 0;
}
