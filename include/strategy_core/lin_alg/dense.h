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

/// Vector operations
bool inner_product(REAL &inner, const RealVector *rva, const RealVector *rvb);

/// RealMatrix operations

bool copy(RealMatrix &M_dest, const RealMatrix &M_src);
bool mm(RealMatrix &C, const RealMatrix &A, const RealMatrix &B);  /// C = A * B
bool sv(RealVector &b, RealSquare &A);
bool neg(RealMatrix &A);
bool add(RealMatrix &C, RealMatrix &A, RealMatrix &B);  /// C = A + B
bool sub(RealMatrix &C, RealMatrix &A, RealMatrix &B);  /// C = A - B
bool e_norm(REAL &norm, const RealMatrix &A);
bool f_norm(REAL &fnorm, const RealMatrix &A);
}
#endif //STRATEGY_CORE_LIN_ALG_DENSE_H_
