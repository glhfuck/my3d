#ifndef MY3D_LIBS_MY3DLIB_SRC_OBJ_LOADER_H_
#define MY3D_LIBS_MY3DLIB_SRC_OBJ_LOADER_H_

#include <string>
#include "Shape.h"
#include <iostream>
#include <fstream>

using vector = boost::numeric::ublas::vector<double>;

class Obj_loader {
 public:
  static Shape getShape(const std::string& path);

 private:
  static void handleVertices(Shape& s, std::ifstream& in);
  static void handleNormals(Shape& s, std::ifstream& in);
  static void handleFacets(Shape& s, std::ifstream& in);
};

Shape Obj_loader::getShape(const std::string& path) {
  Shape res;
  std::ifstream in(path, std::fstream::in);

  std::string str;

  while (in >> str) {
    if (str == "v") {
      handleVertices(res, in);
    } else if (str == "vn") {
      handleNormals(res, in);
    } else if (str == "f") {
      handleFacets(res, in);
    }
  }

  in.close();
  return res;
}

void Obj_loader::handleVertices(Shape& s, std::ifstream& in) {
  vector vertex(4);
  in >> vertex[0] >> vertex[1] >> vertex[2];
  vertex[3] = 1;
  s.vertices.emplace_back(vertex);
}

void Obj_loader::handleNormals(Shape& s, std::ifstream& in) {
  vector normal(4);
  in >> normal[0] >> normal[1] >> normal[2];
  normal[3] = 0;
  s.normals.emplace_back(normal);
}

void Obj_loader::handleFacets(Shape& s, std::ifstream& in) {
  Shape::Facet facet;

  for (auto& ver_info: facet.verInfo) {
    char c; // c = '/'    f v/vt/vn
    in >> ver_info.v_idx >> c >> ver_info.vt_idx >> c >> ver_info.vn_idx;
  }

  s.facets.emplace_back(facet);
}


#endif //MY3D_LIBS_MY3DLIB_SRC_OBJ_LOADER_H_
