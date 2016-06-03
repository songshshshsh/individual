#include "random.h"
#include <ctime>
#include <cstdlib>


using namespace std;

Random::Random(int _size,int _pairs,int _numofobs):graph(_size)
{
	this->size = _size;
	this->pairs = _pairs;
	this->blocks = _numofobs;
}

Graph Random::generate()
{
	srand(time(0));
	for (int i = 0;i < this->pairs * 2;i++)
	{
		int _x = rand() % size;
		int _y = rand() % size;
		Point point(_x,_y,(i+2)/2);
		while (this->graph.index[_x][_y] != 0)
		{
			_x = rand() % size;
			_y = rand() % size;
			point.setx(_x);
			point.sety(_y);
		}
		this->graph.points.push_back(point);
		this->graph.index[_x][_y] = (i+2)/2;
	}
	for (int i = 0;i < this->blocks;i++)
	{
		int _x = rand() % size;
		int _y = rand() % size;
		Point point(_x,_y,i);
		while (this->graph.index[_x][_y] != 0)
		{
			_x = rand() % size;
			_y = rand() % size;
			point.setx(_x);
			point.sety(_y);
		}
		this->graph.blocks.push_back(point);
		this->graph.index[_x][_y] = -1;
	}
	return graph;
}
