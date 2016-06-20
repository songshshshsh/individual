#include "SAT1.h"
#include <cstring>
#include <list>
#include <cstdio>
#include <iostream>
#include "z3++.h"

using namespace std;
using namespace z3;

bool SAT1::optimizedget()
{
	cout<<"optimizedget"<<endl;

	context c;
	expr_vector x(c);


	initializeX(x,c,graph);


	optimize s(c);
	params p(c);
    p.set("priority",c.str_symbol("pareto"));
    s.set(p);

    //optimize parameters

	//droplet movement
	movement(s,c,x,graph);

	//fluidic constraints
	fluidic(c,s,x,graph);

	//blockages
	block(c,s,x,graph);


    expr f = c.bool_const("f");
    expr z = bool_to_int(f,c,false);
    s.add(!f);

    //define the optimizing parameter z
    optimizeRoute(c,s,x,z,graph);
    
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


