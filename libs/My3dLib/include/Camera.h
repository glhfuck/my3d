#ifndef MY3D_LIBS_MY3DLIB_SRC_CAMERA_H_
#define MY3D_LIBS_MY3DLIB_SRC_CAMERA_H_

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/assignment.hpp"
#include "boost/numeric/ublas/io.hpp"

#include "Transformer.h"

using namespace boost::numeric;

class Camera {
  using matrix = ublas::matrix<double>;
  using vector = ublas::vector<double>;
  using zero_vector = ublas::zero_vector<double>;
  using unit_vector = ublas::unit_vector<double>;

 public:
  explicit Camera(const vector& position,
                  const vector& target = zero_vector(3),
                  const vector& upVector = unit_vector(3, 2));

  Camera(const vector& position, double yaw, double pitch, double roll);

  void Rotate(double yaw, double pitch, double roll);
  void Move(double right, double forward, double up);

  void GetParam();

 public:
  mutable Transformer transformer_;
  double yaw = 0;
  double pitch = 0;
  double roll = 0;

  matrix V() const;
};

#endif //MY3D_LIBS_MY3DLIB_SRC_CAMERA_H_
