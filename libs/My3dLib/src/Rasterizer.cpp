#include "Rasterizer.h"

#include <deque>

#include <cmath>
#include <cstring>


#include "SDL2/SDL.h"

bool INSIDE = false;
extern bool BACKFACECULL;

extern SDL_Renderer *renderer;
extern SDL_Texture *texture;

const Rasterizer::Line Rasterizer::NDCtop_ = Line(0, 0, 1, 0, 0, -1); // Y смотрит нахуй вперед а не вверх нужно по Z вертеть
const Rasterizer::Line Rasterizer::NDCleft_ = Line(-1, 0, 0, 1, 0, 0);
const Rasterizer::Line Rasterizer::NDCright_ = Line(1, 0, 0, -1, 0, 0);
const Rasterizer::Line Rasterizer::NDCbottom_ = Line(0, 0, -1, 0, 0, 1);
const Rasterizer::Line Rasterizer::NDCnear_ = Line(0, -1, 0, 0, 1, 0);

Rasterizer::Rasterizer(size_t width, size_t height):
    frame_buff_(height, width),
    depth_buff_(height, width),
    triangle_sides_(2, height),
    et_triangle_sides_(2, height) {
  ClearFrameBuffer();
  ClearDepthBuffer();
  ClearTriangleSides();
}

uint32_t* Rasterizer::GetFrameBuffer() {
  return frame_buff_.GetData();
}

void Rasterizer::Clear() {
  ClearDepthBuffer();
  ClearFrameBuffer();
}

void Rasterizer::DrawShape(const Shape& shape,
                           const Camera& camera,
                           const Lens& lens) {
  if (render_mode_ == RenderingMode::Vertices) {
    DrawShapeVertices(shape, camera, lens);
  } else if (render_mode_ == RenderingMode::Edges) {
    DrawShapeEdges(shape, camera, lens);
  } else if (render_mode_ == RenderingMode::Facets) {
    DrawShapeFacets(shape, camera, lens);
  } else {
    throw std::invalid_argument("Invalid rendering mode.");
  }
}

void Rasterizer::DrawShapeVertices(const Shape& shape, const Camera& camera, const Lens& lens) {
  ublas::matrix<double> MV = ublas::prod(camera.V(), shape.M());
  ublas::matrix<double> MVP = ublas::prod(lens.P(), MV);

  for (auto& vertex : shape.vertices) {
    ublas::vector<double> res = ublas::prod(MVP, vertex);
    for (int i = 0; i < 3; ++i) {
      res[i] /= res[3];
    }

    if (res[0] < -1 || res[0] > 1 ||
        res[2] < -1 || res[2] > 1) {
      continue;
    }

    int x = (res[0] + 1) / 2 * (frame_buff_.kCOLUMNS - 1);
    int y = (-res[2] + 1) / 2 * (frame_buff_.kROWS - 1);

    DrawPixel(Pixel{x, y, res[1], Color::White});
  }
}

void Rasterizer::DrawShapeEdges(const Shape& shape, const Camera& camera, const Lens& lens) {
  ublas::matrix<double> MV = ublas::prod(camera.V(), shape.M());
  ublas::matrix<double> MVP = ublas::prod(lens.P(), MV);

  for (auto& facet : shape.facets) {
    ublas::vector v0 = shape.vertices[facet.v_info[0].v_idx];
    ublas::vector v1 = shape.vertices[facet.v_info[1].v_idx];
    ublas::vector v2 = shape.vertices[facet.v_info[2].v_idx];

    ublas::vector<double> res0 = ublas::prod(MVP, v0);
    ublas::vector<double> res1 = ublas::prod(MVP, v1);
    ublas::vector<double> res2 = ublas::prod(MVP, v2);

    for (int i = 0; i < 3; ++i) {
      res0[i] /= res0[3];
      res1[i] /= res1[3];
      res2[i] /= res2[3];
    }

    if (res0[0] < -1 || res0[0] > 1 ||
        res0[2] < -1 || res0[2] > 1) {
      continue;
    }

    if (res1[0] < -1 || res1[0] > 1 ||
        res1[2] < -1 || res1[2] > 1) {
      continue;
    }

    if (res2[0] < -1 || res2[0] > 1 ||
        res2[2] < -1 || res2[2] > 1) {
      continue;
    }

    int x0 = (res0[0] + 1) / 2 * (frame_buff_.kCOLUMNS - 1);
    int y0 = (-res0[2] + 1) / 2 * (frame_buff_.kROWS - 1);
    double depth0 = res0[1];
    int x1 = (res1[0] + 1) / 2 * (frame_buff_.kCOLUMNS - 1);
    int y1 = (-res1[2] + 1) / 2 * (frame_buff_.kROWS - 1);
    double depth1 = res1[1];
    int x2 = (res2[0] + 1) / 2 * (frame_buff_.kCOLUMNS - 1);
    int y2 = (-res2[2] + 1) / 2 * (frame_buff_.kROWS - 1);
    double depth2 = res2[1];

    DrawLine(Pixel{x0, y0, depth0}, Pixel{x1, y1, depth1}, Color::White);
    DrawLine(Pixel{x1, y1, depth1}, Pixel{x2, y2, depth2}, Color::White);
    DrawLine(Pixel{x2, y2, depth2}, Pixel{x0, y0, depth0}, Color::White);
  }
}

void Rasterizer::DrawShapeFacets(const Shape& shape, const Camera& camera, const Lens& lens) {
  matrix M = shape.M();
  matrix MV = ublas::prod(camera.V(), M);
  matrix MVP = ublas::prod(lens.P(), MV);

  vector light(4);
  light <<= -1, -1, -1, 0;
  light /= ublas::norm_2(light);

  std::deque<Triangle> triangles;

  for (auto& facet : shape.facets) {
    Triangle t = {ublas::prod(MVP, shape.vertices[facet.v_info[0].v_idx]),
                  ublas::prod(MVP, shape.vertices[facet.v_info[1].v_idx]),
                  ublas::prod(MVP, shape.vertices[facet.v_info[2].v_idx])};

    vector facet_normal =
        shape.normals[facet.v_info[0].vn_idx] +
            shape.normals[facet.v_info[1].vn_idx] +
            shape.normals[facet.v_info[2].vn_idx];

    vector res_normal = ublas::prod(M, facet_normal);
    res_normal /= ublas::norm_2(res_normal);

    double prod = ublas::inner_prod(light, res_normal);
    Color res_color(100 + 100 * prod, 100 + 100 * prod, 100 + 100 * prod);

    t.c0 = res_color;
    t.c1 = res_color;
    t.c2 = res_color;

//    vector n0 = ublas::prod(M, shape.normals[facet.v_info[0].vn_idx]);
//    n0 /= ublas::norm_2(n0);
//    vector n1 = ublas::prod(M, shape.normals[facet.v_info[1].vn_idx]);
//    n1 /= ublas::norm_2(n1);
//    vector n2 = ublas::prod(M, shape.normals[facet.v_info[2].vn_idx]);
//    n2 /= ublas::norm_2(n2);
//
//    double p0 = ublas::inner_prod(light, n0);
//    double p1 = ublas::inner_prod(light, n1);
//    double p2 = ublas::inner_prod(light, n2);
//
//    t.c0 = Color(128 + 127 * p0, 128 + 127 * p0, 128 + 127 * p0);
//    t.c1 = Color(128 + 127 * p1, 128 + 127 * p1, 128 + 127 * p1);
//    t.c2 = Color(128 + 127 * p2, 128 + 127 * p2, 128 + 127 * p2);


    // Backface culling
    if (BACKFACECULL && (t.v1[0] * t.v0[3] - t.v0[0] * t.v1[3]) * (t.v2[2] * t.v0[3] - t.v0[2] * t.v2[3]) -
        (t.v1[2] * t.v0[3] - t.v0[2] * t.v1[3]) * (t.v2[0] * t.v0[3] - t.v0[0] * t.v2[3]) < 0) {
      continue;
    }

    Triangle clipped[2];
    int nNewTriangles = 1;
    triangles.clear();
    triangles.emplace_back(t);

    for (int p = 1; p < 8; p++)
    {
      int nTrisToAdd = 0;
      while (nNewTriangles > 0)
      {
        Triangle test = triangles.front();
        triangles.pop_front();
        nNewTriangles--;

        nTrisToAdd = ClipTriangleWithLine(test, p, clipped[0], clipped[1]);

        for (int w = 0; w < nTrisToAdd; w++) {
          triangles.emplace_back(clipped[w]);
        }
      }
      nNewTriangles = triangles.size();
    }

    for (auto& tri : triangles) {

      tri.v0 /= tri.v0[3];
      tri.v1 /= tri.v1[3];
      tri.v2 /= tri.v2[3];

//      DrawLine(NDC2Screen(tri.v0), NDC2Screen(tri.v1), Color::White);
//      DrawLine(NDC2Screen(tri.v1), NDC2Screen(tri.v2), Color::White);
//      DrawLine(NDC2Screen(tri.v2), NDC2Screen(tri.v0), Color::White);

      DrawTriangle(tri);
    }
  }

  //std::cout << "Drawn = " << Drawn << "     " << "Culled = " << Culled << std::endl;
}

void Rasterizer::DrawPixel(const Pixel& p) {
  if (p.x < 0 ||
      p.y < 0 ||
      p.x > frame_buff_.kCOLUMNS - 1 ||
      p.y > frame_buff_.kROWS - 1 ||
      p.depth < -1.0 ||
      p.depth > 1.0) {
    return;
  }

  auto db = depth_buff_.GetBuffer();
  auto fb = frame_buff_.GetBuffer();

  // Depth test
  if (db[p.y][p.x] < p.depth) {
    return;
  }

  db[p.y][p.x] = p.depth;

  if (fill_mode_ == FillingMode::Colored) {
    fb[p.y][p.x] = static_cast<uint32_t>(p.color);
  } else if (fill_mode_ == FillingMode::Depth) {
    uint8_t intensity = 255.0 * (p.depth + 1) / 2;
    Color color(intensity, intensity, intensity);
    fb[p.y][p.x] = static_cast<uint32_t>(color);
  } else {
    throw std::invalid_argument("Invalid filling mode.");
  }
}

void Rasterizer::DrawLine(const Pixel& p1, const Pixel& p2, const Color& color) {
  DrawLineImpl(p1, p2, color, false);
}

void Rasterizer::SetPixelToTriangleSides(const Pixel& p) {
  auto** to = triangle_sides_.GetBuffer();

  if (p.y < 0 || p.y > frame_buff_.kROWS - 1) {
    return;
  }

  if (p.x < to[0][p.y].x) {
    to[0][p.y] = Pixel{std::max(p.x, 0), p.y, p.depth, p.color};
  }

  if (p.x > to[1][p.y].x) {
    to[1][p.y] = Pixel{std::min(p.x, static_cast<int>(frame_buff_.kCOLUMNS) - 1), p.y, p.depth, p.color};
  }
}

void Rasterizer::SetLineToTriangleSides(const Pixel& p1, const Pixel& p2, const Color& color) {
  DrawLineImpl(p1, p2, color, true);
}

void Rasterizer::DrawLineImpl(const Pixel& p1, const Pixel& p2, const Color& color, bool triangle_outline_mode) {
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
      SetPixelToTriangleSides(Pixel{cur_x, cur_y, depth, color});
    } else {
      DrawPixel(Pixel{cur_x, cur_y, depth, color});
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

void Rasterizer::DrawTriangle(const Triangle& t) {
  //ClearTriangleSides();

//  for (int i = 0; i < triangle_sides_.kCOLUMNS; ++i) {
//    triangle_sides_.GetBuffer()[0][i].x = INT32_MAX;
//    //triangle_sides_.GetBuffer()[0][i].y = i;
//    triangle_sides_.GetBuffer()[0][i].depth = -1;
//
//    triangle_sides_.GetBuffer()[1][i].x = 0;
//    //triangle_sides_.GetBuffer()[1][i].y = i;
//    triangle_sides_.GetBuffer()[1][i].depth = -1;
//  }

  //memcpy(&triangle_sides_.GetBuffer()[0][0], &et_triangle_sides_.GetBuffer()[0][0], et_triangle_sides_.DATA_SIZE);

  SetLineToTriangleSides(NDC2Screen(t.v0), NDC2Screen(t.v1), t.c0);
  SetLineToTriangleSides(NDC2Screen(t.v1), NDC2Screen(t.v2), t.c0);
  SetLineToTriangleSides(NDC2Screen(t.v2), NDC2Screen(t.v0), t.c0);

  auto** to = triangle_sides_.GetBuffer();
  for (int y = 0; y < triangle_sides_.kCOLUMNS; ++y) {
    if (to[0][y].x <= to[1][y].x) {
      DrawLine(to[0][y], to[1][y], t.c0);
      triangle_sides_.GetBuffer()[0][y] = Pixel{INT32_MAX, y};
      triangle_sides_.GetBuffer()[1][y] = Pixel{0, y};
    }
  }



//  Pixel p0 = NDC2Screen(t.v0);
//  Pixel p1 = NDC2Screen(t.v1);
//  Pixel p2 = NDC2Screen(t.v2);
//
//  SDL_Vertex vert[3];
//
//  vert[0].position.x = p0.x;
//  vert[0].position.y = p0.y;
//  vert[0].color.r = t.c0.r;
//  vert[0].color.g = t.c0.g;
//  vert[0].color.b = t.c0.b;
//  vert[0].color.a = t.c0.a;
//
//  vert[1].position.x = p1.x;
//  vert[1].position.y = p1.y;
//  vert[1].color.r = t.c1.r;
//  vert[1].color.g = t.c1.g;
//  vert[1].color.b = t.c1.b;
//  vert[1].color.a = t.c1.a;
//
//  vert[2].position.x = p2.x;
//  vert[2].position.y = p2.y;
//  vert[2].color.r = t.c2.r;
//  vert[2].color.g = t.c2.g;
//  vert[2].color.b = t.c2.b;
//  vert[2].color.a = t.c2.a;
//
//  SDL_RenderGeometry(renderer, NULL, vert, 3, NULL, 0);
}

void Rasterizer::ClearDepthBuffer() {
  for (size_t row = 0; row < depth_buff_.kROWS; ++row) {
    for (size_t column = 0; column < depth_buff_.kCOLUMNS; ++column) {
      depth_buff_.GetBuffer()[row][column] = 1.0;
    }
  }
}

void Rasterizer::ClearFrameBuffer() {
  memset(frame_buff_.GetData(), 30, frame_buff_.DATA_SIZE);
}

void Rasterizer::ClearTriangleSides() {
  for (int i = 0; i < triangle_sides_.kCOLUMNS; ++i) {
    et_triangle_sides_.GetBuffer()[0][i] = Pixel{INT32_MAX, i};
    et_triangle_sides_.GetBuffer()[1][i] = Pixel{0, i};
  }
}

void Rasterizer::SetRenderingMode(const Rasterizer::RenderingMode& rm) {
  render_mode_ = rm;
}
void Rasterizer::SetFillingMode(const Rasterizer::FillingMode& fm) {
  fill_mode_ = fm;
}

Pixel Rasterizer::NDC2Screen(const vector& v) const {
  int x = (v[0] + 1) * ((frame_buff_.kCOLUMNS - 1) / 2);
  int y = (-v[2] + 1) * ((frame_buff_.kROWS - 1) / 2);
  double depth = v[1];
  return {x, y, depth};
}

int Rasterizer::ClipTriangleWithLine(Rasterizer::Triangle &t,
                                        int l,
                                        Rasterizer::Triangle &out_t1,
                                        Rasterizer::Triangle &out_t2) {
  // TODO: Эти значения можно напрямую использовать для опрделения пересечения с плоскостю
  // https://fabiensanglard.net/polygon_codec/clippingdocument/Clipping.pdf
//  double d0 = ublas::inner_prod(l.normal, t.v0 - l.pos);
//  double d1 = ublas::inner_prod(l.normal, t.v1 - l.pos);
//  double d2 = ublas::inner_prod(l.normal, t.v2 - l.pos);

  double d0 = 0;
  double d1 = 0;
  double d2 = 0;

  switch(l) {
    case 1: {
      d0 = t.v0[3];
      d1 = t.v1[3];
      d2 = t.v2[3];
      break;
    }
    case 6: {
      d0 = - t.v0[2] + t.v0[3];
      d1 = - t.v1[2] + t.v1[3];
      d2 = - t.v2[2] + t.v2[3];
      break;
    }
    case 7: {
      d0 = t.v0[2] + t.v0[3];
      d1 = t.v1[2] + t.v1[3];
      d2 = t.v2[2] + t.v2[3];
      break;
    }
    case 4: {
      d0 = t.v0[0] + t.v0[3];
      d1 = t.v1[0] + t.v1[3];
      d2 = t.v2[0] + t.v2[3];
      break;
    }
    case 5: {
      d0 = - t.v0[0] + t.v0[3];
      d1 = - t.v1[0] + t.v1[3];
      d2 = - t.v2[0] + t.v2[3];
      break;
    }
    case 2: {
      d0 = - t.v0[1] + t.v0[3];
      d1 = - t.v1[1] + t.v1[3];
      d2 = - t.v2[1] + t.v2[3];
      break;
    }
    case 3: {
      d0 = t.v0[1] + t.v0[3];
      d1 = t.v1[1] + t.v1[3];
      d2 = t.v2[1] + t.v2[3];
      break;
    }

    case 8: {
      throw std::runtime_error("Clip");
    }
  }

  int kInside = 0;
  int kOutside = 0;

  vector* inside_vertices[3] = {nullptr, nullptr, nullptr};
  vector* outside_vertices[3] = {nullptr, nullptr, nullptr};

  if (d0 > 0) {
    inside_vertices[kInside++] = &t.v0;
  } else {
    outside_vertices[kOutside++] = &t.v0;
  }

  if (d1 > 0) {
    inside_vertices[kInside++] = &t.v1;
  } else {
    outside_vertices[kOutside++] = &t.v1;
  }

  if (d2 > 0) {
    inside_vertices[kInside++] = &t.v2;
  } else {
    outside_vertices[kOutside++] = &t.v2;
  }

//  if (l == 2) {
//    std::cout << "     TOP: ";
//  } else if (l == 3) {
//    std::cout << "     BOT: ";
//  } else if (l == 4) {
//    std::cout << "     LEFT: ";
//  } else if (l == 5) {
//    std::cout << "     RIGHT: ";
//  }
//  std::cout << kInside << "  " << kOutside;
//
//  if (l == 5) {
//    std::cout << std::endl;
//  }

  if (kInside == 0) {
    return 0;
  }

  if (kInside == 3) {
    out_t1 = t;
    return 1;
  }

  if (kInside == 1 && kOutside == 2) {
    //std::cout << &t.v0 << "    " << &t.v1 << "      " << &t.v2 << std::endl;
    out_t1.v0 = *inside_vertices[0];
    out_t1.v1 = ClipSegmentWithLine(*inside_vertices[0], *outside_vertices[0], l);
    out_t1.v2 = ClipSegmentWithLine(*inside_vertices[0], *outside_vertices[1], l);
    out_t1.c0 = Color::Red;
    out_t1.c1 = Color::Red;
    out_t1.c2 = Color::Red;
//    out_t1.c = t.c;
    return 1;
  }

  if (kInside == 2 && kOutside == 1) {
    out_t1.v0 = *inside_vertices[0];
    out_t1.v1 = *inside_vertices[1];
    out_t1.v2 = ClipSegmentWithLine(*inside_vertices[0], *outside_vertices[0], l);
    out_t1.c0 = Color::Blue;
    out_t1.c1 = Color::Blue;
    out_t1.c2 = Color::Blue;
//    out_t1.c = t.c;

    out_t2.v0 = *inside_vertices[1];
    out_t2.v1 = ClipSegmentWithLine(*inside_vertices[1], *outside_vertices[0], l);
    out_t2.v2 = out_t1.v2;
    out_t2.c0 = Color::Green;
    out_t2.c1 = Color::Green;
    out_t2.c2 = Color::Green;
//    out_t2.c = t.c;

    return 2;
  }

  throw std::runtime_error("Clipping error");
}


vector Rasterizer::ClipSegmentWithLine(const vector& start, const vector& end, int l) {
  //std::cout << start.size() << "       " << end.size() << std::endl;
  vector vec = end - start;
  double t = 0;
//  double t = ublas::inner_prod(l.normal, l.pos - start)
//      / ublas::inner_prod(l.normal, vec);

  switch(l) {
    case 1: {
      t = start[3] / (start[3] - end[3]);
      break;
    }
    case 6: {
      t = (start[3] - start[2]) / ((start[3] - start[2]) - (end[3] - end[2]));
      break;
    }
    case 7: {
      t = (start[3] + start[2]) / ((start[3] + start[2]) - (end[3] + end[2]));
      break;
    }
    case 4: {
      t = (start[3] + start[0]) / ((start[3] + start[0]) - (end[3] + end[0]));
      break;
    }
    case 5: {
      t = (start[3] - start[0]) / ((start[3] - start[0]) - (end[3] - end[0]));
      break;
    }
    case 2: {
      t = (start[3] - start[1]) / ((start[3] - start[1]) - (end[3] - end[1]));
      break;
    }
    case 3: {
      t = (start[3] + start[1]) / ((start[3] + start[1]) - (end[3] + end[1]));
      break;
    }

    case 8: {
      throw std::runtime_error("Clip");
    }
  }

  return start + (vec * t);
}