#ifndef MY3D_LIBS_MY3DLIB_SRC_SHAPE_H_
#define MY3D_LIBS_MY3DLIB_SRC_SHAPE_H_

#include "Transformable.h"

class Shape: public Transformable {
  using vector = boost::numeric::ublas::vector<double>;

 public:
  Shape();
  explicit Shape(const vector& pos);

  struct Facet {
    struct VertexInfo {
      int v_idx;
      int vt_idx;
      int vn_idx;
    };
    VertexInfo verInfo[3] = {0};
  };

  std::vector<vector> vertices;
  std::vector<vector> normals;
  std::vector<Facet> facets;
};

#endif //MY3D_LIBS_MY3DLIB_SRC_SHAPE_H_
