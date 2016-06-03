#include "Graph.h"
#include <cstring>
#include <cstdio>

using namespace std;

Graph::Graph(int _size)
{
	this->size = _size;
	memset(index,0,sizeof(index));
}


Graph::Graph(const Graph& graph_)
{
	this->size = graph_.size;
	for (int i = 0;i < graph_.size;++i)
		for (int j = 0;j < graph_.size;++j)
			this->index[i][j] = graph_.index[i][j];
	for (int i = 0; i < graph_.points.size();++i)
		this->points.push_back(graph_.points[i]);
	for (int i = 0; i < graph_.blocks.size(); ++i)
		this->blocks.push_back(graph_.blocks[i]);
}

void Graph::printgraph() const
{
	for (int i = 0;i < this->size;i++)
	{
		for (int j = 0;j < this->size;j++)
			if (index[i][j] == -1) printf("- ");
				else printf("%d ",index[i][j]);
		printf("\n");
	}
			
}

Graph& Graph::operator=(const Graph& graph_)
{
	this->size = graph_.size;
	for (int i = 0;i < graph_.size;++i)
		for (int j = 0;j <graph_.size;++j)
			this->index[i][j] = graph_.index[i][j];
	this->points.clear();
	this->blocks.clear();
	for (int i = 0;i < graph_.points.size();++i)
		this->points.push_back(graph_.points[i]);
	for (int j = 0;j < graph_.blocks.size();++j)
		this->blocks.push_back(graph_.blocks[j]);
	return *this;
}
