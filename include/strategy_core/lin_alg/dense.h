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

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strategy_core/lin_alg/base.h>

namespace dense {
class DenseRealVector:public Vector {
 public:
  REAL *V;
  DenseRealVector(const UINT row, const UINT col);
  ~DenseRealVector();

  bool set(const UINT pos, const REAL value);
  bool get(REAL &value, const UINT pos);
};

class DenseRealMatrix:public Matrix {
 public:
	REAL **M;
  ///TODO:
	DenseRealMatrix(const UINT row, const UINT col);
	~DenseRealMatrix();

	UINT get_row() const;
	UINT get_col() const;
};

/// Vector operations
bool inner_product_dd(REAL &inner, const DenseRealMatrix *drva, const DenseRealMatrix *drvb);
bool euclid_norm_d(REAL &norm, const DenseRealMatrix *drv);

/// DenseRealMatrix operations
bool copy(DenseRealMatrix *M_dest, const DenseRealMatrix *M_src);
bool inv(DenseRealMatrix *B, DenseRealMatrix *A); /// B = A^(-1)
bool add(DenseRealMatrix *C, DenseRealMatrix *A, DenseRealMatrix *B);  /// C = A + B
bool sub(DenseRealMatrix *C, DenseRealMatrix *A, DenseRealMatrix *B);  /// C = A - B
bool mul(DenseRealMatrix *C, DenseRealMatrix *A, DenseRealMatrix *B);  /// C = A * B
bool t_mul(DenseRealMatrix *B, DenseRealMatrix *A);  /// B = A * A^T
bool tranv(DenseRealMatrix *B, DenseRealMatrix *A); /// B = A^T
bool f_norm(REAL &fnorm, DenseRealMatrix *A); /// Frobenius norm.
bool m_norm(REAL &mnorm, DenseRealMatrix *A); /// Manhattan norm of matrix
bool get_min(REAL &min, DenseRealMatrix *A); /// get min
bool get_max(REAL &max, DenseRealMatrix *A);/// get max
bool num_mul(DenseRealMatrix *B, DenseRealMatrix *A, const double num); /// B = num * A
bool num_add(DenseRealMatrix *B, DenseRealMatrix *A, const double num); /// B = [num] + A
bool hadamard_mul(DenseRealMatrix *C, DenseRealMatrix *A, DenseRealMatrix *B);
bool kronecker_mul(DenseRealMatrix *C, DenseRealMatrix *A, DenseRealMatrix *B);
}
#endif //STRATEGY_CORE_LIN_ALG_DENSE_H_
