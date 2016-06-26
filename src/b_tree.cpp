#include "./headers.h"

BTree::BTree() {
  root_ = -1;
  file_ = NULL;
  root_ptr_ = NULL;
}

BTree::~BTree() {
  if (root_ptr_) {
    delete root_ptr_;
  }

  char *header = new char[file_->get_blocklength()];
  write_header_(header);
  file_->set_header(header);

  delete[] header;

  if (file_) {
    delete file_;
  }
}

void BTree::init(const char *fname, int block_length) {

  FILE *fp = fopen(fname, "r");
  if (fp) {
    fclose(fp);
    printf("The file \"%s\" exists. Replace? (y/n) ", fname);

    char c = getchar();
    getchar(); // read ENTER
    if (c != 'y' && c != 'Y') {
      error("", true);
    }
    remove(fname);
  }

  file_ = new BlockFile(fname, block_length);

  root_ptr_ = new BNode();
  root_ptr_->init(0, file_);
  root_ = root_ptr_->get_block();
  delete_root_();
}

// restore from disk
void BTree::init_restore(const char *fname) {
  FILE *fp = fopen(fname, "r");
  if (!fp) {
    printf("tree file %s does not exist\n", fname);
    delete[] fname; fname = NULL;
    error("", true);
  }
  fclose(fp);

  file_ = new BlockFile(fname, 0);
  root_ptr_ = NULL;

  char *header = new char[file_->get_blocklength()];
  file_->read_header(header);
  read_header_(header);

  if (header) {
    delete[] header;
    header = NULL;
  }
}

void BTree::read_header_(char *buf) {
  memcpy(&root_, buf, SIZEINT);
}

void BTree::write_header_(char *buf) {
  memcpy(buf, &root_, SIZEINT);
}

void BTree::delete_root_() {
  if (root_ptr_ != NULL) {
    delete root_ptr_;
    root_ptr_ = NULL;
  }
}

void BTree::load_root_() {
	if (root_ptr_ == NULL)  {
		root_ptr_ = new BNode();
		root_ptr_->init_restore(file_, root_);
	}
}

void BTree::bulkload(projection_t *proj_table, int n) {
  BNode *current = NULL;
  BNode *current_prev = NULL;
  BNode *current_child = NULL;

  int last_start_block = -1;
  int last_end_block = -1;

  int current_level = -1;
  int current_start_block = -1;
  int current_end_block = -1;

  bool isFirst = true;
  //  level 0
  current_level = 0;
  
  for (int i = 0; i < n; ++i) {
    // allocate memory
    if (current == NULL) {
      current = new BNode();
      current->init(current_level, file_);
      current_end_block = current->get_block();

      if (isFirst) { // first node
        isFirst = false;
        current_start_block = current_end_block;
      } else {
        current_prev->set_sibling(RIGHT, current_end_block);
        current->set_sibling(LEFT, current_prev->get_block());
        delete current_prev;
        current_prev = NULL;
      }
    }

    // set value
    current->add_new_entry(proj_table[i].value_, proj_table[i].id_);

    // request new memory
    if (current->isFull()) {
      current_prev = current;
      current = NULL;
    }
  }

  if (current) {
    delete current;
    current = NULL;
  }

  if (current_prev) {
    delete current_prev;
    current_prev = NULL;
  }

  // upper
  last_start_block = current_start_block;
  last_end_block = current_end_block;

  while (last_end_block > last_start_block) {
    isFirst = true;
    ++current_level;

    for (int block = last_start_block; block <= last_end_block; ++block) {
      // allocate memory
      if (current == NULL) {
        current = new BNode();
        current->init(current_level, file_);
        current_end_block = current->get_block();

        if (isFirst) {  // first node
          isFirst = false;
          current_start_block = current_end_block;
        } else {
          current_prev->set_sibling(RIGHT, current_end_block);
          current->set_sibling(LEFT, current_prev->get_block());
          delete current_prev;
          current_prev = NULL;
        }
      }

      // set value
      current_child = new BNode();
      current_child->init_restore(file_, block);
      float key = current_child->get_key_of_node();

      //printf("key %f, last block %d, current block %d\n", key, block, current_end_block);

      delete current_child;
      current_child = NULL;

      current->add_new_entry(key, block);

      if (current->isFull()) {
        current_prev = current;
        current = NULL;
      }
    }

    if (current) {
      delete current; current = NULL;
    }
    if (current_prev) {
      delete current_prev; current_prev = NULL;
    }

    last_start_block = current_start_block;
    last_end_block = current_end_block;
  }

  root_ = last_start_block;
}

// search and return a node for cache
BNode *BTree::find_node(float query) {
  load_root_();

  BNode *res = root_ptr_;
  BNode *tmp = NULL;

  int block;

  while (res->get_level() > 0) {
    block = res->find_largest_smaller_than(query);
    if (res->validate_index(block) == false) {
      return NULL;
    }
    block = res->get_son_by_index(block);
    if (res != root_ptr_) delete res; // reserve the root node

    if (block < 0) {
      res = NULL;
      break;
    }

    res = new BNode();
    res->init_restore(file_, block);
  }

  return res;
}