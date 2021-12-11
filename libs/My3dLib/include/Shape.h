#ifndef MY3D_SRC_SHAPE_H_
#define MY3D_SRC_SHAPE_H_

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/assignment.hpp"
#include "boost/numeric/ublas/io.hpp"

using namespace boost::numeric;

using matrix = ublas::matrix<double>;
using vector = ublas::vector<double>;
using zero_vector = ublas::zero_vector<double>;

class Shape {
 public:
  enum Axes {
    Ox,
    Oy,
    Oz
  };

  enum Coords {
    Global,
    Local
  };

  explicit Shape(const vector& pos = zero_vector(3));

  void setPosition(const vector& pos);
  void translate(const vector& vec);
  void rotate(double angle, Axes axis, Coords coord);
  void scale(double x, double y, double z);

 protected:
  matrix M;
  size_t vertices_count_ = 0;
  std::vector<vector> vertices_;

  void localRotate(double angle, Axes axis);
  void globalRotate(double angle, Axes axis);

  void localOxRotate(double angle);
  void localOyRotate(double angle);
  void localOzRotate(double angle);

  void globalOxRotate(double angle);
  void globalOyRotate(double angle);
  void globalOzRotate(double angle);

  static matrix getTranslationMatrix(const vector& vec);

  static matrix getScaleMatrix(double x, double y, double z);

  static matrix getOxRotateMatrix(double angle);
  static matrix getOyRotateMatrix(double angle);
  static matrix getOzRotateMatrix(double angle);
};

#endif //MY3D_SRC_SHAPE_H_