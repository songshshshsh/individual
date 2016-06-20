#include "SAT.h"
#include "Graph.h"
#include "point.h"
#include "random.h"
#include "test.h"
#include <iostream>
#include <ctime>

using namespace std;

int main()
{
	//input parameters deciding the size,pairs and blocks
	int size,pairs,blocks;
	cin>>size>>pairs>>blocks;

	//use random to generate
	Random random(size,pairs,blocks);
	random.generate();
	Graph graph(size,pairs,blocks,"random.txt");
	graph.printgraph(cout);

	//solve the sat problem
	SAT sat(graph);
	cout<<"Computing..."<<endl;
	clock_t start =clock();
	sat.computeroute();
	clock_t end = clock();
	cout<<"run time of algorthm: "<<(double) (end - start) / CLOCKS_PER_SEC<<" s"<<endl;
	//cout<<(double) (end - start) / CLOCKS_PER_SEC<<endl;

	//output the solution
	Graph output = sat.GetSolution();
	output.printgraph(cout);

	//test if it is valid
	cout<<"testing"<<endl;
	Test test(output);
	if (test.check())
		cout<<"valid"<<endl;
	else 
		cout<<"invalid"<<endl;

	return 0;
}
