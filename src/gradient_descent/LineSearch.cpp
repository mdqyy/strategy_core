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

LineSearch::LineSearch() {
  ;
}

LineSearch::~LineSearch() {
  ;
}

bool LineSearch::step(REAL &lambda, REAL step_len, Matrix *w, Matrix *p) {
  bool flag = true;

end:
  return flag;
}
