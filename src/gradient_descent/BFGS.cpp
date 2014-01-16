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
  delete this->lrmodel;
  this->lrmodel = NULL;
  delete this->lrpara;
  this->lrpara= NULL;
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
  if (!BFGS::init()) {
    flag = false;
    goto end;
  }

  for (INT i = 0; i < this->lrpara->max_iter_num; i++)
    matrix::inv(this->B_inv, this->B);
    matrix::mul(this->p, this->B_inv, this->gradient_vector_neg);
    this->ls->step(&this->lambda, this->lrpara->step_len, this->lrmodel->weight_vector, this->p);
    copy_d(this->lrmodel->weight_vector, this->ls->step_vector);
    copy_d(this->last_gradient_vector, this->lrmodel->gradient_vector);
    copy_d(this->last_weight_vector, this->lrmodel->weight_vector);
    this->lrmodel->cal_gradient();
    if (euclid_norm_d(this->lrmodel->gradient_vector) < this->lrpara->epsilon) {
      goto end;
    }
    else {
      matrix::sub(this->gradient_vector_sub, this->lrmodel->gradient_vector, this->last_gradient_vector);
      matrix::sub(this->weight_vector_sub, this->lrmodel->weight_vector, this->last_weight_vector);
      matrix::mul(this->U, this->B, this->delta);
      inner_product_ss(&this->yt_delta, this->y, this->delta);
      matrix::num_mul(this->U, this->U, 1.0/this->yt_delta);  /// U = U / m
      matrix::mul(this->B_delta, this->B, this->delta);
      matrix::mul(this->deltaT_B, this->detaT, this->B);
      matrix::mul(this->V, this->B_delta, this->deltaT_B);
      inner_product_ss(&this->dtBd, this->deltaT_B, this->delta);
      matrix::num_mul(this->V, this->V, 1.0/this->dtBd);  /// V = V / n
      matrix::sub(this->U, this->U, this->V); /// U = U - V
      matrix::add(this->B, this->B, this->U); /// B = B + U
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
