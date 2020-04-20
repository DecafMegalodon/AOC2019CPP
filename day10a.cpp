#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <math.h>

using namespace std;

const int SPACEWIDTH = 21;
const int SPACEHEIGHT = 21; //Hmm, I could have sworn there were three dimensions in space.

//Checks a spot in space and returns what's there. Returns \0 if it's out of bounds
char readSpot(const char* space, const int y, const int x)
{
	printf("Checking %i, %i\n", x,y);
	if(y>=0 && y<SPACEHEIGHT && x>=0 && x<SPACEWIDTH)
		return space[y*SPACEHEIGHT+SPACEWIDTH];
	else
		return '\0';
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

//Copies reference into target
void cloneSpace(const char* reference, char* target)
{
	memcpy(target, reference, SPACEWIDTH*SPACEHEIGHT*sizeof(char));
}

//Prints the current state of space to stdout
void printSpace(const char* space)
{
	for(int line = 0; line < SPACEHEIGHT; line++)
	{
		printf("%21s\n", &space[line*SPACEWIDTH]);
	}
}

//Reads the space map from standard in.
void readSpace(char* space)
{
	for(int line = 0; line < SPACEHEIGHT; line++)
	{
		scanf("%21c\n", &space[line*SPACEWIDTH]);
		//printf("%21s\n", &space[line*SPACEWIDTH]);
	}
}

//Returns true if a number is prime, false otherwise.
//1 is considered prime for the purpose of this program even though it's not really since it's still indivisible evenly
bool isPrime(int num)
{
	if(num == 0)
		return false;
	num = abs(num);
	for(int i = 2; i<=sqrt(num); i++)
		if (num%i == 0) return false;
	return true;
}

//Alters the space map. Replaces invisible asteroids (#) from x,y's vantage point with .
void hideInvisible(char* space, const int y, const int x)
{
	int curX;
	int curY;
	char checkedChar;
	int multiplier;
	bool isOccluding; //If we've found an asteroid, it will occlude ones beyond it
	for(int deltaY = -SPACEHEIGHT; deltaY < SPACEHEIGHT; deltaY++)
	{
		if(!isPrime(deltaY)) continue;
		for(int deltaX = -SPACEHEIGHT; deltaX < SPACEHEIGHT; deltaX++)
		{
			if(!isPrime(deltaX)) continue;
			isOccluding = false;
			for(multiplier = 1; false; multiplier++)
			{
				curX = x+deltaX*multiplier;
				curY = y+deltaY*multiplier;
				checkedChar = readSpot(space, curY, curX);
				if(checkedChar == '\0') //Are we out of bounds?
					break;
				else if(isOccluding) //Have we already lost line of sight to an asteroid?
					writeSpot(space,curY,curX,'.');
				else if(checkedChar == '#') //Will we lose LOS to an asteroid?
					isOccluding = true;
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
	readSpace(refSpace);
	for(int y=0; y<SPACEHEIGHT; y++)
	{
		for(int x = 0; x < SPACEWIDTH; x++)
		{
			if(readSpot(workingSpace,y,x) == '#')
				continue; //The observatory must be on an asteroid
			printf("Checking %i, %i\n", x,y);
			cloneSpace(refSpace, workingSpace);
			hideInvisible(workingSpace, y, x);
			mostAsteroidsVisibleSoFar = max(mostAsteroidsVisibleSoFar, countVisibleAsteroids(workingSpace));
		}
	}
	cout << mostAsteroidsVisibleSoFar << endl;
	return 0;
}