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

#include <cstddef>
#include <strategy_core/common/common.h>

#define REAL double
#define INT size_t
#define UINT unsigned int
#define FAIL -999999
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
  REAL size;
  Matrix(const UINT row, const UINT col);
  inline void cal_sparsity();
};

class Vector:public Matrix {
 public:
  UINT length;
  bool by_row;
  Vector(const UINT row, const UINT col);
  virtual bool set(const UINT pos, const REAL value) = 0;
  virtual bool get(REAL &value, const UINT pos) = 0;
};

class SquareMatrix:Matrix {
 public:
  SquareMatrix(const UINT n):Matrix(n, n) {};
};

#endif //STRATEGY_CORE_LIN_ALG_BASE_H_
