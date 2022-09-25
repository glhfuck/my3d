#ifndef MY3D_LIBS_MY3DLIB_SRC_FRAMEBUFFER_H_
#define MY3D_LIBS_MY3DLIB_SRC_FRAMEBUFFER_H_

#include <cstdlib>

template <typename T>
class FrameBuffer {
 public:
  FrameBuffer(size_t rows, size_t columns);

  T** GetBuffer();
  T* GetData();

  ~FrameBuffer();

  const size_t kROWS;
  const size_t kCOLUMNS;

  const size_t DATA_SIZE;
 private:
  T** buffer_;
  T* data_;
};


template <typename T>
FrameBuffer<T>::FrameBuffer(size_t rows, size_t columns) :
    kROWS(rows),
    kCOLUMNS(columns),
    DATA_SIZE(rows * columns * sizeof(T)) {
  size_t buffer_size = rows * sizeof(T*) + DATA_SIZE;
  buffer_ = (T**) malloc(buffer_size);
  data_ = (T*) (buffer_ + rows);

  for (int i = 0; i < rows; ++i) {
    buffer_[i] = data_ + i * columns;
  }
}

template <typename T>
T** FrameBuffer<T>::GetBuffer() {
  return buffer_;
}

template <typename T>
T* FrameBuffer<T>::GetData() {
  return data_;
}

template <typename T>
FrameBuffer<T>::~FrameBuffer() {
  free(buffer_);
}

#endif //MY3D_LIBS_MY3DLIB_SRC_FRAMEBUFFER_H_
