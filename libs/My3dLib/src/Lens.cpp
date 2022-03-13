#include "../include/Lens.h"
#include <cmath>

Lens::Lens(double fov, double ratio, double near, double far):
    fov(fov * M_PI / 180), ratio(ratio), near(near), far(far) {
}

matrix Lens::P() const {
  matrix P(4, 4);

  double w = std::tan(fov / 2);
  double h = w / ratio;
  double a = far / (far - near);
  double b = - far * near / (far - near);

  P <<=
      1/w, 0, 0, 0,
      0, a, 0, b,
      0, 0, 1/h, 0,
      0, 1, 0, 0;

  return P;
}
void Lens::SetFOV(double new_fov) {
  fov = new_fov * M_PI / 180;
}
