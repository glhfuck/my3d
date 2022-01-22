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
  Transformer();
  
  matrix getRTS() const;

  [[nodiscard]] matrix getResultingMatrix() const;
  [[nodiscard]] matrix getTranslationMatrix() const;
  [[nodiscard]] matrix getRotationMatrix() const;
  [[nodiscard]] matrix getScalingMatrix() const;

  void SetPosition(double x, double y, double z);
  void Translate(double delta_x, double delta_y, double delta_z);
  void SetRotate(double aroundOx, double aroundOy, double aroundOz);
  void Rotate(double aroundOx, double aroundOy, double aroundOz);
  void SetScale(double x, double y, double z);
  void Scale(double x, double y, double z);

 //private:
  double x_position_ = 0;
  double y_position_ = 0;
  double z_position_ = 0;

  double x_scale_ = 1;
  double y_scale_ = 1;
  double z_scale_ = 1;

  double x_angle_ = 0;
  double y_angle_ = 0;
  double z_angle_ = 0;

  static vector cross_prod(const vector& first, const vector& second);
  static matrix TranslationMatrix(double x, double y, double z);
  static matrix OxRotationMatrix(double angle);
  static matrix OyRotationMatrix(double angle);
  static matrix OzRotationMatrix(double angle);
  static matrix ScalingMatrix(double x, double y, double z);
};

#endif //MY3D_LIBS_MY3DLIB_SRC_TRANSFORMER_H_