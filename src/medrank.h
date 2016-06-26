#ifndef _MEDRANK_H
#define _MEDRANK_H

class BNode;
class BTree;
class Query_cache;

class Medrank
{
public:
	Medrank(BTree *btree, RandVector *rvs, int num, int m, int dim, float MINFREQ);
	~Medrank();
	int search(char *query);
private:
	int num_;
	int dim_;
	int m_;
	float MINFREQ_;
	BTree* btree_;
	RandVector* rvs_;
};

#endif
