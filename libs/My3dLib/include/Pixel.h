#ifndef MY3D_LIBS_MY3DLIB_SRC_POINT_H_
#define MY3D_LIBS_MY3DLIB_SRC_POINT_H_

#include "../include/Color.h"

struct Pixel {
  int x = 0;
  int y = 0;
  double depth = -1.0;
  Color color = Color::White;
};

#endif //MY3D_LIBS_MY3DLIB_SRC_POINT_H_
