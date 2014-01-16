//==============================================================================
/*!
 *  \file vector.h
 *
 *  \brief vector computation for linear algebra
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#ifndef STRATEGY_CORE_LIN_ALG_VECTOR_H_
#define STRATEGY_CORE_LIN_ALG_VECTOR_H_

#include <strategy_core/lin_alg/base.h>
#include <strategy_core/lin_alg/dense.h>
#include <strategy_core/lin_alg/sparse.h>

bool inner_product_sd(REAL &inner, const SparseRealVector *srv, const DenseRealVector *drv);
bool inner_product_ss(REAL &inner, const SparseRealVector *srva, const SparseRealVector *srvb);
bool inner_product_dd(REAL &inner, const DenseRealVector *drva, const DenseRealVector *drvb);
bool euclid_norm_d(REAL &norm, const DenseRealVector *drv);
bool copy_d(DenseRealVector *drv_dest, const DenseRealVector *drv_src);
bool copy_s(SparseRealVector *srv_dest, const SparseRealVector *srv_src);

#endif //STRATEGY_CORE_LIN_ALG_VECTOR_H_
