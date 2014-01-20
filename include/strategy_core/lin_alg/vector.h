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

bool inner_product_sd(REAL &inner, const sparse::SparseRealVector *srv, const dense::DenseRealVector *drv);
bool inner_product_ss(REAL &inner, const sparse::SparseRealVector *srva, const sparse::SparseRealVector *srvb);
bool inner_product_dd(REAL &inner, const dense::DenseRealVector *drva, const dense::DenseRealVector *drvb);
bool inner_product(REAL &inner, const Vector *va, const Vector *vb);
bool euclid_norm_d(REAL &norm, const Vector *drv);

#endif //STRATEGY_CORE_LIN_ALG_VECTOR_H_
