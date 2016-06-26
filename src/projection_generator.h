#ifndef _PROJECTION_GENERATOR_H
#define _PROJECTION_GENERATOR_H
#include "./headers.h"

class Projection_generator {
 private:
  int num_;
  int dim_;
  int m_;
  char *data_set_;
  RandVector *rvs_;
  void generate_projections_();
  void sort_projections_();
 public:
  projection_t *projections;
  Projection_generator(int n, int d, int m, char *data, RandVector *rvs);
  ~Projection_generator();
};

#endif
