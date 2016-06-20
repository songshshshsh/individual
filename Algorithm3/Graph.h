#ifndef GRAPH_H
#define GRAPH_H

#include "point.h"
#include <vector>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>

class Graph
{
private:
public:
	Graph(int);
	Graph(int,int,int,const char[]);
	Graph(const Graph&);
	std::vector<Point> points;
	std::vector<Point> blocks;
	void printgraph(std::ostream&) const;
	int size;
	const static int maxsize = 11;
	int index[maxsize][maxsize];
	Graph& operator=(const Graph&);
};

#endif