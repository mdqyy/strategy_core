//==============================================================================
/*!
 *  \file sparse.h
 *
 *  \brief sparse structures for linear algebra
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#ifndef STRATEGY_CORE_LIN_ALG_SPARSE_H_
#define STRATEGY_CORE_LIN_ALG_SPARSE_H_

#include <strategy_core/lin_alg/base.h>
#include <strategy_core/discrete/CrossList.h>

namespace sparse {
// Sparse Vector implementation using Linked List
class RealVectorPoint {
 public:
  UINT index;
  REAL value;
  RealVectorPoint *next;
  RealVectorPoint(const UINT index, const REAL value);
};

class SparseList {
 public:
  RealVectorPoint *head;
  RealVectorPoint *tail;
  INT length;
  SparseList();
  ~SparseList();
};

// Sparse Matrix implementation using Cross List
class RealMatrix:public Matrix {
 public:
  CrossList *cl;

  RealMatrix(const UINT row, const UINT col);
  virtual ~RealMatrix();
  virtual bool append(CrossListNode *clnode);
  virtual void print() const;
};

class ZeroMatrix:RealMatrix {
 public:
  ZeroMatrix(const UINT row, const UINT col):RealMatrix(row, col) {};
};

class RealSquare:public Square,
                 public RealMatrix {
 public:
  RealSquare(const UINT size):Matrix(size, size),
                              Square(size),
                              RealMatrix(size, size) {};
};

// RealVector
class RealVector:public Vector {
 public:
  SparseList *sl;

  RealVector(const UINT row, const UINT col);
  ~RealVector();
  bool append(RealVectorPoint *rvp);
};

}
#endif //STRATEGY_CORE_LIN_ALG_SPARSE_H_
