#include <sys/time.h>
#include "dense.h"

int main() {
  for (UINT i = 0; i < 1; i++)	{
    const UINT d = 5;
    struct timeval start, end;
	  dense::RealVector *b = new dense::RealVector(d, 1);
    b->randomize();
	  b->print();
    REAL norm;
    dense::nrm2(norm, *b);
    printf("eculid norm:%f\n", norm);
    printf("\n");

	  delete b; b = NULL;
  }
  return 0;
}
