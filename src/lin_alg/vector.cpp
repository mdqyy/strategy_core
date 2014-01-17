//==============================================================================
/*!
 *  \file vector.cpp
 *
 *  \brief vector computation for linear algebra
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#include <math.h>
#include <strategy_core/lin_alg/vector.h>

bool inner_product_sd(REAL &inner, const SparseRealVector *srv, const DenseRealVector *drv) {
  bool flag = true;
  if (NULL == srv or NULL = drv) {
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
  SparseVectorPoint curr = srv->head;
  while (NULL != curr) {
    _inner += curr->value * drv.get(curr->pos)
    curr = srv->next;
  }
  inner = _inner;

end:
  return flag;
}

bool inner_product_ss(REAL &inner, const SparseRealVector *srva, const SparseVector *srvb) {
  ;
}

bool inner_product_dd(REAL &inner, const DenseRealVector *drva, const DenseRealVector *drvb) {
  bool flag = true;
  if (NULL == drva or NULL = drvb) {
    L4C_ERROR("Fatal error occurs in inner_product_dd: Vector pointer is NULL!");
    flag = false;
    goto end;
  }
  if (drva->length != drvb->length) {
    L4C_ERROR("Fatal error occurs in inner_product_dd: Vector parameter error!");
    flag = false;
    goto end;
  }

  REAL _inner = 0;
  for (INT i = 0; i < drva->length; i++)
    _inner += drva.get(i) * drvb.get(i);
  inner = _inner;

end:
  return flag;
}


bool inner_product(REAL &inner, const Vector *va, const Vector *vb) {
  bool flag = true;
  if (NULL == va or NULL = vb) {
    L4C_ERROR("Fatal error occurs in inner_product_dd: Vector pointer is NULL!");
    flag = false;
    goto end;
  }
  if ((va->length != vb->length) and (va->by_row) and (not vb->by_row)) {
    L4C_ERROR("Fatal error occurs in inner_product_dd: Vector parameter error!");
    flag = false;
    goto end;
  }

  REAL _inner = 0;

end:
  return flag;
}

bool euclid_norm_d(REAL &norm, const DenseRealVector *drv) {
  bool flag = true;
  if (NULL == drv) {
    L4C_ERROR("Fatal error occurs in euclid_norm_d: Vector pointer is NULL!");
    flag = false;
    goto end;
  }

  REAL _norm = 0;
  inner_product_dd(&_norm, drv, drv);
  norm = sqrt(_norm);

end:
  return flag;
}

bool copy_d(DenseRealVector *drv_dest, const DenseRealVector *drv_src) {
  bool flag = true;
  if (NULL == drv_dest or NULL == drv_src) {
    L4C_ERROR("Fatal error occurs in copy_d: Vector pointer is NULL!");
    flag = false;
    goto end;
  }
  if (drv_dest->length != drv_src->length) {
    L4C_ERROR("Fatal error occurs in copy_d: Vector parameter error!");
    flag = false;
    goto end;
  }

  memmove(drv_dest->V, drv_src->V, drv_src->length);

end:
  return flag;
}
