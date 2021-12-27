#include "../include/Rasterizer.h"
#include <cmath>

Rasterizer::Rasterizer(size_t width, size_t height):
    frame_buff_(height, width),
    depth_buff_(height, width),
    triangular_outline_(2, height) {
  clearFrameBuffer();
  clearDepthBuffer();
}

uint32_t* Rasterizer::getFrameBuffer() {
  return frame_buff_.getData();
}

void Rasterizer::clear() {
  clearDepthBuffer();
  clearFrameBuffer();
}

void Rasterizer::drawShape(const Shape& shape,
                           const Camera& camera,
                           const Lens& lens) {
  if (rendering_mode_ == RenderingMode::Vertices) {
    drawShapeVertices(shape, camera, lens);
  } else if (rendering_mode_ == RenderingMode::Edges) {
    drawShapeEdges(shape, camera, lens);
  } else if (rendering_mode_ == RenderingMode::Facets) {
    drawShapeFacets(shape, camera, lens);
  } else {
    throw std::invalid_argument("Invalid rendering mode.");
  }
}

void Rasterizer::drawShapeVertices(const Shape& shape, const Camera& camera, const Lens& lens) {
  ublas::matrix<double> MV = ublas::prod(camera.V, shape.M());
  ublas::matrix<double> MVP = ublas::prod(lens.P, MV);

  for (auto& vertex : shape.vertices) {
    ublas::vector<double> res = ublas::prod(MVP, vertex);
    for (int i = 0; i < 3; ++i) {
      res[i] /= res[3];
    }

    int x = (-res[0] + 1) / 2 * (frame_buff_.COLUMNS_COUNT - 1);
    int y = (-res[2] + 1) / 2 * (frame_buff_.ROWS_COUNT - 1);

    drawPoint(Point{x, y, -1.0, Color::White});
  }
}

void Rasterizer::drawShapeEdges(const Shape& shape, const Camera& camera, const Lens& lens) {
  ublas::matrix<double> MV = ublas::prod(camera.V, shape.M());
  ublas::matrix<double> MVP = ublas::prod(lens.P, MV);

  for (auto& facet : shape.facets) {
    ublas::vector v0 = shape.vertices[facet.verInfo[0].v_idx];
    ublas::vector v1 = shape.vertices[facet.verInfo[1].v_idx];
    ublas::vector v2 = shape.vertices[facet.verInfo[2].v_idx];

    ublas::vector<double> res0 = ublas::prod(MVP, v0);
    ublas::vector<double> res1 = ublas::prod(MVP, v1);
    ublas::vector<double> res2 = ublas::prod(MVP, v2);

    for (int i = 0; i < 3; ++i) {
      res0[i] /= res0[3];
      res1[i] /= res1[3];
      res2[i] /= res2[3];
    }

    int x0 = (-res0[0] + 1) / 2 * (frame_buff_.COLUMNS_COUNT - 1);
    int y0 = (-res0[2] + 1) / 2 * (frame_buff_.ROWS_COUNT - 1);
    int x1 = (-res1[0] + 1) / 2 * (frame_buff_.COLUMNS_COUNT - 1);
    int y1 = (-res1[2] + 1) / 2 * (frame_buff_.ROWS_COUNT - 1);
    int x2 = (-res2[0] + 1) / 2 * (frame_buff_.COLUMNS_COUNT - 1);
    int y2 = (-res2[2] + 1) / 2 * (frame_buff_.ROWS_COUNT - 1);

    drawLine(Point{x0, y0}, Point{x1, y1}, Color::White);
    drawLine(Point{x1, y1}, Point{x2, y2}, Color::White);
    drawLine(Point{x2, y2}, Point{x0, y0}, Color::White);
  }
}

void Rasterizer::drawShapeFacets(const Shape& shape, const Camera& camera, const Lens& lens) {
  ublas::matrix<double> MV = ublas::prod(camera.V, shape.M());
  ublas::matrix<double> MVP = ublas::prod(lens.P, MV);

  std::vector<ublas::vector<double>> v_res;
  v_res.reserve(shape.vertices.size());

  ublas::vector<double> light(4);
  light <<= 1, 1, 1, 0;
  light /= ublas::norm_2(light);

  for (auto& facet : shape.facets) {
    ublas::vector v0 = shape.vertices[facet.verInfo[0].v_idx];
    ublas::vector v1 = shape.vertices[facet.verInfo[1].v_idx];
    ublas::vector v2 = shape.vertices[facet.verInfo[2].v_idx];

    ublas::vector<double> res0 = ublas::prod(MVP, v0);
    ublas::vector<double> res1 = ublas::prod(MVP, v1);
    ublas::vector<double> res2 = ublas::prod(MVP, v2);

    for (int i = 0; i < 3; ++i) {
      res0[i] /= res0[3];
      res1[i] /= res1[3];
      res2[i] /= res2[3];
    }

    // Backface culling
    if ((res1[0] - res0[0]) * (res2[2] - res0[2]) -
        (res1[2] - res0[2]) * (res2[0] - res0[0]) > 0) {
      continue;
    }

    int x0 = (-res0[0] + 1) / 2 * (frame_buff_.COLUMNS_COUNT - 1);
    int y0 = (-res0[2] + 1) / 2 * (frame_buff_.ROWS_COUNT - 1);
    double depth0 = res0[1];
    int x1 = (-res1[0] + 1) / 2 * (frame_buff_.COLUMNS_COUNT - 1);
    int y1 = (-res1[2] + 1) / 2 * (frame_buff_.ROWS_COUNT - 1);
    double depth1 = res1[1];
    int x2 = (-res2[0] + 1) / 2 * (frame_buff_.COLUMNS_COUNT - 1);
    int y2 = (-res2[2] + 1) / 2 * (frame_buff_.ROWS_COUNT - 1);
    double depth2 = res2[1];


    ublas::vector<double> facet_normal =
        shape.normals[facet.verInfo[0].vn_idx] +
        shape.normals[facet.verInfo[1].vn_idx] +
        shape.normals[facet.verInfo[2].vn_idx];

    ublas::vector<double> res_normal = ublas::prod(MV, facet_normal);
    res_normal /= ublas::norm_2(res_normal);

    double prod = ublas::inner_prod(light, res_normal);
    Color res_color(155 + 100 * prod,
                    155 + 100 * prod,
                    155 + 100 * prod);

    drawTriangle(Point{x0, y0, depth0},
                 Point{x1, y1, depth1},
                 Point{x2, y2, depth2},
                 res_color);
  }
}

void Rasterizer::drawPoint(const Point& p) {
  if (p.x < 0 ||
      p.y < 0 ||
      p.x > frame_buff_.COLUMNS_COUNT - 1 ||
      p.y > frame_buff_.ROWS_COUNT - 1 ||
      p.depth < -1.0 ||
      p.depth > 1.0) {
    return;
  }

  // Depth test
  if (depth_buff_.getBuffer()[p.y][p.x] <= p.depth) {
    return;
  }

  depth_buff_.getBuffer()[p.y][p.x] = p.depth;

  if (filling_mode_ == FillingMode::Colored) {
    frame_buff_.getBuffer()[p.y][p.x] = static_cast<uint32_t>(p.color);
  } else if (filling_mode_ == FillingMode::Depth) {
    uint8_t intensity = 255.0 * (p.depth + 1) / 2;
    Color color(intensity, intensity, intensity);
    frame_buff_.getBuffer()[p.y][p.x] = static_cast<uint32_t>(color);
  } else {
    throw std::invalid_argument("Invalid filling mode.");
  }
}

void Rasterizer::drawLine(const Point& p1, const Point& p2, const Color& color) {
  drawLineImpl(p1, p2, color, false);
}

void Rasterizer::setPointToTriangleOutline(const Point& p) {
  auto** to = triangular_outline_.getBuffer();

  if (p.y < 0 || p.y > frame_buff_.ROWS_COUNT - 1) {
    return;
  }

  if (p.x < to[0][p.y].x) {
    to[0][p.y] = Point{std::max(p.x, 0), p.y, p.depth, p.color};
  }

  if (p.x > to[1][p.y].x) {
    to[1][p.y] = Point{std::min(p.x, static_cast<int>(frame_buff_.COLUMNS_COUNT) - 1), p.y, p.depth, p.color};
  }
}

void Rasterizer::setEdgeToTriangleOutline(const Point& p1, const Point& p2, const Color& color) {
  drawLineImpl(p1, p2, color, true);
}

void Rasterizer::drawLineImpl(const Point& p1, const Point& p2, const Color& color, bool triangle_outline_mode) {
  int x = p1.x;
  int y = p1.y;
  int dx = abs(p2.x - p1.x);
  int dy = abs(p2.y - p1.y);
  int sx = p1.x < p2.x ? 1 : -1;
  int sy = p1.y < p2.y ? 1 : -1;

  bool inverted = dy > dx;

  if (inverted) {
    std::swap (x, y);
    std::swap (dx, dy);
    std::swap (sx, sy);
  }

  double depth = p1.depth;
  double dDepth = p2.depth - p1.depth;
  double sDepth = dDepth / dx;

  int deviation = 2 * dy - dx;

  for (int i = 0; i <= dx; ++i) {
    int cur_x;
    int cur_y;

    if (inverted) {
      cur_x = y;
      cur_y = x;
    } else {
      cur_x = x;
      cur_y = y;
    }

    if (triangle_outline_mode) {
      setPointToTriangleOutline(Point{cur_x, cur_y, depth, color});
    } else {
      drawPoint(Point{cur_x, cur_y, depth, color});
    }

    if (deviation > 0) {
      y += sy;
      deviation += 2 * dy - 2 * dx;
    } else {
      deviation += 2 * dy;
    }

    depth += sDepth;
    x += sx;
  }
}

void Rasterizer::drawTriangle(const Point& p1, const Point& p2, const Point& p3, const Color& color) {
  clearTriangularOutline();

  setEdgeToTriangleOutline(p1, p2, color);
  setEdgeToTriangleOutline(p2, p3, color);
  setEdgeToTriangleOutline(p3, p1, color);

  auto** to = triangular_outline_.getBuffer();
  for (int y = 0; y < triangular_outline_.COLUMNS_COUNT; ++y) {
    if (to[0][y].x <= to[1][y].x) {
      drawLine(to[0][y], to[1][y], color);
    }
  }
}

void Rasterizer::clearDepthBuffer() {
  for (size_t row = 0; row < depth_buff_.ROWS_COUNT; ++row) {
    for (size_t column = 0; column < depth_buff_.COLUMNS_COUNT; ++column) {
      depth_buff_.getBuffer()[row][column] = 1.0;
    }
  }
}

void Rasterizer::clearFrameBuffer() {
  memset(frame_buff_.getData(),0, frame_buff_.DATA_SIZE);
}

void Rasterizer::clearTriangularOutline() {
  for (int i = 0; i < triangular_outline_.COLUMNS_COUNT; ++i) {
    triangular_outline_.getBuffer()[0][i] = Point{INT32_MAX, i};
  }

  for (int i = 0; i < triangular_outline_.COLUMNS_COUNT; ++i) {
    triangular_outline_.getBuffer()[1][i] = Point{0, i};
  }
}

void Rasterizer::setRenderingMode(const Rasterizer::RenderingMode& rm) {
  rendering_mode_ = rm;
}
void Rasterizer::setFillingMode(const Rasterizer::FillingMode& fm) {
  filling_mode_ = fm;
}
