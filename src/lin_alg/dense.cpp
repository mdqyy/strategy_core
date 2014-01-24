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
/// RealMatrix
RealMatrix::RealMatrix(const UINT row, const UINT col):Matrix(row, col) {
	this->M = new REAL *[row];
	for (UINT i = 0; i < row; i++) {
		this->M[i] = new REAL[col];
		memset(this->M[i], 0.0, col * sizeof(REAL));
	}
}

RealMatrix::~RealMatrix() {
	for (UINT i = 0; i < this->row; i++) {
		delete []this->M[i]; this->M[i] = NULL;
	}
	delete []this->M; this->M = NULL;
}

void RealMatrix::print() const {
	std::cout << "[\n";
	for (UINT i = 0; i < this->row; i++) {
		for (UINT j = 0; j < this->col - 1; j++)	{
			std::cout << this->M[i][j] << ",";
		}
		std::cout << this->M[i][this->col - 1] << std::endl;
	}
	std::cout << "]\n";
}

/// DenseRealVector
bool RealVector::set(const UINT pos, const REAL value) {
  bool flag = true;
  if (pos < 0 or pos >= Vector::size) {
    L4C_ERROR("Array reference out of bound in RealVector::set()!");
    flag = false;
    goto end;
  }

  if (Vector::by_row) {
    RealMatrix::M[0][pos] = value;
  }
  else {
    RealMatrix::M[pos][0] = value;
  }

end:
  return flag;
}

bool RealVector::get(REAL &value, const UINT pos) {
  bool flag = true;
  if (pos < 0 or pos >= Vector::size) {
    L4C_ERROR("Array reference out of bound in RealVector::get()!");
    flag = false;
    goto end;
  }

  if (Vector::by_row) {
    value = RealMatrix::M[0][pos];
  }
  else {
    value = RealMatrix::M[pos][0];
  }

end:
  return flag;
}

/// DenseRealVector operations
bool inner_product(REAL &inner, const RealVector *rva, const RealVector *rvb) {
  bool flag = true;

end:
  return flag;
}

/// RealMatrix operations
bool copy(RealMatrix *M_dest, const RealMatrix *M_src) {
  bool flag = true;
  if (NULL == M_dest or NULL == M_src) {
    L4C_ERROR("Fatal error occurs in copy: RealMatrix pointer is NULL!");
    flag = false;
    goto end;
  }
  if (not (M_dest->row == M_src->row and M_dest->col == M_src->col)) {
    L4C_ERROR("Fatal error occurs in copy: RealMatrix parameter error!");
    flag = false;
    goto end;
  }

  memmove((void *)M_dest->M, (void *)M_src->M, M_src->row * M_src->col * sizeof(REAL));

end:
  return flag;
}

bool inv(RealMatrix *B, const RealMatrix *A) {/// B = A^(-1)
  /// Gauss-Jordan method
  bool flag = true;
	INT *is, *js, i, j, k, l, u, v, n = A->row;
	REAL d, p;
	is = new INT[n];
	js = new INT[n];
	memset(is, 0, sizeof(is) * sizeof(INT));
	memset(js, 0, sizeof(js) * sizeof(INT));

	if (NULL == A || NULL == B) {
		L4C_ERROR("Fatal error occurs in inv: RealMatrix pointer is NULL!");
		flag = false;
		goto end;
	}
	if (A->row != A->col || B->row != B->col || A->row != B->row) {
		L4C_ERROR("Fatal error occurs in inv: RealMatrix parameter error!");
    flag = false;
		goto end;
	}

	copy(B, A);

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
				L4C_WARN("Warning in inv: Determinant of A equals zero!");
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
bool add(RealMatrix *C, RealMatrix *A, RealMatrix *B) { /// C = A + B
  bool flag = true;
	if (A == NULL || B == NULL || C == NULL) {
		L4C_ERROR("Fatal error occurs in add: RealMatrix pointer is NULL!");
		flag = false;
		goto end;
	}
	if (A->row != B->row || A->col != B->col ||
      B->row != C->row || B->col != C->col) {
		L4C_ERROR("Fatal error occurs in add: RealMatrix parameter error!");
		return -1;
	}

	UINT i, j;
	for(i = 0; i < A->row; i++)
		for(j = 0; j < A->col; j++)
			C->M[i][j] = A->M[i][j] + B->M[i][j];

end:
	return flag;
}

bool sub(RealMatrix *C, RealMatrix *A, RealMatrix *B) { /// C = A - B
  bool flag = true;
	if (NULL == A || NULL == B || NULL == C) {
		L4C_ERROR("Fatal error occurs in sub: RealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->row != B->row || A->col != B->col || B->row != C->row || B->col != C->col) {
		L4C_ERROR("Fatal error occurs in sub: RealMatrix parameter error!");
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

bool mul(RealMatrix *C, const RealMatrix *A, const RealMatrix *B) { /// C = A * B
  bool flag = true;
	if (NULL == A || NULL == B || NULL == C) {
		L4C_ERROR("Fatal error occurs in mul: RealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->col != B->row || A->row != C->row || B->col != C->col) {
		L4C_ERROR("Fatal error occurs in mul: RealMatrix parameter error!");
    flag = false;
		goto end;
	}

	UINT i, j, k;
	REAL sum;
	for(i = 0; i < A->row; i++)
		for(j = 0; j < B->col; j++) {
			sum = 0.0;
			for(k = 0; k < A->col; k++)
				sum += A->M[i][k] * B->M[k][j];
			C->M[i][j] = sum;
		}

end:
	return flag;
}

bool tranv(RealMatrix *B, RealMatrix *A) { /// B = A^T
  bool flag = true;
	if (NULL == A || NULL == B) {
		L4C_ERROR("Fatal error occurs in tranv: RealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->row != B->col|| A->col != B->row) {
		L4C_ERROR("Fatal error occurs in tranv: RealMatrix parameter error!");
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

bool t_mul(RealMatrix *B, RealMatrix *A) {  /// B = A*A^T
  ///TODO:;
  bool flag = true;

end:
  return flag;
}

bool e_norm(REAL &norm, const RealMatrix *A) { /// norm = |V|
  bool flag = true;
  REAL temp = 0.0;
  if (NULL == A) {
    L4C_ERROR("Fatal error occurs in euclid_norm_d: RealMatrix pointer is NULL!");
    flag = false;
    goto end;
  }

  norm = sqrt(f_norm(norm, A));

end:
  return flag;
}

bool f_norm(REAL &fnorm, const RealMatrix *A) { /// Frobenius norm of RealMatrix
  bool flag = true;
  REAL sum = 0.0;
  REAL tmp = 0.0;
  UINT i, j;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in f_norm: RealMatrix pointer is NULL!");
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

bool m_norm(REAL &mnorm, const RealMatrix *A) { /// Manhattan norm of RealMatrix
  bool flag = true;
  REAL sum = 0.0;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in m_norm: RealMatrix pointer is NULL!");
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

bool get_min(REAL &min, const RealMatrix *A) {
  bool flag = true;
  REAL _min = Max;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in get_min: RealMatrix pointer is NULL!");
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

bool get_max(REAL &max, const RealMatrix *A) {
  bool flag = true;
  REAL _max = Min;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in get_max: RealMatrix pointer is NULL!");
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

bool num_mul(RealMatrix *B, RealMatrix *A, const REAL real) { /// B = k * A
  bool flag = true;
	if (NULL == A || NULL == B) {
		L4C_ERROR("Fatal error occurs in num_mul: RealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->row != B->row || A->col != B->col) {
		L4C_ERROR("Fatal error occurs in num_mul: RealMatrix parameter error!");
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

bool num_add(RealMatrix *B, RealMatrix *A, REAL real) { /// B = [k] + A
  bool flag = true;
	if (NULL == A || NULL == B) {
		L4C_ERROR("Fatal error occurs in num_add: RealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->row != B->row || A->col != B->col) {
		L4C_ERROR("Fatal error occurs in num_add: RealMatrix parameter error!");
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

bool hadamard_mul(RealMatrix *C, const RealMatrix *A, const RealMatrix *B) {///  hadamard mul
  bool flag = true;
	if (NULL == A || NULL == B || NULL == C) {
		L4C_ERROR("Fatal error occurs in hadamard_mul: RealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->col != B->row || A->row != C->row || B->col != C->col) {
		L4C_ERROR("Fatal error occurs in hadamard_mul: RealMatrix parameter error!");
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

bool kronecker_mul(RealMatrix *C, const RealMatrix *A, const RealMatrix *B) {///  kronecker mul
  bool flag = true;
	if (NULL == A || NULL == B || NULL == C) {
		L4C_ERROR("Fatal error occurs in kronecker_mul: RealMatrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (C->row != A->row*B->row) {
		L4C_ERROR("Fatal error occurs in kronecker_mul: RealMatrix C must be a RealMatrix of d(C) = d(A)*d(B)!");
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
