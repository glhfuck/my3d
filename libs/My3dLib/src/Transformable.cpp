#include "Transformable.h"

using namespace boost::numeric;
using I = ublas::identity_matrix<double>;

Transformable::Transformable(): localScale_(I(4, 4)),
                localRotate_(I(4, 4)),
                localTranslate_(I(4, 4)),
                globalScale_(I(4, 4)),
                globalRotate_(I(4, 4)),
                globalTranslate_(I(4, 4)) {
}

Transformable::Transformable(const vector& pos) : Transformable() {
  setPosition(pos);
}

void Transformable::setPosition(const vector& pos, Coords coord) {
  if (pos.size() < 3) {
    throw std::invalid_argument("Invalid size vector");
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
void Transformable::translate(const vector& vec, Coords coord) {
  if (vec.size() < 3) {
    throw std::invalid_argument("Invalid size vector");
  }

  if (coord == Coords::Global) {;
    globalTranslate_ = ublas::prod(globalTranslate_, getTranslationMatrix(vec));
  } else if (coord == Coords::Local) {
    localTranslate_ = ublas::prod(localTranslate_, getTranslationMatrix(vec));
  } else {
    throw std::invalid_argument("Invalid coordinate system.");
  }
}
void Transformable::rotate(double angle, Axes axis, Coords coord) {
  if (coord == Coords::Global) {
    globalRotate(angle, axis);
  } else if (coord == Coords::Local) {
    localRotate(angle, axis);
  } else {
    throw std::invalid_argument("Invalid coordinate system.");
  }
}
void Transformable::scale(double x, double y, double z, Coords coord) {
  if (coord == Coords::Global) {
    globalScale_ = ublas::prod(globalScale_, getScaleMatrix(x, y, z));
  } else if (coord == Coords::Local) {
    localScale_ = ublas::prod(localScale_, getScaleMatrix(x, y, z));
  } else {
    throw std::invalid_argument("Invalid coordinate system.");
  }
}

void Transformable::localRotate(double angle, Axes axis) {
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
void Transformable::globalRotate(double angle, Axes axis) {
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

void Transformable::localOxRotate(double angle) {
  localRotate_ = ublas::prod(localRotate_, getOxRotateMatrix(angle));
}
void Transformable::localOyRotate(double angle) {
  localRotate_ = ublas::prod(localRotate_, getOyRotateMatrix(angle));
}
void Transformable::localOzRotate(double angle) {
  localRotate_ = ublas::prod(localRotate_, getOzRotateMatrix(angle));
}
void Transformable::globalOxRotate(double angle) {
  globalRotate_ = ublas::prod(globalRotate_, getOxRotateMatrix(angle));
}
void Transformable::globalOyRotate(double angle) {
  globalRotate_ = ublas::prod(globalRotate_, getOyRotateMatrix(angle));
}
void Transformable::globalOzRotate(double angle) {
  globalRotate_ = ublas::prod(globalRotate_, getOzRotateMatrix(angle));
}

Transformable::matrix Transformable::getTranslationMatrix(const vector& vec) {
  matrix T(4, 4);

  T <<=
      1, 0, 0, vec[0],
      0, 1, 0, vec[1],
      0, 0, 1, vec[2],
      0, 0, 0, 1;

  return T;
}
Transformable::matrix Transformable::getScaleMatrix(double x, double y, double z) {
  matrix S(4, 4);

  S <<=
      x, 0, 0, 0,
      0, y, 0, 0,
      0, 0, z, 0,
      0, 0, 0, 1;

  return S;
}
Transformable::matrix Transformable::getOxRotateMatrix(double angle) {
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
Transformable::matrix Transformable::getOyRotateMatrix(double angle) {
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
Transformable::matrix Transformable::getOzRotateMatrix(double angle) {
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
Transformable::matrix Transformable::M() {
  // M = gT * gR * gS * lT * lR * lS
  matrix res = ublas::prod(globalTranslate_, globalRotate_);
  res = ublas::prod(res, globalScale_);
  res = ublas::prod(res, localTranslate_);
  res = ublas::prod(res, localRotate_);
  res = ublas::prod(res, localScale_);
  return res;
}
