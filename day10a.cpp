#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <math.h>

using namespace std;

const int SPACEWIDTH = 10;
const int SPACEHEIGHT = 10; //Hmm, I could have sworn there were three dimensions in space.

//Checks a spot in space and returns what's there. Returns \0 if it's out of bounds
char readSpot(const char* space, const int y, const int x)
{
	if(y>=0 && y<SPACEHEIGHT && x>=0 && x<SPACEWIDTH)
		return space[y*SPACEWIDTH+x];
	else
		return '\0';
}

//Writes to a spot in the universe. Returns true, but returns false if outside of bounds
bool writeSpot(char* space, const int y, const int x, const char c)
{
	if(y>=0 && y<SPACEHEIGHT && x>=0 && x<SPACEWIDTH)
	{
		space[y*SPACEWIDTH+x] = c;
		return true;
	}
	else
		return false;
}

//Copies reference into target
void cloneSpace(const char* reference, char* target)
{
	memcpy(target, reference, SPACEWIDTH*SPACEHEIGHT*sizeof(char));
}

//Prints the current state of space to stdout
void printSpace(const char* space)
{
	cout << "Space so far...\n";
	for(int line = 0; line < SPACEHEIGHT; line++)
	{
		printf("%.*s\n", SPACEWIDTH, &space[line*SPACEWIDTH]);
	}
}

//Reads the space map from standard in.
void readSpace(char* space)
{
	for(int line = 0; line < SPACEHEIGHT; line++)
	{
		scanf("%10c\n", &space[line*SPACEWIDTH]);
	}
}

//Returns true if a number is prime, false otherwise.
//1 and 0 are considered prime for the purpose of this program even though it's not really since they're still indivisible evenly
bool isPrime(int num)
{
	num = abs(num);
	for(int i = 2; i<=sqrt(num); i++)
		if (num%i == 0) return false;
	return true;
}

//Alters the space map. Replaces invisible asteroids (#) from y,x's vantage point with .
void hideInvisible(char* space, const int y, const int x)
{
	int curX;
	int curY;
	char checkedChar;
	int multiplier;
	bool isOccluding; //If we've found an asteroid, it will occlude ones beyond it
	//printf("Pruning %i, %i\n", x,y);
	for(int deltaY = -SPACEHEIGHT; deltaY < SPACEHEIGHT; deltaY++)
	{
		if(!isPrime(deltaY)) continue;
		for(int deltaX = -SPACEHEIGHT; deltaX < SPACEHEIGHT; deltaX++)
		{
			if(!isPrime(deltaX)) continue;
			if(deltaX==0 && deltaY==0) continue;
			isOccluding = false;
			for(multiplier = 1; true; multiplier++)
			{
				curX = x+deltaX*multiplier;
				curY = y+deltaY*multiplier;
				checkedChar = readSpot(space, curY, curX);
				if(checkedChar == '\0') //Are we out of bounds?
					break;
				//printf("deltaX = %i, deltaY = %i, curX = %i, curY = %i, mult = %i, curChar=%c\n",deltaX, deltaY, curX, curY, multiplier, checkedChar);
				if(isOccluding) //Have we already lost line of sight to an asteroid?
				{
					writeSpot(space,curY,curX,',');
					//printSpace(space);
				}
				else if(checkedChar == '#') //Will we lose LOS to an asteroid?
				{
					isOccluding = true;
					//cout << "Pruning mode activated\n";
				}
			}

		}
	}
}

//Counts the asteroids that are still visible
int countVisibleAsteroids(char* space)
{
	int count=0;
	for(int charNum = 0; charNum < SPACEHEIGHT*SPACEWIDTH; charNum++)
		if(space[charNum] == '#')
			count++;
	return count;
}

int main()
{
	char* refSpace = new char[SPACEHEIGHT*SPACEWIDTH];
	char* workingSpace = new char[SPACEHEIGHT*SPACEWIDTH];
	int mostAsteroidsVisibleSoFar = -1;
	int visibleHere;
	readSpace(refSpace);
	printSpace(refSpace);
	for(int y=0; y<SPACEHEIGHT; y++)
	{
		for(int x = 0; x < SPACEWIDTH; x++)
		{
			if(readSpot(refSpace,y,x) == '#') //Observatories can only go on asteroids
			{
				cloneSpace(refSpace, workingSpace);
				hideInvisible(workingSpace, y, x);
				visibleHere = countVisibleAsteroids(workingSpace);
				mostAsteroidsVisibleSoFar = max(mostAsteroidsVisibleSoFar, visibleHere);
				printf("At %i, %i there are %i visible\n", x, y, visibleHere);
			}
		}
	}
	cout << mostAsteroidsVisibleSoFar-1 << endl;
	return 0;
}