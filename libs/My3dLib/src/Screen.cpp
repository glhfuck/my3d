#include "../include/Screen.h"
#include <cmath>

Screen::Screen(double fov,
               double ratio,
               double near,
               double far): P(4,4) {
  double rad_fov = fov * M_PI / 180;
  double tan_fov = std::tan(rad_fov);

  double w = 1 / tan_fov;
  double h = w / ratio;

  // z_res = (a*z + b)/z
  double a = far / (far - near);
  double b = - far * near / (far - near);

  P <<=
      w, 0, 0, 0,
      0, a, 0, b,
      0, 0, h, 0,
      0, 1, 0, 0;
}
