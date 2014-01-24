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
  this->sparsity = (REAL)(this->nz) / (this->row * this->col);
}

Vector::Vector(const UINT row, const UINT col):Matrix(row, col) {
	if (1 == row) {
		this->size = col;
		this->by_row = false;
	}
	else {
		this->size = row;
		this->by_row = true;
	}
}
