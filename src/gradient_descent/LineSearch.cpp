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
  this->step_vector = new dense::RealMatrix(row, col);
}

LineSearch::~LineSearch() {
  delete this->step_vector; this->step_vector = NULL;
}

bool LineSearch::step(REAL &lambda, REAL step_len, Matrix *w, Matrix *p) {
  bool flag = true;

end:
  return flag;
}
