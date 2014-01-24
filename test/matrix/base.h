#include <cstddef>
#include <cmath>

#define REAL float
#define UINT unsigned int
#define INT size_t

class Matrix {
 public:
  UINT row;
  UINT col;
  Matrix(UINT row, UINT col):row(row),
													 col(col) {};
};

class Vector:virtual public Matrix {
 public:
  UINT size;
	bool by_row;
  Vector(UINT row, UINT col);
};

class Square:virtual public Matrix {
 public:
  UINT size;
  Square(UINT size):Matrix(size, size), 
									 size(size)	{};
};

