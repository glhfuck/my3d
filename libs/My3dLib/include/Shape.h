#ifndef MY3D_SRC_SHAPE_H_
#define MY3D_SRC_SHAPE_H_

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/assignment.hpp"
#include "boost/numeric/ublas/io.hpp"

using namespace boost::numeric;

class Shape {
 public:
  explicit Shape(const ublas::vector<double>& pos = ublas::zero_vector<double>(3));
  void setModelMatrix(const ublas::vector<double>& pos);

  void localOxRotate(double angle);
  void localOyRotate(double angle);
  void localOzRotate(double angle);

  void globalOxRotate(double angle);
  void globalOyRotate(double angle);
  void globalOzRotate(double angle);

 protected:
  ublas::matrix<double> M;
  std::vector<ublas::vector<double>> vertices_;

  static ublas::matrix<double> getOxRotateMatrix(double angle);
  static ublas::matrix<double> getOyRotateMatrix(double angle);
  static ublas::matrix<double> getOzRotateMatrix(double angle);
};

#endif //MY3D_SRC_SHAPE_H_