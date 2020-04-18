#include <iostream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

const int SPACEWIDTH = 21;
const int SPACEHEIGHT = 21; //Hmm, I could have sworn there were three dimensions in space.

//Checks a spot in space and returns what's there. Returns \0 if it's out of bounds
char readSpot(const char* space, const int y, const int x)
{
	return space[y*SPACEHEIGHT+SPACEWIDTH];
}

//Writes to a spot in the universe. Returns true, but returns false if outside of bounds
bool writeSpot(char* space, const int y, const int x, const char c)
{
	if(y>=0 && y<SPACEHEIGHT && x>=0 && x<SPACEWIDTH)
	{
		space[y*SPACEHEIGHT+SPACEWIDTH] = c;
		return true;
	}
	else
		return false;
}

void printSpace(const char* space)
{
	for(int line = 0; line < SPACEHEIGHT; line++)
	{
		//scanf("%21c\n", &space[line*SPACEWIDTH]);
		//printf("%21s\n", &space[line*SPACEWIDTH]);
	}
}

void readSpace(char* space)
{
	for(int line = 0; line < SPACEHEIGHT; line++)
	{
		scanf("%21c\n", &space[line*SPACEWIDTH]);
		printf("%21s\n", &space[line*SPACEWIDTH]);
	}
}


int main()
{
	printf("Running\n");
	char* space = new char[SPACEHEIGHT*SPACEWIDTH];
	readSpace(space);
	//printSpace(space);
	delete space; //Goodbye, world
	return 0;
}