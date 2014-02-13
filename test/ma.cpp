#include <sys/time.h>
#include "dense.h"

int main() {
  for (UINT i = 0; i < 1; i++)	{
    const UINT d = 5;
    struct timeval start, end;
	  dense::RealSquare *A = new dense::RealSquare(d);
    dense::RealSquare *B = new dense::RealSquare(d);
    A->randomize();
    B->randomize();
    A->print();
    B->print();
    
    dense::add(*A, -1, 1, *B);
    A->print();

	  delete A; A = NULL;
    delete B; B = NULL;
  }
  return 0;
}

