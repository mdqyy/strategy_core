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

namespace sparse {
/// Cross List
class CrossListNode {
 public:
  INT i, j; /// position of a non-zero element, a[i,j]
  REAL e;
  CrossListNode *right, *down;
  CrossListNode (const UINT i, const UINT j, const REAL e);
};

class SingleList {
 public:
  CrossListNode *head;
  CrossListNode *tail;
  INT length;
  SingleList () {
    head = NULL;
    tail = NULL;
    length = 0;
  }
  ~SingleList();
};

class CrossList {
 public:
  SingleList **rslArray, **lslArray;
  INT row, col, nz; /// num of rows, columns and non-zero elements.
  CrossList(const INT row, const INT col);
  ~CrossList();

  bool append(CrossListNode *clnode);
  SingleList *get_row(const INT row_no);
  SingleList *get_col(const INT col_no);
  bool output_row(const INT row_no);
  bool output_col(const INT col_no);
  void output_all(void);
};

/*! Sparse Vector implementation using Linked List */
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

class SparseRealVector:Vector {
 public:
  SingleList *sl;
  SparseRealVector(const UINT row, const UINT col);
  ~SparseRealVector();

  bool append(const RealVectorPoint *rvp);
};

/*! Sparse Matrix implementation using Cross List */
class SparseRealMatrix:Matrix {
 public:
  CrossList *cl;

  SparseRealMatrix(const INT row, const INT col);
  ~SparseRealMatrix();
};

class DiagMatrix:SparseRealMatrix {
 public:
  DiagMatrix(const INT n);
  ~DiagMatrix();
};
#endif
}
#endif //STRATEGY_CORE_LIN_ALG_SPARSE_H_
