//==============================================================================
/*!
 *  \file dense.cpp
 *
 *  \brief dense structures for linear algebra
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

#include <strategy_core/lin_alg/dense.h>
#include <strategy_core/common/log4c.h>

namespace dense {
DenseREALVector::DenseREALVector (const INT length) {
  this->V = new REAL[length];
}

DenseREALVector::~DenseREALVector() {
  delete []this->V;
  this->V = NULL;
}

bool DenseREALVector::set(const INT pos, const REAL value) {
  bool flag = true;
  if (pos < 0 or pos >= this->length) {
    L4C_ERROR("Array reference out of bound in DenseREALVector::set()!");
    flag = false;
    goto end;
  }

  this->V[pos] = value;

end:
  return flag;
}

bool DenseREALVector::get(const REAL &value, const INT pos) {
  bool flag = true;
  if (pos < 0 or pos >= this->length) {
    L4C_ERROR("Array reference out of bound in DenseREALVector::get()!");
    flag = false;
    goto end;
  }

  value = this->V[pos];

end:
  return flag;
}

DenseRealMatrix::DenseRealMatrix(const INT row, const INT col) {
  this->M = new REAL[row];
  for (INT i = 0; i < col; i++)
    this->M[i] = new REAL[col];
}

DenseRealMatrix::~DenseRealMatrix() {
  for (INT i = 0; i < this->row; i++) {
    delete []this->M[i];
  this->M = NULL;
}

}
