#include "../include/Screen.h"
#include <cmath>

Screen::Screen(double fov,
               double ratio,
               double near,
               double far): P(4,4) {
  double rad_fov = fov * M_PI / 180;
  rad_fov /= 2;
  double tan_fov = std::tan(rad_fov);

  double w = tan_fov;
  double h = w / ratio;

  // y_res = (a*y + b)/y
  double a = far / (far - near);
  double b = - far * near / (far - near);

  P <<=
      1/w, 0, 0, 0,
      0, a, 0, b,
      0, 0, 1/h, 0,
      0, 1, 0, 0;
}
