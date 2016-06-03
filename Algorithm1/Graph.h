#ifndef GRAPH_H
#define GRAPH_H

#include "point.h"
#include <vector>

class Graph
{
private:
public:
	Graph(int);
	Graph(const Graph&);
	std::vector<Point> points;
	std::vector<Point> blocks;
	void printgraph() const;
	int size;
	const static int maxsize = 10;
	int index[maxsize][maxsize];
};

#endif