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
#include <strategy_core/discrete/CrossList.h>

namespace sparse {
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

/// Sparse Matrix implementation using Cross List
class RealMatrix:public Matrix {
 public:
  CrossList *cl;

  RealMatrix(const UINT row, const UINT col);
  virtual ~RealMatrix();
  virtual bool append(CrossListNode *clnode);
  virtual void print() const;
};

class ZeroMatrix:RealMatrix {
 public:
  ZeroMatrix(const UINT row, const UINT col):RealMatrix(row, col) {};
};

class RealSquare:public Square,
                 public RealMatrix {
 public:
  RealSquare(const UINT size):Matrix(size, size),
                              Square(size),
                              RealMatrix(size, size) {};
};

/// RealVector
class RealVector:public Vector {
 public:
  SparseList *sl;

  RealVector(const UINT row, const UINT col);
  ~RealVector();
  bool append(RealVectorPoint *rvp);
};

/// Matrix operations
bool copy(RealMatrix *M_dest, const RealMatrix *M_src);
bool inv(RealMatrix *B, RealMatrix *A); /// B = A^(-1)
bool add(RealMatrix *C, RealMatrix *A, RealMatrix *B);  /// C = A + B
bool sub(RealMatrix *C, RealMatrix *A, RealMatrix *B);  /// C = A - B
bool mul(RealMatrix *C, RealMatrix *A, RealMatrix *B);  /// C = A * B
bool t_mul(RealMatrix *B, RealMatrix *A);  /// B = A * A^T
bool tranv(RealMatrix *B, RealMatrix *A); /// B = A^T
bool f_norm(REAL &fnorm, RealMatrix *A); /// Frobenius norm.
bool m_norm(REAL &mnorm, RealMatrix *A); /// Manhattan norm of matrix
bool get_min(REAL &min, RealMatrix *A); /// get min
bool get_max(REAL &max, RealMatrix *A);/// get max
bool num_mul(RealMatrix *B, RealMatrix *A, const double num); /// B = num * A
bool num_add(RealMatrix *B, RealMatrix *A, const double num); /// B = [num] + A
bool nlz(RealMatrix *B, RealMatrix *A); /// Normalize
bool hadamard_mul(RealMatrix *C, RealMatrix *A, RealMatrix *B);
bool kronecker_mul(RealMatrix *C, RealMatrix *A, RealMatrix *B);

/// Vector operations
bool inner_product(REAL &inner, const RealVector *rva, const RealVector *rvb);
}
#endif //STRATEGY_CORE_LIN_ALG_SPARSE_H_
