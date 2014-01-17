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
  this->nz = 0;
  this->sparsity = 0.0;
  this->is_vector = false;
  this->is_sparse = false;
  this->is_diag = false;
  this->is_square = false;
  if (row == col) {
    this->is_square = true;
  }
}

Vector::Vector(const UINT row, const UINT col) {
  Matrix::row = row;
  Matrix::col = col;
  if (1 == row and 1 < col) {
    this->by_row = true;
    this->length = col;
  }
  else if (1 == col and 1 < row) {
    this->by_row = false;
    this->length = row;
  }
  Matrix::is_vector = true;
  Matrix::is_square = false;
}

#endif //STRATEGY_CORE_LIN_ALG_BASE_H_
