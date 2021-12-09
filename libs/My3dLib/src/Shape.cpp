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

ublas::matrix<double> Shape::getOxRotateMatrix(double angle) {
  ublas::matrix<double> R(4, 4);

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

ublas::matrix<double> Shape::getOyRotateMatrix(double angle) {
  ublas::matrix<double> R(4, 4);

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

ublas::matrix<double> Shape::getOzRotateMatrix(double angle) {
  ublas::matrix<double> R(4, 4);

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
