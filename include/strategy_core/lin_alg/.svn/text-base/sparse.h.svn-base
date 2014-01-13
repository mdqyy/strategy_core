//==============================================================================
/*!
 *  \file sparse.h
 *
 *  \brief sparse structures for linear algebra
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

#ifndef STRATEGY_CORE_LIN_ALG_SPARSE_H_
#define STRATEGY_CORE_LIN_ALG_SPARSE_H_

#include <strategy_core/lin_alg/base.h>

namespace sparse {
#ifndef ElemType
#define ElemType REAL

/*! Sparse Vector implementation using Linked List */
class SparseVectorPoint {
 public:
  INT index;
  REAL value;
};

class SparseVector {
};

class CrossListNode {
 public:
  INT i, j; /// position of a non-zero element, a[i,j]
  ElemType e;
  CrossListNode *right, *down;
  CrossListNode ();
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

/*! Sparse Matrix implementation using Cross List */
class SparseMatrix {
 public:
  INT row;
  INT col;
  CrossList *cl;

  SparseMatrix(const INT row, const INT col);
  ~SparseMatrix();
}

class Diag:SparseMatrix {
 public:
  Diag(const INT n);
  ~Diag();
}
#endif
}
#endif //STRATEGY_CORE_LIN_ALG_SPARSE_H_
