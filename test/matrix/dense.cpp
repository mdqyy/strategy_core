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
  printf("\n");
  for (UINT i = 0; i < row; i++) {
    printf("(");
    for (UINT j = 0; j < col; j++)  {
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
      REAL r = (REAL)rand()/RAND_MAX - 0.5; // r ~ U(-0.5, 0.5)
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

/// Blas level 1 operations
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

bool dot(REAL &inner, const RealVector &x, const RealVector &y) {
  bool flag = true;
  UINT N; INT incX, incY;
  N = x.size;
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

  inner = cblas_sdot(N, x.M, incX, y.M, incY);

end:
  return flag;
}

bool nrm2(REAL &norm, const RealVector &x) {
  bool flag = true;
  UINT N, incX;
  N = x.size;
  incX = 1;
  if (NULL == &x) {
    flag = false;
    goto end;
  }

  norm = cblas_snrm2(N, x.M, incX);

end:
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

/// Blas level 2 operations
bool gemv(RealVector &y, const REAL alpha, const REAL beta, const RealMatrix &A, const RealVector &x) { /// y = alpha*A*x + beta*y
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
    goto end;
  }
  if (A.col != x.size || A.row != y.size) {
    flag = false;
    goto end;
  }

  cblas_sgemv(Order, TransA, M, N, alpha, A.M, lda, x.M, incX, beta, y.M, incY);

end:
  return flag;
}

bool mv(RealVector &y, const RealMatrix &A, const RealVector &x) { /// y = A*x + y
  bool flag = true;
  if (NULL == &A || NULL == &y || NULL == &x) {
    flag = false;
    goto end;
  }
  if (A.col != x.size || A.row != y.size) {
    flag = false;
    goto end;
  }

  gemv(y, 1, 0, A, x);

end:
  return flag;
}

bool gevtm(RealVector &y, const REAL alpha, const REAL beta, const RealVector &x, const RealMatrix &A) { /// y = xt *A + y
  bool flag = true;
  const enum CBLAS_ORDER Order = CblasRowMajor;
  const enum CBLAS_TRANSPOSE Transx = CblasNoTrans;
  const enum CBLAS_TRANSPOSE TransA = CblasNoTrans;
  if (NULL == &A || NULL == &y || NULL == &x) {
    flag = false;
    goto end;
  }
  if (x.row != A.row || x.col != y.row || A.col != y.col) {
    flag = false;
    goto end;
  }

  cblas_sgemm(Order, Transx, TransA, x.col, A.col, x.row, alpha, x.M, x.row, A.M, A.col, beta, y.M, y.col);

end:
  return flag;
}

bool vtm(RealVector &y, const RealVector &x, const RealMatrix &A) { /// y = xt *A + y
  bool flag = true;
  if (NULL == &A || NULL == &y || NULL == &x) {
    flag = false;
    goto end;
  }
  if (x.row != A.row || x.col != y.row || A.col != y.col) {
    flag = false;
    goto end;
  }

  gevtm(y, 1, 0, x, A);

end:
  return flag;
}
/// Blas level 3 operations
bool gemm(RealMatrix &C, const REAL alpha, const REAL beta, const RealMatrix &A, const RealMatrix &B) { /// C = alpha*A*B + beta*C
  bool flag = true;
  const enum CBLAS_ORDER Order = CblasRowMajor;
  const enum CBLAS_TRANSPOSE TransA = CblasNoTrans;
  const enum CBLAS_TRANSPOSE TransB = CblasNoTrans;
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

  gemm(C, 1, 0, A, B);

end:
  return flag;
}

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
    goto end;
  }

  cblas_sgemm(Order, Transx, Transy, x.row, y.row, x.col, alpha, x.M, x.col, y.M, y.row, beta, C.M, C.col);

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

bool add(RealMatrix &A, const REAL alpha, const REAL beta, const RealMatrix &B) { /// A = alpha*A + beta*B
  bool flag = true;
  RealSquare *X = new RealSquare(B.col);
  X->unitise();
  if (NULL == &A || NULL == &B) {
    flag = false;
    goto end;
  }
  if (A.row != B.row || A.col != B.col) {
    flag = false;
    goto end;
  }

  gemm(A, alpha, beta, *X, B);

end:
  delete X; X = NULL;
  return flag;
}

/// LAPACK
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
}

