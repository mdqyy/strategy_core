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

#define REAL double
#define INT size_t
#define UINT unsigned int

class Matrix {
 public:
  UINT row;
  UINT col;
  UINT nz;
  REAL sparsity;
  bool is_vector;
  bool is_sparse;
  bool is_square;
  bool is_diag;

  Matrix(const UINT row, const UINT col);
  ~Matrix();
};

class Vector:Matrix {
 public:
  UINT length;
  bool by_row;
  Vector(const UINT row, const UINT col);
  ~Vector();
  virtual bool set(UINT pos, REAL value) = 0;
  virtual REAL get(UINT pos) = 0;
};
#endif //STRATEGY_CORE_LIN_ALG_BASE_H_
