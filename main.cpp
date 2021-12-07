#include <iostream>
#include "Shape.h"

int main() {
  Shape shape1;
  shape1.printModelMatrix();
  Shape shape2(ublas::scalar_vector<double>(3, 1));
  shape2.printModelMatrix();
  return 0;
}
