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

inline void Matrix::cal_sparsity() {
  sparsity = (REAL)(nz) / (row * col);
}

Vector::Vector(UINT row, UINT col):Matrix(row, col) {
  if (1 == row) {
	  size = col;
	  by_row = false;
  }
  else {
	  size = row;
	  by_row = true;
  }
}
