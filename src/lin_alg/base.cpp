//==============================================================================
/*!
 *  \file base.h
 *
 *  \brief implementation of bash.h
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#include <strategy_core/lin_alg/base.h>

Matrix::Matrix(const UINT row, const UINT col) {
  this->row = row;
  this->col = col;
  this->size = row * col;
  this->nz = 0;
  this->sparsity = 0.0;
}

inline void Matrix::cal_sparsity() {
  ///TODO?
  this->sparsity = (REAL)(this->nz) / (this->size);
}

Vector::Vector(const UINT row, const UINT col):Matrix(row, col) {
  if (1 == row and 1 < col) {
    this->by_row = true;
    this->length = col;
  }
  else if (1 == col and 1 < row) {
    this->by_row = false;
    this->length = row;
  }
}
