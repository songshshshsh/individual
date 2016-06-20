#include "SAT1.h"
#include "SAT2.h"
#include "SAT3.h"
#include "way.h"
#include "Strategy.h"
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

	Way way(new SAT1(graph));
	clock_t start =clock();
	way.getWay()->computeroute();
	clock_t end = clock();
	cout<<"run time of algorthm1: "<<(double) (end - start) / CLOCKS_PER_SEC<<" s"<<endl;


	way.setWay(new SAT2(graph));
	start =clock();
	way.getWay()->computeroute();
	end = clock();
	cout<<"run time of algorthm2: "<<(double) (end - start) / CLOCKS_PER_SEC<<" s"<<endl;

	way.setWay(new SAT3(graph));
	start =clock();
	way.getWay()->computeroute();
	end = clock();
	cout<<"run time of algorthm3: "<<(double) (end - start) / CLOCKS_PER_SEC<<" s"<<endl;

	//output the solution
	Graph output = way.getWay()->GetSolution();
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
