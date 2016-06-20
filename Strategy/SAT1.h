#ifndef SAT1_H
#define SAT1_H

#include "Graph.h"
#include "point.h"
#include "Strategy.h"
#include <vector>
#include "z3++.h"

class SAT1 : public Strategy
{
public:
	SAT1(Graph _graph):Strategy(_graph){};
private:
 	bool optimizedget();
};

#endif