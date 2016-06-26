#include "./headers.h"

int compare(const void * a, const void * b) {
  return ( ((projection_t *)a)->value_ - ((projection_t *)b)->value_);
}

// display an error message
void error(const char* msg, bool is_exit) {
  printf("%s\n", msg);
  if (is_exit) exit(1);
}

float inner_product(RandVector& vec_a, char* vec_b, int dimension) {
	float ans = 0.0;
	for (int i = 0; i < dimension; i++) {
    ans += vec_b[i] * vec_a[i];
  }
	return ans;
}

void read_vector_from_file(char *filename, char **data) {
  Performance_analyze performance_analyze;
  performance_analyze.start(0);
  std::ifstream f;
  // header info of the file
  char header;
  int i, j;
  int num, dim;
  if (strcmp(filename, "./data/Mnist.ds") == 0) {
    num = 60000;
    dim = 784;
  } else if (strcmp(filename , "./data/Mnist.q") == 0) {
    num = 100;
    dim = 784;
  } else {
    error("cannot open data set", 1);
  }
  
  f.open(filename, std::ios::binary);
  *data = new char[num * dim];
  // 16 bit header
  for (i = 0; i < 16; ++i) {
    f.read(&header, 1);
  }

  for (i = 0; i < num; ++i) {
    for (j = 0; j < dim; ++j) {
      f.read(*data + i * dim + j, 1);
    }
  }
  
  f.close();
  performance_analyze.end("read file");
}


void print_vector(char *data_set, int id, int dim) {
  printf("\n========\n");
  for (int i = 0; i < dim; ++i) {
    printf("%d ", data_set[id * dim + i]);
  }
  printf("\n========\n");
}

void get_parameters(int argc, char *argv[], int *n, int *dim,
                int *queryNumber, char **Mnist_ds, char **Mnist_q) {
  	for (int i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-n") == 0) {
			*n = atoi(argv[i + 1]);
			if (*n <= 0) error("cardinality should be larger than 0", true);
		}
		else if (strcmp(argv[i], "-d") == 0) {
			*dim = atoi(argv[i + 1]);
			if (*dim <= 0) error("dimension should be larger than 0", true);
		}
		else if (strcmp(argv[i], "-qn") == 0) {
			*queryNumber = atoi(argv[i + 1]);
			if (*queryNumber <= 0) error("queryNumber should be larger than 0", true);
		}
		else if (strcmp(argv[i], "-ds") == 0) {
			*Mnist_ds = argv[i + 1];
		}
		else if (strcmp(argv[i], "-qs") == 0) {
			*Mnist_q = argv[i + 1];
		}
	}
}

double get_distance(char* v1, char *v2, int dim) {
  double dis = 0;
  for (int i = 0; i < dim; i++) {
    dis += (v1[i] - v2[i]) * (v1[i] - v2[i]);
  }
  return sqrt(dis);
}

double calculate_ratio(char *data_set, char *query_set, int q_id, int m_id, int dim) {
  double min = MAXREAL;
  for (int k = 0; k < 60000; k++) {
    double dis = get_distance(data_set + k * dim, query_set + q_id * dim, dim);
    if (dis < min) min = dis;
  }

  return get_distance(data_set + m_id * dim, query_set + q_id * dim, dim) / min;
}
