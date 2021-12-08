#ifndef MY3D_SRC_SCREEN_H_
#define MY3D_SRC_SCREEN_H_

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/assignment.hpp"
#include "boost/numeric/ublas/io.hpp"

using namespace boost::numeric;

class Screen {
 public:
  explicit Screen(double fov = 75,
                  double ratio = 1,
                  double near = 1,
                  double far = 100);
 private:
  ublas::matrix<double> P;
};

#endif //MY3D_SRC_SCREEN_H_
