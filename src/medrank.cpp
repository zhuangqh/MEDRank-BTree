#include "./headers.h"

Medrank::Medrank(BTree *btree, RandVector *rvs, int num, int m, int dim, float MINFREQ) {
	btree_ = btree;
	rvs_ = rvs;
	dim_ = dim;
	num_ = num;
	MINFREQ_ = MINFREQ;
	m_ = m;
}

Medrank::~Medrank() {
}

int Medrank::search(char* query_vector) {
	float* btree_queries = new float[m_];
	for (int j = 0; j < m_; ++j) {
		btree_queries[j] = inner_product(rvs_[j], query_vector, dim_);
	}

	Query_cache *qc = new Query_cache[m_];
	int *isFirst = new int[m_];
	memset(isFirst, 0, SIZEINT * m_);
	int *fre = new int[num_];
	memset(fre, 0, SIZEINT * num_);

	int i = 0, id_to_vote;
	bool hasAns = false;

	while (hasAns == false) {
		for (i = 0; i < m_; ++i) {
			if (isFirst[i] == 0) {
				BNode *tmp = btree_[i].find_node(btree_queries[i]);
				if (tmp == NULL) { // this should not appear
					return 0;
				}
				qc[i].init(tmp, btree_queries[i]); // cache a query
				isFirst[i] = 1;
			}
			//printf("get id %d\n", id_to_vote);
			id_to_vote = qc[i].get_id();

			if (++fre[id_to_vote] > MINFREQ_ * m_) {
				hasAns = true;
				break;
			}
		}
	}

	delete[] btree_queries;
	delete[] qc;
	delete[] isFirst;
	delete[] fre;

	return id_to_vote;
}
