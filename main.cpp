#include <cstdio>
#include <iostream>
#include <queue>
#include<deque>
#include "Input.h"
#include "parser.cc"
using namespace std;

#define MAX(a, b) ((a) > (b) ? (a) : (b))
// Node structure to store information of decision
struct node
{
	// level  --> Level of node in decision tree (or index
	//             in arr[]
	// profit --> Profit of nodes on path from root to this
	//            node (including this node)
	// bound ---> Upper bound of maximum profit in subtree
	//            of this node
	// machine--> A set containing machines owned by CVCM at the given node
	int level, profit, bound;
	deque<int>machine;
	bool operator<(const node& rhs) const
	{
		return profit < rhs.profit;
	}
};

int lb(const Test &test)
{
	int lower = test.C;
	for(int i = 0; i != test.N; i++)
	{
		int temp = 0;
		if(test.m[i].P < test.C) temp = (test.D - test.m[i].D) * test.m[i].G - test.m[i].P + test.m[i].R + test.C;
		if(lower < temp) lower = temp;
	}
	return lower;
}

int Pmin(const int &day, const Test &test)
{
	int P = 80000;
	for(int i = 0; i < test.N; i++)
	{
		if(day < test.m[i].D && P > test.m[i].P) P = test.m[i].P;
	}
	return P;
}

int Rmax(const int &day, const Test &test)
{
	int R = 0;
	for(int i = 0; i < test.N; i++)
	{
		if(day < test.m[i].D && R < test.m[i].R) R = test.m[i].R;
	}
	return R;
}

int Gmax(const int &day, const Test &test)
{
	int G = 0;
	for(int i = 0; i < test.N; i++)
	{
		if(day < test.m[i].D && G < test.m[i].G) G = test.m[i].G;
	}
	return G;
}

int calculateBound(const int &day, const Test &test, const deque<int>&machine)
{
	int bound = 0;
	if(machine.empty()) bound = (test.D - day) * Gmax(day, test) + Rmax(day, test) - Pmin(day, test);
	else bound = (test.D - day) * MAX(Gmax(day, test),test.m[machine.front()].G);
	return bound;
}

int profit(const Test &test)
{
	int maxprofit = test.C;
	map<int, vector<int>>level = test.level;

	if(test.N > 0)
	{
		maxprofit = lb(test);
		// make a queue for traversing the node
		priority_queue<node> Q;
		node u, v;

		// dummy node at starting
		v.level = 0;
		v.profit = test.C;
		v.bound = v.profit + calculateBound(0, test, v.machine);
		Q.push(v);

		while (!Q.empty())
		{
			v = Q.top();
			Q.pop();


			if(v.bound > maxprofit)// Check if node is still promising
			{
				
				map<int, vector<int>>::iterator iter = level.begin();
				map<int, vector<int>>::iterator iter1 = level.begin();
				advance(iter, v.level);
				if(v.level > 0) advance(iter1, v.level - 1);
	
				if(!v.machine.empty()) v.profit += (iter->first - 1 - iter1->first) * test.m[v.machine.front()].G;
				for(int i = 0; i < level.begin()->second.size(); i++)
				{
					if(v.profit >= test.m[iter->second[i]].P)
					{
						bool creatnode = 1;
						u.profit = v.profit - test.m[iter->second[i]].P + test.m[iter->second[i]].R;
						if(!v.machine.empty() && test.m[iter->second[i]].G < test.m[v.machine.front()].G) creatnode = 0;
						if(creatnode)
						{
							u.machine.push_front(iter->second[i]);
							u. bound = u.profit + calculateBound(iter->first, test, u.machine);
							u.level = v.level + 1;
							if(u.level < test.level.size()) Q.push(u);
							else
							{
								u.profit += (test.D - iter->first) * test.m[u.machine.front()].G;
							}
							if (u. profit > maxprofit)
								maxprofit = u. profit;
						}
					}
				}

				u.level = v.level + 1;
				if(!v.machine.empty()) 
				{
					v.profit += test.m[v.machine.front()].G;
				}
				u.profit = v.profit;
				if (u. profit > maxprofit)
					maxprofit = u. profit;
				u. bound = u.profit + calculateBound(iter->first, test, v.machine);
				u.machine = v.machine;
				if(u.level < test.level.size()) Q.push(u);
				else
				{
					if(!v.machine.empty()) v.profit += (test.D - iter->first) * test.m[v.machine.front()].G;
					if (v. profit > maxprofit)
						maxprofit = v. profit;
				}

			}

		}
	}

	return maxprofit;
}

int main()
{
	Instance ins; // Contains the information of an instance
	string s = "./instances/data.txt";
	ins = data(s); //The second argument specifies the number of runways

	for(int i = 0; i<ins.n_tests;i++)
		cout << "Case " << i+1 << ": " << profit(ins.test[i]) << endl;


	cout << endl;
	std::getchar();
	return 0;
}