#include "SAT.h"
#include <cstring>
#include <list>
#include <cstdio>
#include <iostream>
#include "z3++.h"

using namespace std;
using namespace z3;

const static int X[5] = {0,1,-1,0,0};
const static int Y[5] = {0,0,0,1,-1};

SAT::SAT(Graph _graph):graph(_graph),solution(this->graph.size){}

void SAT::computeroute()
{
	if (!optimizedget()) printf("Opt can't be solved\n");
}

int SAT::computeindex(int x,int y)
{
	return y*this->graph.size+x;
}

void SAT::computexy(int index,int& x,int& y)
{
	x = index % this->graph.size;
	y = index / this->graph.size;
}

expr SAT::bool_to_int(expr a,context& c,bool special)
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

void SAT::initializeX(expr_vector& x,context& c,Graph& currentgraph)
{
	for (unsigned int i = 0;i < currentgraph.points.size()/2;++i)
		for (unsigned int j = 0;j < currentgraph.size*currentgraph.size;++j)
		{
			std::stringstream x_name;

			x_name << "x_"<<i<<'_'<<j;
			x.push_back(c.bool_const(x_name.str().c_str()));
		}
}

void SAT::setst(optimize& s,context& c,expr_vector& x,Graph& currentgraph)
{
	for (unsigned i = 0;i < currentgraph.points.size()/2;++i)
	{
		int start = computeindex(currentgraph.points[i*2].x,currentgraph.points[i*2].y);
		int terminal = computeindex(currentgraph.points[i*2+1].x,currentgraph.points[i*2+1].y);
		s.add(x[i*currentgraph.size*currentgraph.size + start]);
		s.add(x[i*currentgraph.size*currentgraph.size + terminal]);
	}
}

void SAT::movement(optimize& s,context& c,expr_vector& x,Graph& currentgraph)
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

void SAT::fluidic(context& c,optimize& s,expr_vector& x,Graph& currentgraph)
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

void SAT::block(context& c,optimize& s,expr_vector& x,Graph& currentgraph)
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

void SAT::optimizeRoute(context& c,optimize& s,expr_vector& x,expr& z,Graph& currentgraph)
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

int SAT::exp(const int x)
{
	int count = 1;
	for (int i = 1;i <= x; ++i)
		count = 2 *count;
	return count;
}

int SAT::checknum(int x,int size)
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

bool SAT::optimizedget()
{
	cout<<"optimizedget"<<endl;

	bool get = false;

	int currentnum = this->graph.points.size()/2;

	int minimum = 1000000;

	const int totalcase = exp(this->graph.points.size()/2);

	//iterately decreace the max set number
	while (!get && currentnum >= 0)
	{
		for (int i = 0;i < totalcase;++i)
		{
			Graph currentgraph(this->graph.size);
			//generate all the possible situation
			if (checknum(i,this->graph.points.size()/2) == currentnum)
			{

				int k = 0;
				while (k < this->graph.points.size()/2)
				{
					if  ((i >> k) % 2)
					{
						currentgraph.points.push_back(this->graph.points[2*k]);
						currentgraph.points.push_back(this->graph.points[2*k+1]);
						currentgraph.index[this->graph.points[2*k].x][this->graph.points[2*k].y] = k+1;
						currentgraph.index[this->graph.points[2*k+1].x][this->graph.points[2*k+1].y] = k+1;
					}
					k++;
				}
				for (int j = 0;j < this->graph.blocks.size();++j)
				{
					currentgraph.blocks.push_back(this->graph.blocks[j]);
					currentgraph.index[this->graph.blocks[j].x][this->graph.blocks[j].y] = -1;
				}


				context c;
				expr_vector x(c);

				initializeX(x,c,currentgraph);


				optimize s(c);
				params p(c);
			    p.set("priority",c.str_symbol("pareto"));
			    s.set(p);

			    //optimize parameters

			    //set start and end
			    setst(s,c,x,currentgraph);

				//droplet movement
				movement(s,c,x,currentgraph);


				//fluidic constraints
				fluidic(c,s,x,currentgraph);



				//blockages
				block(c,s,x,currentgraph);


			    expr f = c.bool_const("f");
			    expr z = bool_to_int(f,c,false);
			    s.add(!f);

			    //define the optimizing parameter z
			    optimizeRoute(c,s,x,z,currentgraph);
			    
			    optimize::handle h1 = s.minimize(z);

					
				if (sat == s.check())
				{
					get = true;
					//calculate the result.
					model m = s.get_model();
					Graph temp(this->graph.size);
					for (unsigned i = 0; i < currentgraph.points.size()/2; ++i) 
					{
				        for (unsigned j = 0; j < currentgraph.size*currentgraph.size; ++j)
				        	if (eq(m.eval(x[i * currentgraph.size*currentgraph.size + j]),c.bool_val(true)))
				        	{
				        		temp.index[j%temp.size][j/temp.size] = 
				        		currentgraph.index[currentgraph.points[i*2].x][currentgraph.points[i*2].y]; 
				        	}
					}
					//print the result
					stringstream xx;
					xx<<s.upper(h1);
					int length;
					xx>>length;
					if (length < minimum) 
					{
						minimum = length;
						solution = temp;
					}
				}
			}
		}
		currentnum --;
	}

	
	cout<<"total connected number: "<< currentnum + 1<<endl;
	cout<<"total length: "<< minimum <<endl;
	//return
	if (get)
	{
    	return true;
	}
	else
	{
		printf("unsat\n");
		return false;
	} 
	return false;
}

Graph SAT::GetSolution()
{
	return this->solution;
}

