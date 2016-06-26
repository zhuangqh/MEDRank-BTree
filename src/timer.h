#ifndef _PERFROMANCE_ANALYZE_H
#define _PERFROMANCE_ANALYZE_H

class Performance_analyze {
 private:
  int type_;
 public:
  static clock_t c;
  Performance_analyze();
  void start(int type);
  void end(const char *);
};
#endif
