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
    VertexInfo v_info[3] = {0};
  };

  [[nodiscard]] matrix M() const;

  void SetPosition(double x, double y, double z, Coords coord);
  void Translate(double delta_x, double delta_y, double delta_z, Coords coord);
  void SetRotate(double aroundOx, double aroundOy, double aroundOz, Coords coord);
  void Rotate(double aroundOx, double aroundOy, double aroundOz, Coords coord);
  void SetScale(double x, double y, double z, Coords coord);
  void Scale(double x, double y, double z, Coords coord);

  std::vector<vector> vertices;
  std::vector<vector> normals;
  std::vector<Facet> facets;

 private:
  Transformer global_transformer_;
  Transformer local_transformer_;
};

#endif //MY3D_LIBS_MY3DLIB_SRC_SHAPE_H_
