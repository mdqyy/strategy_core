//==============================================================================
/*!
 *  \file LineSearch.h
 *
 *  \brief LineSearch.h
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#ifndef STRATEGY_CORE_GRADIENT_DESCENT_LINESEARCH_H_
#define STRATEGY_CORE_GRADIENT_DESCENT_LINESEARCH_H_

#include <strategy_core/common/common.h>

class LineSearch {
  ///TODO:
 public:
  dense::DenseRealVector *step_vector;
  LineSearch();
  ~LineSearch();

  bool step(REAL &lambda, REAL step_len);

 private:
  void *target_fun();
};

#endif //STRATEGY_CORE_GRADIENT_DESCENT_LINESEARCH_H_
