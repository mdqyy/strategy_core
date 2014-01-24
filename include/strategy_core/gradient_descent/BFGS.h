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
  dense::RealMatrix *B, *B_inv; /// A symmetric positive definite matrix
  dense::RealMatrix *p;
  dense::RealMatrix *wk;
  dense::RealMatrix *gradient_vector_neg; /// -g
  REAL gradient_vector_norm;
  dense::RealMatrix *last_gradient_vector;
  dense::RealMatrix *y;/// y = g^(k + 1) - g^k
  dense::RealMatrix *last_weight_vector;
  dense::RealMatrix *delta; /// delta = w^(k + 1) - w^k
  dense::RealMatrix *deltaT; /// deltaT = delta^T
  dense::RealMatrix *U; /// y_yt = y * y^T
  REAL yt_delta; /// yt_delta = y^T * delta
  dense::RealMatrix *B_delta; /// B_delta = B * delta
  dense::RealMatrix *deltaT_B; /// deltaT_B = delta^T * B
  dense::RealMatrix *V;
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
