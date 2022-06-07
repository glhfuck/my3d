#ifndef MY3D_LIBS_MY3DLIB_SRC_LENS_H_
#define MY3D_LIBS_MY3DLIB_SRC_LENS_H_

#include "boost/numeric/ublas/matrix.hpp"
#include "boost/numeric/ublas/vector.hpp"
#include "boost/numeric/ublas/assignment.hpp"
#include "boost/numeric/ublas/io.hpp"

using namespace boost::numeric;

using matrix = ublas::matrix<double>;
using vector = ublas::vector<double>;

class Lens {
 public:
  explicit Lens(double fov = 75,
                double ratio = 1,
                double near = 1,
                double far = 100);
  void SetFOV(double fov);

  [[nodiscard]] matrix P() const;

 private:
  double fov_ = 0;
  double ratio_ = 0;
  double near_ = 0;
  double far_ = 0;
};

#endif //MY3D_LIBS_MY3DLIB_SRC_LENS_H_
