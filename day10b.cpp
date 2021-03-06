#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <math.h>
#include <algorithm>    // std::sort

using namespace std;

const int SPACEWIDTH = 21;
const int SPACEHEIGHT = 21; //Hmm, I could have sworn there were three (or more) dimensions in space.


//Technically, these are meteroids according to the "lore" of the challenge
struct meteor
{
	int x;
	int y;
	double angle;
	
	meteor(int xpos, int ypos, double ang)
	{
		x=xpos;
		y=ypos;
		angle=ang;
	}
	
	meteor()
	{
		
	}
	
	bool operator<(const meteor meteor2)
	{
		return angle < meteor2.angle;
	}
	
};

double calcAngle(int x, int y)
{
	double protoangle = atan2(x,y);
	return fmod(protoangle+2*M_PI, 2*M_PI);
}

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
char readSpot(const char* space, const int x, const int y)
{
	if(y>=0 && y<SPACEHEIGHT && x>=0 && x<SPACEWIDTH)
		return space[y*SPACEWIDTH+x];
	else
		return '\0';
}

//Hides a meteor at the given y,x coordinate. If there's no meteorite, it does nothing.
//Returns false if outside of bounds and does nothing.
bool hideSpot(char* space, const int x, const int y)
{
	if(y>=0 && y<SPACEHEIGHT && x>=0 && x<SPACEWIDTH)
	{
		if(space[y*SPACEWIDTH+x] == '#')
			space[y*SPACEWIDTH+x] = '+';
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

//Alters the space map. Replaces invisible asteroids (#) from y,x's vantage point with +
void hideInvisible(char* space, const int x, const int y)
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
			curChar = readSpot(space,curX,curY);
			if(curChar == '#') //Are we checking an asteroid?
			{
				deltaX = curX-x;
				deltaY = curY-y;
				gcd = GCD(deltaX, deltaY);
				deltaY /= gcd;
				deltaX /= gcd;
				multiplier = 1;
				while(hideSpot(space, curX+(deltaX*multiplier), curY+(deltaY*multiplier)))
					multiplier++;
				
			}
		}
	}
}

//Counts the asteroids that are still visible.
int countVisibleAsteroids(const char* space)
{
	int count=0;
	for(int charNum = 0; charNum < SPACEHEIGHT*SPACEWIDTH; charNum++)
		if(space[charNum] == '#')
			count++;
	return count; //This include the observatory asteroid
}

//returns an x,y pair with the location of the best (most visible asteroids) observatory
void findBestStationLocation(const char* space, int* bestX, int* bestY)
{
	char* workingSpace = new char[SPACEHEIGHT*SPACEWIDTH];
	int mostAsteroidsVisibleSoFar = -1;
	int visibleHere;
	for(int y=0; y<SPACEHEIGHT; y++)
	{
		for(int x = 0; x < SPACEWIDTH; x++)
		{
			if(readSpot(space,x,y) == '#') //Observatories can only go on asteroids
			{
				cloneSpace(space, workingSpace);
				hideInvisible(workingSpace, x, y);
				visibleHere = countVisibleAsteroids(workingSpace);
				if(visibleHere > mostAsteroidsVisibleSoFar)
				{
					mostAsteroidsVisibleSoFar = visibleHere;
					*bestX=x;
					*bestY=y;
				}
				mostAsteroidsVisibleSoFar = max(mostAsteroidsVisibleSoFar, visibleHere);
			}
		}
	}
	delete workingSpace;
	cout << "At the ideal spot, there are this many visible: "<< mostAsteroidsVisibleSoFar-1 << endl;
}

//Restores hidden asteroids ('+') to normal ('#')
void restoreInvisibleAsteroids(char* space)
{
	for(int charNum = 0; charNum < SPACEHEIGHT*SPACEWIDTH; charNum++)
	{
		if(space[charNum] == '+')
			space[charNum] = '#';
	}
}

//Queues all visible meteoroids for vaporization, and marks them as gone on the map
//startpoint is how many meteors we've already vaporized
//Returns how many were vaporized this phase
int queueVaporizations(char* space, meteor meteorList[], const int startPoint, const int observX, const int observY)
{
	int vaporized = 0;
	int x;
	int y;
	double angle;
	for(int charNum = 0; charNum < SPACEHEIGHT*SPACEWIDTH; charNum++)
	{
		if(space[charNum] == '#')
		{
			space[charNum] = '.';
			x = charNum % SPACEWIDTH;
			y = (charNum-x)/SPACEWIDTH;
			meteorList[startPoint+vaporized] = meteor(x, y, calcAngle(x-observX,observY-y));
			vaporized++;
		}
	}
	return vaporized;
}

void dumpVaporization(meteor queue[], int numMeteorites)
{
	meteor curObj;
	for(int i = 0; i < numMeteorites; i++)
	{
		curObj = queue[i];
		printf("[%i] %i, %i, %f\n", i, curObj.x, curObj.y, curObj.angle);
	}
	
}


int main()
{
	char* refSpace = new char[SPACEHEIGHT*SPACEWIDTH];
	readSpace(refSpace);
	int totalAsteroids = countVisibleAsteroids(refSpace)-1; //Don't include observatory
	int bestX;
	int bestY;
	findBestStationLocation(refSpace, &bestX, &bestY);
	refSpace[bestY*SPACEWIDTH + bestX] = '@'; //You are here
	printf("The best spot is at %i, %i.\n", bestX, bestY);
	meteor* vaporizationQueue = new meteor[totalAsteroids-1];
	int meteorsVaporizedSoFar = 0;
	int meteorsVaporizedThisPhase;
	while(countVisibleAsteroids(refSpace) != 0)
	{
		hideInvisible(refSpace, bestX, bestY); //Find visible asteroids
		meteorsVaporizedThisPhase = queueVaporizations(refSpace, vaporizationQueue, meteorsVaporizedSoFar,
										bestX, bestY); //Add them to the array, and VAPORIZE them
		std::sort(&vaporizationQueue[meteorsVaporizedSoFar], &vaporizationQueue[meteorsVaporizedSoFar+meteorsVaporizedThisPhase]); //Sort the meteoroids from this phase
		meteorsVaporizedSoFar += meteorsVaporizedThisPhase;
		restoreInvisibleAsteroids(refSpace);
	}
	printf("The 200th was at %i, %i\n", vaporizationQueue[199].x, vaporizationQueue[199].y);
	printf("The answer is %i\n", vaporizationQueue[199].x*100 + vaporizationQueue[199].y);	
	return 0;
}