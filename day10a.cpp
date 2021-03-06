#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <math.h>

using namespace std;

const int SPACEWIDTH = 21;
const int SPACEHEIGHT = 21; //Hmm, I could have sworn there were three dimensions in space.

//Computes the Greatest Common Demoninator of the two numbers.
int GCD(int a, int b)
{
	int t;
	while (b != 0)
	{
		t = b;
		b = a % b;
		a = t;
	}
	return abs(a);
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
		scanf("%21c\n", &space[line*SPACEWIDTH]);
	}
}

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
		//printf("Writing %c to %i, %i\n", c, x, y);
		space[y*SPACEWIDTH+x] = c;
		//printSpace(space);
		return true;
	}
	else
		return false;
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
	char curChar;
	int deltaX;
	int deltaY;
	int multiplier;
	int gcd;
	for(int curY = 0; curY < SPACEWIDTH; curY++)
	{
		for(int curX = 0; curX < SPACEWIDTH; curX++)
		{
			if(curX == x && curY == y)
				continue; //Our origin asteroid doesn't occlude others
			curChar = readSpot(space,curY,curX);
			if(curChar == '#') //Are we checking an asteroid?
			{
				deltaX = curX-x;
				deltaY = curY-y;
				gcd = GCD(deltaX, deltaY);
				deltaY /= gcd;
				deltaX /= gcd;
				multiplier = 1;
				while(writeSpot(space, curY+(deltaY*multiplier), curX+(deltaX*multiplier), 'X'))
					multiplier++;
				
			}
		}
	}
}

//Counts the asteroids that are still visible.
//Assumes one of them is the observatory, which isn't counted
int countVisibleAsteroids(char* space)
{
	int count=0;
	for(int charNum = 0; charNum < SPACEHEIGHT*SPACEWIDTH; charNum++)
		if(space[charNum] == '#')
			count++;
	return count - 1;
}

int main()
{
	char* refSpace = new char[SPACEHEIGHT*SPACEWIDTH];
	char* workingSpace = new char[SPACEHEIGHT*SPACEWIDTH];
	int mostAsteroidsVisibleSoFar = -1;
	int visibleHere;
	readSpace(refSpace);
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
			}
		}
	}
	cout << mostAsteroidsVisibleSoFar << endl;
	return 0;
}