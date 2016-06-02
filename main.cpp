#include "SAT.h"
#include "Graph.h"
#include "point.h"
#include "random.h"
#include <iostream>

using namespace std;

int main()
{
	int size,pairs,blocks;
	cin>>size>>pairs>>blocks;
	Random random(size,pairs,blocks);
	Graph graph = random.generate();
	graph.printgraph();
	SAT sat(graph);
	//cout<<"wodiaole!!!"<<endl;
	sat.computeroute();
	
	return 0;
}
