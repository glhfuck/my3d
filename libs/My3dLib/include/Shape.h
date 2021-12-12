#ifndef MY3D_SRC_SHAPE_H_
#define MY3D_SRC_SHAPE_H_

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/assignment.hpp"
#include "boost/numeric/ublas/io.hpp"


class Shape {
  using matrix = boost::numeric::ublas::matrix<double>;
  using vector = boost::numeric::ublas::vector<double>;

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

  Shape();
  explicit Shape(const vector& pos);

  void setPosition(const vector& pos, Coords coord = Coords::Global);
  void translate(const vector& vec, Coords coord = Coords::Global);
  void rotate(double angle, Axes axis, Coords coord);
  void scale(double x, double y, double z, Coords coord = Coords::Local);

  std::vector<vector> vertices_;
  std::vector<vector> normals_;
  matrix M();
 protected:
  // TODO Maybe add pos?

 private:
  matrix localScale_;
  matrix localRotate_;
  matrix localTranslate_;
  matrix globalScale_;
  matrix globalRotate_;
  matrix globalTranslate_;

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