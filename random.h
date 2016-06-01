#ifndef RANDOM_H
#define RANDOM_H

#include "Graph.h"
#include "point.h"

class Random
{
public:
	Random(int,int,int);
	Graph generate();
private:
	Graph graph;
	int size,pairs,blocks;
};

#endif