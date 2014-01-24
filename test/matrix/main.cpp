#include "dense.h"

int main() {
	for (UINT i = 0; i < 1; i++)	{
		dense::RealMatrix *A = new dense::RealSquare(15);
		dense::RealMatrix *B = new dense::RealSquare(15);
		dense::inv(B, A);
		B->print();

		delete A; A = NULL;
		delete B; B = NULL;
	}
	return 0;
}
