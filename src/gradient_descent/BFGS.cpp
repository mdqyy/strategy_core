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
}

BFGS::~BFGS() {
  ///TODO:
  /*
  this->lrmodel->weight_vector = ;
  this->B = new Diag() ;
  this->B_inv = ;
  this->p = ;
  this->wk = ;
  this->ls = new LineSearch();
  this->lrmodel->cal_gradient();
  if (euclid_norm_d(this->lrmodel->gradient_vector) < this->lrpara->epsilon) {
    break;
  }
  */
}

bool BFGS::init() {
  bool flag = true;
  ///TODO:
  /*
  this->lrmodel->weight_vector = ;
  this->B = new Diag() ;
  this->B_inv = ;
  this->p = ;
  this->wk = ;
  this->ls = new LineSearch();
  this->lrmodel->cal_gradient();
  if (euclid_norm_d(this->lrmodel->gradient_vector) < this->lrpara->epsilon) {
    break;
  }
  */
end:
  return flag;
}

bool BFGS::solve() {
  bool flag = true;
  if (!this->init()) {
    flag = false;
    L4C_ERROR("BFGS init failed!");
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
      dense::sub(this->gradient_vector_sub, this->lrmodel->gradient_vector, this->last_gradient_vector);
      dense::sub(this->weight_vector_sub, this->lrmodel->weight_vector, this->last_weight_vector);
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
  }

end:
  return flag;
}

/// LBFGS
bool LBFGS::init() {
}

bool LBFGS::solve() {
}
