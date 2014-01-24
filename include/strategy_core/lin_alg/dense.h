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
#include <strategy_core/lin_alg/base.h>

namespace dense {
class RealMatrix:public Matrix {
 public:
	REAL **M;
	RealMatrix(const UINT row, const UINT col);
	virtual ~RealMatrix();
  virtual void print() const;
};

class RealSquare:public Square,
                 public RealMatrix {
 public:
	RealSquare(const UINT size):Matrix(size, size),
                              Square(size),
                              RealMatrix(size, size) {};
};

class RealVector:public Vector,
                 public RealMatrix {
 public:
	RealVector(const UINT row, const UINT col):Matrix(row, col),
                                             Vector(row, col),
                                             RealMatrix(row, col) {};
  bool set(const UINT pos, const REAL value);
  bool get(REAL &value, const UINT pos);
};

/// RealMatrix operations
bool copy(RealMatrix *M_dest, const RealMatrix *M_src);
bool inv(RealMatrix *B, const RealMatrix *A); /// B = A^(-1)
bool add(RealMatrix *C, RealMatrix *A, RealMatrix *B);  /// C = A + B
bool sub(RealMatrix *C, RealMatrix *A, RealMatrix *B);  /// C = A - B
bool mul(RealMatrix *C, const RealMatrix *A, const RealMatrix *B);  /// C = A * B
bool t_mul(RealMatrix *B, RealMatrix *A);  /// B = A * A^T
bool tranv(RealMatrix *B, RealMatrix *A); /// B = A^T
bool e_norm(REAL &norm, const RealMatrix *A); /// Euclid norm.
bool f_norm(REAL &fnorm, const RealMatrix *A); /// Frobenius norm.
bool m_norm(REAL &mnorm, const RealMatrix *A); /// Manhattan norm of matrix
bool get_min(REAL &min, const RealMatrix *A); /// get min
bool get_max(REAL &max, const RealMatrix *A);/// get max
bool num_mul(RealMatrix *B, RealMatrix *A, const REAL num); /// B = [num] * A
bool num_add(RealMatrix *B, RealMatrix *A, const REAL num); /// B = [num] + A
bool h_mul(RealMatrix *C, const RealMatrix *A, const RealMatrix *B); /// hardamard mul
bool k_mul(RealMatrix *C, const RealMatrix *A, const RealMatrix *B); /// kronecker mul

/// Vector operations
bool inner_product(REAL &inner, const RealVector *rva, const RealVector *rvb);
}
#endif //STRATEGY_CORE_LIN_ALG_DENSE_H_
