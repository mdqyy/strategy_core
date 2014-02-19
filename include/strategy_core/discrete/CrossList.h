//==============================================================================
/*!
 *  \file CrossList.h
 *
 *  \brief CrossList interface
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#ifndef STRATEGY_CORE_DISCRETE_CROSS_LIST_H_
#define STRATEGY_CORE_DISCRETE_CROSS_LIST_H_

#include <strategy_core/lin_alg/base.h>

/// Cross List
class CrossListNode {
 public:
  UINT i, j;          // position of a non-zero element, a[i,j]
  REAL e;
  CrossListNode *right, *down;
  CrossListNode() {right = NULL; down = NULL;};
  CrossListNode(const UINT i, const UINT j, const REAL e);
};

class SingleList {
 public:
  CrossListNode *head, *tail;
  UINT length;
  SingleList();
  ~SingleList();
};

class CrossList {
 public:
  SingleList **rslArray, **lslArray;
  UINT row, col, nz; // num of rows, columns and non-zero elements.
  CrossList(const UINT row, const UINT col);
  ~CrossList();

  bool append(CrossListNode *clnode);
  SingleList *get_row(const UINT row_no);
  SingleList *get_col(const UINT col_no);
  bool output_row(const UINT row_no);
  bool output_col(const UINT col_no);
  void output_all();
};

#endif //STRATEGY_CORE_DISCRETE_CROSS_LIST_H_
