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

#include <strategy_core/lin_alg/base.h>

namespace dense {
extern "C" {
#include <cblas.h>
}

class DenseRealVector:Vector {
 public:
  REAL *V;
  DenseRealVector(const UINT row, const UINT col);
  ~DenseRealVector();

  UINT get_length();
  bool is_by_row();
  bool set(const UINT pos, const REAL value);
  bool get(REAL &value, const UINT pos);
};

class DenseRealMatrix:Matrix {
 public:
	REAL **M;
  ///TODO:
	DenseRealMatrix(const UINT row, const UINT col);
	~DenseRealMatrix();

	UINT get_row();
	UINT get_col();
};

}
#endif //STRATEGY_CORE_LIN_ALG_DENSE_H_
