#ifndef MY3D_SRC_CAMERA_H_
#define MY3D_SRC_CAMERA_H_

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/assignment.hpp"
#include "boost/numeric/ublas/io.hpp"

using namespace boost::numeric;

using matrix = ublas::matrix<double>;
using vector = ublas::vector<double>;
using zero_vector = ublas::zero_vector<double>;
using unit_vector = ublas::unit_vector<double>;
using scalar_vector = ublas::scalar_vector<double>;

class Camera {
 public:
  explicit Camera(const vector& pos = scalar_vector(3, 5),
                  const vector& look_at = zero_vector(3),
                  const vector& up = unit_vector(3, 2));

 public:
  matrix V;
};

#endif //MY3D_SRC_CAMERA_H_
