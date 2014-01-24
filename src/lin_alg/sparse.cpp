//==============================================================================
/*!
 *  \file sparse.cpp
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

#include <strategy_core/lin_alg/sparse.h>
#include <strategy_core/common/log4c.h>

namespace sparse {
/// SparseRealMatrix
RealMatrix::RealMatrix(const UINT row, const UINT col):Matrix(row, col){
  this->cl = new CrossList(row, col);
}

RealMatrix::~RealMatrix() {
  delete this->cl;
  this->cl = NULL;
}

bool RealMatrix::append(CrossListNode *clnode) {
  bool flag = true;
  if (this->cl->append(clnode)) {
    L4C_ERROR("RealMatrix appending failed!");
    flag = false;
    goto end;
  }

end:
  return flag;
}

void RealMatrix::print() const {
  this->cl->output_all();
}

/// SparseRealVector
RealVectorPoint::RealVectorPoint(const UINT index, const REAL value) {
  this->index = index;
  this->value = value;
}

SparseList::SparseList() {
  this->head = NULL;
  this->tail = NULL;
  this->length = 0;
}

SparseList::~SparseList() {
  RealVectorPoint *curr = this->head;
  RealVectorPoint *next;
  while (NULL != curr) {
    next = curr->next; /// Delete the node row by row.
    delete curr;
    curr = next;
  }
  this->head = NULL;
  this->tail = NULL;
}

RealVector::RealVector(const UINT row, const UINT col):Matrix(row, col),
                                                                   Vector(row, col){
  this->sl = new SparseList();
}

RealVector::~RealVector() {
  delete this->sl;
  this->sl = NULL;
}

bool RealVector::append(RealVectorPoint *rvp){
  bool flag = true;
  if (NULL == rvp) {
    L4C_ERROR("RealVectorPoint pointer is NULL!");
    flag = false;
    goto end;
  }

  this->sl->tail = rvp;
  this->sl->length++;
  Matrix::nz++;

end:
  return flag;
}

///TODO
///Matrix Op
///Vector Op
}
