#include "./headers.h"

int main(int argc, char *argv[]) {
  srand(time(NULL));
  Performance_analyze performance_analyze;

  int cardinality, dimension, queryNumber;
	char *Mnist_ds, *Mnist_q;
  // read in parameters from command line
  get_parameters(argc, argv, &cardinality, &dimension,
                &queryNumber, &Mnist_ds, &Mnist_q);

  int m = 50;

  char *data_set, *query_set;
  read_vector_from_file(Mnist_ds, &data_set);
  read_vector_from_file(Mnist_q, &query_set);

  RandVector *rvs = new RandVector[m];
  for (int i = 0; i < m; ++i) {
    rvs[i].set_dimension(dimension); // get a random normalize vector
  }

  // generate projections and sort
  printf("generating projections...");
  Projection_generator pg(cardinality, dimension, m, data_set, rvs);

  printf("generating btrees...\n");
  performance_analyze.start(1);
  BTree *btrees = new BTree[m];
  char btree_filename[30] = "./data/btree---.btree"; //store in disk
  for (int i = 0; i < m; ++i) {
    sprintf(btree_filename + 14, "%02d.btree", i);
    btrees[i].init(btree_filename, 1024);
    btrees[i].bulkload(pg.projections + i * cardinality, cardinality);
  }
  performance_analyze.end("generate btrees");

  printf("start medrank...\n");
  Medrank medrank(btrees, rvs, cardinality, m, dimension, 0.5);
  
  // query using medrank algorithm
  for (int i = 0; i < queryNumber; ++i) {
    printf("===============query.....%d===================\n", i);
    performance_analyze.start(1);
    //print_vector(query_set, i, dimension);
    //print_vector(data_set, medrank.search(query_set + i * dimension), dimension);
    int mid = medrank.search(query_set + i * dimension);
    performance_analyze.end("medrank ");
    performance_analyze.start(0);
    printf("Overall ratio: %f\n", calculate_ratio(data_set, query_set, i, mid, dimension));
    performance_analyze.end("calculate ratio");
  }

  if (data_set) delete[] data_set;
  if (query_set) delete[] query_set;

  return 0;
}
