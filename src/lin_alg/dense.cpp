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
/// DenseRealMatrix
RealMatrix::RealMatrix(UINT row, UINT col):Matrix(row, col) {
  M = new REAL[row*col];
  srand((UINT)time(NULL));
}

RealMatrix::~RealMatrix() {
  delete []M; M = NULL;
}

void RealMatrix::print() const {
  printf("\n");
  for (UINT i = 0; i < row; i++) {
    printf("(");
	  for (UINT j = 0; j < col; j++)	{
      printf(" %0.20f", M[i*col + j]);
	  }
	  printf(" )\n");
  }
  printf("\n");
}

void RealMatrix::zeroize() {
  memset(M, 0, Matrix::row * Matrix::col * sizeof(REAL));
}

void RealMatrix::randomize() {
  for (UINT i = 0; i < Matrix::row; i++) {
	  for (UINT j = 0; j < Matrix::col; j++) {
		  REAL r = (REAL)rand()/RAND_MAX; // r ~ U(0, 1)
		  M[i*Matrix::col + j] = r;
	  }
  }
}

void RealSquare::unitise() {
  for (UINT i = 0; i < Square::size; i++) {
    RealSquare::M[i*Square::size + i] = 1.0;
  }
}

/// DenseRealVector
bool RealVector::set(const UINT pos, const REAL value) {
  bool flag = true;
  if (pos < 0 or pos >= Vector::size) {
    flag = false;
    goto end;
  }

  RealMatrix::M[pos] = value;

end:
  return flag;
}

REAL RealVector::get(const UINT pos) const {
  return RealMatrix::M[pos];
}

void RealVector::unitise() {
  for (UINT i = 0; i < RealVector::size; i++) {
    RealMatrix::M[i] = 1.0;
  }
}
/* ===========================================================================
 * level 1 BLAS opertations
 * ===========================================================================*/
// M_dest <- M_src
bool copy(RealMatrix &M_dest, const RealMatrix &M_src) {
  bool flag = true;
  if (NULL == &M_dest or NULL == &M_src) {
    L4C_ERROR("Fatal error occurs in copy: pointer is NULL!");
    flag = false;
    goto end;
  }
  if (not (M_dest.row == M_src.row and M_dest.col == M_src.col)) {
    L4C_ERROR("Fatal error occurs in copy: size does not fit!");
    flag = false;
    goto end;
  }

  memmove((void *)M_dest.M, (void *)M_src.M, M_src.row*M_src.col*sizeof(REAL));

end:
  return flag;
}

//inner = x*y
bool dot(REAL &inner, const RealVector &x, const RealVector &y) {
  bool flag = true;
  UINT N; INT incX, incY;
  N = x.size;
  incX = 1;
  incY = 1;
  if (NULL == &x || NULL == &y) {
    flag = false;
    L4C_ERROR("Fatal error occurs in dot: pointer is NULL!");
	  goto end;
  }
  if (x.size != y.size) {
    flag = false;
    L4C_ERROR("Fatal error occurs in dot: size does not fit!");
	  goto end;
  }

  inner = cblas_sdot(N, x.M, incX, y.M, incY);

end:
  return flag;
}

// norm = ||x||
bool nrm2(REAL &norm, const RealVector &x) {
  bool flag = true;
  UINT N, incX;
  N = x.size;
  incX = 1;
  if (NULL == &x) {
    flag = false;
    L4C_ERROR("Fatal error occurs in nrm2: pointer is NULL!");
	  goto end;
  }

  norm = cblas_snrm2(N, x.M, incX);

end:
  return flag;
}

// y = a*x + y
bool axpy(RealVector &y, const REAL alpha, const RealVector &x) {
  bool flag = true;
  UINT n, incX, incY;
  n = x.size;
  incX = 1;
  incY = 1;
  if (NULL == &x || NULL == &y) {
    flag = false;
    L4C_ERROR("Fatal error occurs in axpy: pointer is NULL!");
	  goto end;
  }
  if (x.size != y.size) {
    flag = false;
    L4C_ERROR("Fatal error occurs in axpy: size does not fit!");
	  goto end;
  }

  cblas_saxpy(n, alpha, x.M, incX, y.M, incY);

end:
  return flag;
}

/* ===========================================================================
 * level 2 BLAS opertations
 * ===========================================================================*/
// y = alpha*A*x + beta*y
bool gemv(RealVector &y, const REAL alpha, const REAL beta, const RealMatrix &A,
          const RealVector &x) {
  bool flag = true;
  const enum CBLAS_ORDER Order = CblasRowMajor;
  const enum CBLAS_TRANSPOSE TransA = CblasNoTrans;
  const enum CBLAS_TRANSPOSE TransB = CblasNoTrans;
  UINT M, N; INT incX, incY, lda;
  M = A.row;
  N = A.col;
  incX = 1;
  incY = 1;
  lda = M;
  if (NULL == &A || NULL == &y || NULL == &x) {
    flag = false;
    L4C_ERROR("Fatal error occurs in gemv: pointer is NULL!");
	  goto end;
  }
  if (A.col != x.size || A.row != y.size) {
    flag = false;
    L4C_ERROR("Fatal error occurs in gemv: size does not fit!");
	  goto end;
  }

  cblas_sgemv(Order, TransA, M, N, alpha, A.M, lda, x.M, incX, beta, y.M, incY);

end:
  return flag;
}

// y = A * x + y
bool mv(RealVector &y, const RealMatrix &A, const RealVector &x) {
  bool flag = true;
  if (NULL == &A || NULL == &y || NULL == &x) {
    flag = false;
    L4C_ERROR("Fatal error occurs in gemv: pointer is NULL!");
	  goto end;
  }
  if (A.col != x.size || A.row != y.size) {
    flag = false;
    L4C_ERROR("Fatal error occurs in gemv: size does not fit!");
	  goto end;
  }

  gemv(y, 1, 0, A, x);

end:
  return flag;
}

// y = alpha*xt *A + beta*y
bool gevtm(RealVector &y, const REAL alpha, const REAL beta,
           const RealVector &x, const RealMatrix &A) {
  bool flag = true;
  const enum CBLAS_ORDER Order = CblasRowMajor;
  const enum CBLAS_TRANSPOSE Transx = CblasNoTrans;
  const enum CBLAS_TRANSPOSE TransA = CblasNoTrans;
  if (NULL == &A || NULL == &y || NULL == &x) {
    flag = false;
    L4C_ERROR("Fatal error occurs in gevtm: pointer is NULL!");
	  goto end;
  }
  if (x.row != A.row || x.col != y.row || A.col != y.col) {
    flag = false;
    L4C_ERROR("Fatal error occurs in gevtm: size does not fit!");
	  goto end;
  }

  cblas_sgemm(Order, Transx, TransA, x.col, A.col, x.row, alpha, x.M, x.row,
              A.M, A.col, beta, y.M, y.col);

end:
  return flag;
}

// y = xt *A + y
bool vtm(RealVector &y, const RealVector &x, const RealMatrix &A) {
  bool flag = true;
  if (NULL == &A || NULL == &y || NULL == &x) {
    flag = false;
    L4C_ERROR("Fatal error occurs in gevtm: pointer is NULL!");
	  goto end;
  }
  if (x.row != A.row || x.col != y.row || A.col != y.col) {
    flag = false;
    L4C_ERROR("Fatal error occurs in gevtm: size does not fit!");
	  goto end;
  }

  gevtm(y, 1, 0, x, A);

end:
  return flag;
}

/* ===========================================================================
 * level 3 BLAS opertations
 * ===========================================================================*/
// C = alpha*A*B + beta*C
bool gemm(RealMatrix &C, const REAL alpha, const REAL beta, const RealMatrix &A,
          const RealMatrix &B) {
  bool flag = true;
  const enum CBLAS_ORDER Order = CblasRowMajor;
  const enum CBLAS_TRANSPOSE TransA = CblasNoTrans;
  const enum CBLAS_TRANSPOSE TransB = CblasNoTrans;

  if (NULL == &A || NULL == &B || NULL == &C) {
    flag = false;
    L4C_ERROR("Fatal error occurs in sgemm: pointer is NULL!");
	  goto end;
  }
  if (A.col != B.row || A.row != C.row || B.col != C.col) {
    flag = false;
    L4C_ERROR("Fatal error occurs in sgemm: size does not fit!");
	  goto end;
  }

  cblas_sgemm(Order, TransA, TransB, A.row, B.col, A.col, alpha, A.M, A.col,
              B.M, B.col, beta, C.M, C.col);

end:
  return flag;
}

// C = A * B
bool mm(RealMatrix &C, const RealMatrix &A, const RealMatrix &B) {
  bool flag = true;
  const enum CBLAS_ORDER Order = CblasRowMajor;
  const enum CBLAS_TRANSPOSE TransA = CblasNoTrans;
  const enum CBLAS_TRANSPOSE TransB = CblasNoTrans;
  const float alpha = 1;
  const float beta = 0;

  if (NULL == &A || NULL == &B || NULL == &C) {
    flag = false;
    L4C_ERROR("Fatal error occurs in mm: pointer is NULL!");
	  goto end;
  }
  if (A.col != B.row || A.row != C.row || B.col != C.col) {
    flag = false;
    L4C_ERROR("Fatal error occurs in mm: size does not fit!");
	  goto end;
  }

  gemm(C, 1, 0, A, B);

end:
  return flag;
}

// C = x * y^t
bool vvt(RealSquare &C, const RealVector &x, const RealVector &y) {
  bool flag = true;
  const enum CBLAS_ORDER Order = CblasRowMajor;
  const enum CBLAS_TRANSPOSE Transx = CblasNoTrans;
  const enum CBLAS_TRANSPOSE Transy = CblasNoTrans;
  const float alpha = 1;
  const float beta = 0;

  if (NULL == &x || NULL == &y || NULL == &C) {
    flag = false;
	  goto end;
  }

  if (x.size != y.size || x.size != C.size ) {
    flag = false;
    L4C_ERROR("Fatal error occurs in vvt: size does not fit!");
	  goto end;
  }

  cblas_sgemm(Order, Transx, Transy, x.row, y.row, x.col, alpha, x.M, x.col,
              y.M, y.row, beta, C.M, C.col);

end:
  return flag;
}

// A = -A
bool neg(RealMatrix &A) {
  bool flag = true;
  UINT i, j;
	if (NULL == &A) {
		L4C_ERROR("Fatal error occurs in f_norm: pointer is NULL!");
    flag = false;
		goto end;
	}

	for (i = 0; i < A.row; i++)
		for(j = 0; j < A.col; j++) {
			A.M[i*A.col + j] = - A.M[i*A.col + j];
		}

end:
	return flag;
}

// A = alpha*A + beta*B
bool add(RealMatrix &A, const REAL alpha, const REAL beta,
         const RealMatrix &B) {
  bool flag = true;
  RealSquare *X = new RealSquare(B.col);
  X->unitise();
  if (NULL == &A || NULL == &B) {
    flag = false;
    L4C_ERROR("Fatal error occurs in add: pointer is NULL!");
	  goto end;
  }
  if (A.row != B.row || A.col != B.col) {
    flag = false;
    L4C_ERROR("Fatal error occurs in vvt: size does not fit!");
	  goto end;
  }

  gemm(A, alpha, beta, *X, B);

end:
  delete X; X = NULL;
  return flag;
}

/* ===========================================================================
 * LAPACK opertations
 * ===========================================================================*/
// solve Ax = b
bool sv(RealVector &b, RealSquare &A) {
  bool flag = true;
  lapack_int n, nrhs, lda, ldb, info;
	lapack_int *ipiv;
  n = A.size, nrhs = 1, lda = n, ldb = nrhs;
  ipiv = new lapack_int[n];
  if (NULL == &A || NULL == &b) {
    L4C_ERROR("Fatal error occurs in sv: pointer is NULL!");
    flag = false;
	  goto end;
  }
  if (A.size != b.size) {
    L4C_ERROR("Fatal error occurs in sv: size does not fit!");
    flag = false;
	  goto end;
  }

  info = LAPACKE_sgesv(LAPACK_ROW_MAJOR, n, nrhs, A.M, lda, ipiv, b.M, ldb);

  if (info > 0) {
    L4C_WARN("The diagonal element of the triangular factor of A,\n");
    L4C_WARN("U(%i, %i) is zero, so that A is singular;\n", info, info);
    L4C_WARN("the solution could not be computed.\n");
    flag = false;
    goto end;
  }

end:
  delete []ipiv; ipiv = NULL;
  return flag;
}

}
