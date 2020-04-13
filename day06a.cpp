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

auto compareStringPointers = [](const string* str1, const string* str2)
{
	printf("Comparing >>%s<< and >>%s<<\n", str1->data(), str2->data());
	return str1->compare(*str2);
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
int injectOrbit(Universe* universe, orbitPair* orbPair)
{
	int distance; //Distance to COM
	//cout << orbPair->parent->data() << endl;
	//cout << "Trying to find a home for " << orbPair->parent.data() << ")" << orbPair->child.data() << endl;
	if(orbPair->parent.compare("COM") == 0) //Does it directly orbit COM?
	{
		//cout << "Direct COM orbit found!\n";
		distance = 1;
		universe->emplace(orbPair->child, distance);
		delete orbPair;
		return distance;
	}
	else //Maybe the parent is present?
	{
		auto iter = universe->find(orbPair->parent);
		if(iter != universe->end()) //The parent exists
		{
			distance = iter->second + 1;
			universe->emplace(orbPair->child, distance);
			delete orbPair;
			return distance;
		}
		return -1; //Couldn't find where to insert it. Maybe next time?
	}
}


int main()
{
	Universe* universe = new Universe(); //No programmer should have this much power
	orbitList* orbList = readOrbits();
	
	int totalDepth = 0;
	int returnedDepth;
	orbitList* curCel = orbList;
	orbitList* delCel = NULL; //If we need to //delete a cell
	
	while(orbList != NULL)
	{
		returnedDepth = injectOrbit(universe, curCel->data);
		if(returnedDepth != -1) //We found the parent (or COM) in the universe!
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
			
			totalDepth += returnedDepth;
		}
		else
			curCel = curCel->next;
		
		if(curCel == NULL)
		{
			curCel = orbList;
		}
	}
	cout << totalDepth << endl;
	return 0;
}

