//==============================================================================
/*!
 *  \file matrix.cpp
 *
 *  \brief matrix operations.
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

#include <strategy_core/lin_alg/matrix.h>

namespace matrix {
bool inv(Matrix *B, Matrix *A) {/// B = A^(-1)
  /*!
   *  Gauss-Jordan method
   */
  bool flag = true;
	if (NULL == A || NULL == B) {
		L4C_ERROR("Fatal error occurs in inv: Matrix pointer is NULL!");
		flag = false;
		goto end;
	}
	if (A->row != A->col || B->row != B->col || A->row != B->row) {
		L4C_ERROR("Fatal error occurs in inv: Matrix parameter error!");
    flag = false;
		goto end;
	}

	INT *is, *js, i, j, k, l, u, v, n = A->row;
	REAL d,p;
	memmove((void *)B->M, (void *)A->M, n*n*sizeof(REAL));
	is = new INT[n * sizeof(INT)];
	js = new INT[n * sizeof(INT)];

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
				print(A);
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
bool add(Matrix *C, Matrix *A, Matrix *B) { /// C = A + B
  bool flag = true;
	if (A == NULL || B == NULL || C == NULL) {
		L4C_ERROR("Fatal error occurs in add: Matrix pointer is NULL!");
		flag = false;
		goto end;
	}
	if (A->row != B->row || A->col != B->col ||
      B->row != C->row || B->col != C->col) {
		L4C_ERROR("Fatal error occurs in add: Matrix parameter error!");
		return -1;
	}

	UINT i, j;
	for(i = 0; i < A->row; i++)
		for(j = 0; j < A->col; j++)
			C->M[i][j] = A->M[i][j] + B->M[i][j];

end:
	return flag;
}

bool sub(Matrix *C, Matrix *A, Matrix *B) { /// C = A - B
  bool flag = true;
	if (NULL == A || NULL == B || NULL == C) {
		L4C_ERROR("Fatal error occurs in sub: Matrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->row != B->row || A->col != B->col || B->row != C->row || B->col != C->col) {
		L4C_ERROR("Fatal error occurs in sub: Matrix parameter error!");
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

bool mul(Matrix *C, Matrix *A, Matrix *B) { /// C = A * B
  bool flag = true;
	if (NULL == A || NULL == B || NULL == C) {
		L4C_ERROR("Fatal error occurs in mul: Matrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->col != B->row || A->row != C->row || B->col != C->col) {
		L4C_ERROR("Fatal error occurs in mul: Matrix parameter error!");
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

bool tranv(Matrix *B, Matrix *A) { /// B = A^T
  bool flag = true;
	if (NULL == A || NULL == B) {
		L4C_ERROR("Fatal error occurs in tranv: Matrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->row != B->col|| A->col != B->row) {
		L4C_ERROR("Fatal error occurs in tranv: Matrix parameter error!");
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

bool t_mul(Matrix *B, Matrix *A) {  /// B = A*A^T
  ///TODO:;
}

bool f_norm(REAL &fnorm, Matrix *A, ) { /// Frobenius norm of matrix
  bool flag = true;
  REAL sum = 0.0;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in f_norm: Matrix pointer is NULL!");
    flag = false;
		goto end;
	}

	UINT i, j;
	REAL tmp = 0.0;
	for (i = 0; i < A->row; i++)
		for(j = 0; j < A->col; j++) {
			tmp = A->M[i][j];
			sum += tmp * tmp;
		}
  fnorm = sum;

end:
	return  flag;
}

bool m_norm(REAL &mnorm, Matrix *A) { /// Manhattan norm of matrix
  bool flag = true;
  REAL sum = 0.0;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in m_norm: Matrix pointer is NULL!");
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

bool get_min(REAL &min, Matrix *A) {
  bool flag = true;
  REAL _min = Max;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in get_min: Matrix pointer is NULL!");
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

REAL get_max(REAL &max, Matrix *A) {
  bool flag = true;
  REAL _max = Min;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in get_max: Matrix pointer is NULL!");
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

bool num_mul(Matrix *B, Matrix *A, REAL real) { /// B = k * A
  bool flag = true;
	if (NULL == A || NULL == B) {
		L4C_ERROR("Fatal error occurs in num_mul: Matrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->row != B->row || A->col != B->col) {
		L4C_ERROR("Fatal error occurs in num_mul: Matrix parameter error!");
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

bool num_add(Matrix *B, Matrix *A, REAL real) { /// B = [k] + A
  bool flag = true;
	if (NULL == A || NULL == B) {
		L4C_ERROR("Fatal error occurs in num_add: Matrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->row != B->row || A->col != b->col) {
		L4C_ERROR("Fatal error occurs in num_add: Matrix parameter error!");
    flag = false;
		goto end;
	}

	UINT i, j;
	for (i = 0; i < A->row; i++)
		for(j = 0; j < A->col; j++)
			B->M[i][j] = real + A->M[i][j];

end:
	return flag;
}

bool nlz(Matrix *B, Matrix *A) { /// Normalize
  bool flag = true;
	if (NULL == A || NULL == B) {
		L4C_ERROR("Fatal error occurs in nlz: Matrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->row != B->row || A->col != B->col) {
		L4C_ERROR("Fatal error occurs in nlz: Matrix parameter error!");
    flag = false;
		goto end;
	}

	UINT i, j;
	REAL min, incr, f_norm;
	///TODO:
	Matrix T;
	T.row = row;
	T.col = col;
	T.M = new REAL[sizeof(REAL)*T.row*T.col];
	min = get_min(A);
	incr = fabs(min) + VERY_SMALL_NUM;
	Matrix_num_add(A, incr, &T);
	f_norm = Matrix_m_norm(&T);
	if (f_norm + 0.0 == 0.0) {
		L4C_WARN("The Frobenius-Norm of matrix is zero. Apparently this matrix is zero-matrix!");
		flag = false;
		goto flag_nlz;
	}
	for (i = 0; i < row; i++)
		for(j = 0; j < col; j++) {
			//B->M[i][j] = T.M[i][j]/f_norm;
			B->M[i][j] = 2 * atan(T.M[i][j])/PI;
		}

end:
	delete []T.M;
	T.M = NULL;
  return flag;
}

bool print(const Matrix *A) {
  bool flag = true;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in print: Matrix pointer is NULL!");
    flag = false;
		goto end;
	}

	UINT i, j;
	for (i = 0; i < A->row; i++) {
		for(j = 0; j < A->col; j++)
			printf("%f ", A->M[i][j]);
		printf("\n");
	}

end:
	return flag;
}

bool hadamard_mul(Matrix *C, Matrix *A, Matrix *B) {///  hadamard mul
  bool flag = true;
	if (NULL == A || NULL == B || NULL == C) {
		L4C_ERROR("Fatal error occurs in hadamard_mul: Matrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (A->col != B->row || A->row != C->row || B->col != C->col) {
		L4C_ERROR("Fatal error occurs in hadamard_mul: Matrix parameter error!");
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

bool kronecker_mul(Matrix *C, Matrix *A, Matrix *B) {///  kronecker mul
  bool flag = true;
	if (NULL == A || NULL == B || NULL == C) {
		L4C_ERROR("Fatal error occurs in kronecker_mul: Matrix pointer is NULL!");
    flag = false;
		goto end;
	}
	if (C->row != A->row*B->row) {
		L4C_ERROR("Fatal error occurs in kronecker_mul: Matrix C must be a matrix of d(C) = d(A)*d(B)!");
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
