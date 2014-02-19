#include <sys/time.h>
#include "dense.h"

int main() {
  for (UINT i = 0; i < 1; i++)	{
    const UINT m = 5;
    const UINT n = 3;
    struct timeval start, end;
    dense::RealVector *x = new dense::RealVector(m, 1);
    dense::RealVector *A = new dense::RealVector(m, n);
    dense::RealVector *y = new dense::RealVector(1, n);
    A->randomize();
    x->randomize();
    y->randomize();
      A->print();
    x->print();
    y->print();

    dense::vtm(*y, *x, *A);
    y->print();
    printf("---------------------------------------------------\n");
    
    delete A; A = NULL;
    delete x; x = NULL;
    delete y; y = NULL;
  }
  return 0;
}
