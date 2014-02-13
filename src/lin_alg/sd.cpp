//==============================================================================
/*!
 *  \file sd.h
 *
 *  \brief operation for both sparse and dense structures implementation
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#include <strategy_core/lin_alg/sd.h>

namespace sd {
/// Vector Opertations.
bool dot(REAL &inner, SingleList &sl, dense::RealVector &drv) {
  bool flag = true;
  REAL _inner = 0;
  CrossListNode *curr = sl.head->right;
  if (NULL == &sl or NULL == &drv) {
    L4C_ERROR("Fatal error occurs in inner_product_sd: Vector pointer is NULL!");
    flag = false;
    goto end;
  }

  while (curr) {
    _inner += curr->e * drv.get(curr->j);
    curr = curr->right;
  }
  inner = _inner;

end:
  return flag;
}

}
