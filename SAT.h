#ifndef SAT_H
#define SAT_H

#include "Graph.h"
#include "point.h"
#include <vector>
#include "z3++.h"

class SAT
{
public:
	SAT(Graph);
	void computeroute();
	Graph GetSolution();
private:
	Graph graph,solution;
	z3::expr bool_to_int(z3::expr a,z3::context& c,bool special);
 	bool optimizedget();
	int computeindex(int,int);
	void computexy(int,int&,int&);
	void initializeX(z3::expr_vector&,z3::context&);
	void movement(z3::optimize&,z3::context&,z3::expr_vector&);
	void fluidic(z3::context&,z3::optimize&,z3::expr_vector&);
	void block(z3::context&,z3::optimize&,z3::expr_vector&);
	void optimizeRoute(z3::context&,z3::optimize&,z3::expr_vector&,z3::expr&);
};

#endif