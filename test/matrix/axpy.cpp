#include <sys/time.h>
#include "dense.h"
#include <stdio.h>
#include <lapacke/lapacke.h>

int main() {
  for (UINT i = 0; i < 1; i++)  {
    const UINT d = 5;
    dense::RealVector *y = new dense::RealVector(d, 1);
    dense::RealVector *x = new dense::RealVector(d, 1);
    REAL alpha = -1;
    y->randomize();
    x->randomize();
    y->print();
    x->print();
    printf("%f\n", alpha);
    dense::axpy(*y, alpha, *x);
    y->print();
    x->print();
    printf("---------------------------------------------------\n");

    delete y; y = NULL;
    delete x; x = NULL;
  }
  return 0;
}

