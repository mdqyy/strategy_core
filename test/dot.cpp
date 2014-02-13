#include <sys/time.h>
#include "dense.h"

int main() {
  for (UINT i = 0; i < 1; i++)	{
    const UINT d = 5;
	  dense::RealVector *a = new dense::RealVector(d, 1);
	  dense::RealVector *b = new dense::RealVector(d, 1);
    a->randomize();
    b->randomize();
    a->print();
	  b->print();
    REAL inner;
    dense::dot(inner, *a, *b);
    printf("inner product:%f\n\n", inner);

	  delete a; a = NULL;
	  delete b; b = NULL;
  }
  return 0;
}

