#ifndef MY3D_LIBS_MYRASTER_SRC_RASTERIZER_H_
#define MY3D_LIBS_MYRASTER_SRC_RASTERIZER_H_

#include <cstdint>
#include <vector>

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/assignment.hpp"
#include "boost/numeric/ublas/io.hpp"

#include "../include/Framebuffer.h"
#include "../include/Shape.h"
#include "../include/Camera.h"
#include "../include/Screen.h"
#include "../include/Color.h"

using namespace boost::numeric;

class Rasterizer {
 public:
  using matrix = ublas::matrix<double>;
  using vector = ublas::vector<double>;

  Rasterizer(size_t width, size_t heigth);

  void rasterize(const Shape& shape, const Camera& camera, const Screen& screen);
  uint32_t* getFramebuffer();
 private:
  Framebuffer<uint32_t> framebuff_;
  Framebuffer<double> depthbuff_;
};

#endif //MY3D_LIBS_MYRASTER_SRC_RASTERIZER_H_
