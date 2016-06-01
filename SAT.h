#ifndef SAT_H
#define SAT_H

#include "Graph.h"
#include "point.h"
#include <vector>

class SAT
{
public:
	SAT(Graph);
	void computeroute();
private:
	Graph graph;
	int direction[Graph::maxsize][Graph::maxsize];
	std::vector<std::vector<std::vector<bool > > > c;
	std::vector<std::vector<std::vector<Point> > > routes;
	bool violentget();
	bool optimizedget();
	//bool checkblocking();
	//bool checkcrossing();
	//bool checkconsistency();
	//void checkfluidic(int,solver&,expr&);
	int minimumlength;
	void dfs(const Point,const Point,const int,std::vector<Point>&,std::vector<std::vector<Point> >&);
	bool checkroute(const Point&,const std::vector<Point>&);
	void PickUpARoute(const int,std::vector<int>&);
	void check(const std::vector<int>&);
	//static const int maxsize = 10;
	int computeindex(int,int);
	void computexy(int,int&,int&);
};

#endif