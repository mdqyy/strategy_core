#include "base.h"
#include <iostream>
#include <cstring>
extern "C"
{
#include <blas/cblas.h>
#include <lapacke/lapacke.h> 
}

namespace dense {
class RealMatrix:virtual public Matrix {
 public:
 	REAL *M;
  RealMatrix(UINT row, UINT col);
  virtual ~RealMatrix();
  virtual void print() const;
  virtual void zeroize();
  virtual void randomize();
};

class RealVector:public Vector,
 								 public RealMatrix {
 public:
  RealVector(UINT row, UINT col):Matrix(row, col),
															   Vector(row, col),
															   RealMatrix(row, col) {};
  virtual bool set(const UINT pos, const REAL value);
  virtual bool get(REAL &value, const UINT pos);
};

class RealSquare:public Square,
							   public RealMatrix {
 public:
  RealSquare(UINT size):Matrix(size, size),
											  Square(size),
											  RealMatrix(size, size) {};
  virtual void unitise();
};

bool copy(RealMatrix &M_dest, const RealMatrix &M_src);
bool neg(RealMatrix &A);
bool mm(RealMatrix &C, const RealMatrix &A, const RealMatrix &B);
bool sv(RealVector &b, RealSquare &A);
bool axpy(RealVector &y, const REAL a, const RealVector &x);
}
