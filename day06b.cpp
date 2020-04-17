/*
--- Day 6: Universal Orbit Map ---

You've landed at the Universal Orbit Map facility on Mercury. Because navigation in space often involves transferring between orbits, the orbit maps here are useful for finding efficient routes between, for example, you and Santa. You download a map of the local orbits (your puzzle input).

Except for the universal Center of Mass (COM), every object in space is in orbit around exactly one other object.

Our challenge is to find the total number of orbits, indirect or direct. F.E. COM)bbb)ccc is c orbiting b, which is itself orbiting COM
There are a total of 3 orbits, 1 direct from bbb, and 1 direct and 1 indirect from ccc
*/
#include <iostream>
#include <string.h>
#include <unordered_map>
#include <vector>

using namespace std;

struct orbitBody
{
	bool visited = false; //Tracks if we've visited this body for this path searched
	vector<orbitBody*> adjacents = vector<orbitBody*>();
	
	orbitBody(orbitBody* parent)
	{
		adjacents.emplace_back(parent);
	}
};

//Basically a fancy pair with better names that first and second
struct orbitPair
{
	string parent;
	string child;
	
	orbitPair(string par, string chi)
	{
		parent = par;
		child = chi;
	}
};

struct orbitList //LL for orbits we haven't figured out where they go in the universe yet
{
	orbitList* next;
	orbitList* last;
	orbitPair* data;

	
	orbitList(orbitList* nex, orbitList* las, orbitPair* pair)
	{
		next = nex;
		last = las;
		data = pair;
	}
};

typedef unordered_map<string, int> Universe; //Name of body, steps to reach the COM


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
		
		orbList = new orbitList(orbList, NULL, orbPair);
		if(orbList->next != NULL)
			orbList->next->last = orbList;
		
		
	}
	return orbList;
}

//Dumps parent)child pairs that haven't been added to the universe yet
void dumpOrbList(orbitList* list)
{
	orbitList* curCel = list;
	cout << "These pairs still need a home:\n";
	while(curCel != NULL)
	{
		printf("%s)%s\n", curCel->data->parent.data(), curCel->data->child.data());
		curCel = curCel->next;
	}
}

void dumpUniverse(Universe* univ)
{
	cout << "The universe so far... (Body name, Steps to COM)\n";
	for(auto it = univ->begin(); it != univ->end(); it++)
	{
		printf("%s %i\n", it->first.data(), it->second);
	}
}

//Puts a child into orbit if the parent is already present, returning the steps to COM, -1 otherwise
//Cleans up memory objects that are no longer needed
bool injectOrbit(Universe* universe, orbitPair* orbPair)
{
	int distance = -99999; //Until the universe object is reworked for B side
	if(orbPair->parent.compare("COM") == 0) //Does it directly orbit COM?
	{
		universe->emplace(orbPair->child, 0);
		delete orbPair;
		return true;
	}
	else //Maybe the parent is present?
	{
		auto iter = universe->find(orbPair->parent);
		if(iter != universe->end()) //The parent exists
		{
			distance = iter->second + 1;
			universe->emplace(orbPair->child, distance);
			delete orbPair;
			return true;
		}
		return false; //Couldn't find where to insert it. Maybe next time?
	}
}

//Let's find homes for all these things floating in space!
void buildUniverse(Universe* universe, orbitList* orbList)
{
	orbitList* curCel = orbList;
	orbitList* delCel = NULL; //If we need to delete a cell
	
	while(orbList != NULL)
	{
		if(injectOrbit(universe, curCel->data)) //We found the parent (or COM) in the universe!
		{
			if(curCel->last == NULL) //Are we looking at the first cell?
				orbList = curCel->next;
			else
				curCel->last->next = curCel->next;
			
			
			if(curCel->next != NULL) //Looking at the last cell?
				curCel->next->last = curCel->last;
			
			
			delCel = curCel;
			curCel = curCel->next;
			delete delCel;
		}
		else
			curCel = curCel->next;
		
		if(curCel == NULL)
		{
			curCel = orbList;
		}
	}
}


int main()
{
	Universe* universe = new Universe(); //No programmer should have this much power
	orbitList* orbList = readOrbits();
	buildUniverse(universe, orbList);
	
	return 0;
}

