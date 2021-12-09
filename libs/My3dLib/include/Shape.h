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

 protected:
  ublas::matrix<double> M;
  std::vector<ublas::vector<double>> vertices_;

  // TODO rotate matrices
};

#endif //MY3D_SRC_SHAPE_H_