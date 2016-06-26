#include "./headers.h"

int IO_cost;
clock_t Performance_analyze::c = 0;

Performance_analyze::Performance_analyze() {
}

void Performance_analyze::start(int type) {
  type_ = type;
  c = clock();
  IO_cost = 0;
}

void Performance_analyze::end(const char *info) {
  if (type_ == 1) {
    printf("IO cost: %d\n", IO_cost);
  }
  if (info) {
    printf("%s takes time: %.2fs\n", info, (double)(clock() - c)/CLOCKS_PER_SEC);
  }
}
