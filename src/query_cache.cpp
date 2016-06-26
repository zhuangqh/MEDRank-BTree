#include "./headers.h"

Query_cache::Query_cache() {
  h_ = l_ = -1;
  h_node = l_node = NULL;
}

Query_cache::~Query_cache() {
  if (h_node == l_node) l_node = NULL;
  if (h_node) {
    delete h_node;
  }
  if (l_node) {
    delete l_node;
  }
}

void Query_cache::init(BNode *dest, float query) {
  h_node = l_node = dest;
  query_ = query;
  //printf("init1\n");
  dest->locate_ptr(&l_, &h_, query_);
  //printf("init2 l %d, h %d\n", l_, h_);
}

int Query_cache::get_id() {
  //printf("in get id, l %d, h %d\n", l_, h_);
  float dis_l, dis_h;

  POS pos = LEFT;
  if (h_ != -1 && l_ != -1) {
    dis_l = get_distance_(l_node->get_key_by_index(l_), query_);
    dis_h = get_distance_(h_node->get_key_by_index(h_), query_);
    if (dis_l < dis_h) {
      pos = LEFT;
    } else {
      pos = RIGHT;
    }
  } else if (h_ == -1) {
    pos = LEFT;
  } else if (l_ == -1) {
    pos = RIGHT;
  }

  int id = -1;

  if (pos == LEFT) {
    assert(l_ != -1);
    id = l_node->get_son_by_index(l_);
    update_l_();
  } else {
    assert(h_ != -1);
    id = h_node->get_son_by_index(h_);
    update_h_();
  }
  return id;
}

float Query_cache::get_distance_(float a, float b) {
  return sqrt((a - b) * (a - b));
}

void Query_cache::update_l_() {
  if (l_ != - 1 && l_node->validate_index(--l_) == false) {
    BNode *ll = l_node->get_sibling(LEFT);
    if (l_node != h_node) delete l_node;
    l_node = ll;
    if (l_node == NULL)
      l_ = -1;
    else
      l_ = l_node->get_num_entries() - 1;
  }
}

void Query_cache::update_h_() {
  if (h_ != -1 && h_node->validate_index(++h_) == false) {
    BNode *hh = h_node->get_sibling(RIGHT);
    if (l_node != h_node) delete h_node;
    h_node = hh;
    if (h_node == NULL)
      h_ = -1;
    else
      h_ = 0;
  }
}
