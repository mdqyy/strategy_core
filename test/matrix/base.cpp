#include "base.h"

Vector::Vector(UINT row, UINT col):Matrix(row, col) {
  if (1 == row) {
    size = col;
    by_row = false;
  }
  else {
    size = row;
    by_row = true;
  }
}

