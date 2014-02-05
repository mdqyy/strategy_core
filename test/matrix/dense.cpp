#include "dense.h"

namespace dense {
RealMatrix::RealMatrix(UINT row, UINT col):Matrix(row, col) {
  M = new REAL[row*col];
  memset(M, 0, row * col * sizeof(REAL));
}

RealMatrix::~RealMatrix() {
  delete []M; M = NULL;
}

void RealMatrix::print() const {
  std::cout << "[\n";
  for (UINT i = 0; i < row; i++) {
	  for (UINT j = 0; j < col - 1; j++)	{
		  std::cout << M[i*col + j] << ",";
	  }
	  std::cout << M[i*col + col - 1] << std::endl;
  }
  std::cout << "]\n";
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

bool mul(RealMatrix &C, const RealMatrix &A, const RealMatrix &B) { /// C = A * B
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
}

