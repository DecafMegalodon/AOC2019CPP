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
	string* parentName;
};

typedef map<string*, orbitObject*> orbitMap;
typedef pair<string*, string*> orbitPair; //first: parent, second: child


void printOrbits(orbitMap* orbits)
{
	
}

//Injects an object into orbit. Returns the depth of the orbit relative to the universal center of mass (COM)
//Returns a zero if it can't find the parent.
int injectIntoOrbit(orbitMap* orbitMap, const string* parentName, const string* objName)
{
	//cout << parentName->size() << endl;
	if(parentName->compare("COM") == 0) //Initialize ~~the universe~~
	{
		int depth = 1;
		orbitObject* orbOb = new orbitObject;
		orbOb->parentName = new string(*parentName);
		orbOb->depth = 1;
		orbitMap->emplace(new string(*objName), orbOb);
		return depth;
	}
	else
	{
		//auto iterator = orbits->find(parentName);
		//Hunt through the tree and try to find the parent in the tree as a child.
		// if(orbitMap.find(parent) != prbitMap.end())
			// cout << "yay" << endl;
		// else
			// cout << "yeet" << endl;
		
	}

	return 0; //We couldn't find it
}

void readOrbits(vector<string*>* parents, vector<string*>* children )
{
	string parent = string("   ");
	string child = string("   ");
	while(scanf("%3s)%3s", &parent[0], &child[0]) == 2) //Ugly hack is ugly
	{
		parents->push_back(new string(parent));
		children->push_back(new string(child));
		//printf("%s is orbiting %s\n", children->back()->data(), parents->back()->data());
	}
	cout<<"Finished reading orbits\n";
}

void dumpUniverse(orbitMap* orbitMap)
{
	for(auto iter=orbitMap->begin(); iter != orbitMap->end(); ++iter)
		cout << iter->second->parentName->data() << ")" << iter->first->data() << endl;
}

int main()
{
	
	printf("Running\n");
	vector<string*>* unMappedParents = new vector<string*>;
	vector<string*>* unMappedChildren = new vector<string*>;
	readOrbits(unMappedParents, unMappedChildren);
	orbitMap* space = new orbitMap;
	
	//vector*<string*>::iterator childrenIT;
	//vector*<string*>::iterator parentsIT;
	int orbitDepth;
	auto childrenIT = *unMappedChildren->begin();
	auto parentIT = *unMappedParents->begin();
	
	while(!unMappedChildren->empty())
	{	

		cout<<childrenIT->data()<<endl;
		unMappedChildren->erase(childrenIT);
		childrenIT = *unMappedChildren->begin();
	}

	injectIntoOrbit(space, unMappedParents->at(0),unMappedChildren->at(0));
	injectIntoOrbit(space, unMappedParents->at(1),unMappedChildren->at(1));
	injectIntoOrbit(space, unMappedParents->at(2),unMappedChildren->at(2));
	//if(unMappedChildren->at(0)->compare(*unMappedParents->at(0)) == 0)
	dumpUniverse(space);
	return 0;
}

