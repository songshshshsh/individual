#include <cstring>
#include <list>
#include <cstdio>
#include <iostream>
#include "z3++.h"
#include "Strategy.h"

using namespace std;
using namespace z3;

const static int X[5] = {0,1,-1,0,0};
const static int Y[5] = {0,0,0,1,-1};

Strategy::Strategy(Graph _graph):graph(_graph),solution(this->graph.size){}

void Strategy::computeroute()
{
	if (!optimizedget()) printf("Opt can't be solved\n");
}

int Strategy::computeindex(int x,int y)
{
	return y*this->graph.size+x;
}

void Strategy::computexy(int index,int& x,int& y)
{
	x = index % this->graph.size;
	y = index / this->graph.size;
}

expr Strategy::bool_to_int(expr a,context& c,bool special)
{
	if (!special)
	{
		expr one  = c.int_val(1);
	    expr zero = c.int_val(0);
		return to_expr(c,Z3_mk_ite(c,a,one,zero));
	}
	else
	{
		expr one  = c.int_val(1);
	    expr zero = c.int_val(1000);
		return to_expr(c,Z3_mk_ite(c,a,one,zero));
	}
}

void Strategy::initializeX(expr_vector& x,context& c,Graph& currentgraph)
{
	for (unsigned int i = 0;i < currentgraph.points.size()/2;++i)
		for (unsigned int j = 0;j < currentgraph.size*currentgraph.size;++j)
		{
			std::stringstream x_name;

			x_name << "x_"<<i<<'_'<<j;
			x.push_back(c.bool_const(x_name.str().c_str()));
		}
}

void Strategy::setst(optimize& s,context& c,expr_vector& x,Graph& currentgraph)
{
	for (unsigned i = 0;i < currentgraph.points.size()/2;++i)
	{
		int start = computeindex(currentgraph.points[i*2].x,currentgraph.points[i*2].y);
		int terminal = computeindex(currentgraph.points[i*2+1].x,currentgraph.points[i*2+1].y);
		s.add(x[i*currentgraph.size*currentgraph.size + start]);
		s.add(x[i*currentgraph.size*currentgraph.size + terminal]);
	}
}

void Strategy::movement(optimize& s,context& c,expr_vector& x,Graph& currentgraph)
{
	for (unsigned i = 0;i < currentgraph.points.size()/2;i++)
		for (unsigned j = 0;j <currentgraph.size*currentgraph.size;j++)
		{
			int tem = computeindex(currentgraph.points[2*i].x,currentgraph.points[2*i].y);
			int start = computeindex(currentgraph.points[2*i+1].x,currentgraph.points[2*i+1].y);
			int currentx,currenty; 
			computexy(j,currentx,currenty);
			expr constfalse = c.bool_val(false);
			if (j == tem || j == start)
			{
				expr tempted = bool_to_int(constfalse,c,false);
				for (int k = 1;k <= 4;k++)
				{
					int newk = j + X[k]+Y[k] *currentgraph.size;
					int newkx,newky;
					computexy(newk,newkx,newky);
					if ((abs(newkx - currentx) + abs(newky - currenty)) == 1)
					if (int(j + X[k]+Y[k] *currentgraph.size) >= 0 &&
					 j + X[k] +Y[k] *currentgraph.size < currentgraph.size*currentgraph.size)
						tempted = tempted + bool_to_int(x[i*currentgraph.size*currentgraph.size+newk],c,false);
				}
				s.add(tempted == 1 && x[i*currentgraph.size*currentgraph.size+ j] || !x[i*currentgraph.size*currentgraph.size+ j] );
			}
			else 
			{
				expr conjecture =  !x[i*currentgraph.size*currentgraph.size+j];
				expr tempted = bool_to_int(constfalse,c,false);
				for (int k = 1;k <= 4;++k)
				{
						int newk = j + X[k]+Y[k] *currentgraph.size;
						int newkx,newky;
						computexy(newk,newkx,newky);
						if (newk >= 0 &&
						 newk < currentgraph.size*currentgraph.size
						 	&& (newkx == currentx || newky == currenty))
						 	tempted = tempted + bool_to_int(x[i*currentgraph.size*currentgraph.size+newk],c,false);
				}
				s.add((tempted == 2) || (conjecture));
			}
		}
}

void Strategy::fluidic(context& c,optimize& s,expr_vector& x,Graph& currentgraph)
{
	for (unsigned j = 0;j < currentgraph.size*currentgraph.size;j++)
	{
			expr temp = c.bool_val(false);
			for (unsigned k = 0;k < currentgraph.points.size()/2;k++)
			{
				expr conjecture = x[k*currentgraph.size*currentgraph.size+j];
				for (unsigned i = 0;i < currentgraph.points.size()/2;i++)
					if (i != k)
						conjecture = conjecture && (!x[i*currentgraph.size*currentgraph.size+j]);
				temp = temp || conjecture;
			} 
			expr conjecture = c.bool_val(true);
			for (unsigned k = 0;k < currentgraph.points.size()/2;k++)
				conjecture = conjecture && (!x[k*currentgraph.size*currentgraph.size+j]);
			temp = temp || conjecture;
			s.add(temp);
	}
}

void Strategy::block(context& c,optimize& s,expr_vector& x,Graph& currentgraph)
{
	for (unsigned i = 0;i < currentgraph.blocks.size();i++)
	{
		expr temp = c.bool_val(false);
		expr con = temp;
		int p = computeindex(currentgraph.blocks[i].x,currentgraph.blocks[i].y);
		for (unsigned k = 0;k < currentgraph.points.size()/2;k++)
			con = con || x[k*currentgraph.size*currentgraph.size+p];
		s.add(!con);
	}
}

void Strategy::optimizeRoute(context& c,optimize& s,expr_vector& x,expr& z,Graph& currentgraph)
{
	for (unsigned i = 0;i < currentgraph.points.size()/2;i++)
    {
    	int tem = computeindex(currentgraph.points[2*i].x,currentgraph.points[2*i].y);
		int start = computeindex(currentgraph.points[2*i+1].x,currentgraph.points[2*i+1].y);
    	for (unsigned j = 0;j < currentgraph.size*currentgraph.size;j++)
    	{
    		bool special = false;
    		if (j == tem || j == start) special = true;
    		z = z + bool_to_int(x[i*currentgraph.size*currentgraph.size+j],c,special);
    	}
    }
}

int Strategy::exp(const int x)
{
	int count = 1;
	for (int i = 1;i <= x; ++i)
		count = 2 *count;
	return count;
}

int Strategy::checknum(int x,int size)
{
	int count = 0;
	int i = 0;
	while (i < size)
	{
		count += (x >> i) % 2;
		i++;
	}
	return count;
}

Graph Strategy::GetSolution()
{
	return this->solution;
}

