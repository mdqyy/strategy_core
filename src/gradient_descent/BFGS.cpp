//==============================================================================
/*!
 *  \file BFGS.cpp
 *
 *  \brief Broyden–Fletcher–Goldfarb–Shanno
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

#include <strategy_core/gradient_descent/BFGS.h>

/// BFGS
BFGS::BFGS(LrModel *lrmodel, LrPara *lrpara) {
  const UINT d = lrmodel->ss->feature_num;
  this->lrmodel = lrmodel;
  this->lrpara = lrpara;
  gv_norm = 0;
  lambda = 0;
  step_len = lrpara->step_len;
  epsilon = lrpara->epsilon;
  wv = lrmodel->wv;
  gv = lrmodel->gv;
  B = new dense::RealSquare(d);
  B_copy = new dense::RealSquare(d);
  pv = new dense::RealVector(d, 1);
  yv = new dense::RealVector(d, 1);
  ls = new LineSearch(d, 1);
}

BFGS::~BFGS() {
  delete B; B = NULL;
  delete B_copy; B_copy = NULL;
  delete pv; pv = NULL;
  delete yv; yv = NULL;
  delete ls; ls = NULL;
}

bool BFGS::init() {
  bool flag = true;
  B->unitise();
  wv->randomize();
  lrmodel->cal_gradient();
  dense::e_norm(gv_norm, *gv);
  if (gv_norm < epsilon) {
    L4C_WARN("BFGS will not begin iteration, please check the parameters!");
    flag = false;
    goto end;
  }

end:
  return flag;
}

bool BFGS::iter() {
  bool flag = true;

  for (INT i = 0; i < lrpara->max_iter_num; i++) {
    ///TODO:
    dense::copy(*B_copy, *B);
    dense::copy(*pv, *gv);
    dense::neg(*pv);
    dense::sv(*pv, *B_copy);
    ls->step(lambda, step_len, *wv, *pv);
    //L4C_INFO("BFGS iteration turn %d completed!", (i + 1));
  }

end:
  return flag;
}

bool BFGS::solve() {
  bool flag = true;

  L4C_INFO("BFGS inition begins!");
  if (!init()) {
    L4C_FATAL("BFGS inition failed!");
    flag = false;
    goto end;
  }
  L4C_INFO("BFGS inition finished!");

  L4C_INFO("BFGS iteration begins!");
  if (!iter()) {
    L4C_FATAL("BFGS iteration failed!");
    flag = false;
    goto end;
  }
  L4C_INFO("BFGS iteration finished!");

end:
  return flag;
}

/// LBFGS
bool LBFGS::init() {
}

bool LBFGS::iter() {
}
