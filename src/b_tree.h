#ifndef _BTREE_H
#define _BTREE_H

class BNode;
class BlockFile;
struct projection_t;

class BTree {
 public:
  int root_;

  BlockFile *file_;
  BNode *root_ptr_;

  BTree();
  ~BTree();

  void init(const char *fname, int block_length);

  void init_restore(const char *fname);

  void bulkload(projection_t *proj_table, int n);

  BNode *find_node(float query);
 private:
  void read_header_(char *buf);
  void write_header_(char *buf);

  void load_root_();
  void delete_root_();
};
#endif
