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
#include <strategy_core/lin_alg/vector.h>
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
  const LrPara *lrpara;

  Matrix *B, *B_inv; /// A symmetric positive definite matrix
  Matrix *p;
  Matrix *wk;
  Matrix *gradient_vector_neg; /// -g
  REAL gradient_vector_norm;
  Matrix *last_gradient_vector;
  Matrix *gradient_vector_sub;
  Matrix *y; /// y = g^(k + 1) - g^k
  Matrix *last_weight_vector;
  Matrix *weight_vector_sub;
  Matrix *delta; /// delta = w^(k + 1) - w^k
  Matrix *detaT; /// deltaT = delta^T
  Matrix *U; /// y_yt = y * y^T
  REAL yt_delta; /// yt_delta = y^T * delta
  Matrix *B_delta; /// B_delta = B * delta
  Matrix *deltaT_B; /// deltaT_B = delta^T * B
  Matrix *V;
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
};

#endif //STRATEGY_CORE_GRADIENT_DESCENT_BFGS_H_
