#ifndef MY3D_LIBS_MY3DLIB_SRC_SHAPE_H_
#define MY3D_LIBS_MY3DLIB_SRC_SHAPE_H_

#include "Transformable.h"

class Shape: public Transformable {
  using vector = boost::numeric::ublas::vector<double>;

 public:

  struct Facet {
    size_t vertex_idx[3];
    size_t normal_idx[3];
  };

  std::vector<vector> vertices_;
  std::vector<vector> normals_;
  std::vector<Facet> facets_;
};

#endif //MY3D_LIBS_MY3DLIB_SRC_SHAPE_H_
