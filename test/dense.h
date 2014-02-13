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

/* ===========================================================================
 * level 1 BLAS opertations
 * ===========================================================================*/
bool copy(RealMatrix &M_dest, const RealMatrix &M_src);
bool dot(REAL &inner, const RealVector &x, const RealVector &y); /// inner = x * y
bool nrm2(REAL &norm, const RealVector &x); /// norm = ||x||
bool axpy(RealVector &y, const REAL alpha, const RealVector &x); /// y = a*x + y

/* ===========================================================================
 * level 2 BLAS opertations
 * ===========================================================================*/
// y = alpha*A*x + beta*y
bool gemv(RealVector &y, const REAL alpha, const REAL beta, const RealMatrix &A,
          const RealVector &x);

// y = A * x + y
bool mv(RealVector &y, const RealMatrix &A, const RealVector &x);

// y = alpha*xt *A + beta*y
bool gevtm(RealVector &y, const REAL alpha, const REAL beta,
           const RealVector &x, const RealMatrix &A);

// y = xt *A + y
bool vtm(RealVector &y, const RealVector &x, const RealMatrix &A);

/* ===========================================================================
 * level 3 BLAS opertations
 * ===========================================================================*/
// C = alpha*A*B + beta*C
bool gemm(RealMatrix &C, const REAL alpha, const REAL beta, const RealMatrix &A, const RealMatrix &B);

// C = A * B
bool mm(RealMatrix &C, const RealMatrix &A, const RealMatrix &B);

// C = x * y^t
bool vvt(RealSquare &C, const RealVector &x, const RealVector &y);

// A = -A
bool neg(RealMatrix &A);

// A = alpha*A + beta*Bbool sub(RealMatrix &C, RealMatrix &A, RealMatrix &B);
bool add(RealMatrix &A, const REAL alpha, const REAL beta, const RealMatrix &B);

/* ===========================================================================
 * LAPACK opertations
 * ===========================================================================*/
// solve Ax = b
bool sv(RealVector &b, RealSquare &A);
}
