#ifndef MY3D_LIBS_MY3DLIB_SRC_RASTERIZER_H_
#define MY3D_LIBS_MY3DLIB_SRC_RASTERIZER_H_

#include <cstdint>

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/assignment.hpp"
#include "boost/numeric/ublas/io.hpp"

#include "FrameBuffer.h"
#include "Shape.h"
#include "Camera.h"
#include "Lens.h"
#include "Color.h"
#include "Pixel.h"

using namespace boost::numeric;

class Rasterizer {
  using matrix = ublas::matrix<double>;
  using vector = ublas::vector<double>;

 public:
  enum RenderingMode {
    Vertices,
    Edges,
    Facets
  };

  enum FillingMode {
    Colored,
    Depth
  };

  struct Triangle {
    vector v0;
    vector v1;
    vector v2;

    Color c{Color::White};
  };
  
  struct Line {
    Line(double px, double py, double pz, double nx, double ny, double nz) : pos(4), normal(4) {
      pos <<= px, py, pz, 1;
      normal <<= nx, ny, nz, 0;
    }

    vector pos;
    vector normal;
  };

  static const Line NDCtop_;
  static const Line NDCbottom_;
  static const Line NDCleft_;
  static const Line NDCright_;
  static const Line NDCnear_;


  Rasterizer(size_t width, size_t height);

  uint32_t* GetFrameBuffer();
  void DrawShape(const Shape& shape, const Camera& camera, const Lens& lens);
  void SetRenderingMode(const RenderingMode& rm);
  void SetFillingMode(const FillingMode& fm);
  void Clear();

 private:
  FrameBuffer<uint32_t> frame_buff_;
  FrameBuffer<double> depth_buff_;
  FrameBuffer<Pixel> triangle_sides_;

  RenderingMode render_mode_ = Facets;
  FillingMode fill_mode_ = Colored;

  int ClipTriangleWithLine (Triangle& t, int l, Triangle& out_t1, Triangle& out_t2);
  vector ClipSegmentWithLine(const vector& start, const vector& end, int l);
  Pixel NDC2Screen(const vector& v) const;

  void DrawShapeVertices(const Shape& shape, const Camera& camera, const Lens& lens);
  void DrawShapeEdges(const Shape& shape, const Camera& camera, const Lens& lens);
  void DrawShapeFacets(const Shape& shape, const Camera& camera, const Lens& lens);

  void DrawPixel(const Pixel& p);
  void DrawLine(const Pixel& p1, const Pixel& p2, const Color& color);
  void DrawTriangle(const Triangle& t);

  void SetPixelToTriangleSides(const Pixel& p);
  void SetLineToTriangleSides(const Pixel& p1, const Pixel& p2, const Color& color);

  void DrawLineImpl(const Pixel& p1, const Pixel& p2, const Color& color, bool triangle_outline_mode);

  void ClearFrameBuffer();
  void ClearDepthBuffer();
  void ClearTriangleSides();
};

#endif //MY3D_LIBS_MY3DLIB_SRC_RASTERIZER_H_
