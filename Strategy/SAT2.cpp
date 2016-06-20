#include "SAT2.h"
#include <cstring>
#include <list>
#include <cstdio>
#include <iostream>
#include "z3++.h"

using namespace std;
using namespace z3;

bool SAT2::optimizedget()
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
