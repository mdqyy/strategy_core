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
  REAL step_len;
  REAL gv_norm;
  REAL lambda;
  REAL epsilon;
  dense::RealSquare *B; /// A symmetric positive definite matrix
  dense::RealSquare *B_copy;
  dense::RealVector *pv;
  dense::RealVector *wv;
  dense::RealVector *gv;
  dense::RealVector *yv;/// y = g^(k + 1) - g^k
  LineSearch *ls;
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
