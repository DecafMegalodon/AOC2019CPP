#include <iostream>
#include <string>
#include <bits/stdc++.h> 
const int BOARDSIZE = 40000; //You may need to increase this for your puzzle input
const int STARTPOINT = BOARDSIZE/2;
typedef unsigned short WireRow[BOARDSIZE]; //I don't pretend like this is memory efficient. ~3Gigs of memory at 40k board size

//typedef WireBoard

using namespace std;

void initWireboard(WireRow* wireboard)
{
	for(int row=0; row<BOARDSIZE; row++)
	{
		for(int col=0; col<BOARDSIZE; col++)
			wireboard[row][col] = (unsigned short) 0;
	}
}

// void printWireboard(WireRow* wireboard) //Probably a poor decision to run this on a full-sized wireboard
// {
	// for(int row=0; row<BOARDSIZE; row++)
	// {
		// for(int col=0; col<BOARDSIZE; col++)
			// cout << (wireboard[row].test(col) == true ? 'X' : '.');
		// cout << "\n";
	// }
	// cout << "\n";
// }

void readWire(WireRow* wireboard, int wirenum, int* closest)
{
	char direction;
	int distance;
	char newlineORcomma; //We'll see a newline at this point in the input if we're at the end of a wire, comma otherwise
	int x=STARTPOINT;
	int y=STARTPOINT;
	unsigned short wireDistance = 0;
	int ydiff;//These show how the wires will move for a given direction
	int xdiff;
	int mindistance = INT_MAX; //We'll only use this for wire 2
	int distanceFromOrigin; //The taxicab distance from the start point where the two wires crossed. Wire 2 only.
	while(scanf("%c%d%c",&direction, &distance, &newlineORcomma))
	{
		//printf("%c%d\n", direction, distance);
		switch(direction)
		{
			case 'U':
				ydiff = -1;
				xdiff = 0;
				break;
			case 'D':
				ydiff = 1;
				xdiff = 0;
				break;
			case 'L':
				xdiff = -1;
				ydiff = 0;
				break;
			case 'R':
				xdiff = 1;
				ydiff = 0;
				break;
			default:
				cout << "UNEXPECTED DIRECTION\n";
				exit(-1);
			
		}
		
		for(;distance>0;distance--) //We're doomed if distance doesn't get scanf'd for some reason
		{
			x+=xdiff;
			y+=ydiff;
			wireDistance += 1;
			if(wirenum == 1)
				wireboard[y][x] = wireDistance;
			else
			{
				if(wireboard[y][x] != 0) //If the wires cross
				{
					distanceFromOrigin = wireboard[y][x] + wireDistance;
					if(distanceFromOrigin < *closest)
					{
						//printf("Closer crossing at %d, %d! Distance from origin = %d\n", x, y, distanceFromOrigin);
						*closest = distanceFromOrigin;
					}

				}
			}
			//printf("%d,%d\n", x,y);
		}
		
		if(newlineORcomma == '\n')
			break;
	}
	
}


int main()
{
	int closest = INT_MAX;
	WireRow* wireboard = new WireRow [BOARDSIZE];
	initWireboard(wireboard);
	readWire(wireboard,1, &closest);
	readWire(wireboard,2, &closest);
	cout << closest;
	return 0;
}
