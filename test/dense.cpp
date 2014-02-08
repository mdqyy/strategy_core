#include "dense.h"

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
  for (UINT i = 0; i < row; i++) {
    printf("(");
	  for (UINT j = 0; j < col; j++)	{
      printf(" %f", M[i*col + j]);
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

bool RealVector::get(REAL &value, const UINT pos) {
  bool flag = true;
  if (pos < 0 or pos >= Vector::size) {
    flag = false;
    goto end;
  }

  value = RealMatrix::M[pos];

end:
  return flag;
}

bool copy(RealMatrix &M_dest, const RealMatrix &M_src) {
  bool flag = true;
  if (NULL == &M_dest or NULL == &M_src) {
    std::cout << "Fatal error occurs in copy: RealMatrix pointer is NULL!" << std::endl;
    flag = false;
    goto end;
  }
  if (not (M_dest.row == M_src.row and M_dest.col == M_src.col)) {
    std::cout << "Fatal error occurs in copy: RealMatrix parameter error!" << std::endl;
    flag = false;
    goto end;
  }

  memmove((void *)M_dest.M, (void *)M_src.M, M_src.row * M_src.col * sizeof(REAL));

end:
  return flag;
}


bool neg(RealMatrix &A) {
  bool flag = true;
  UINT i, j;
	if (NULL == &A) {
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

bool mm(RealMatrix &C, const RealMatrix &A, const RealMatrix &B) { /// C = A * B
  bool flag = true;
  const enum CBLAS_ORDER Order = CblasRowMajor;
  const enum CBLAS_TRANSPOSE TransA = CblasNoTrans;
  const enum CBLAS_TRANSPOSE TransB = CblasNoTrans;
  const float alpha = 1;
  const float beta = 0;

  if (NULL == &A || NULL == &B || NULL == &C) {
    flag = false;
	  goto end;
  }
  if (A.col != B.row || A.row != C.row || B.col != C.col) {
    flag = false;
	  goto end;
  }

  cblas_sgemm(Order, TransA, TransB, A.row, B.col, A.col, alpha, A.M, A.col, B.M, B.col, beta, C.M, C.col);

end:
  return flag;
}

bool sv(RealVector &b, RealSquare &A) {
  bool flag = true;
  lapack_int n, nrhs, lda, ldb, info;
	lapack_int *ipiv;
  n = A.size, nrhs = 1, lda = n, ldb = nrhs;
  ipiv = new lapack_int[n];
  if (NULL == &A || NULL == &b) {
    flag = false;
	  goto end;
  }
  if (A.size != b.size) {
    flag = false;
	  goto end;
  }

  info = LAPACKE_sgesv(LAPACK_ROW_MAJOR, n, nrhs, A.M, lda, ipiv, b.M, ldb);

  if (info > 0) {
    printf("The diagonal element of the triangular factor of A,\n");
    printf("U(%i,%i) is zero, so that A is singular;\n", info, info);
    printf("the solution could not be computed.\n");
    flag = false;
    goto end;
  }

end:
  delete []ipiv; ipiv = NULL;
  return flag;
}

bool axpy(RealVector &y, const REAL alpha, const RealVector &x) {
  bool flag = true;
  UINT n, incX, incY;
  n = x.size;
  incX = 1;
  incY = 1;
  if (NULL == &x || NULL == &y) {
    flag = false;
	  goto end;
  }
  if (x.size != y.size) {
    flag = false;
	  goto end;
  }

  cblas_saxpy(n, alpha, x.M, incX, y.M, incY);

end:
  return flag;
}

}

