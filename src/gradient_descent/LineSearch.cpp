//==============================================================================
/*!
 *  \file LineSearch.cpp
 *
 *  \brief LineSearch.cpp
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#include <strategy_core/gradient_descent/LineSearch.h>

LineSearch::LineSearch(LogisticReg::LrModel *lrmodel) {
  this->lrmodel = lrmodel;
}

bool LineSearch::step(REAL &lambda, const REAL step, const REAL step_len,
                      dense::RealVector &wv, dense::RealVector &pv) {
  bool flag = true;
  REAL min_target = Max;
  REAL s = 0, min = lambda;
  while (s <= step_len) {
    dense::axpy(wv, step, pv);
    lrmodel->cal_target();
    //printf("lrmodel->target:%f\n", lrmodel->target);
    if (lrmodel->target < min_target) {
      min = s;
      min_target = lrmodel->target;
    }
    s += step;
  }
  lambda = min;

end:
  return flag;
}
