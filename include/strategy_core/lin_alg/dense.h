//==============================================================================
/*!
 *  \file dense.h
 *
 *  \brief dense structures for linear algebra
 *
 *  \author chenqinyu
 *          contact:
 *              qinyu.chen@corp.elong.com
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par http://www.elong.com<BR>
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
  INT length;
  REAL *V;
  DenseRealVector(const INT length);
  ~DenseRealVector();
  bool set(const INT pos, const REAL value);
  bool get(REAL &value, const INT pos);
};

class DenseRealMatrix:Matrix {
 public:
	INT row;
	INT col;
	REAL **M;
	DenseRealMatrix();
	~DenseRealMatrix();
};

}
#endif //STRATEGY_CORE_LIN_ALG_DENSE_H_
