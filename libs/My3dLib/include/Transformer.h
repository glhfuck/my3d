#ifndef MY3D_LIBS_MY3DLIB_SRC_TRANSFORMER_H_
#define MY3D_LIBS_MY3DLIB_SRC_TRANSFORMER_H_

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/assignment.hpp"
#include "boost/numeric/ublas/io.hpp"

using namespace boost::numeric;

class Transformer {
  using matrix = ublas::matrix<double>;
  using vector = ublas::vector<double>;

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

  Transformer();

  void setPosition(const vector& pos);
  void translate(const vector& vec);
  void rotate(double angle, Axes axis);
  void scale(double x, double y, double z);

  matrix getResultMatrix() const;

 private:
  matrix scale_;
  matrix rotate_;
  matrix translate_;

  static matrix getTranslationMatrix(const vector& vec);
  static matrix getOxRotateMatrix(double angle);
  static matrix getOyRotateMatrix(double angle);
  static matrix getOzRotateMatrix(double angle);
  static matrix getScaleMatrix(double x, double y, double z);
};

#endif //MY3D_LIBS_MY3DLIB_SRC_TRANSFORMER_H_