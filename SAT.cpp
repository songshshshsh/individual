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

const static int a[9] = {0,0,0,1,1,1,-1,-1,-1};
const static int b[9] = {0,1,-1,-1,0,1,-1,0,1};

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

	
	//if (!violentget()) printf("Violent can't be solved.\n");
	
	//cout << "womeidiao" << endl;
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

expr bool_to_int(expr a,context& c,bool special)
{
	if (!special)
	{
		expr one  = c.int_val(1);
	    expr zero = c.int_val(0);
		return to_expr(c,Z3_mk_ite(c,a,one,zero));
	}
	else
	{
		expr one  = c.int_val(1);
	    expr zero = c.int_val(1000);
		return to_expr(c,Z3_mk_ite(c,a,one,zero));
	}
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
    cout << "asdfasdfasdf"<< endl;


	//source and target configuration
	// for (unsigned i = 0;i < this->graph.points.size()/2;i++)
	// {
	// 	int temp = i * this->graph.getsize()*this->graph.getsize() + computeindex(this->graph.points[2*i].getx(),this->graph.points[2*i].gety());
	// 	s.add(x[temp]);
	// 	temp = i * this->graph.getsize()*this->graph.getsize() + computeindex(this->graph.points[2*i+1].getx(),this->graph.points[2*i+1].gety());
	// 	s.add(x[temp]);
	// }

	cout<<"source and target"<<endl;


	//droplet movement
	for (unsigned i = 0;i < this->graph.points.size()/2;i++)
		for (unsigned j = 0;j <this->graph.getsize()*this->graph.getsize();j++)
		{
			int tem = this->graph.points[2*i].getx() + this->graph.getsize() *this->graph.points[2*i].gety();
			int start = this->graph.points[2*i+1].getx() + this->graph.getsize() *this->graph.points[2*i+1].gety();
			int currentx = j % this->graph.getsize();
			int currenty = j / this->graph.getsize();
			if (j == tem || j == start)
			{
				expr constfalse = c.bool_val(false);
				expr tempted = bool_to_int(constfalse,c,false);
				for (int k = 1;k <= 4;k++)
				{
					int newk = j + X[k]+Y[k] *this->graph.getsize();
					int newkx = newk % this->graph.getsize();
					int newky = newk / this->graph.getsize();
					if ((abs(newkx - currentx) + abs(newky - currenty)) == 1)
					if (int(j + X[k]+Y[k] *this->graph.getsize()) >= 0 &&
					 j + X[k] +Y[k] *this->graph.getsize() < this->graph.getsize()*this->graph.getsize())
						tempted = tempted + bool_to_int(x[i*this->graph.getsize()*this->graph.getsize()+newk],c,false);
				}
				s.add(tempted == 1 && x[i*this->graph.getsize()*this->graph.getsize()+ j] || !x[i*this->graph.getsize()*this->graph.getsize()+ j] );
				//cout<<conjecture<<endl;
			}
			else 
			{
				expr conjecture =  !x[i*this->graph.getsize()*this->graph.getsize()+j];
				expr constfalse = c.bool_val(false);
				expr tempted = bool_to_int(constfalse,c,false);
				for (int k = 1;k <= 4;++k)
				{
						int newk = j + X[k]+Y[k] *this->graph.getsize();
						int newkx = newk % this->graph.getsize();
						int newky = newk / this->graph.getsize();
						if (newk >= 0 &&
						 newk < this->graph.getsize()*this->graph.getsize()
						 	&& (newkx == currentx || newky == currenty))
						 	tempted = tempted + bool_to_int(x[i*this->graph.getsize()*this->graph.getsize()+newk],c,false);
				}
				//cout<<tempted<<endl;
				try{
					s.add((tempted == 2 && !conjecture) || (conjecture));
				} catch(z3::exception& ex)
				{
					cout<<"exception: "<<ex<<endl;
				}
				// expr fivegrids = c.bool_val(true);
				// for (int k = 1;k <= 4;k++)
				// 	for (int u = 1;u <= 4; u++)
				// 	{
				// 		int newu = j + X[u]+Y[u] *this->graph.getsize();
				// 		int newux = newu % this->graph.getsize();
				// 		int newuy = newu / this->graph.getsize();
				// 		int newk = j + X[k]+Y[k] *this->graph.getsize();
				// 		int newkx = newk % this->graph.getsize();
				// 		int newky = newk / this->graph.getsize();
				// 		if (u != k 
				// 			&& newu >= 0 &&
				// 		 newu < this->graph.getsize()*this->graph.getsize()
				// 			&& newk >= 0 &&
				// 		 newk < this->graph.getsize()*this->graph.getsize()
				// 		 	&& (newux == currentx || newuy == currenty) 
				// 		 	&& (newkx == currentx || newky == currenty))
				// 		{
				// 			// expr added = c.bool_val(true);
				// 		 // 	for (int p = 1;p <= 8; ++p)
				// 		 // 		for (int q = 1;q <= 8; ++q)
				// 		 // 		{
				// 		 // 			int newpq =  j + a[p]+b[q] *this->graph.getsize();
				// 		 // 			if (newpq >=0 && newpq < this->graph.getsize() * this->graph.getsize())
				// 		 // 			{
				// 			//  			if (((a[p] == X[u]) && (b[q] == Y[u]))|| ((a[p] == X[k]) && (b[q] == Y[k])))
				// 			// 				added = added && x[i*this->graph.getsize()*this->graph.getsize()+newpq];
				// 			// 			else 
				// 			// 			{
				// 			// 				added = added && (!x[i*this->graph.getsize()*this->graph.getsize()+newpq]);
				// 			// 			}
				// 			// 		}
				// 			// 	}
				// 			// fivegrids = fivegrids || added;
							
				// 			//cout<<X[u]<<Y[u]<<endl;
				// 			//cout<<X[k]<<Y[k]<<endl;
				// 			expr added = c.bool_val(true);
				// 			if ( (newkx == newux) || (newky == newuy) )
				// 			{
				// 			 	for (int p = 1;p <= 4; ++p)
				// 			 		{
				// 			 			int newp =  j + X[p]+Y[p] *this->graph.getsize();
				// 			 			if (newp >=0 && newp < this->graph.getsize() * this->graph.getsize())
				// 			 			{
				// 				 			if ((newp == newu)|| (newp == newk))
				// 								added = added && x[i*this->graph.getsize()*this->graph.getsize()+newp];
				// 							else 
				// 							{
				// 								added = added && (!x[i*this->graph.getsize()*this->graph.getsize()+newp]);
				// 							}
				// 						}
				// 					}
				// 				fivegrids = fivegrids || added;
				// 				cout<<"added"<<added<<endl;
				// 			}
				// 			else
				// 			{
				// 			 	for (int p = 1;p <= 8; ++p)
				// 			 		for (int q = 1;q <= 8; ++q)
				// 			 		{
				// 			 			int newpq =  j + a[p]+b[q] *this->graph.getsize();
				// 			 			int newpqx = newpq % this->graph.getsize();
				// 			 			int newpqy = newpq / this->graph.getsize();
				// 			 			if (newpq >=0 && newpq < this->graph.getsize() * this->graph.getsize()
				// 			 				&& (abs(newpqx - currentx) + abs(newpqy - currenty) == 1) )
				// 			 			{
				// 				 			if (((a[p] == X[u]) && (b[q] == Y[u]))|| ((a[p] == X[k]) && (b[q] == Y[k])))
				// 								added = added && x[i*this->graph.getsize()*this->graph.getsize()+newpq];
				// 							else 
				// 							{
				// 								added = added && (!x[i*this->graph.getsize()*this->graph.getsize()+newpq]);
				// 							}
				// 						}
				// 					}
				// 				fivegrids = fivegrids || added;
				// 				cout<<"added0"<<added<<endl;
				// 			}
				// 		}
				// 	}
				// conjecture = conjecture && fivegrids;
				//s.add(conjecture);
				//cout<<i<<' '<<j<<endl;
				//cout<<conjecture<<endl;
			}
		}

	cout<<"droplet"<<endl<<endl;



	//fluidic constraints
	try
	{
		for (unsigned j = 0;j < this->graph.getsize()*this->graph.getsize();j++)
		{
				expr temp = c.bool_val(false);
				for (unsigned k = 0;k < this->graph.points.size()/2;k++)
				{
					expr conjecture = x[k*this->graph.getsize()*this->graph.getsize()+j];
					for (unsigned i = 0;i < this->graph.points.size()/2;i++)
						if (i != k)
							conjecture = conjecture && (!x[i*this->graph.getsize()*this->graph.getsize()+j]);
					temp = temp || conjecture;
				} 
				expr conjecture = c.bool_val(true);
				for (unsigned k = 0;k < this->graph.points.size()/2;k++)
					conjecture = conjecture && (!x[k*this->graph.getsize()*this->graph.getsize()+j]);
				temp = temp || conjecture;
				//cout<<temp<<endl;
				s.add(temp);
		}
	}
	catch(z3::exception &ex)
	{
		cout<<"exception: "<<ex<<endl;
	}
	cout<<"fluidic"<<endl<<endl;


	//blockages
	for (unsigned i = 0;i < this->graph.blocks.size();i++)
	{
		expr temp = c.bool_val(false);
		expr con = temp;
		int p = this->graph.blocks[i].getx()+this->graph.blocks[i].gety()*this->graph.getsize();
		for (unsigned k = 0;k < this->graph.points.size()/2;k++)
			con = con || x[k*this->graph.getsize()*this->graph.getsize()+p];
		s.add(!con);
		//cout<<con<<endl;
	}

	cout<<"blockages"<<endl<<endl;


    expr f = c.bool_const("f");
    expr z = bool_to_int(f,c,false);
    s.add(!f);

    for (unsigned i = 0;i < this->graph.points.size()/2;i++)
    {
    	int tem = this->graph.points[2*i].getx() + this->graph.getsize() *this->graph.points[2*i].gety();
			int start = this->graph.points[2*i+1].getx() + this->graph.getsize() *this->graph.points[2*i+1].gety();
    	for (unsigned j = 0;j < this->graph.getsize()*this->graph.getsize();j++)
    	{
    		bool special = false;
    		if (j == tem || j == start) special = true;
    				try{
    		z = z + bool_to_int(x[i*this->graph.getsize()*this->graph.getsize()+j],c,special);
    		}
		catch(z3::exception & ex)
		{
			cout<<"exception: "<<ex<<endl;
		}
    	}
    }
    optimize::handle h1 = s.minimize(z);

    //cout<<s<<endl;


    Graph haha(this->graph.getsize());

    //model m = s.get_model();
    //cout<<m<<endl;
    cout<<"hhh"<<endl;

    bool get = false;
		
		if (sat == s.check())
		{
			get = true;
		}

		model m = s.get_model();
		for (unsigned i = 0; i < this->graph.points.size()/2; ++i) 
		{
	        for (unsigned j = 0; j < this->graph.getsize()*this->graph.getsize(); ++j)
	        	if (eq(m.eval(x[i * this->graph.getsize()*this->graph.getsize() + j]),c.bool_val(true)))
	        	{
	        		haha.index[j%haha.getsize()][j/haha.getsize()] = i + 1; 
	        		//cout<<x[i * this->graph.getsize()*this->graph.getsize() + j]<<endl;
	        	}
    	}
    	haha.printgraph();
		cout<<s.upper(h1)<<endl;
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


