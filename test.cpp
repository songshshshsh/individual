#include "test.h"
#include <cstdlib>

using namespace std;

const static int X[5] = {0,1,-1,0,0};
const static int Y[5] = {0,0,0,1,-1};

Test::Test(const Graph& graph_): graph(graph_)
{
	this->size = graph.size;
	for (int i = 0;i < size;++i)
		for (int j = 0;j < size;++j)
			this->index[i][j] = graph.index[i][j];
}

bool Test::check()
{
	for (int i = 0;i < this->graph.points.size()/2;++i)
	{
		int sourcex = this->graph.points[2*i].x,sourcey = this->graph.points[2*i].y;
		int terminalx = this->graph.points[2*i+1].x,terminaly = this->graph.points[2*i+1].y;
		int ix = sourcex, iy = sourcey, direction = 0;
		int count = 0;
		while (ix != terminalx && iy != terminaly)
		{
			bool hasroute = false;
			for (int k = 1;k <= 4;++k)
			{
				int currentx = sourcex + X[k];
				int currenty = sourcey + Y[k];
				if (currenty >= 0 && currenty <this->graph.size
					&& currentx >= 0 && currentx <this->graph.size
					&& (abs(currentx - ix) + abs(currenty - iy)) == 1
					&& notblock(currentx,currenty)
					&& reverse(k) != direction)
				{
					if (index[currentx][currenty])
					{
						count++;
						hasroute = true;
						ix = currentx;
						iy = currenty;
						direction = k;
					}
				}
			}
			if (count != 1) return false;
			if (!hasroute) return false;
		}
	}
	return true;
}

int Test::computeindex(int x,int y)
{
	return y*this->graph.size+x;
}

void Test::computexy(int index,int& x,int& y)
{
	x = index % this->graph.size;
	y = index / this->graph.size;
}

bool Test::notblock(int x,int y)
{
	for (int i = 0;i < this->graph.blocks.size();++i)
		if (this->graph.blocks[i].x == x
			&& this->graph.blocks[i].y == y)
			return false;
	return true;
}

int Test::reverse(int x)
{
	if (x == 1) return 2;
	if (x == 2) return 1;
	if (x == 3) return 4;
	if (x == 4) return 3;
	return 0;
}