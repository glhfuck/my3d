#ifndef MY3D_LIBS_MY3DLIB_SRC_COLOR_H_
#define MY3D_LIBS_MY3DLIB_SRC_COLOR_H_

#include <cstdint>

struct Color {
  uint8_t b;
  uint8_t g;
  uint8_t r;
  uint8_t a;

  Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0);
  explicit operator uint32_t() const;

  static const Color Red;
  static const Color Green;
  static const Color Blue;
  static const Color Yellow;
  static const Color Pink;
  static const Color Cyan;
  static const Color White;
  static const Color Black;
};

#endif //MY3D_LIBS_MY3DLIB_SRC_COLOR_H_
