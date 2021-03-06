//==============================================================================
/*!
 *  \file algorithm.h
 *
 *  \brief algorithm
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================
#ifndef STRATEGY_CORE_COMMON_ALGORITHM_H_
#define STRATEGY_CORE_COMMON_ALGORITHM_H_

#include "common.h"

namespace algorithm {
class Model {
 public:
  virtual bool write() = 0;
  virtual bool read() = 0;
};
}
#endif //STRATEGY_CORE_COMMON_ALGORITHM_H_
