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
#include "LineSearch.h"

using namespace LogisticReg;

class BFGS {
 public:
  LrModel *lrmodel;
  BFGS(LrModel *lrmodel, LrPara *lrpara);
  ~BFGS();
  bool solve();

 protected:
  bool init();
  bool iter();

 private:
  const LrPara *lrpara;
  dense::DenseRealMatrix *B, *B_inv; /// A symmetric positive definite matrix
  dense::DenseRealMatrix *p;
  dense::DenseRealMatrix *wk;
  dense::DenseRealMatrix *gradient_vector_neg; /// -g
  REAL gradient_vector_norm;
  dense::DenseRealMatrix *last_gradient_vector;
  dense::DenseRealMatrix *y;/// y = g^(k + 1) - g^k
  dense::DenseRealMatrix *last_weight_vector;
  dense::DenseRealMatrix *delta; /// delta = w^(k + 1) - w^k
  dense::DenseRealMatrix *detaT; /// deltaT = delta^T
  dense::DenseRealMatrix *U; /// y_yt = y * y^T
  REAL yt_delta; /// yt_delta = y^T * delta
  dense::DenseRealMatrix *B_delta; /// B_delta = B * delta
  dense::DenseRealMatrix *deltaT_B; /// deltaT_B = delta^T * B
  dense::DenseRealMatrix *V;
  LineSearch *ls;
  REAL dtBd;
  REAL lambda;
};

class LBFGS:public BFGS {
 public:
  LrModel *lrmodel;
  LBFGS(const LrModel *lrmodel, const LrPara *lrpara);
  ~LBFGS();

 protected:
  bool init();
  bool iter();
};

#endif //STRATEGY_CORE_GRADIENT_DESCENT_BFGS_H_
