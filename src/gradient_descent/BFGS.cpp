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
  this->gradient_vector_norm = 0;
  this->yt_delta = 0;
  this->dtBd = 0;
  this->lambda = 0;
  this->B = new dense::DenseRealMatrix(d, d);
  this->B_inv = new dense::DenseRealMatrix(d, d);
  this->p = new dense::DenseRealMatrix(d, 1);
  this->gradient_vector_neg = new dense::DenseRealMatrix(d, 1);
  this->last_gradient_vector = new dense::DenseRealMatrix(d, 1);
  this->y = new dense::DenseRealMatrix(d, 1);
  this->last_weight_vector = new dense::DenseRealMatrix(d, 1);
  this->delta = new dense::DenseRealMatrix(d, 1);
  this->deltaT = new dense::DenseRealMatrix(1, d);
  this->U = new dense::DenseRealMatrix(d, 1);
  this->B_delta = new dense::DenseRealMatrix(d, 1);
  this->deltaT_B = new dense::DenseRealMatrix(1, d);
  this->V = new dense::DenseRealMatrix(d, d);
  this->ls = new LineSearch(d, 1);
}

BFGS::~BFGS() {
  delete this->B; this->B = NULL;
  delete this->B_inv; this->B_inv = NULL;
  delete this->p; this->p = NULL;
  delete this->gradient_vector_neg; this->gradient_vector_neg = NULL;
  delete this->last_gradient_vector;  this->last_gradient_vector = NULL;
  delete this->y; this->y = NULL;
  delete this->last_weight_vector;  this->last_weight_vector = NULL;
  delete this->delta; this->delta = NULL;
  delete this->deltaT; this->deltaT = NULL;
  delete this->U; this->U = NULL;
  delete this->B_delta; this->B_delta = NULL;
  delete this->deltaT_B;  this->deltaT_B = NULL;
  delete this->V; this->V = NULL;
  delete this->ls;  this->ls = NULL;
}

bool BFGS::init() {
  bool flag = true;

  if (dense::euclid_norm_d(this->gradient_vector_norm, this->lrmodel->gradient_vector) < this->lrpara->epsilon) {
    L4C_WARN("Won't begin iteration, please check the parameters!");
    flag = false;
    goto end;
  }

end:
  return flag;
}

bool BFGS::iter() {
  bool flag = true;

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
      dense::mul(this->deltaT_B, this->deltaT, this->B);
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
