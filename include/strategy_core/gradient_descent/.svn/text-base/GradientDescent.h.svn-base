//==============================================================================
/*!
 *  \file gradient_descent.h
 *
 *  \brief gradient_descent.h
 *
 *  \author chenqinyu
 *          contact:
 *              qinyu.chen@corp.elong.com
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par http://www.elong.com<BR>
 */
//==============================================================================

#ifndef STRATEGY_CORE_GRADIENT_DESCENT_GRADIENT_DESCENT_H_
#define STRATEGY_CORE_GRADIENT_DESCENT_GRADIENT_DESCENT_H_

#include <strategy_core/lin_alg/base.h>
#include <strategy_core/lin_alg/sparse.h>
#define VectorType sparse::SparseVector

namespace GradientDescent {
class GradientDescent {
public:
  VectorType *x; /// X
  VectorType *g; /// Gradient
  REAL y; /// Y
  REAL step_len;
  INT curr_iter_num;
  INT max_iter_num;
  REAL stop_cond;

  bool init();
  void step(REAL (*target_fun)(), VectorType (*gradient_fun)());
  GradientDescent();
  ~GradientDescent();
protected:
private:
};

}
#endif //STRATEGY_CORE_GRADIENT_DESCENT_GRADIENT_DESCENT_H_
