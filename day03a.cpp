#include <iostream>
#include <string>
#include <bits/stdc++.h> 
const int BOARDSIZE = 50000;
const int STARTPOINT = BOARDSIZE/2;
typedef std::bitset<BOARDSIZE> WireRow;

//typedef WireBoard

using namespace std;

void initWireboard(WireRow* wireboard)
{
	for(int row=0; row<BOARDSIZE; row++)
	{
		//cout<<row<<'\n';
		wireboard[row].reset();
	}
}

void printWireboard(WireRow* wireboard) //Probably a poor decision to run this on a full-sized wireboard
{
	for(int row=0; row<BOARDSIZE; row++)
	{
		for(int col=0; col<BOARDSIZE; col++)
			cout << (wireboard[row].test(col) == true ? 'X' : '.');
		cout << "\n";
	}
	cout << "\n";
}

void readWire(WireRow* wireboard, int wirenum)
{
	char direction;
	int distance;
	char newlineORcomma; //We'll see a newline at this point in the input if we're at the end of a wire, comma otherwise
	int x=STARTPOINT;
	int y=STARTPOINT;
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
			if(wirenum == 1)
				wireboard[y].set(x);
			else
			{
				if(wireboard[y].test(x)) //If the wires cross
				{
					distanceFromOrigin = abs(STARTPOINT-x) + abs(STARTPOINT-y);
					if(distanceFromOrigin < mindistance)
					{
						printf("Closer crossing at %d, %d! Distance from origin = %d\n", x, y, abs(STARTPOINT-x) + abs(STARTPOINT-y));
						mindistance = distanceFromOrigin;
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
	printf("Running\n");
	WireRow* wireboard = new WireRow [BOARDSIZE];
	initWireboard(wireboard);
	readWire(wireboard,1);
	readWire(wireboard,2);
	return 0;
}
