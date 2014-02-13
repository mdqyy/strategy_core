#include <sys/time.h>
#include "dense.h"

int main() {
  for (UINT i = 0; i < 1; i++)	{
    const UINT d = 5;
    struct timeval start, end;
	  dense::RealVector *a = new dense::RealVector(d, 1);
    dense::RealSquare *C = new dense::RealSquare(d);
    a->randomize();
    a->print();

    dense::vvt(*C, *a, *a);
    C->print();

	  delete a; a = NULL;
    delete C; C = NULL;
  }
  return 0;
}

