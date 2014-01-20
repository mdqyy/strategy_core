//==============================================================================
/*!
 *  \file sparse.h
 *
 *  \brief sparse structures for linear algebra
 *
 *  \author chenqinyu
 *          contact:
 *              superthierry14@gmail.com
 *  \date 2013-07
 *
 *  \par
 */
//==============================================================================

#ifndef STRATEGY_CORE_LIN_ALG_SPARSE_H_
#define STRATEGY_CORE_LIN_ALG_SPARSE_H_

#include <strategy_core/lin_alg/base.h>

namespace sparse {
/// Cross List
class CrossListNode {
 public:
  INT i, j; /// position of a non-zero element, a[i,j]
  REAL e;
  CrossListNode *right, *down;
  CrossListNode(const UINT i, const UINT j, const REAL e);
};

class SingleList {
 public:
  CrossListNode *head;
  CrossListNode *tail;
  UINT length;
  SingleList();
  ~SingleList();
};

class CrossList {
 public:
  SingleList **rslArray, **lslArray;
  UINT row, col, nz; /// num of rows, columns and non-zero elements.
  CrossList(const UINT row, const UINT col);
  ~CrossList();

  bool append(CrossListNode *clnode);
  SingleList *get_row(const UINT row_no);
  SingleList *get_col(const UINT col_no);
  bool output_row(const UINT row_no);
  bool output_col(const UINT col_no);
  void output_all(void);
};

/*! Sparse Vector implementation using Linked List */
class RealVectorPoint {
 public:
  UINT index;
  REAL value;
  RealVectorPoint *next;
  RealVectorPoint(const UINT index, const REAL value);
};

class SparseList {
 public:
  RealVectorPoint *head;
  RealVectorPoint *tail;
  INT length;
  SparseList();
  ~SparseList();
};

class SparseRealVector:public Vector {
 public:
  SparseList *sl;
  SparseRealVector(const UINT row, const UINT col);
  ~SparseRealVector();

  bool append(RealVectorPoint *rvp);
};

/*! Sparse Matrix implementation using Cross List */
class SparseRealMatrix:public Matrix {
 public:
  CrossList *cl;

  SparseRealMatrix(const UINT row, const UINT col);
  ~SparseRealMatrix();
};

class SparseSquareMatrix:public SparseRealMatrix {
 public:
  SparseSquareMatrix(const UINT n):SparseRealMatrix(n, n) {};
};

class DiagMatrix:public SparseSquareMatrix {
 public:
  DiagMatrix(const UINT n);
};

class ZeroMatrix:public SparseRealMatrix {
 public:
  ZeroMatrix(const UINT row, const UINT col):SparseRealMatrix(row, col) {};
};

/// Matrix operations
bool copy(Matrix *M_dest, const Matrix *M_src);
bool inv(Matrix *B, Matrix *A); /// B = A^(-1)
bool add(Matrix *C, Matrix *A, Matrix *B);  /// C = A + B
bool sub(Matrix *C, Matrix *A, Matrix *B);  /// C = A - B
bool mul(Matrix *C, Matrix *A, Matrix *B);  /// C = A * B
bool t_mul(Matrix *B, Matrix *A);  /// B = A * A^T
bool tranv(Matrix *B, Matrix *A); /// B = A^T
bool f_norm(REAL &fnorm, Matrix *A); /// Frobenius norm.
bool m_norm(REAL &mnorm, Matrix *A); /// Manhattan norm of matrix
bool get_min(REAL &min, Matrix *A); /// get min
bool get_max(REAL &max, Matrix *A);/// get max
bool num_mul(Matrix *B, Matrix *A, const double num); /// B = num * A
bool num_add(Matrix *B, Matrix *A, const double num); /// B = [num] + A
bool nlz(Matrix *B, Matrix *A); /// Normalize
bool print(const Matrix *A);  /// Output matrix.
bool hadamard_mul(Matrix *C, Matrix *A, Matrix *B);
bool kronecker_mul(Matrix *C, Matrix *A, Matrix *B);
}
#endif //STRATEGY_CORE_LIN_ALG_SPARSE_H_
