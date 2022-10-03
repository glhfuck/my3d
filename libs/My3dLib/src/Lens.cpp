#include "Lens.h"

#include <cmath>


Lens::Lens(double fov, double ratio, double near, double far) :
    fov_(fov * M_PI / 180), ratio_(ratio), near_(near), far_(far) {
}

matrix Lens::P() const {
  matrix P(4, 4);

  double w = std::tan(fov_ / 2);
  double h = w / ratio_;
  double a = far_ / (far_ - near_);
  double b = -far_ * near_ / (far_ - near_);

  P <<=
      1/w, 0, 0  , 0,
      0  , a, 0  , b,
      0  , 0, 1/h, 0,
      0  , 1, 0  , 0;

  return P;
}
void Lens::SetFOV(double new_fov) {
  fov_ = new_fov * M_PI / 180;
}
