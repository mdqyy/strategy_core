#include "dense.h"

int main() {
	for (UINT i = 0; i < 1; i++)	{
		dense::RealMatrix *A = new dense::RealSquare(3);
		dense::RealMatrix *B = new dense::RealSquare(3);
		REAL M[3][3] = {0,1,2,1,2,3,2,3,4};
		memmove((void *)A->M, (void *)M, 3 * 3 * sizeof(REAL));
		dense::inv(B, A);
		B->print();

		delete A; A = NULL;
		delete B; B = NULL;
	}
	return 0;
}
