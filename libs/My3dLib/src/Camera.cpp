#include "Camera.h"
#include "cmath"

using namespace boost::numeric;

Camera::Camera(const vector& position,
               const vector& target,
               const vector& upVector) {
  vector forward = target - position;
  if (ublas::norm_2(forward) == 0) {
    throw std::invalid_argument("The camera position is equal to the target.");
  }
  forward /= ublas::norm_2(forward);

  vector right = Transformer::cross_prod(forward, upVector);
  if (ublas::norm_2(right) == 0) {
    throw std::invalid_argument("The forward vector is collinear with the up vector.");
  }
  right /= ublas::norm_2(right);

  vector up = Transformer::cross_prod(right, forward);
  up /= ublas::norm_2(up); // Don't need to normalize as cross product of two unit vectors, but just to be sure.

  if (forward[2] == 1) {
    pitch = M_PI_2;
    roll = 0;
    yaw = -atan2(up[0], right[0]);
  } else if (forward[2] == -1) {
    pitch = -M_PI_2;
    roll = 0;
    yaw = atan2(up[0], right[0]);
  } else {
    pitch = asin(forward[2]);
    yaw = atan2(-forward[0] / cos(pitch), forward[1] / cos(pitch));
    roll = atan2(-right[2] / cos(pitch), up[2] / cos(pitch));
  }

  transformer_.SetPosition(position[0], position[1], position[2]);
  transformer_.SetRotate(pitch, roll, yaw);
}

Camera::matrix Camera::V() const {
  //std::cout << "yaw:" << yaw << std::endl << "pitch:" << pitch << std::endl << "roll:" << roll << std::endl;
  matrix reversed_rotation = ublas::trans(transformer_.getRotationMatrix());
  matrix reversed_translation = transformer_.getTranslationMatrix();
  reversed_translation(0, 3) *= -1;
  reversed_translation(1, 3) *= -1;
  reversed_translation(2, 3) *= -1;
  return ublas::prod(reversed_rotation, reversed_translation);
}

Camera::Camera(const Camera::vector& position, double yaw, double pitch, double roll) {
  transformer_.SetPosition(position[0], position[1], position[2]);
  transformer_.SetRotate(pitch, roll, yaw);
}
void Camera::Move(double right, double forward, double up) {
  vector camera_v(4);
  camera_v <<= right, forward, up, 0;

  vector global_v = ublas::prod(transformer_.getRotationMatrix(), camera_v);

  transformer_.Translate(global_v[0], global_v[1], global_v[2]);
}
void Camera::Rotate(double yaw, double pitch, double roll) {
  transformer_.Rotate(pitch, roll, yaw);
}

