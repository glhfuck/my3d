#include "../include/Shape.h"

Shape::Shape(const ublas::vector<double>& pos) : M(4, 4) {
  setModelMatrix(pos);
}
void Shape::setModelMatrix(const ublas::vector<double>& pos) {
  if (pos.size() < 3) {
    throw std::invalid_argument("invalid size vector");
  }

  M <<=
      1, 0, 0, pos[0],
      0, 1, 0, pos[1],
      0, 0, 1, pos[2],
      0, 0, 0, 1;
}
