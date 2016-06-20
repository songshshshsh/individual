#ifndef SAT2_H
#define SAT2_H

#include "Graph.h"
#include "point.h"
#include "Strategy.h"
#include <vector>
#include "z3++.h"

class SAT2: public Strategy
{
public:
	SAT2(Graph _graph):Strategy(_graph){};
private:
 	bool optimizedget();
};

#endif