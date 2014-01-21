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
  this->lrmodel = lrmodel;
  this->lrpara = lrpara;
  const UINT d = this->lrmodel->ss->feature_num;

  this->B = new dense::DenseRealMatrix(d, d);
  this->B_inv = new dense::DenseRealMatrix(d, d);
  this->p = new dense::DenseRealMatrix(d, 1);
  this->gradient_vector_neg = new dense::DenseRealMatrix(d, 1);
  this->last_gradient_vector = new dense::DenseRealMatrix(d, 1);
  this->y = new dense::DenseRealMatrix(d, 1);
  this->last_weight_vector = new dense::DenseRealMatrix(d, 1);
  this->delta = new dense::DenseRealMatrix(d, 1);
  this->detaT = new dense::DenseRealMatrix(1, d);
  this->U = new dense::DenseRealMatrix(d, 1);
  this->B_delta = new dense::DenseRealMatrix(d, 1);
  this->deltaT_B = new dense::DenseRealMatrix(1, d);
  this->V = new dense::DenseRealMatrix(d, d);
  this->ls = new LineSearch(d, 1);
}

BFGS::~BFGS() {
  delete this->B;
  delete this->B_inv;
  delete this->p;
  delete this->gradient_vector_neg;
  delete this->last_gradient_vector;
  delete this->y;
  delete this->last_weight_vector;
  delete this->delta;
  delete this->detaT;
  delete this->U;
  delete this->B_delta;
  delete this->deltaT_B;
  delete this->V;
  delete ls;
}

bool BFGS::init() {
  bool flag = true;

end:
  return flag;
}

bool BFGS::iter() {
  bool flag = true;
  if (dense::euclid_norm_d(this->gradient_vector_norm, this->lrmodel->gradient_vector) < this->lrpara->epsilon) {
    goto end;
  }

  for (INT i = 0; i < this->lrpara->max_iter_num; i++) {
    dense::inv(this->B_inv, this->B);
    dense::mul(this->p, this->B_inv, this->gradient_vector_neg);
    this->ls->step(this->lambda, this->lrpara->step_len, this->lrmodel->weight_vector, this->p);
    dense::copy(this->lrmodel->weight_vector, (const dense::DenseRealMatrix *)this->ls->step_vector);
    dense::copy(this->last_gradient_vector, this->lrmodel->gradient_vector);
    dense::copy(this->last_weight_vector, this->lrmodel->weight_vector);
    this->lrmodel->cal_gradient();
    if (dense::euclid_norm_d(this->gradient_vector_norm, this->lrmodel->gradient_vector) < this->lrpara->epsilon) {
      goto end;
    }
    else {
      dense::sub(this->y, this->lrmodel->gradient_vector, this->last_gradient_vector);
      dense::sub(this->delta, this->lrmodel->weight_vector, this->last_weight_vector);
      dense::mul(this->U, this->B, this->delta);
      dense::inner_product_dd(this->yt_delta, this->y, this->delta);
      dense::num_mul(this->U, this->U, 1.0/this->yt_delta);  /// U = U / m
      dense::mul(this->B_delta, this->B, this->delta);
      dense::mul(this->deltaT_B, this->detaT, this->B);
      dense::mul(this->V, this->B_delta, this->deltaT_B);
      dense::inner_product_dd(this->dtBd, this->deltaT_B, this->delta);
      dense::num_mul(this->V, this->V, 1.0/this->dtBd);  /// V = V / n
      dense::sub(this->U, this->U, this->V); /// U = U - V
      dense::add(this->B, this->B, this->U); /// B = B + U
    }
    L4C_INFO("BFGS iteration turn %d completed!", (i + 1));
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

/// LBFGS
bool LBFGS::init() {
}

bool LBFGS::iter() {
}
