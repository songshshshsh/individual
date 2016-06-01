#include "Graph.h"
#include <cstring>
#include <cstdio>

using namespace std;

Graph::Graph(int _size)
{
	this->size = _size;
	memset(index,0,sizeof(index));
}

void Graph::printgraph() const
{
	for (int i = 0;i < this->size;i++)
	{
		for (int j = 0;j < this->size;j++)
			printf("%d ",index[i][j]);
		printf("\n");
	}
			
}