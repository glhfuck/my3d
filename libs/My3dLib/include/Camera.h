#ifndef MY3D_SRC_CAMERA_H_
#define MY3D_SRC_CAMERA_H_

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/assignment.hpp"
#include "boost/numeric/ublas/io.hpp"

using namespace boost::numeric;

class Camera {
 public:
  explicit Camera(const ublas::vector<double>& pos = ublas::scalar_vector(3),
                  const ublas::vector<double>& look_at = ublas::zero_vector(3),
                  const ublas::vector<double>& up = ublas::unit_vector(3, 2));

 public:
  ublas::matrix<double> V;
};

#endif //MY3D_SRC_CAMERA_H_
