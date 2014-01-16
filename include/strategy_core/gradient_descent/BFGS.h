//==============================================================================
/*!
 *  \file BFGS.h
 *
 *  \brief Broyden–Fletcher–Goldfarb–Shanno and LBFGS(Limited-memory BFGS)
 *          (BFGS) method is a method for solving unconstrained nonlinear
 *          optimization problems.
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#ifndef STRATEGY_CORE_GRADIENT_DESCENT_BFGS_H_
#define STRATEGY_CORE_GRADIENT_DESCENT_BFGS_H_

#include <strategy_core/common/common.h>
#include <strategy_core/supervised/LogisticReg.h>
#include <strategy_core/lin_alg/matrix.h>
#include "LineSearch.h"

using namespace LogisticReg;

class BFGS {
 public:
  LrModel *lrmodel;
  BFGS(LrModel *lrmodel, LrPara *lrpara);
  ~BFGS();

  bool init();
  bool solve();

 private:
  LrPara *lrpara;

  dense::DenseRealMatrix *B, *B_inv; /// A symmetric positive definite matrix
  dense::DenseRealVector *p;
  dense::DenseRealVector *wk;
  dense::DenseRealVector *gradient_vector_neg; /// -g
  dense::DenseRealVector *last_gradient_vector;
  dense::DenseRealVector *y; /// y = g^(k + 1) - g^k
  dense::DenseRealVector *last_weight_vector;
  dense::DenseRealVector *delta; /// delta = w^(k + 1) - w^k
  dense::DenseRealMatrix *U; /// y_yt = y * y^T
  REAL yt_delta; /// yt_delta = y^T * delta
  dense::DenseRealMatrix *B_delta; /// B_delta = B * delta
  dense::DenseRealMatrix *deltaT_B; /// deltaT_B = delta^T * B
  dense::DenseRealMatrix *V;
  REAL dtBd;

  LineSearch *ls;
  REAL lambda;
};


class LBFGS:BFGS {
 public:
  LrModel *lrmodel;

  LBFGS(const LrModel *lrmodel, const LrPara *lrpara);
  ~LBFGS();

  bool init();
  bool solve();

 private:
  LrPara *lrpara;
  LineSearch *ls;
  REAL lambda;
};

#endif //STRATEGY_CORE_GRADIENT_DESCENT_BFGS_H_
