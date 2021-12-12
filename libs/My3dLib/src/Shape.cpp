#include "Shape.h"

using namespace boost::numeric;
using I = ublas::identity_matrix<double>;

Shape::Shape(): localScale_(I(4, 4)),
                localRotate_(I(4, 4)),
                localTranslate_(I(4, 4)),
                globalScale_(I(4, 4)),
                globalRotate_(I(4, 4)),
                globalTranslate_(I(4, 4)) {
}

Shape::Shape(const vector& pos) : Shape() {
  setPosition(pos);
}

void Shape::setPosition(const vector& pos, Coords coord) {
  if (pos.size() < 3) {
    throw std::invalid_argument("invalid size vector");
  }

  if (coord == Coords::Global) {;
    for (int i = 0; i < 3; ++i) {
      globalTranslate_(i, 3) = pos[i];
    }
  } else if (coord == Coords::Local) {
    for (int i = 0; i < 3; ++i) {
      localTranslate_(i, 3) = pos[i];
    }
  } else {
    throw std::invalid_argument("Invalid coordinate system.");
  }
}
void Shape::translate(const vector& vec, Coords coord) {
  if (vec.size() < 3) {
    throw std::invalid_argument("invalid size vector");
  }

  if (coord == Coords::Global) {;
    globalTranslate_ = ublas::prod(globalTranslate_, getTranslationMatrix(vec));
  } else if (coord == Coords::Local) {
    localTranslate_ = ublas::prod(localTranslate_, getTranslationMatrix(vec));
  } else {
    throw std::invalid_argument("Invalid coordinate system.");
  }
}
void Shape::rotate(double angle, Axes axis, Coords coord) {
  if (coord == Coords::Global) {
    globalRotate(angle, axis);
  } else if (coord == Coords::Local) {
    localRotate(angle, axis);
  } else {
    throw std::invalid_argument("Invalid coordinate system.");
  }
}
void Shape::scale(double x, double y, double z, Coords coord) {
  if (coord == Coords::Global) {
    globalScale_ = ublas::prod(globalScale_, getScaleMatrix(x, y, z));
  } else if (coord == Coords::Local) {
    localScale_ = ublas::prod(localScale_, getScaleMatrix(x, y, z));
  } else {
    throw std::invalid_argument("Invalid coordinate system.");
  }
}

void Shape::localRotate(double angle, Axes axis) {
  switch (axis) {
    case Axes::Ox :
      localOxRotate(angle);
      return;
    case Axes::Oy :
      localOyRotate(angle);
      return;
    case Axes::Oz :
      localOzRotate(angle);
      return;
  }

  throw std::invalid_argument("Invalid axis.");
}
void Shape::globalRotate(double angle, Axes axis) {
  switch (axis) {
    case Axes::Ox :
      globalOxRotate(angle);
      return;
    case Axes::Oy :
      globalOyRotate(angle);
      return;
    case Axes::Oz :
      globalOzRotate(angle);
      return;
  }

  throw std::invalid_argument("Invalid axis.");
}

void Shape::localOxRotate(double angle) {
  localRotate_ = ublas::prod(localRotate_, getOxRotateMatrix(angle));
}
void Shape::localOyRotate(double angle) {
  localRotate_ = ublas::prod(localRotate_, getOyRotateMatrix(angle));
}
void Shape::localOzRotate(double angle) {
  localRotate_ = ublas::prod(localRotate_, getOzRotateMatrix(angle));
}
void Shape::globalOxRotate(double angle) {
  globalRotate_ = ublas::prod(globalRotate_, getOxRotateMatrix(angle));
}
void Shape::globalOyRotate(double angle) {
  globalRotate_ = ublas::prod(globalRotate_, getOyRotateMatrix(angle));
}
void Shape::globalOzRotate(double angle) {
  globalRotate_ = ublas::prod(globalRotate_, getOzRotateMatrix(angle));
}

Shape::matrix Shape::getTranslationMatrix(const vector& vec) {
  matrix T(4, 4);

  T <<=
      1, 0, 0, vec[0],
      0, 1, 0, vec[1],
      0, 0, 1, vec[2],
      0, 0, 0, 1;

  return T;
}
Shape::matrix Shape::getScaleMatrix(double x, double y, double z) {
  matrix S(4, 4);

  S <<=
      x, 0, 0, 0,
      0, y, 0, 0,
      0, 0, z, 0,
      0, 0, 0, 1;

  return S;
}
Shape::matrix Shape::getOxRotateMatrix(double angle) {
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
Shape::matrix Shape::getOyRotateMatrix(double angle) {
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
Shape::matrix Shape::getOzRotateMatrix(double angle) {
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
Shape::matrix Shape::M() {
  // M = gT * gR * gS * lT * lR * lS
  matrix res = ublas::prod(globalTranslate_, globalRotate_);
  res = ublas::prod(res, globalScale_);
  res = ublas::prod(res, localTranslate_);
  res = ublas::prod(res, localRotate_);
  res = ublas::prod(res, localScale_);
  return res;
}
