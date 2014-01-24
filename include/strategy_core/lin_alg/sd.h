//==============================================================================
/*!
 *  \file sd.h
 *
 *  \brief operation for both sparse and dense structures interface
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#ifndef STRATEGY_CORE_LIN_ALG_SD_H_
#define STRATEGY_CORE_LIN_ALG_SD_H_

#include <strategy_core/lin_alg/dense.h>
#include <strategy_core/lin_alg/sparse.h>

namespace sd {
/// Matrix Opertations.

/// Vector Opertations.
bool inner_product(REAL &inner, const sparse::RealVector *srv, const dense::RealVector *drv);
}

#endif //STRATEGY_CORE_LIN_ALG_SD_H_
