#include "invert.h"
#include "Camera.h"

using namespace boost::numeric;

Camera::Camera(const vector& pos,
               const vector& look_at,
               const vector& up) : V(4, 4) {
  vector y = look_at - pos;
  y /= ublas::norm_2(y);

  vector x(3);
  x <<=
      y[2] * up[1] - y[1] * up[2],
      y[0] * up[2] - y[2] * up[0],
      y[1] * up[0] - y[0] * up[1];
  x /= ublas::norm_2(x);

  vector z(3);
  z <<=
      x[2] * y[1] - x[1] * y[2],
      x[0] * y[2] - x[2] * y[0],
      x[1] * y[0] - x[0] * y[1];

  V <<=
      x[0], y[0], z[0], pos[0],
      x[1], y[1], z[1], pos[1],
      x[2], y[2], z[2], pos[2],
      0, 0, 0, 1;

  bool flag = false;
  V = gjinverse(V, flag);
}

