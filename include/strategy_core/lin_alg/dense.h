//==============================================================================
/*!
 *  \file dense.h
 *
 *  \brief dense structures for linear algebra
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#ifndef STRATEGY_CORE_LIN_ALG_DENSE_H_
#define STRATEGY_CORE_LIN_ALG_DENSE_H_


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <strategy_core/lin_alg/base.h>
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
  virtual REAL get(const UINT pos) const;
  virtual void unitise();
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
// M_dest <- M_src
bool copy(RealMatrix &M_dest, const RealMatrix &M_src);

//inner = x*y
bool dot(REAL &inner, const RealVector &x, const RealVector &y);

// norm = ||x||
bool nrm2(REAL &norm, const RealVector &x);

// y = a*x + y
bool axpy(RealVector &y, const REAL alpha, const RealVector &x);

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
bool gemm(RealMatrix &C, const REAL alpha, const REAL beta, const RealMatrix &A,
          const RealMatrix &B);

// C = A * B
bool mm(RealMatrix &C, const RealMatrix &A, const RealMatrix &B);

// C = x * y^t
bool vvt(RealSquare &C, const RealVector &x, const RealVector &y);

// A = -A
bool neg(RealMatrix &A);

// A = alpha*A + beta*B
bool add(RealMatrix &A, const REAL alpha, const REAL beta, const RealMatrix &B);

/* ===========================================================================
 * LAPACK opertations
 * ===========================================================================*/
// solve Ax = b
bool sv(RealVector &b, RealSquare &A);
}
#endif //STRATEGY_CORE_LIN_ALG_DENSE_H_
