#include "../include/Shape.h"

void Shape::setPosition(const Shape::vector& pos, Shape::Coords coord) {
  switch (coord) {
    case Coords::Global:
      global_transformer_.setPosition(pos);
      break;
    case Coords::Local:
      local_transformer_.setPosition(pos);
      break;
    default:
      throw std::invalid_argument("Invalid coordinate system.");
  }
}

void Shape::translate(const Shape::vector& vec, Shape::Coords coord) {
  switch (coord) {
    case Coords::Global:
      global_transformer_.translate(vec);
      break;
    case Coords::Local:
      local_transformer_.translate(vec);
      break;
    default:
      throw std::invalid_argument("Invalid coordinate system.");
  }
}

void Shape::rotate(double angle, Transformer::Axes axis, Shape::Coords coord) {
  switch (coord) {
    case Coords::Global:
      global_transformer_.rotate(angle, axis);
      break;
    case Coords::Local:
      local_transformer_.rotate(angle, axis);
      break;
    default:
      throw std::invalid_argument("Invalid coordinate system.");
  }
}

void Shape::scale(double x, double y, double z, Shape::Coords coord) {
  switch (coord) {
    case Coords::Global:
      global_transformer_.scale(x, y, z);
      break;
    case Coords::Local:
      local_transformer_.scale(x, y, z);
      break;
    default:
      throw std::invalid_argument("Invalid coordinate system.");
  }
}

Shape::matrix Shape::M() const {
  return ublas::prod(global_transformer_.getResultMatrix(),
                     local_transformer_.getResultMatrix());
}
