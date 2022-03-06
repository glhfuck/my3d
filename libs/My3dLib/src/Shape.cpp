#include "../include/Shape.h"

void Shape::SetPosition(double x, double y, double z, Coords coord) {
  switch (coord) {
    case Coords::Global:
      global_transformer_.SetPosition(x, y, z);
      break;
    case Coords::Local:
      local_transformer_.SetPosition(x, y, z);
      break;
    default:
      throw std::invalid_argument("Invalid coordinate system.");
  }
}

void Shape::Translate(double delta_x, double delta_y, double delta_z, Coords coord) {
  switch (coord) {
    case Coords::Global:
      global_transformer_.Translate(delta_x, delta_y, delta_z);
      break;
    case Coords::Local:
      local_transformer_.Translate(delta_x, delta_y, delta_z);
      break;
    default:
      throw std::invalid_argument("Invalid coordinate system.");
  }
}

void Shape::SetRotate(double aroundOx, double aroundOy, double aroundOz, Coords coord) {
  switch (coord) {
    case Coords::Global:
      global_transformer_.SetRotate(aroundOx, aroundOy, aroundOz);
      break;
    case Coords::Local:
      local_transformer_.SetRotate(aroundOx, aroundOy, aroundOz);
      break;
    default:
      throw std::invalid_argument("Invalid coordinate system.");
  }
}

void Shape::Rotate(double aroundOx, double aroundOy, double aroundOz, Coords coord) {
  switch (coord) {
    case Coords::Global:
      global_transformer_.Rotate(aroundOx, aroundOy, aroundOz);
      break;
    case Coords::Local:
      local_transformer_.Rotate(aroundOx, aroundOy, aroundOz);
      break;
    default:
      throw std::invalid_argument("Invalid coordinate system.");
  }
}

void Shape::SetScale(double x, double y, double z, Shape::Coords coord) {
  switch (coord) {
    case Coords::Global:
      global_transformer_.SetScale(x, y, z);
      break;
    case Coords::Local:
      local_transformer_.SetScale(x, y, z);
      break;
    default:
      throw std::invalid_argument("Invalid coordinate system.");
  }
}

void Shape::Scale(double x, double y, double z, Shape::Coords coord) {
  switch (coord) {
    case Coords::Global:
      global_transformer_.Scale(x, y, z);
      break;
    case Coords::Local:
      local_transformer_.Scale(x, y, z);
      break;
    default:
      throw std::invalid_argument("Invalid coordinate system.");
  }
}

Shape::matrix Shape::M() const {
  return ublas::prod(global_transformer_.getResultingMatrix(),
                     local_transformer_.getResultingMatrix());
}
