//==============================================================================
/*!
 *  \file matrix.cpp
 *
 *  \brief matrix operations.
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#include <strategy_core/lin_alg/matrix.h>
#include <strategy_core/common/log4c.h>

using namespace dense;

namespace matrix {

bool copy(Matrix *M_dest, const Matrix *M_src) {
  bool flag = true;
  if (NULL == M_dest or NULL == M_src) {
    L4C_ERROR("Fatal error occurs in copy_d: Matrix pointer is NULL!");
    flag = false;
    goto end;
  }
  if ((M_dest->row == M_src->row) and (M_dest->col == M_src->col) ) {
    L4C_ERROR("Fatal error occurs in copy_d: Matrix parameter error!");
    flag = false;
    goto end;
  }

  if (typeid(M_dest).name == typeid(sparse::SparseRealMatrix).name) {
    ///TODO
  }
  else if (M_dest->is_sparse == true and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == false and M_src->is_sparse == false) {
    dense::copy((DenseRealMatrix *)M_dest, (DenseRealMatrix *)M_src);
  }

end:
  return flag;
}

/// invert
bool inv(Matrix *B, Matrix *A) {/// B = A^(-1)
  /// Gauss-Jordan method
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

  if (M_dest->is_sparse == false and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == true and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == false and M_src->is_sparse == false) {
    dense::inv((DenseRealMatrix *)B, (DenseRealMatrix *)A);
  }

end:
	return flag;
}

// C = A + B
bool add(Matrix *C, const Matrix *A, const Matrix *B) { /// C = A + B
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

  if (M_dest->is_sparse == false and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == true and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == false and M_src->is_sparse == false) {
    dense::add((DenseRealMatrix *)C, (DenseRealMatrix *)A, (DenseRealMatrix *)B);
  }

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

  if (M_dest->is_sparse == false and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == true and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == false and M_src->is_sparse == false) {
    dense::sub((DenseRealMatrix *)C, (DenseRealMatrix *)A, (DenseRealMatrix *)B);
  }

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

  if (M_dest->is_sparse == false and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == true and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == false and M_src->is_sparse == false) {
    dense::mul((DenseRealMatrix *)C, (DenseRealMatrix *)A, (DenseRealMatrix *)B);
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

  if (M_dest->is_sparse == false and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == true and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == false and M_src->is_sparse == false) {
    dense::tranv((DenseRealMatrix *)B, (DenseRealMatrix *)A);
  }

end:
	return flag;
}

bool t_mul(Matrix *B, Matrix *A) {  /// B = A*A^T
  ///TODO
}

bool f_norm(REAL &fnorm, Matrix *A) { /// Frobenius norm of matrix
  bool flag = true;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in f_norm: Matrix pointer is NULL!");
    flag = false;
		goto end;
	}

  if (M_dest->is_sparse == false and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == true and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == false and M_src->is_sparse == false) {
    dense::f_norm(fnorm, (DenseRealMatrix *)A);
  }

end:
	return  flag;
}

bool m_norm(REAL &mnorm, Matrix *A) { /// Manhattan norm of matrix
  bool flag = true;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in m_norm: Matrix pointer is NULL!");
    flag = false;
		goto end;
	}

  if (M_dest->is_sparse == false and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == true and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == false and M_src->is_sparse == false) {
    dense::m_norm(mnorm, (DenseRealMatrix *)A);
  }

end:
	return  flag;
}

bool get_min(REAL &min, Matrix *A) {
  bool flag = true;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in get_min: Matrix pointer is NULL!");
    flag = false;
		goto end;
	}

  if (M_dest->is_sparse == false and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == true and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == false and M_src->is_sparse == false) {
    dense::get_min(min, (DenseRealMatrix *)A);
  }

end:
  return  flag;
}

REAL get_max(REAL &max, Matrix *A) {
  bool flag = true;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in get_max: Matrix pointer is NULL!");
    flag = false;
		goto end;
	}

  if (M_dest->is_sparse == false and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == true and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == false and M_src->is_sparse == false) {
    dense::get_max(max, (DenseRealMatrix *)A);
  }

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

  if (M_dest->is_sparse == false and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == true and M_src->is_sparse == true) {
    ///TODO
  }
  else if (M_dest->is_sparse == false and M_src->is_sparse == false) {
    dense::num_mul((DenseRealMatrix *)B, (DenseRealMatrix *)A, REAL real);
  }

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

  if (A->is_sparse == true) {
    ///TODO
  }
  else {
    dense::num_add((DenseRealMatrix *)B, (DenseRealMatrix *)A, REAL real);
  }

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

  if (A->is_sparse == true) {
    ///TODO
  }
  else {
    dense::nlz((DenseRealMatrix *)B, (DenseRealMatrix *)A);
  }

end:
  return flag;
}

bool print(const Matrix *A) {
  bool flag = true;
	if (NULL == A) {
		L4C_ERROR("Fatal error occurs in print: Matrix pointer is NULL!");
    flag = false;
		goto end;
	}

  if (A->is_sparse == true) {
    ///TODO
  }
  else {
    dense::print(const (DenseRealMatrix *)A);
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

  if (A->is_sparse == false and B->is_sparse == true) {
    ///TODO
  }
  if (A->is_sparse == true and B->is_sparse == false) {
    ///TODO
  }
  else if (A->is_sparse == true and B->is_sparse == true) {
    ///TODO
  }
  else if (A->is_sparse == false and B->is_sparse == false) {
    dense::hadamard_mul((DenseRealMatrix *)C, (DenseRealMatrix *)B, (DenseRealMatrix *)A);
  }

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

  if (A->is_sparse == false and B->is_sparse == true) {
    ///TODO
  }
  if (A->is_sparse == true and B->is_sparse == false) {
    ///TODO
  }
  else if (A->is_sparse == true and B->is_sparse == true) {
    ///TODO
  }
  else if (A->is_sparse == false and B->is_sparse == false) {
    dense::hadamard_mul((DenseRealMatrix *)C, (DenseRealMatrix *)B, (DenseRealMatrix *)A);
  }

end:
	return flag;
}

}
