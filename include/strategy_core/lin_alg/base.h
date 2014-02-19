//==============================================================================
/*!
 *  \file base.h
 *
 *  \brief base definition for linear algebra
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================
#ifndef STRATEGY_CORE_LIN_ALG_BASE_H_
#define STRATEGY_CORE_LIN_ALG_BASE_H_

#include <cmath>
#include <strategy_core/common/common.h>

#define REAL float
#define INT size_t
#define UINT unsigned int
#define Min -9999999999
#define Max 9999999999
#define VERY_SMALL_NUM 1e-100
#define PI 3.1415926

class Matrix {
 public:
  UINT row;
  UINT col;
  UINT nz;
  REAL sparsity;
  Matrix(const UINT row, const UINT col):row(row),
                                         col(col),
                                         nz(0),
                                         sparsity(0.0) {};
  inline void cal_sparsity();
};

class Vector:virtual public Matrix {
 public:
  UINT size;
  bool by_row;
  Vector(const UINT row, const UINT col);
};

class Square:virtual public Matrix {
 public:
  UINT size;
  Square(const UINT size):Matrix(size, size),
                          size(size) {};
};
#endif //STRATEGY_CORE_LIN_ALG_BASE_H_
