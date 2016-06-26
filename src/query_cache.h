#ifndef _QUERY_CACHE_H
#define _QUERY_CACHE_H

class Query_cache {
 private:
  BNode *h_node, *l_node;
  int h_, l_;
  float query_;
  float get_distance_(float a, float b);
  void update_l_();
  void update_h_();
 public:
   Query_cache();
   ~Query_cache();
   void init(BNode *dest, float query);
   int get_id();
};

#endif
