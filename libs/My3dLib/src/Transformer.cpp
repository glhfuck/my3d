#include "Transformer.h"

using namespace boost::numeric;
using I = ublas::identity_matrix<double>;

Transformer::Transformer() {
}

Transformer::matrix Transformer::getRTS() const {
  matrix res = ublas::prod(getRotationMatrix(), getTranslationMatrix());
  res = ublas::prod(res, getScalingMatrix());
  return res;
}

Transformer::matrix Transformer::getResultingMatrix() const {
  matrix res = ublas::prod(getTranslationMatrix(), getRotationMatrix());
  res = ublas::prod(res, getScalingMatrix());
  return res;
}

Transformer::matrix Transformer::getTranslationMatrix() const {
  return TranslationMatrix(x_position_, y_position_, z_position_);
}

Transformer::matrix Transformer::getRotationMatrix() const {
  matrix res = ublas::prod(OzRotationMatrix(z_angle_), OxRotationMatrix(x_angle_));
  res = ublas::prod(res, OyRotationMatrix(y_angle_));
  return res;
}

Transformer::matrix Transformer::getScalingMatrix() const {
  return ScalingMatrix(x_scale_, y_scale_, z_scale_);
}

void Transformer::SetPosition(double x, double y, double z) {
  x_position_ = x;
  y_position_ = y;
  z_position_ = z;
}

void Transformer::Translate(double delta_x, double delta_y, double delta_z) {
  x_position_ += delta_x;
  y_position_ += delta_y;
  z_position_ += delta_z;
}

void Transformer::SetRotate(double aroundOx, double aroundOy, double aroundOz) {
  x_angle_ = aroundOx;
  y_angle_ = aroundOy;
  z_angle_ = aroundOz;
}

void Transformer::Rotate(double aroundOx, double aroundOy, double aroundOz) {
  x_angle_ += aroundOx;
  y_angle_ += aroundOy;
  z_angle_ += aroundOz;
}

void Transformer::SetScale(double x, double y, double z) {
  x_scale_ = x;
  y_scale_ = y;
  z_scale_ = z;
}

void Transformer::Scale(double x, double y, double z) {
  x_scale_ *= x;
  y_scale_ *= y;
  z_scale_ *= z;
}

//-------------------------PRIVATE-------------------------

Transformer::matrix Transformer::TranslationMatrix(double x, double y, double z) {
  matrix T(4, 4);

  T <<=
      1, 0, 0, x,
      0, 1, 0, y,
      0, 0, 1, z,
      0, 0, 0, 1;

  return T;
}

Transformer::matrix Transformer::ScalingMatrix(double x, double y, double z) {
  matrix S(4, 4);

  S <<=
      x, 0, 0, 0,
      0, y, 0, 0,
      0, 0, z, 0,
      0, 0, 0, 1;

  return S;
}

Transformer::matrix Transformer::OxRotationMatrix(double angle) {
  matrix R(4, 4);
  double c = std::cos(angle);
  double s = std::sin(angle);

  R <<=
      1, 0, 0, 0,
      0, c,-s, 0,
      0, s, c, 0,
      0, 0, 0, 1;

  return R;
}

Transformer::matrix Transformer::OyRotationMatrix(double angle) {
  matrix R(4, 4);
  double c = std::cos(angle);
  double s = std::sin(angle);

  R <<=
      c, 0, s, 0,
      0, 1, 0, 0,
     -s, 0, c, 0,
      0, 0, 0, 1;

  return R;
}

Transformer::matrix Transformer::OzRotationMatrix(double angle) {
  matrix R(4, 4);
  double c = std::cos(angle);
  double s = std::sin(angle);

  R <<=
      c,-s, 0, 0,
      s, c, 0, 0,
      0, 0, 1, 0,
      0, 0, 0, 1;

  return R;
}
Transformer::vector Transformer::cross_prod(const Transformer::vector& first, const Transformer::vector& second) {
  vector res(3);
  res <<= first[2] * second[1] - first[1] * second[2],
          first[0] * second[2] - first[2] * second[0],
          first[1] * second[0] - first[0] * second[1];
  return res;
}
