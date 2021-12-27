#ifndef MY3D_LIBS_MY3DLIB_SRC_SHAPE_H_
#define MY3D_LIBS_MY3DLIB_SRC_SHAPE_H_

#include "Transformer.h"

class Shape {
  using matrix = boost::numeric::ublas::matrix<double>;
  using vector = boost::numeric::ublas::vector<double>;

 public:
  enum Coords {
    Global,
    Local
  };

  struct Facet {
    struct VertexInfo {
      int v_idx;
      int vt_idx;
      int vn_idx;
    };
    VertexInfo verInfo[3] = {0};
  };

  void setPosition(const vector& pos, Coords coord);
  void translate(const vector& vec, Coords coord);
  void rotate(double angle, Transformer::Axes axis, Coords coord);
  void scale(double x, double y, double z, Coords coord);

  matrix M() const;


  std::vector<vector> vertices;
  std::vector<vector> normals;
  std::vector<Facet> facets;

 private:
  Transformer global_transformer_;
  Transformer local_transformer_;
};

#endif //MY3D_LIBS_MY3DLIB_SRC_SHAPE_H_
