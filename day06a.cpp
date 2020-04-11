/*
--- Day 6: Universal Orbit Map ---

You've landed at the Universal Orbit Map facility on Mercury. Because navigation in space often involves transferring between orbits, the orbit maps here are useful for finding efficient routes between, for example, you and Santa. You download a map of the local orbits (your puzzle input).

Except for the universal Center of Mass (COM), every object in space is in orbit around exactly one other object.

Our challenge is to find the total number of orbits, indirect or direct. F.E. COM)bbb)ccc is c orbiting b, which is itself orbiting COM
There are a total of 3 orbits, 1 direct from bbb, and 1 direct and 1 indirect from ccc
*/
#include <iostream>
#include <string.h>
#include <map>
#include <vector>

using namespace std;

struct orbitPair
{
	string* parent;
	string* child;
	
	orbitPair(string par, string chi)
	{
		parent = new string(par);
		child = new string(chi);
	}
};

struct orbitList //LL for orbits we haven't figured out where they go in the universe yet
{
	orbitList* next;
	orbitPair* data;
	
	orbitList(orbitList* nex, orbitPair* pair)
	{
		next = nex;
		data = pair;
	}
};

//Reads the parent-child lists from standard in, in the format of parent)child
orbitList* readOrbits()
{
	string parent = string("   ");
	string child = string("   ");
	orbitPair* orbPair;
	orbitList* orbList = NULL;
	while(scanf("%3s)%3s", &parent[0], &child[0]) == 2) //Ugly hack is ugly
	{
		orbPair = new orbitPair(parent, child);
		orbList = new orbitList(orbList, orbPair);
		cout << parent.data() << ")" << child.data() << endl;
	}
	return orbList;
}


int main()
{
	//printf("Running\n");
	readOrbits();
	return 0;
}

