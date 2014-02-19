#include <sys/time.h>
#include "dense.h"

int main() {
  for (UINT i = 0; i < 1; i++)  {
    const UINT d = 5;
    struct timeval start, end;
      dense::RealSquare *A = new dense::RealSquare(d);
    dense::RealSquare *A_copy = new dense::RealSquare(d);
      dense::RealVector *b = new dense::RealVector(d, 1);
    dense::RealVector *b_copy = new dense::RealVector(d, 1);
    dense::RealVector *c = new dense::RealVector(d, 1);
    A->unitise();
    b->randomize();
    dense::copy(*A_copy, *A);
    dense::copy(*b_copy, *b);
    dense::neg(*b_copy);
    A->print();
      b->print();
    b_copy->print();

    dense::sv(*b_copy, *A);
    A->print();
    b_copy->print();

    dense::mm(*c, *A_copy, *b_copy);
    c->print();
    printf("---------------------------------------------------\n");
    
    delete A; A = NULL;
    delete A_copy; A_copy = NULL;
    delete b; b = NULL;
    delete b_copy; b_copy = NULL;
    delete c; c = NULL;
  }
  return 0;
}

