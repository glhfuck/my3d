#include "Shape.h"

Shape::Shape(const vector& pos) : M(4, 4) {
  M <<=
      1, 0, 0, 0,
      0, 1, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1;

  setPosition(pos);
}

void Shape::setPosition(const vector& pos) {
  if (pos.size() < 3) {
    throw std::invalid_argument("invalid size vector");
  }

  for (int i = 0; i < 3; ++i) {
    M(i, 3) = pos[i];
  }
}

void Shape::translate(const vector& pos) {
  M = ublas::prod(getTranslationMatrix(pos), M);
}

void Shape::localOxRotate(double angle) {
  M = ublas::prod(M, getOxRotateMatrix(angle));
}

void Shape::localOyRotate(double angle) {
  M = ublas::prod(M, getOyRotateMatrix(angle));
}

void Shape::localOzRotate(double angle) {
  M = ublas::prod(M, getOzRotateMatrix(angle));
}

void Shape::globalOxRotate(double angle) {
  M = ublas::prod(getOxRotateMatrix(angle), M);
}

void Shape::globalOyRotate(double angle) {
  M = ublas::prod(getOyRotateMatrix(angle), M);
}

void Shape::globalOzRotate(double angle) {
  M = ublas::prod(getOzRotateMatrix(angle), M);
}

matrix Shape::getOxRotateMatrix(double angle) {
  matrix R(4, 4);

  double rad_angle = angle * M_PI / 180;
  double c = std::cos(rad_angle);
  double s = std::sin(rad_angle);

  R <<=
      1, 0, 0, 0,
      0, c,-s, 0,
      0, s, c, 0,
      0, 0, 0, 1;

  return R;
}

matrix Shape::getOyRotateMatrix(double angle) {
  matrix R(4, 4);

  double rad_angle = angle * M_PI / 180;
  double c = std::cos(rad_angle);
  double s = std::sin(rad_angle);

  R <<=
      c, 0, s, 0,
      0, 0, 0, 0,
     -s, 0, c, 0,
      0, 0, 0, 1;

  return R;
}

matrix Shape::getOzRotateMatrix(double angle) {
  matrix R(4, 4);

  double rad_angle = angle * M_PI / 180;
  double c = std::cos(rad_angle);
  double s = std::sin(rad_angle);

  R <<=
      c,-s, 0, 0,
      s, c, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1;

  return R;
}
matrix Shape::getTranslationMatrix(const vector& pos) {
  matrix T(4, 4);

  if (pos.size() < 3) {
    throw std::invalid_argument("invalid size vector");
  }

  T <<=
      1, 0, 0, pos[0],
      0, 1, 0, pos[1],
      0, 0, 1, pos[2],
      0, 0, 0, 1;

  return T;
}



