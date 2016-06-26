#include "./headers.h"

Projection_generator::Projection_generator(int n, int dim, int m, char *data_set, RandVector *rvs) {
  this->num_ = n;
  this->dim_ = dim;
  this->m_ = m;
  this->data_set_ = data_set;
  this->rvs_ = rvs;

  generate_projections_();
  printf("sorting...");
  sort_projections_();
}

Projection_generator::~Projection_generator() {
  if (projections) delete projections;
}

void Projection_generator::generate_projections_() {
  Performance_analyze performance_analyze;
  performance_analyze.start(0);
  projections = new projection_t[num_ * m_];

  for (int i = 0; i < m_; ++i) {
    for (int j = 0; j < num_; ++j) {
      // internal product
      int projection = 0;
      for (int k = 0; k < dim_; ++k) {
        projection += rvs_[i][k] * data_set_[j * dim_ + k];
      }

      projections[i * num_ + j] = projection_t(j, projection);
    }
  }
  performance_analyze.end("generate projections");
}

void Projection_generator::sort_projections_() {
  Performance_analyze performance_analyze;
  performance_analyze.start(0);
  for (int i = 0; i < m_; ++i) {
    qsort(projections + i * num_, num_, sizeof(projection_t), compare);
  }

  performance_analyze.end("sort projections");
}
