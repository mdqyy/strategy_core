#include "dense.h"

namespace dense {
RealMatrix::RealMatrix(UINT row, UINT col):Matrix(row, col) {
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

bool copy(RealMatrix *M_dest, const RealMatrix *M_src) {
  bool flag = true;
  if (NULL == M_dest or NULL == M_src) {
    std::cout << "Fatal error occurs in copy: RealMatrix pointer is NULL!" << std::endl;
    flag = false;
    goto end;
  }
  if (not (M_dest->row == M_src->row and M_dest->col == M_src->col)) {
    std::cout << "Fatal error occurs in copy: RealMatrix parameter error!" << std::endl;
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
		std::cout << "Fatal error occurs in inv: RealMatrix pointer is NULL!" << std::endl;
		flag = false;
		goto end;
	}
	if (A->row != A->col || B->row != B->col || A->row != B->row) {
		std::cout << "Fatal error occurs in inv: RealMatrix parameter error!" << std::endl;
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
				std::cout << "Warning in inv: Determinant of A equals zero!" << std::endl;
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

}
