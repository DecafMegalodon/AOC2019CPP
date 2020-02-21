/*
--- Day 6: Universal Orbit Map ---

You've landed at the Universal Orbit Map facility on Mercury. Because navigation in space often involves transferring between orbits, the orbit maps here are useful for finding efficient routes between, for example, you and Santa. You download a map of the local orbits (your puzzle input).

Except for the universal Center of Mass (COM), every object in space is in orbit around exactly one other object.



The solution here is estimated to take between O(n^2) and O(n^3) time where n is the number of objects
*/
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

//A glorified unordered n-tree with names for planets and other orbiting bodies
struct orbitTree
{
	char* name;
	vector<orbitTree> suborbits;
	
	map<char* [4],orbitTree>* potato = new map<char[4],orbitTree>;
	
	//Injects an object into orbit. Returns the depth of the orbit relative to the universal center of mass (COM)
	//Returns a negative number if it can't find the parent.
	int inject(char* parentName, char* objName)
	{
		if(objName == "COM") //Special case to initialize ~~the universe~~
		{
			name = objName;
			return 0;
		}
		else
		{
			//Hunt through the tree and try to find the parent in the tree as a child.
		}
		return -1; //Womp. We couldn't find it
	}
	
};

void printOrbits(orbitTree* orbits)
{
	
}

void readOrbits(vector<char*>* parents, vector<char*>* children )
{
	char* parent = new char[4];
	char* child = new char[4];
	while(scanf("%3s)%3s", parent, child) == 2)
	{
		//printf("%s orbits %s\n", child, parent);
		parents->push_back(new char[4]);
		strncpy(parents->back(),parent,4);
		children->push_back(new char[4]);
		strncpy(children->back(),child,4);
	}
	cout<<"Finished reading orbits\n";
}

int main()
{
	
	printf("Running\n");
	vector<char*>* unMappedParents = new vector<char*>;
	vector<char*>* unMappedChildren = new vector<char*>;
	readOrbits(unMappedParents, unMappedChildren);
	orbitTree* Space = new orbitTree;
	// cout << unMappedChildren->at(0) << "\n";
	// cout << unMappedChildren->at(1) << "\n";
	return 0;
}
// if(!strcmp(parent,child))
	// cout<<"BLAH";
