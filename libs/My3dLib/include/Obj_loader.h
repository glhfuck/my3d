#ifndef MY3D_LIBS_MY3DLIB_SRC_OBJ_LOADER_H_
#define MY3D_LIBS_MY3DLIB_SRC_OBJ_LOADER_H_

#include <string>
#include <cstdio>
#include <iostream>
#include <fstream>

#include "Shape.h"

using vector = boost::numeric::ublas::vector<double>;

class Obj_loader {
 public:
  static Shape getShape(const std::string& path);

 private:
  static void handleVertices(Shape& s, FILE* in);
  static void handleNormals(Shape& s, FILE* in);
  static void handleFacets(Shape& s, FILE* in);
};

Shape Obj_loader::getShape(const std::string& path) {
  Shape res;
  std::FILE* in = fopen(path.c_str(), "r");

  while (!feof(in)) {
    char mode[2] = {0};
    fscanf(in, "%2s", mode);
    if (strcmp(mode, "v") == 0) {
      handleVertices(res, in);
    } else  if (strcmp(mode, "vn") == 0) {
      handleNormals(res, in);
    } else if (strcmp(mode, "f") == 0) {
      handleFacets(res, in);
    }
  }

  fclose(in);
  return res;
}

void Obj_loader::handleVertices(Shape& s, FILE* in) {
  vector vertex(4);
  fscanf(in, "%lf %lf %lf", &vertex[0], &vertex[1], &vertex[2]);
  vertex[3] = 1;
  s.vertices.emplace_back(vertex);
}

void Obj_loader::handleNormals(Shape& s, FILE* in) {
  vector normal(4);
  fscanf(in, "%lf %lf %lf", &normal[0], &normal[1], &normal[2]);
  normal[3] = 0;
  s.normals.emplace_back(normal);
}

void Obj_loader::handleFacets(Shape& s, FILE* in) {
  Shape::Facet facet;

  for (auto& ver_info : facet.verInfo) {
    std::string str;
    fscanf(in, "%d/", &ver_info.v_idx);
    fscanf(in, "%d", &ver_info.vt_idx);
    fscanf(in, "/%d", &ver_info.vn_idx);
    --ver_info.v_idx;
    --ver_info.vt_idx;
    --ver_info.vn_idx;
  }


  s.facets.emplace_back(facet);
}

#endif //MY3D_LIBS_MY3DLIB_SRC_OBJ_LOADER_H_
