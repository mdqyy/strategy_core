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
bool inner_product(REAL &inner, const sparse::RealVector *srv, const dense::RealVector *drv) {
  bool flag = true;
  /*
  if (NULL == srv or NULL == drv) {
    L4C_ERROR("Fatal error occurs in inner_product_sd: Vector pointer is NULL!");
    flag = false;
    goto end;
  }
  if (srv->length != drv->length) {
    L4C_ERROR("Fatal error occurs in inner_product_sd: Vector parameter error!");
    flag = false;
    goto end;
  }

  REAL _inner = 0;
  RealVectorPoint curr = srv->head;
  while (NULL != curr) {
    _inner += curr->value * drv.get(curr->pos);
    curr = srv->next;
  }
  inner = _inner;
  */
end:
  return flag;
}

}
