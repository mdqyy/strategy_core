#include "base.h"
#include <iostream>
#include <cstring>
extern "C"
{
#include <cblas.h> 
}

namespace dense {
class RealMatrix:virtual public Matrix {
 public:
 	REAL *M;
	RealMatrix(UINT row, UINT col);
	virtual ~RealMatrix();
	virtual void print() const;
};

class RealVector:public Vector,
 								 public RealMatrix {
 public:
	RealVector(UINT row, UINT col):Matrix(row, col),
																 Vector(row, col),
																 RealMatrix(row, col) {};
};

class RealSquare:public Square,
								 public RealMatrix {
 public:
	RealSquare(UINT size):Matrix(size, size),
												Square(size),
												RealMatrix(size, size) {};
};

bool copy(RealMatrix &M_dest, const RealMatrix &M_src);
bool mul(RealMatrix &C, const RealMatrix &A, const RealMatrix &B);
}
