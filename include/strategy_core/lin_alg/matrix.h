//==============================================================================
/*!
 *  \file matrix.h
 *
 *  \brief matrix operations definition.
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================


#ifndef STRATEGY_CORE_LIN_ALG_MATRIX_H_
#define STRATEGY_CORE_LIN_ALG_MATRIX_H_

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strategy_core/lin_alg/dense.h>

#define FAIL -999999
#define Min -9999999999
#define Max 9999999999
#define VERY_SMALL_NUM 1e-100
#define PI 3.1415926

namespace matrix {
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
bool nlz(Matrix *B, Matrix *A); /// Normalize
bool print(const Matrix *A);  /// Output matrix.
bool hadamard_mul(Matrix *C, Matrix *A, Matrix *B);
bool kronecker_mul(Matrix *C, Matrix *A, Matrix *B);
}
#endif //STRATEGY_CORE_LIN_ALG_MATRIX_H_
