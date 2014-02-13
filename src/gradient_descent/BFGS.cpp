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
  step = lrpara->step;
  epsilon = lrpara->epsilon;
  this->wv = lrmodel->wv;
  this->gv = lrmodel->gv;
  B = new dense::RealSquare(d);
  B_copy = new dense::RealSquare(d);
  pv = new dense::RealVector(d, 1);
  ls = new LineSearch(lrmodel);
  last_wv = new dense::RealVector(d, 1);
  wv_step = new dense::RealVector(d, 1);
  last_gv = new dense::RealVector(d, 1);
  yv = new dense::RealVector(d, 1);
  dv = new dense::RealVector(d, 1);
  U = new dense::RealSquare(d);
  Bd  = new dense::RealVector(d, 1);
  dB  = new dense::RealVector(1, d);
}

BFGS::~BFGS() {
  delete B; B = NULL;
  delete B_copy; B_copy = NULL;
  delete pv; pv = NULL;
  delete ls; ls = NULL;
  delete last_wv; last_wv = NULL;
  delete wv_step; wv_step = NULL;
  delete last_gv; last_gv = NULL;
  delete yv; yv = NULL;
  delete dv; dv = NULL;
  delete U; U = NULL;
  delete Bd; Bd = NULL;
  delete dB; dB = NULL;
}

bool BFGS::init() {
  bool flag = true;
  B->unitise();
  wv->unitise();
  lrmodel->cal_gradient();
  wv->print();
  gv->print();
  dense::nrm2(gv_norm, *gv);
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
  //for (INT i = 0; i < 1; i++) {
    dense::copy(*B_copy, *B);
    dense::copy(*pv, *gv);
    dense::neg(*pv);                               // pv = -gv
    dense::sv(*pv, *B_copy);                       // pv = {x|A*x = pv}
    dense::copy(*last_wv, *wv);                    // last_wv = wv
    dense::copy(*wv_step, *wv);                    // wv_step = wv
    dense::copy(*last_gv, *gv);
    ls->step(lambda, step, step_len, *wv, *pv);    // find lamba when min { f(wv + lambda * pv) }
    //printf("lambda:%f\n", lambda);
    //wv->print();
    dense::axpy(*wv_step, lambda, *pv);            // wv_step = lambda * pv + wv_step
    dense::copy(*wv, *wv_step);                    // wv = wv_step;
    //gv->print();
    lrmodel->cal_gradient();                       // gv = g(wv)
    dense::nrm2(gv_norm, *gv);                     // gv_norm = ||gv||
    //wv->print();
    //gv->print();
    printf("gv_norm:%f\n", gv_norm);
    if (gv_norm < epsilon) {
      L4C_WARN("Now euclide norm of gradient vector is %f < epsilon: %f, \
               iter ends!", gv_norm, epsilon);
      goto end;
    }
    dense::copy(*yv, *gv);
    dense::copy(*dv, *wv);
    dense::axpy(*yv, -1., *last_gv);               // yv = - last_gv + gv
    dense::axpy(*dv, -1., *last_wv);               // dv = - last_wv + wv
    dense::vvt(*U, *yv, *yv);                      // U = yv * yvt
    //gv->print();
    //U->print();
    dense::dot(k1, *yv, *dv);                      // k1 = yv * dv
    dense::add(*B, 1., 1./k1, *U);                 // B = B + U/k1
    dense::mv(*Bd, *B, *dv);                       // Bd = B * dv
    dense::vtm(*dB, *dv, *B);                      // dB = dvt * B
    dense::mm(*U, *Bd, *dB);                       // U = Bd * dB
    //U->print();
    dense::dot(k2, *dB, *dv);                      // k2 = dB * wv
    dense::add(*B, 1., -1./k2, *U);                // B = B - U/k2
    printf("BFGS iteration turn %d completed!\n", (i + 1));
    ///L4C_INFO("BFGS iteration turn %d completed!", (i + 1));
  }

end:
  return flag;
}

bool BFGS::solve() {
  bool flag = true;

  L4C_INFO("BFGS inition begins!");
  if (!this->init()) {
    L4C_FATAL("BFGS inition failed!");
    flag = false;
    goto end;
  }
  L4C_INFO("BFGS inition finished!");

  L4C_INFO("BFGS iteration begins!");
  if (!this->iter()) {
    L4C_FATAL("BFGS iteration failed!");
    flag = false;
    goto end;
  }
  L4C_INFO("BFGS iteration finished!");

end:
  return flag;
}

// LBFGS
bool LBFGS::init() {
}

bool LBFGS::iter() {
}
