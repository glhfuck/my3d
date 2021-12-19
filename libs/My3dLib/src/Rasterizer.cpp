#include "../include/Rasterizer.h"

Rasterizer::Rasterizer(size_t width, size_t heigth):
framebuff_(heigth, width),
depthbuff_(heigth, width) {

}

void Rasterizer::rasterize(const Shape& shape, const Camera& camera, const Screen& screen) {
  ublas::matrix<double> MV = ublas::prod(camera.V, shape.M());
  ublas::matrix<double> MVP = ublas::prod(screen.P, MV);

  std::vector<ublas::vector<double>> v_res;
  v_res.reserve(shape.vertices.size());

  for (auto& vertex : shape.vertices) {
    ublas::vector<double> res = ublas::prod(MVP, vertex);
    for (int i = 0; i < 3; ++i) {
      res[i] /= -res[3];
    }

    size_t x = ((res[0] + 1) / 2 * framebuff_.MAX_COLUMN_INDEX);
    size_t y = ((res[2] + 1) / 2 * framebuff_.MAX_ROW_INDEX);

    auto buffer = framebuff_.getBuffer();
    buffer[y][x] = static_cast<uint32_t>(Color::White);
  }
}
uint32_t* Rasterizer::getFramebuffer() {
  return framebuff_.getData();
}
