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
 *              qinyu.chen@corp.elong.com
 *  \date 2013-07
 *
 *  \par http://www.elong.com<BR>
 */
//==============================================================================

#ifndef STRATEGY_CORE_GRADIENT_DESCENT_BFGS_H_
#define STRATEGY_CORE_GRADIENT_DESCENT_BFGS_H_

#include <strategy_core/common/common.h>
#include <strategy_core/supervised/LogisticReg.h>
#include "LineSearch.h"

using namespace LogisticReg;

class BFGS {
 public:
  LrModel *lrmodel;
  BFGS(const LrModel *lrmodel, const LrPara *lrpara);
  ~BFGS();

  bool init();
  bool solve();

 private:
  LrPara *lrpara;

  DenseRealMatrix *B, *B_inv; /// A symmetric positive definite matrix
  DenseRealVector *p;
  DenseRealVector *wk;
  DenseRealVector *gradient_vector_neg; /// -g
  DenseRealVector *last_gradient_vector;
  DenseRealVector *y; /// y = g^(k + 1) - g^k
  DenseRealVector *last_weight_vector;
  DenseRealVector *delta; /// delta = w^(k + 1) - w^k
  DenseRealMatrix *U; /// y_yt = y * y^T
  REAL yt_delta; /// yt_delta = y^T * delta
  DenseRealMatrix *B_delta; /// B_delta = B * delta
  DenseRealMatrix *deltaT_B; /// deltaT_B = delta^T * B
  DenseRealMatrix *V;
  REAL dtBd;

  LineSearch *ls;
  REAL lambda;
};


class LBFGS:BFGS {
 public:
  LrModel *lrmodel;

  BFGS(const LrModel *lrmodel, const LrPara *lrpara);
  ~BFGS();

  bool init();
  bool solve();

 private:
  LrPara *lrpara;
  LineSearch *ls;
  REAL lambda;
};

#endif //STRATEGY_CORE_GRADIENT_DESCENT_BFGS_H_
