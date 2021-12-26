#ifndef MY3D_SRC_SCREEN_H_
#define MY3D_SRC_SCREEN_H_

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/assignment.hpp"
#include "boost/numeric/ublas/io.hpp"

using namespace boost::numeric;

using matrix = ublas::matrix<double>;
using vector = ublas::vector<double>;
using zero_vector = ublas::zero_vector<double>;

class Lens {
 public:
  explicit Lens(double fov = 75,
                  double ratio = 1,
                  double near = 1,
                  double far = 100);
 public:
  matrix P;
};

#endif //MY3D_SRC_SCREEN_H_
