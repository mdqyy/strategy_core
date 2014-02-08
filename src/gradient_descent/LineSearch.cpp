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

LineSearch::LineSearch(const UINT row, const UINT col) {
  step_v = new dense::RealVector(row, col);
}

LineSearch::~LineSearch() {
  delete step_v; step_v = NULL;
}

bool LineSearch::step(REAL &lambda, const REAL step_len, dense::RealVector &wv, dense::RealVector &pv) {
  bool flag = true;

end:
  return flag;
}
