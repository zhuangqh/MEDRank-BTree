#ifndef _RAND_NORMAL_VECTOR_H
#define _RAND_NORMAL_VECTOR_H

class RandVector {
 private:
  float get_gussian_random_();
  float * data_;
  int dim_;
 public:
  RandVector(int d); // dimensional
  RandVector();
  ~RandVector();
  int get_dimension();
  void set_dimension(int d);
  float operator[](int i);
};

#endif
