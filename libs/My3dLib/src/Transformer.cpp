#include "Transformer.h"

using namespace boost::numeric;
using I = ublas::identity_matrix<double>;

Transformer::Transformer():
                scale_(I(4, 4)),
                rotate_(I(4, 4)),
                translate_(I(4, 4)) {
}

void Transformer::setPosition(const vector& pos) {
  if (pos.size() < 3) {
    throw std::invalid_argument("Invalid size vector");
  }

  for (int i = 0; i < 3; ++i) {
    translate_(i, 3) = pos[i];
  }
}

void Transformer::translate(const vector& vec) {
  if (vec.size() < 3) {
    throw std::invalid_argument("Invalid size vector");
  }

  translate_ = ublas::prod(translate_, getTranslationMatrix(vec));
}

void Transformer::rotate(double angle, Axes axis) {
  switch (axis) {
    case Axes::Ox :
      rotate_ = ublas::prod(rotate_, getOxRotateMatrix(angle));
      break;
    case Axes::Oy :
      rotate_ = ublas::prod(rotate_, getOyRotateMatrix(angle));
      break;
    case Axes::Oz :
      rotate_ = ublas::prod(rotate_, getOzRotateMatrix(angle));
      break;
    default:
      throw std::invalid_argument("Invalid axis.");
  }
}

void Transformer::scale(double x, double y, double z) {
  scale_ = ublas::prod(scale_, getScaleMatrix(x, y, z));
}

Transformer::matrix Transformer::getResultMatrix() const {
  matrix res = ublas::prod(translate_, rotate_);
  res = ublas::prod(res, scale_);
  return res;
}

//-------------------------PRIVATE-------------------------

Transformer::matrix Transformer::getTranslationMatrix(const vector& vec) {
  matrix T(4, 4);

  T <<=
      1, 0, 0, vec[0],
      0, 1, 0, vec[1],
      0, 0, 1, vec[2],
      0, 0, 0, 1;

  return T;
}

Transformer::matrix Transformer::getScaleMatrix(double x, double y, double z) {
  matrix S(4, 4);

  S <<=
      x, 0, 0, 0,
      0, y, 0, 0,
      0, 0, z, 0,
      0, 0, 0, 1;

  return S;
}

Transformer::matrix Transformer::getOxRotateMatrix(double angle) {
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

Transformer::matrix Transformer::getOyRotateMatrix(double angle) {
  matrix R(4, 4);

  double rad_angle = angle * M_PI / 180;
  double c = std::cos(rad_angle);
  double s = std::sin(rad_angle);

  R <<=
      c, 0, s, 0,
      0, 1, 0, 0,
     -s, 0, c, 0,
      0, 0, 0, 1;

  return R;
}

Transformer::matrix Transformer::getOzRotateMatrix(double angle) {
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
