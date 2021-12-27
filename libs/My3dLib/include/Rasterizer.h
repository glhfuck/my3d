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
#include "../include/Point.h"

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

  void drawShape(const Shape& shape,
            const Camera& camera,
            const Lens& lens);

  void drawShapeVertices(const Shape& shape,
                         const Camera& camera,
                         const Lens& lens);

  void drawShapeEdges(const Shape& shape,
                      const Camera& camera,
                      const Lens& lens);

  void drawShapeFacets(const Shape& shape,
                       const Camera& camera,
                       const Lens& lens);

  void setRenderingMode(const RenderingMode& rm);
  void setFillingMode(const FillingMode& fm);
  uint32_t* getFrameBuffer();
  void clear();

 private:
  RenderingMode rendering_mode_ = Facets;
  FillingMode filling_mode_ = Colored;

  FrameBuffer<uint32_t> frame_buff_;
  FrameBuffer<double> depth_buff_;
  FrameBuffer<Point> triangular_outline_;

  void drawPoint(const Point& p);
  void drawLine(const Point& p1, const Point& p2, const Color& color);
  void drawTriangle(const Point& p1, const Point& p2, const Point& p3, const Color& color);

  void setPointToTriangleOutline(const Point& p);
  void setEdgeToTriangleOutline(const Point& p1, const Point& p2, const Color& color);

  void drawLineImpl(const Point& p1, const Point& p2, const Color& color, bool triangle_outline_mode);

  void clearFrameBuffer();
  void clearDepthBuffer();
  void clearTriangularOutline();
};

#endif //MY3D_LIBS_MY3DLIB_SRC_RASTERIZER_H_
