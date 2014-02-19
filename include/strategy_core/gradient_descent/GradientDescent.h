//==============================================================================
/*!
 *  \file gradient_descent.h
 *
 *  \brief gradient_descent.h
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#ifndef STRATEGY_CORE_GRADIENT_DESCENT_GRADIENT_DESCENT_H_
#define STRATEGY_CORE_GRADIENT_DESCENT_GRADIENT_DESCENT_H_

#include <strategy_core/lin_alg/base.h>
#include <strategy_core/lin_alg/sparse.h>

namespace GradientDescent {
class GradientDescent {
public:
  Vector *x;        // X
  Vector *g;        // Gradient
  REAL y;           // Y
  REAL step_len;
  INT curr_iter_num;
  INT max_iter_num;
  REAL stop_cond;

  GradientDescent();
  ~GradientDescent();
  bool init();
  void step(REAL (*target_fun)(), Vector (*gradient_fun)());

protected:
private:
};

}
#endif //STRATEGY_CORE_GRADIENT_DESCENT_GRADIENT_DESCENT_H_
