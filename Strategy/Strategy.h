#ifndef STRATEGY_H
#define STRATEGY_H


#include "Graph.h"
#include "point.h"
#include <vector>
#include "z3++.h"

class Strategy
{
public:
	Strategy(Graph);
	void computeroute();
	Graph GetSolution();
	virtual ~Strategy(){}
protected:
	Graph graph,solution;
	z3::expr bool_to_int(z3::expr a,z3::context& c,bool special);
 	virtual bool optimizedget() = 0;
	int computeindex(int,int);
	void computexy(int,int&,int&);
	void initializeX(z3::expr_vector&,z3::context&,Graph&);
	void movement(z3::optimize&,z3::context&,z3::expr_vector&,Graph&);
	void fluidic(z3::context&,z3::optimize&,z3::expr_vector&,Graph&);
	void block(z3::context&,z3::optimize&,z3::expr_vector&,Graph&);
	void setst(z3::optimize&,z3::context&,z3::expr_vector&,Graph&);
	int exp(const int);
	int checknum(int,int);
	void optimizeRoute(z3::context&,z3::optimize&,z3::expr_vector&,z3::expr&,Graph&);
};

#endif