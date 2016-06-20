#ifndef SAT3_H
#define SAT3_H

#include "Graph.h"
#include "point.h"
#include "Strategy.h"
#include <vector>
#include "z3++.h"

class SAT3: public Strategy
{
public:
	SAT3(Graph _graph):Strategy(_graph){};
private:
 	bool optimizedget();
};

#endif