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
#include <strategy_core/common/algorithm.h>
#include <strategy_core/supervised/LogisticReg.h>
#include <strategy_core/lin_alg/base.h>
#include <strategy_core/lin_alg/dense.h>

class LineSearch {
 public:
  LogisticReg::LrModel *lrmodel;

  LineSearch(LogisticReg::LrModel *lrmodel);
  ~LineSearch() {};
  bool step(REAL &lambda, const REAL step, const REAL step_len,
            dense::RealVector &wv, dense::RealVector &pv);
};

#endif //STRATEGY_CORE_GRADIENT_DESCENT_LINESEARCH_H_
