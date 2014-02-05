#include "base.h"

Vector::Vector(UINT row, UINT col):Matrix(row, col) {
  if (1 == row) {
	  this->size = col;
	  this->by_row = false;
  }
  else {
	  this->size = row;
	  this->by_row = true;
  }
}

