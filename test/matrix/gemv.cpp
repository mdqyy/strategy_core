#include <sys/time.h>
#include "dense.h"

int main() {
  for (UINT i = 0; i < 1; i++)  {
    const UINT m = 2;
    const UINT n = 2;
    struct timeval start, end;
    dense::RealVector *A = new dense::RealVector(m, n);
    dense::RealVector *x = new dense::RealVector(n, 1);
    dense::RealVector *y = new dense::RealVector(m, 1);
    A->randomize();
    x->randomize();
    y->randomize();
    A->print();
    x->print();
    y->print();

    dense::mv(*y, *A, *x);
    y->print();
    printf("---------------------------------------------------\n");
    
    delete A; A = NULL;
    delete x; x = NULL;
    delete y; y = NULL;
  }
  return 0;
}
