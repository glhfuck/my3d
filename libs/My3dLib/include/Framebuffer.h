#ifndef MY3D_LIBS_MY3DLIB_SRC_FRAMEBUFFER_H_
#define MY3D_LIBS_MY3DLIB_SRC_FRAMEBUFFER_H_

#include <cstdlib>

template <typename T>
class Framebuffer {
 public:
  Framebuffer(size_t rows, size_t columns);

  T** getBuffer();
  T* getData();

  ~Framebuffer();

  const size_t MAX_COLUMN_INDEX;
  const size_t MAX_ROW_INDEX;

 private:
  T** buff_;
  T* data_;
};

template <typename T>
Framebuffer<T>::Framebuffer(size_t rows, size_t columns) :
    MAX_COLUMN_INDEX(columns - 1),
    MAX_ROW_INDEX(rows - 1) {
  size_t sz = rows * sizeof(T*) + rows * columns * sizeof(T);
  buff_ = (T**) calloc(sz, 1);
  data_ = (T*) (buff_ + rows);

  for (int i = 0; i < rows; ++i) {
    buff_[i] = data_ + i * columns;
  }
}

template <typename T>
T** Framebuffer<T>::getBuffer() {
  return buff_;
}

template <typename T>
T* Framebuffer<T>::getData() {
  return data_;
}

template <typename T>
Framebuffer<T>::~Framebuffer() {
  free(buff_);
}

#endif //MY3D_LIBS_MY3DLIB_SRC_FRAMEBUFFER_H_
