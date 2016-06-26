#include "./headers.h"

float RandVector::get_gussian_random_() {
  return sqrt(-2 * (log((double)rand() / RAND_MAX))) * cos(2 * PI * (double)rand() / RAND_MAX);
}

RandVector::RandVector(int d) {
  set_dimension(d);
}

RandVector::~RandVector() {
  if (data_) delete[] data_;
}

RandVector::RandVector() {
  dim_ = 0;
  data_ = NULL;
}

int RandVector::get_dimension() {
  return dim_;
}

float RandVector::operator[](int i) {
  assert(i >= 0 && i < dim_);
  return data_[i];
}

void RandVector::set_dimension(int d) {
  dim_ = d;
  data_ = new float[d];
  for (int i = 0; i < d; ++i) {
    data_[i] = get_gussian_random_();
  }
}
