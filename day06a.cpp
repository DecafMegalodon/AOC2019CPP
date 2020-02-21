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

struct orbitObject
{
	int depth; //The number of steps until the COM is reached
	char parentName[4];
};
typedef map<char[4], orbitObject*> orbitMap;


void printOrbits(orbitMap* orbits)
{
	
}

//Injects an object into orbit. Returns the depth of the orbit relative to the universal center of mass (COM)
//Returns a zero if it can't find the parent.
int injectIntoOrbit(const char parentName[4], const char objName[4])
{
	
	// if(!strncmp(objName,"COM",4)) //Special case to initialize ~~the universe~~
	// {
		// strncpy(name, objName,4);
		// depth = 1;
		// return depth;
	// }
	// else
	// {
		// auto iterator = orbits->find(parentName);
		// //Hunt through the tree and try to find the parent in the tree as a child.
	// }
	return 0; //We couldn't find it
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
	// orbitTree* Space = new orbitTree;
	// cout << unMappedChildren->at(0) << "\n";
	// cout << unMappedChildren->at(1) << "\n";
	return 0;
}
// if(!strcmp(parent,child))
	// cout<<"BLAH";