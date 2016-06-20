#ifndef TEST_H
#define TEST_H

#include "graph.h"

class Test
{
public:
	Test(const Graph&);
	bool check();
private:
	Graph graph;
	int size;
	const static int maxsize = 10;
	int index[maxsize][maxsize];
	int computeindex(int,int);
	void computexy(int,int&,int&);
	bool notblock(int,int);
	int reverse(int);
};

#endif