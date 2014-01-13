//==============================================================================
/*!
 *  \file algorithm.h
 *
 *  \brief algorithm
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
#ifndef STRATEGY_CORE_COMMON_ALGORITHM_H_
#define STRATEGY_CORE_COMMON_ALGORITHM_H_

#include "common.h"

namespace algorithm {
class Model {
 public:
  ///TODO:
  Model();
  ~Model();
  virtual bool write() = 0;
  virtual bool read() = 0;
};

class Para {
 public:
  ///TODO:
  Para();
  ~Para();
};
}
#endif //STRATEGY_CORE_COMMON_ALGORITHM_H_