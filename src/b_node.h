#ifndef _BNODE_H
#define _BNODE_H

class BlockFile;
enum POS;

class BNode {
 private:
  // disk params
  char   level_;		// level of b-tree (level > 0)
  int    num_entries_;		// number of entries in this node
  int    left_sibling_;         // addr in disk for left  sibling
  int    right_sibling_;	// addr in disk for right sibling
  float* key_;			// keys
  int*   son_;			// addr of son node or object id

  // other params
  bool   dirty_;		// if dirty, write back to file
  int    block_;		// addr of disk for this node
  int    capacity_;		// max num of entries can be stored
  BlockFile* file_;		// disk file of this node
 public:
  BNode();
  ~BNode();
  void init(int level, BlockFile *file);
  void init_restore(BlockFile *file, int block);
  void read_from_buffer(char *buf);
  void write_to_buffer(char *buf);

  // get
  char get_level();
  int get_block();
  int get_capacity();
  int get_num_entries();
  float get_key_by_index(int index);
  int get_son_by_index(int index);
  int get_key_of_node();
  BNode * get_sibling(POS);

  // set
  void set_sibling(POS , int sibling);
  void add_new_entry(float key, int son);

  // logic
  bool isFull();
  bool validate_index(int index);

  void locate_ptr(int *l, int *h, float query);
  int find_largest_smaller_than(float query);

  // temporary function for test
  void printAll() {
    printf("===========\n");
    for (int i = 0; i < num_entries_; ++i) {
      printf("[%d]%.2f ", i, key_[i]);
    }
    printf("===========\n");
  }
  
};

#endif
