//==============================================================================
/*!
 *  \file dense.cpp
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

#include <strategy_core/lin_alg/dense.h>
#include <strategy_core/common/log4c.h>

namespace dense {
/// DenseRealVector
DenseRealVector::DenseRealVector (const INT row , const INT col) {
  Matrix::is_sparse = false;
  this->V = new REAL[Vector::length];
}

DenseRealVector::~DenseRealVector() {
  delete []this->V;
  this->V = NULL;
}

UINT DenseRealVector::get_length() {
  return Vector::length;
}

bool DenseRealVector::is_by_row() {
  return Vector::by_row;
}

bool DenseRealVector::set(const INT pos, const REAL value) {
  bool flag = true;
  if (pos < 0 or pos >= Vector::length) {
    L4C_ERROR("Array reference out of bound in DenseRealVector::set()!");
    flag = false;
    goto end;
  }

  this->V[pos] = value;

end:
  return flag;
}

bool DenseRealVector::get(const REAL &value, const INT pos) {
  bool flag = true;
  if (pos < 0 or pos >= Vector::length) {
    L4C_ERROR("Array reference out of bound in DenseRealVector::get()!");
    flag = false;
    goto end;
  }

  value = this->V[pos];

end:
  return flag;
}

/// DenseRealMatrix
DenseRealMatrix::DenseRealMatrix(const INT row, const INT col) {
  Matrix::is_vector = false;
  Matrix::is_sparse = false;
  Matrix::is_square = false;
  Matrix::is_diag = false;
  if (row == col)
    Matrix::is_square = true;
  this->M = new REAL[row];
  for (INT i = 0; i < col; i++)
    this->M[i] = new REAL[col];
}

DenseRealMatrix::~DenseRealMatrix() {
  for (INT i = 0; i < this->row; i++) {
    delete []this->M[i];
    this->M[i] = NULL;
  }
  this->M = NULL;
}

DenseRealMatrix::get_row() {
  return Matrix::row;
}

DenseRealMatrix::get_col() {
  return Matrix::col;
}

}
