#include "./headers.h"

extern int IO_cost;

BNode::BNode() {
  level_ = -1;
  num_entries_ = -1;
  left_sibling_ = -1;
  right_sibling_ = -1;
  key_ = NULL;
  son_ = NULL;

  dirty_ = false;
  block_ = -1;
  capacity_ = -1;
  file_ = NULL;
}

BNode::~BNode() {
  ++IO_cost;
  char *buf = NULL;
  if (dirty_) { // write back to disk
    int block_length = file_->get_blocklength();
    buf = new char[block_length];
    int test;
    write_to_buffer(buf);

    file_->write_block(buf, block_);
    
    delete[] buf;
    buf = NULL;
  }

  if (key_) {
    delete[] key_;
    key_ = NULL;
  }

  if (son_) {
    delete[] son_;
    son_ = NULL;
  }
}

void BNode::init(int level, BlockFile *file) {
  file_ = file;
  level_ = (char)level;

  num_entries_ = 0;
  dirty_ = true;

  capacity_ = get_capacity();
  assert(capacity_ > 50);

  key_ = new float[capacity_];
  for (int i = 0; i < capacity_; ++i) {
    key_[i] = MINREAL;
  }

  son_ = new int[capacity_];
  for (int i = 0; i < capacity_; ++i) {
    son_[i] = -1;
  }

  char *new_block = new char[file_->get_blocklength()];
  block_ = file_->append_block(new_block);
  delete[] new_block;
  new_block = NULL;
}

void BNode::init_restore(BlockFile *file, int block_index) {
  ++IO_cost;
  file_ = file;
  block_ = block_index;
  dirty_ = false;

  capacity_ = get_capacity();
  assert(capacity_ > 50);

  key_ = new float[capacity_]; // just allocate memory
  son_ = new int[capacity_];

  char *blk = new char[file_->get_blocklength()];
  file_->read_block(blk, block_);

  read_from_buffer(blk); // initial the value of private varible

  delete[] blk;
  blk = NULL;
}

void BNode::read_from_buffer(char *buf) {
  assert(buf != NULL);

  int i = 0;
  memcpy(&level_, &buf[i], SIZECHAR);
  i += SIZECHAR;

  memcpy(&num_entries_, &buf[i], SIZEINT);
  i += SIZEINT;

  memcpy(&left_sibling_, &buf[i], SIZEINT);
  i += SIZEINT;

  memcpy(&right_sibling_, &buf[i], SIZEINT);
  i += SIZEINT;

  for (int j = 0; j < num_entries_; ++j) {
    memcpy(&key_[j], &buf[i], SIZEFLOAT);
    i += SIZEFLOAT;

    memcpy(&son_[j], &buf[i], SIZEINT);
    i += SIZEINT;
  }
}

void BNode::write_to_buffer(char *buf) {
  if (buf == NULL) return;

  int i = 0;
  memcpy(&buf[i], &level_, SIZECHAR);
  i += SIZECHAR;

  memcpy(&buf[i], &num_entries_, SIZEINT);
  i += SIZEINT;

  memcpy(&buf[i], &left_sibling_, SIZEINT);
  i += SIZEINT;

  memcpy(&buf[i], &right_sibling_, SIZEINT);
  i += SIZEINT;

  for (int j = 0; j < num_entries_; ++j) {
    memcpy(&buf[i], &key_[j], SIZEFLOAT);
    i += SIZEFLOAT;

    memcpy(&buf[i], &son_[j], SIZEINT);
    i += SIZEINT;
  }
}

char BNode::get_level() {
  return level_;
}

int BNode::get_block() {
  return block_;
}

int BNode::get_capacity() {
  if (file_ == NULL) return 0;

  int block_length = file_->get_blocklength();
  // <level_> : SIZEFLOAT
  // <num_entries> <left_sibling_> <right_sibling_) : SIZEINT
  int header_size = SIZECHAR + SIZEINT * 3;
  // <key_> : SIZEFLOAT
  // <son_> : SIZEINT
  int entry_size = SIZEFLOAT + SIZEINT;

  return ((block_length - header_size) / entry_size);
}

int BNode::get_num_entries() {
  return num_entries_;
}

float BNode::get_key_by_index(int index) {
  assert(index >= 0 && index < num_entries_);

  return key_[index];
}

int BNode::get_son_by_index(int index) {
  if (index < 0 && index >= num_entries_)
    return -1;
  else
    return son_[index];
}

int BNode::get_key_of_node() {
  //printf("num entry %d\n", num_entries_);
  assert(num_entries_ > 0);
  return key_[0];
}

// get sibling by specify position
BNode* BNode::get_sibling(POS pos) {
  BNode *node = NULL;
  int sibling = -1;
  if (pos == LEFT) {
    sibling = left_sibling_;
  } else if (pos == RIGHT) {
    sibling = right_sibling_;
  }

  if (sibling != -1) {
    node = new BNode();
    node->init_restore(file_, sibling);
  }

  return node;
}

// set sibling's block id
void BNode::set_sibling(POS pos, int sibling) {
  if (pos == LEFT) {
    left_sibling_ = sibling;
  } else if (pos == RIGHT) {
    right_sibling_ = sibling;
  }
}

void BNode::add_new_entry(float key, int son) {
  assert(num_entries_ < capacity_);

  key_[num_entries_] = key;
  son_[num_entries_] = son;

  num_entries_++;
  dirty_ = true;
}

bool BNode::isFull() {
  return (num_entries_ >= capacity_);
}

bool BNode::validate_index(int idx) {
  return (idx >=0 && idx < num_entries_);
}

int BNode::find_largest_smaller_than(float query) {
  int left = 0, right = num_entries_;
	int mid;
	if (right < 2 || query < key_[left]) return 0;
	else if (query >= key_[right - 1]) return right - 1;


	while (left + 1 < right) {
		mid = left + ((right-left) / 2);
		if (key_[mid] <= query) left = mid;
		else right = mid;
	}

	if (key_[right] == query) return right;
	return left;
}

// locate ptr for search
void BNode::locate_ptr(int *l, int *h, float query) {
  *l = find_largest_smaller_than(query);
  if (key_[*l] > query) {
    *h = *l;
  } else {
    *h = *l + 1;
    if (*h >= num_entries_)
      *h = num_entries_ - 1;
  }
}
