#include "Color.h"


Color::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a): r(r), g(g), b(b), a(a) {
}

Color::operator uint32_t() const {
  uint32_t res = 0;

  res |= (a << 24);
  res |= (r << 16);
  res |= (g << 8);
  res |= b;

  return res;
}

const Color Color::Red = Color(255, 0, 0);
const Color Color::Green = Color(0, 255, 0);
const Color Color::Blue = Color(0, 0, 255);
const Color Color::Yellow = Color(255, 255, 0);
const Color Color::Pink = Color(255, 0, 255);
const Color Color::Cyan = Color(0, 255, 255);
const Color Color::White = Color(255, 255, 255);
const Color Color::Black = Color(0, 0, 0);
