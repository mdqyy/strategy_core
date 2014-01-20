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

/// Matrix operations
bool copy(Matrix *M_dest, const Matrix *M_src);
bool inv(Matrix *B, Matrix *A); /// B = A^(-1)
bool add(Matrix *C, Matrix *A, Matrix *B);  /// C = A + B
bool sub(Matrix *C, Matrix *A, Matrix *B);  /// C = A - B
bool mul(Matrix *C, Matrix *A, Matrix *B);  /// C = A * B
bool t_mul(Matrix *B, Matrix *A);  /// B = A * A^T
bool tranv(Matrix *B, Matrix *A); /// B = A^T
bool f_norm(REAL &fnorm, Matrix *A); /// Frobenius norm.
bool m_norm(REAL &mnorm, Matrix *A); /// Manhattan norm of matrix
bool get_min(REAL &min, Matrix *A); /// get min
bool get_max(REAL &max, Matrix *A);/// get max
bool num_mul(Matrix *B, Matrix *A, const double num); /// B = num * A
bool num_add(Matrix *B, Matrix *A, const double num); /// B = [num] + A
bool print(const Matrix *A);  /// Output matrix.
bool hadamard_mul(Matrix *C, Matrix *A, Matrix *B);
bool kronecker_mul(Matrix *C, Matrix *A, Matrix *B);
}
#endif //STRATEGY_CORE_LIN_ALG_DENSE_H_
