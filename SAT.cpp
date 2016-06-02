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

void SAT::initializeX(expr_vector& x,context& c)
{
	for (unsigned int i = 0;i < this->graph.points.size()/2;++i)
		for (unsigned int j = 0;j < this->graph.size*this->graph.size;++j)
		{
			std::stringstream x_name;

			x_name << "x_"<<i<<'_'<<j;
			x.push_back(c.bool_const(x_name.str().c_str()));
		}
}

void SAT::movement(optimize& s,context& c,expr_vector& x)
{
	for (unsigned i = 0;i < this->graph.points.size()/2;i++)
		for (unsigned j = 0;j <this->graph.size*this->graph.size;j++)
		{
			int tem = computeindex(this->graph.points[2*i].x,this->graph.points[2*i].y);
			int start = computeindex(this->graph.points[2*i+1].x,this->graph.points[2*i+1].y);
			int currentx,currenty; 
			computexy(j,currentx,currenty);
			expr constfalse = c.bool_val(false);
			if (j == tem || j == start)
			{
				expr tempted = bool_to_int(constfalse,c,false);
				for (int k = 1;k <= 4;k++)
				{
					int newk = j + X[k]+Y[k] *this->graph.size;
					int newkx,newky;
					computexy(newk,newkx,newky);
					if ((abs(newkx - currentx) + abs(newky - currenty)) == 1)
					if (int(j + X[k]+Y[k] *this->graph.size) >= 0 &&
					 j + X[k] +Y[k] *this->graph.size < this->graph.size*this->graph.size)
						tempted = tempted + bool_to_int(x[i*this->graph.size*this->graph.size+newk],c,false);
				}
				s.add(tempted == 1 && x[i*this->graph.size*this->graph.size+ j] || !x[i*this->graph.size*this->graph.size+ j] );
			}
			else 
			{
				expr conjecture =  !x[i*this->graph.size*this->graph.size+j];
				expr tempted = bool_to_int(constfalse,c,false);
				for (int k = 1;k <= 4;++k)
				{
						int newk = j + X[k]+Y[k] *this->graph.size;
						int newkx,newky;
						computexy(newk,newkx,newky);
						if (newk >= 0 &&
						 newk < this->graph.size*this->graph.size
						 	&& (newkx == currentx || newky == currenty))
						 	tempted = tempted + bool_to_int(x[i*this->graph.size*this->graph.size+newk],c,false);
				}
				s.add((tempted == 2) || (conjecture));
			}
		}
}

void SAT::fluidic(context& c,optimize& s,expr_vector& x)
{
	for (unsigned j = 0;j < this->graph.size*this->graph.size;j++)
	{
			expr temp = c.bool_val(false);
			for (unsigned k = 0;k < this->graph.points.size()/2;k++)
			{
				expr conjecture = x[k*this->graph.size*this->graph.size+j];
				for (unsigned i = 0;i < this->graph.points.size()/2;i++)
					if (i != k)
						conjecture = conjecture && (!x[i*this->graph.size*this->graph.size+j]);
				temp = temp || conjecture;
			} 
			expr conjecture = c.bool_val(true);
			for (unsigned k = 0;k < this->graph.points.size()/2;k++)
				conjecture = conjecture && (!x[k*this->graph.size*this->graph.size+j]);
			temp = temp || conjecture;
			s.add(temp);
	}
}

void SAT::block(context& c,optimize& s,expr_vector& x)
{
	for (unsigned i = 0;i < this->graph.blocks.size();i++)
	{
		expr temp = c.bool_val(false);
		expr con = temp;
		int p = computeindex(this->graph.blocks[i].x,this->graph.blocks[i].y);
		for (unsigned k = 0;k < this->graph.points.size()/2;k++)
			con = con || x[k*this->graph.size*this->graph.size+p];
		s.add(!con);
	}
}

void SAT::optimizeRoute(context& c,optimize& s,expr_vector& x,expr& z)
{
	for (unsigned i = 0;i < this->graph.points.size()/2;i++)
    {
    	int tem = computeindex(this->graph.points[2*i].x,this->graph.points[2*i].y);
		int start = computeindex(this->graph.points[2*i+1].x,this->graph.points[2*i+1].y);
    	for (unsigned j = 0;j < this->graph.size*this->graph.size;j++)
    	{
    		bool special = false;
    		if (j == tem || j == start) special = true;
    		z = z + bool_to_int(x[i*this->graph.size*this->graph.size+j],c,special);
    	}
    }
}

bool SAT::optimizedget()
{
	cout<<"optimizedget"<<endl;

	context c;
	expr_vector x(c);


	initializeX(x,c);


	optimize s(c);
	params p(c);
    p.set("priority",c.str_symbol("pareto"));
    s.set(p);

    //optimize parameters

	//droplet movement
	movement(s,c,x);

	//fluidic constraints
	fluidic(c,s,x);

	//blockages
	block(c,s,x);


    expr f = c.bool_const("f");
    expr z = bool_to_int(f,c,false);
    s.add(!f);

    //define the optimizing parameter z
    optimizeRoute(c,s,x,z);
    
    optimize::handle h1 = s.minimize(z);

    bool get = false;
		
	if (sat == s.check())
	{
		get = true;
	}

	//calculate the result.
	model m = s.get_model();
	for (unsigned i = 0; i < this->graph.points.size()/2; ++i) 
	{
        for (unsigned j = 0; j < this->graph.size*this->graph.size; ++j)
        	if (eq(m.eval(x[i * this->graph.size*this->graph.size + j]),c.bool_val(true)))
        	{
        		solution.index[j%solution.size][j/solution.size] = i + 1; 
        	}
	}
	//print the result
	stringstream xx;
	xx<<s.upper(h1);
	int length;
	xx>>length;
	cout<<"total connected number: "<< (this->graph.points.size() * 1000 + 1000 - length)/2000<<endl;
	cout<<"total length: "<< length % 1000<<endl;
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

