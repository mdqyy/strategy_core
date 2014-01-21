//==============================================================================
/*!
 *  \file dense.cpp
 *
 *  \brief dense structures for linear algebra
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#include <strategy_core/lin_alg/dense.h>
#include <strategy_core/common/log4c.h>

namespace dense {
/// DenseRealVector
DenseRealVector::DenseRealVector(const INT row , const INT col)
                                :Vector(row, col) {
  this->V = new REAL[Vector::length];
}

DenseRealVector::~DenseRealVector() {
  delete []this->V;
  this->V = NULL;
}

bool DenseRealVector::set(const INT pos, const REAL value) {
  bool flag = true;
  if (pos < 0 or pos >= Vector::length) {
    L4C_ERROR("Array reference out of bound in DenseRealVector::set()!");
    flag = false;
    goto end;
  }

  this->V[pos] = value;

end:
  return flag;
}

bool DenseRealVector::get(REAL &value, const INT pos) {
  bool flag = true;
  if (pos < 0 or pos >= Vector::length) {
    L4C_ERROR("Array reference out of bound in DenseRealVector::get()!");
    flag = false;
    goto end;
  }

  value = this->V[pos];

end:
  return flag;
}

/// DenseRealMatrix
DenseRealMatrix::DenseRealMatrix(const INT row, const INT col)
                                :Matrix(row, col) {
  this->M = new REAL *[row];
  for (UINT i = 0; i < this->row; i++)
    this->M[i] = new REAL[col];
}

DenseRealMatrix::~DenseRealMatrix() {
  for (INT i = 0; i < Matrix::row; i++) {
    delete []this->M[i];
    this->M[i] = NULL;
  }
  this->M = NULL;
}

UINT DenseRealMatrix::get_row() const {
  return Matrix::row;
}

UINT DenseRealMatrix::get_col() const {
  return Matrix::col;
}

/// DenseRealVector operations
bool inner_product_dd(REAL &inner, const DenseRealMatrix *drva, const DenseRealMatrix *drvb) {
  bool flag = true;

end:
  return flag;
}

/// norm = |V|
bool euclid_norm_d(REAL &norm, const DenseRealMatrix *drv) {
  bool flag = true;
  REAL temp = 0.0;
  if (NULL == drv) {
    L4C_ERROR("Fatal error occurs in euclid_norm_d: DenseRealMatrix pointer is NULL!");
    flag = false;
    goto end;
  }

  inner_product_dd(temp, (const DenseRealMatrix *)drv, (const DenseRealMatrix *)drv);
  norm = sqrt(temp);

end:
  return flag;
}

/// DenseRealMatrix operations
bool copy(DenseRealMatrix *M_dest, const DenseRealMatrix *M_src) {
  bool flag = true;
  if (NULL == M_dest or NULL == M_src) {
    L4C_ERROR("Fatal error occurs in copy_d: DenseRealMatrix pointer is NULL!");
    flag = false;
    goto end;
  }
  if (M_dest->row == M_src->row and M_dest->col == M_src->col) {
    L4C_ERROR("Fatal error occurs in copy: DenseRealMatrix parameter error!");
    flag = false;
    goto end;
  }

  memmove((void *)M_dest->M, (void *)M_src->M, M_src->row * M_src->col *sizeof(REAL));

end:
  return flag;
}

bool inv(DenseRealMatrix *B, DenseRealMatrix *A) {/// B = A^(-1)
  /*!
   *  Gauss-Jordan method
   */
  bool flag = true;
	INT *is, *js, i, j, k, l, u, v, n = A->row;
	REAL d,p;
	is = new INT[n * sizeof(INT)];
	js = new INT[n * sizeof(INT)];
	if (NULL == A || NULL == B) {
		L4C_ERROR("Fatal error occurs in inv: DenseRealMatrix pointer is NULL!");
		flag = false;
		goto end;
	}
	if (A->row != A->col || B->row != B->col || A->row != B->row) {
		L4C_ERROR("Fatal error occurs in inv: DenseRealMatrix parameter error!");
    flag = false;
		goto end;
	}

	memmove((void *)B->M, (void *)A->M, n*n*sizeof(REAL));

	for (k = 0; k < n ; k++) {
		d = 0.0;
		for (i = k; i < n; i++)
			for (j = k; j < n; j++) {
				p = fabs(B->M[i][j]);
				if (p > d) {
					d = p;
					is[k] = i;
					js[k] = j;
				}
			}
			if (d + 1.0 == 1.0) { /// det(A) == 0
				L4C_WARN("Warning in inv: det(A) equals zero!");
				flag = false;
				goto end;
			}
			if (is[k] != k) {
				for (j = 0; j < n; j++) {
					p = B->M[k][j];
					B->M[k][j] = B->M[is[k]][j];
					B->M[is[k]][j] = p;
				}
			}
			if (js[k] != k) {
				for (i = 0; i < n; i++) {
					p = B->M[i][k];
					B->M[i][k] = B->M[i][js[k]];
					B->M[i][js[k]] = p;
				}
			}
			B->M[k][k]=(REAL)(1.0/B->M[k][k]);
			for (j = 0; j < n; j++) {
				if (j != k) {
					B->M[k][j] = B->M[k][j]*B->M[k][k];
				}
			}
			for (i = 0; i < n; i++) {
				if (i != k) {
					for (j = 0; j < n; j++) {
						if (j != k) {
							B->M[i][j] = B->M[i][j] - B->M[i][k] * B->M[k][j];
						}
					}
				}
			}
			for (i = 0; i < n; i++) {
				if (i != k) {
					B->M[i][k] = - B->M[i][k] * B->M[k][k];
				}
			}
	}

	for (k = n - 1; k >= 0; k--) {
		if (js[k] != k) {
			for (j = 0; j < n; j++) {
				p = B->M[k][j];
				B->M[k][j] = B->M[js[k]][j];
				B->M[js[k]][j] = p;
			}
		}
		if (is[k] != k) {
			for (i = 0; i < n; i++) {
				p = B->M[i][k];
				B->M[i][k] = B->M[i][is[k]];
				B->M[i][is[k]] = p;
			}
		}
	}

end:
	delete[] is;
	delete[] js;
	is = NULL;
	js = NULL;
	return flag;
}

// C = A + B
bool add(DenseRealMatrix *C, DenseRealMatrix *A, DenseRealMatrix *B) { /// C = A + B
  bool flag = true;
	if (A == NULL || B == NULL || C == NULL) {
		L4C_ERROR("Fatal error occurs in add: DenseRealMatrix pointer is NULL!");
		flag = false;
		goto end;
	}
	if (A->row != B->row || A->col != B->col ||
      B->row != C->row || B->col != C->col) {
		L4C_ERROR("Fatal error occurs in add: DenseRealMatrix parameter error!");
		return -1;
	}

	UINT i, j;
	for(i = 0; i < A->row; i++)
		for(j = 0; j < A->col; j++)
			C->M[i][j] = A->M[i][j] + B->M[i][j];

end:
	return flag;
}

bool sub(DenseRealMatrix *C, DenseRealMatrix *A, DenseRealMatrix *B) { /// C = A - B
  bool flag = true;
	if (NULL == A || NULL == B || NULL == C) {
		L4C_ERROR("Fatal error occurs in sub: DenseRealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->row != B->row || A->col != B->col || B->row != C->row || B->col != C->col) {
		L4C_ERROR("Fatal error occurs in sub: DenseRealMatrix parameter error!");
    flag = false;
		goto end;
	}

	UINT i, j;
	for(i = 0; i < A->row; i++)
		for(j = 0; j < A->col; j++)
			C->M[i][j] = A->M[i][j] - B->M[i][j];

end:
	return flag;
}

bool mul(DenseRealMatrix *C, DenseRealMatrix *A, DenseRealMatrix *B) { /// C = A * B
  bool flag = true;
	if (NULL == A || NULL == B || NULL == C) {
		L4C_ERROR("Fatal error occurs in mul: DenseRealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->col != B->row || A->row != C->row || B->col != C->col) {
		L4C_ERROR("Fatal error occurs in mul: DenseRealMatrix parameter error!");
    flag = false;
		goto end;
	}

	UINT i, j, k;
	REAL sum;
	for(i = 0; i < A->row; i++)
		for(j = 0; j < B->col; j++) {
			sum = 0;
			for(k = 0; k < A->col; k++)
				sum += A->M[i][k] * B->M[k][j];
			C->M[i][j] = sum;
		}

end:
	return flag;
}

bool tranv(DenseRealMatrix *B, DenseRealMatrix *A) { /// B = A^T
  bool flag = true;
	if (NULL == A || NULL == B) {
		L4C_ERROR("Fatal error occurs in tranv: DenseRealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->row != B->col|| A->col != B->row) {
		L4C_ERROR("Fatal error occurs in tranv: DenseRealMatrix parameter error!");
    flag = false;
		goto end;
	}

	UINT i, j;
	for (i = 0; i < A->row; i++)
		for(j = 0; j < A->col; j++)
			B->M[j][i] = A->M[i][j];

end:
	return flag;
}

bool t_mul(DenseRealMatrix *B, DenseRealMatrix *A) {  /// B = A*A^T
  ///TODO:;
  bool flag = true;

end:
  return flag;
}

bool f_norm(REAL &fnorm, DenseRealMatrix *A) { /// Frobenius norm of DenseRealMatrix
  bool flag = true;
  REAL sum = 0.0;
  REAL tmp = 0.0;
  UINT i, j;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in f_norm: DenseRealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}

	for (i = 0; i < A->row; i++)
		for(j = 0; j < A->col; j++) {
			tmp = A->M[i][j];
			sum += tmp * tmp;
		}
  fnorm = sum;

end:
	return flag;
}

bool m_norm(REAL &mnorm, DenseRealMatrix *A) { /// Manhattan norm of DenseRealMatrix
  bool flag = true;
  REAL sum = 0.0;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in m_norm: DenseRealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}

	UINT i, j;
	REAL tmp;
	for (i = 0; i < A->row; i++)
		for(j = 0; j < A->col; j++) {
			tmp = A->M[i][j];
			sum += tmp;
		}
  mnorm = sum;

end:
	return  flag;
}

bool get_min(REAL &min, DenseRealMatrix *A) {
  bool flag = true;
  REAL _min = Max;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in get_min: DenseRealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}

	UINT i, j;
	REAL tmp;
	for (i = 0; i < A->row; i++)
		for(j = 0; j < A->col; j++) {
			tmp = A->M[i][j];
			if (tmp < _min)
				_min = tmp;
		}
  min = _min;

end:
  return  flag;
}

bool get_max(REAL &max, DenseRealMatrix *A) {
  bool flag = true;
  REAL _max = Min;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in get_max: DenseRealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}

	UINT i, j;
	REAL tmp;
	for (i = 0; i < A->row; i++)
		for(j = 0; j < A->col; j++) {
			tmp = A->M[i][j];
			if (tmp > _max)
				_max = tmp;
		}
  max = _max;

end:
	return  flag;
}

bool num_mul(DenseRealMatrix *B, DenseRealMatrix *A, REAL real) { /// B = k * A
  bool flag = true;
	if (NULL == A || NULL == B) {
		L4C_ERROR("Fatal error occurs in num_mul: DenseRealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->row != B->row || A->col != B->col) {
		L4C_ERROR("Fatal error occurs in num_mul: DenseRealMatrix parameter error!");
    flag = false;
		goto end;
	}

	UINT i, j;
	for (i = 0; i < A->row; i++)
		for(j = 0; j < A->col; j++)
			B->M[i][j] = real * A->M[i][j];

end:
	return flag;
}

bool num_add(DenseRealMatrix *B, DenseRealMatrix *A, REAL real) { /// B = [k] + A
  bool flag = true;
	if (NULL == A || NULL == B) {
		L4C_ERROR("Fatal error occurs in num_add: DenseRealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->row != B->row || A->col != B->col) {
		L4C_ERROR("Fatal error occurs in num_add: DenseRealMatrix parameter error!");
    flag = false;
		goto end;
	}

  if (0.0 == real) {
    copy(B, A);
    goto end;
  }
	UINT i, j;
	for (i = 0; i < A->row; i++)
		for(j = 0; j < A->col; j++)
			B->M[i][j] = real + A->M[i][j];

end:
	return flag;
}

bool hadamard_mul(DenseRealMatrix *C, DenseRealMatrix *A, DenseRealMatrix *B) {///  hadamard mul
  bool flag = true;
	if (NULL == A || NULL == B || NULL == C) {
		L4C_ERROR("Fatal error occurs in hadamard_mul: DenseRealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->col != B->row || A->row != C->row || B->col != C->col) {
		L4C_ERROR("Fatal error occurs in hadamard_mul: DenseRealMatrix parameter error!");
    flag = false;
		goto end;
	}

	UINT i, j;
	for(i = 0; i < A->row; i++)
		for(j = 0; j < B->col; j++)
			C->M[i][j] = A->M[i][j] * B->M[i][j];

end:
	return flag;
}

bool kronecker_mul(DenseRealMatrix *C, DenseRealMatrix *A, DenseRealMatrix *B) {///  kronecker mul
  bool flag = true;
	if (NULL == A || NULL == B || NULL == C) {
		L4C_ERROR("Fatal error occurs in kronecker_mul: DenseRealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (C->row != A->row*B->row) {
		L4C_ERROR("Fatal error occurs in kronecker_mul: DenseRealMatrix C must be a DenseRealMatrix of d(C) = d(A)*d(B)!");
    flag = false;
		goto end;
	}

	UINT i, j, k, l, m, n;
	for (i = 0; i < A->row; i++)
		for (j = 0; j < A->col; j++)
			for (k = 0; k < B->row; k++)
				for (l = 0; l < B->col; l++) {
					m = B->row * i + k;
					n = B->col * j + l;
					C->M[m][n] = A->M[i][j] * B->M[k][l];
				}

end:
	return flag;
}

}
