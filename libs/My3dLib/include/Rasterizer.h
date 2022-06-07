#ifndef MY3D_LIBS_MY3DLIB_SRC_RASTERIZER_H_
#define MY3D_LIBS_MY3DLIB_SRC_RASTERIZER_H_

#include <cstdint>

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/assignment.hpp"
#include "boost/numeric/ublas/io.hpp"

#include "../include/FrameBuffer.h"
#include "../include/Shape.h"
#include "../include/Camera.h"
#include "../include/Lens.h"
#include "../include/Color.h"
#include "../include/Pixel.h"

using namespace boost::numeric;

class Rasterizer {
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

  Rasterizer(size_t width, size_t height);

  uint32_t* GetFrameBuffer();
  void DrawShape(const Shape& shape, const Camera& camera, const Lens& lens);
  void SetRenderingMode(const RenderingMode& rm);
  void SetFillingMode(const FillingMode& fm);
  void Clear();

 private:
  FrameBuffer<uint32_t> frame_buff_;
  FrameBuffer<double> depth_buff_;
  FrameBuffer<Pixel> triangular_outline_;

  RenderingMode rendering_mode_ = Facets;
  FillingMode filling_mode_ = Colored;

  void DrawShapeVertices(const Shape& shape, const Camera& camera, const Lens& lens);
  void DrawShapeEdges(const Shape& shape, const Camera& camera, const Lens& lens);
  void DrawShapeFacets(const Shape& shape, const Camera& camera, const Lens& lens);

  void DrawPixel(const Pixel& p);
  void DrawLine(const Pixel& p1, const Pixel& p2, const Color& color);
  void DrawTriangle(const Pixel& p1, const Pixel& p2, const Pixel& p3, const Color& color);

  void SetPixelToTriangleOutline(const Pixel& p);
  void SetEdgeToTriangleOutline(const Pixel& p1, const Pixel& p2, const Color& color);

  void DrawLineImpl(const Pixel& p1, const Pixel& p2, const Color& color, bool triangle_outline_mode);

  void ClearFrameBuffer();
  void ClearDepthBuffer();
  void ClearTriangularOutline();
};

#endif //MY3D_LIBS_MY3DLIB_SRC_RASTERIZER_H_
