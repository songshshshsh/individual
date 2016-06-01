#include "SAT.h"
#include <cstring>
#include <list>
#include <cstdio>
#include <iostream>
#include "z3++.h"

using namespace std;
using namespace z3;

const static int X[5] = {0,1,-1,0,0};
const static int Y[5] = {0,0,0,1,-1};

SAT::SAT(Graph _graph):graph(_graph)
{
	memset(direction,0,sizeof(direction));
	minimumlength = 20;
	vector<bool> temp;
	vector<vector<bool > > temptemp;
	temp.assign(this->graph.getsize()*this->graph.getsize(),0);
	temptemp.assign(this->graph.getsize()*this->graph.getsize(),temp);
	c.assign(this->graph.points.size(),temptemp);
}

void SAT::computeroute()
{
	// int left = 1,right = this->graph.getsize() * this->graph.getsize();
	// int middle = (left + right)/2;
	// int t;
	// while (left < right)
	// {
	// 	int currentlength = violentget(middle);
	// 	if (currentlength<minimumlength)
	// 	{
	// 		t = middle;
	// 		minimumlength = currentlength;
	// 		right = middle;
	// 		middle = (left + right)/2;
	// 	}
	// 	else
	// 	{
	// 		left = middle;
	// 		middle = (left + right)/2;
	// 	}
	// }
	if (!violentget()) printf("Violent can't be solved.\n");
	if (!optimizedget()) printf("Opt can't be solved\n");
}

bool SAT::violentget()
{	
	bool found = true;
	for (int i = 0;i <this->graph.points.size()/2;i++)
	{
		vector<vector<Point> > currentindexroute;
		vector<Point> currentroute;
		Point source = this->graph.points[2*i],target = this->graph.points[2*i+1];
		dfs(source,target,0,currentroute,currentindexroute);
		if (currentindexroute.size() == 0)
		{
			found = false;
			break;
		}
		else routes.push_back(currentindexroute);
	}
	if (!found) return false;
	vector<int> routenumber;
	PickUpARoute(0,routenumber);
	return true;
}	

void SAT::dfs(const Point a,const Point b,const int lastdir,
	vector<Point>& currentroute,vector<vector<Point> >& currentindexroute)
{
	if (lastdir == 0) currentroute.push_back(a);
	int m_sourceIndex = computeindex(a.getx(),a.gety());
	int m_targetIndex = computeindex(b.getx(),b.gety());
	if (m_sourceIndex == m_targetIndex)
	{
		currentindexroute.push_back(currentroute);
	}
	for (int i = 1;i <= 4;i++)
		if (i != lastdir 
			&& a.getx()+X[i] >= 0&&a.getx()+X[i]<this->graph.getsize()
			&& a.gety()+Y[i] >= 0&&a.gety()+Y[i]<this->graph.getsize())
		{
			Point currentpoint(a.getx()+X[i],a.gety()+Y[i]);
			if (checkroute(currentpoint,currentroute))
			{
				currentroute.push_back(currentpoint);
				dfs(currentpoint,b,i,currentroute,currentindexroute);
				currentroute.pop_back();
			}
		}
}

bool SAT::checkroute(const Point& a,const vector<Point>& currentroute)
{
	for (int i = 0;i < currentroute.size()-1;i++)
		for (int j = 1;j <= 4;j++)
			if (a == Point(currentroute[i].getx()+X[j],currentroute[i].gety()+Y[j])) return false;
	return true;
}

void SAT::PickUpARoute(const int index,vector<int>& routenumber)
{
	if (index == this->graph.points.size())
	{
		check(routenumber);
	}
	else
	{
		for (int i = 0;i < routes[index].size();i++)
		{
			routenumber.push_back(i);
			PickUpARoute(index+1,routenumber);
			routenumber.pop_back();
		}
	}
}

void SAT::check(const vector<int>& routenumber)
{

}

int SAT::computeindex(int x,int y)
{
	return y*this->graph.getsize()+x;
}

void SAT::computexy(int index,int& x,int& y)
{
	x = index % this->graph.getsize();
	y = index / this->graph.getsize();
}

bool SAT::optimizedget()
{
	cout<<"optimizedget"<<endl;

	context c;

	expr_vector x(c);

	for (unsigned int i = 0;i < this->graph.points.size()/2;++i)
		for (unsigned int j = 0;j < this->graph.getsize()*this->graph.getsize();++j)
		{
			std::stringstream x_name;

			x_name << "x_"<<i<<'_'<<j;
			x.push_back(c.bool_const(x_name.str().c_str()));
		}
	optimize s(c);
	params p(c);
    p.set("priority",c.str_symbol("pareto"));
    s.set(p);

    //optimize parameters

    printf("233\n");

	//source and target configuration
	for (unsigned i = 0;i < this->graph.points.size()/2;i++)
	{
		int temp = i * this->graph.getsize()*this->graph.getsize() + computeindex(this->graph.points[2*i].getx(),this->graph.points[2*i].gety());
		s.add(x[temp]);
		temp = i * this->graph.getsize()*this->graph.getsize() + computeindex(this->graph.points[2*i+1].getx(),this->graph.points[2*i+1].gety());
		s.add(x[temp]);
	}

	    printf("233\n");

	//droplet movement
	for (unsigned i = 0;i < this->graph.points.size()/2;i++)
		for (unsigned j = 0;j <this->graph.getsize()*this->graph.getsize();j++)
		{
			expr conjecture =  !x[i*this->graph.getsize()*this->graph.getsize()+j];
			for (int k = 1;k <= 4;k++)
				if (int(j + X[k]+Y[k] *this->graph.getsize()) >= 0 &&
				 j + X[k] +Y[k] *this->graph.getsize() < this->graph.getsize()*this->graph.getsize())
				conjecture = conjecture || x[i*this->graph.getsize()*this->graph.getsize()+j + X[k] + Y[k]*this->graph.getsize()];
			s.add(conjecture);
		}

    printf("233\n");

	//fluidic constraints
	try
	{
		for (unsigned j = 0;j < this->graph.getsize()*this->graph.getsize();j++)
		{
				expr temp(c);
				printf("233\n");
				for (unsigned k = 0;k < this->graph.points.size()/2;k++)
				{
					printf("233\n");
					expr conjecture = x[k*this->graph.getsize()*this->graph.getsize()+j];
					for (unsigned i = 0;i < this->graph.points.size()/2;i++)
						if (i != k)
							conjecture = conjecture && (!x[i*this->graph.getsize()*this->graph.getsize()+j]);
					printf("233\n");
					temp = temp || conjecture;
					printf("233\n");
				} 
			
				s.add(temp);
				
				printf("233\n");
		}
	}
	catch(z3::exception &ex)
	{
		cout<<"exception: "<<ex<<endl;
		printf("233\n");
	}

    printf("233\n");


	//blockages
	for (unsigned k = 0;k < this->graph.points.size();k++)
	{
		expr temp(c);
		for (unsigned i = 0;i < this->graph.blocks.size();i++)
		{
			int p = this->graph.blocks[i].getx()+this->graph.blocks[i].gety()*this->graph.getsize();
			temp = temp || x[k*this->graph.getsize()*this->graph.getsize()+p];
		}
		s.add(!temp);
	}
	    printf("233\n");
	    printf("!!!233\n");


    expr z(c);

    for (unsigned i = 0;i < this->graph.points.size()/2;i++)
    	for (unsigned j = 0;j < this->graph.getsize()*this->graph.getsize();j++)
    		z = z + x[i*this->graph.getsize()*this->graph.getsize()+j];
    optimize::handle h1 = s.minimize(z);

        printf("233\n");




	if (sat == s.check())
	{
		model m = s.get_model();
		for (unsigned i = 0; i < this->graph.points.size()/2; ++i) 
		{
	        for (unsigned j = 0; j < this->graph.getsize()*this->graph.getsize(); ++j)
	            std::cout << m.eval(x[i * this->graph.getsize()*this->graph.getsize() + j]);
	        std::cout << '\n';
    	}
    	return true;
	}
	else
	{
		printf("unsat\n");
		return false;
	} 
	return false;
}

// bool SAT::checkblocking()
// {

// }

// bool SAT::checkcrossing()
// {

// }


// bool checkconsistency()
// {

// }

// void SAT::checkfluidic(int index,solver& s,expr& conjecture)
// {
	

// }

// void sudoku_example() {
//     std::cout << "sudoku example\n";

//     context c;

//     // 9x9 matrix of integer variables 
//     expr_vector x(c);
//     for (unsigned i = 0; i < 9; ++i)
//         for (unsigned j = 0; j < 9; ++j) {
//             std::stringstream x_name;

//             x_name << "x_" << i << '_' << j;
//             x.push_back(c.int_const(x_name.str().c_str()));
//         }

//     solver s(c);

//     // each cell contains a value in {1, ..., 9}
//     for (unsigned i = 0; i < 9; ++i)
//         for (unsigned j = 0; j < 9; ++j) {
//             s.add(x[i * 9 + j] >= 1 && x[i * 9 + j] <= 9);
//         }

//     // each row contains a digit at most once
//     for (unsigned i = 0; i < 9; ++i) {
//         expr_vector t(c);
//         for (unsigned j = 0; j < 9; ++j)
//             t.push_back(x[i * 9 + j]);
//         s.add(distinct(t));
//     }

//     // each column contains a digit at most once
//     for (unsigned j = 0; j < 9; ++j) {
//         expr_vector t(c);
//         for (unsigned i = 0; i < 9; ++i)
//             t.push_back(x[i * 9 + j]);
//         s.add(distinct(t));
//     }

//     // each 3x3 square contains a digit at most once
//     for (unsigned i0 = 0; i0 < 3; i0++) {
//         for (unsigned j0 = 0; j0 < 3; j0++) {
//             expr_vector t(c);
//             for (unsigned i = 0; i < 3; i++)
//                 for (unsigned j = 0; j < 3; j++)
//                     t.push_back(x[(i0 * 3 + i) * 9 + j0 * 3 + j]);
//             s.add(distinct(t));
//         }
//     }

//     // sudoku instance, we use '0' for empty cells
//     int instance[9][9] = {{0,0,0,0,9,4,0,3,0},
//                           {0,0,0,5,1,0,0,0,7},
//                           {0,8,9,0,0,0,0,4,0},
//                           {0,0,0,0,0,0,2,0,8},
//                           {0,6,0,2,0,1,0,5,0},
//                           {1,0,2,0,0,0,0,0,0},
//                           {0,7,0,0,0,0,5,2,0},
//                           {9,0,0,0,6,5,0,0,0},
//                           {0,4,0,9,7,0,0,0,0}};

//     for (unsigned i = 0; i < 9; i++)
//         for (unsigned j = 0; j < 9; j++)
//             if (instance[i][j] != 0)
//                 s.add(x[i * 9 + j] == instance[i][j]);

//     std::cout << s.check() << std::endl;
//     std::cout << s << std::endl;

//     model m = s.get_model();
//     for (unsigned i = 0; i < 9; ++i) {
//         for (unsigned j = 0; j < 9; ++j)
//             std::cout << m.eval(x[i * 9 + j]);
//         std::cout << '\n';
//     }
// }


