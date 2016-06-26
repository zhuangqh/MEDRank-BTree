#ifndef _UTIL_H
#define _UTIL_H
struct projection_t {
  int id_;
  float value_;
  projection_t(int id, float value) {
    id_ = id;
    value_ = value;
  }
  projection_t() {}
};

enum POS {
  LEFT, RIGHT
};

// compare the second item
int compare(const void * a, const void * b);

void error(const char* msg, bool is_exit);

float inner_product(RandVector& vec_a, char* vec_b, int dimension);

void read_vector_from_file(char *filename, char **data);

void print_vector(char *data_set, int i, int dim);

void get_parameters(int argc, char *argv[], int *n, int *dim,
                int *queryNumber, char **Mnist_ds, char **Mnist_1);

double get_distance(char* v1, char *v2, int dim);

double calculate_ratio(char *data_set, char *query_set, int q_id, int m_id, int dim);

#endif
