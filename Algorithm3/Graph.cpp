#include "Graph.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

Graph::Graph(int _size)
{
	this->size = _size;
	memset(index,0,sizeof(index));
}

Graph::Graph(int _size,int _pairs,int _blocks,const char filename[])
{
	FILE* in = fopen(filename,"r");
	this->size = _size;
	this->points.assign(2 * _pairs,Point(-1,-1,-1));
	for (int i = 0; i < _size;++i)
		for (int j = 0;j < _size;++j)
		{
			fscanf(in,"%d",&index[i][j]);
			if (index[i][j] == -1)
				this->blocks.push_back(Point(i,j,index[i][j]));
			else if (index[i][j] > 0)
			{
				if (this->points[2*(index[i][j]-1)].index == -1)
					this->points[2*(index[i][j]-1)] = Point(i,j,index[i][j]);
				else 
					this->points[2*(index[i][j]-1) + 1] = Point(i,j,index[i][j]);
			}
		}
	fclose(in);
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

void Graph::printgraph(ostream& output) const
{
	for (int i = 0;i < this->size;i++)
	{
		for (int j = 0;j < this->size;j++)
			 output<<index[i][j]<<' ';
		output<<endl;
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
