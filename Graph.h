#ifndef GRAPH_H
#define GRAPH_H

#include "point.h"
#include <vector>

class Graph
{
private:
	int size;
public:
	Graph(int);
	std::vector<Point> points;
	std::vector<Point> blocks;
	void printgraph() const;
	int getsize()
	{
		return size;
	}
	const static int maxsize = 10;
	int index[maxsize][maxsize];
};

#endif