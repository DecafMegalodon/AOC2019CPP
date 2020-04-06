#include <iostream>
#include <string>

const int WIDTH = 25;
const int HEIGHT = 6;

const int TRACKEDINTS = 3; //Starting from 0

using namespace std;

void zeroArray(int array[TRACKEDINTS])
{
	for(int i = 0; i < TRACKEDINTS; i++)
		array[i] = 0;
}

int main()
{
	//printf("Running\n");
	int read;
	int scanfReturn;
	int counts[TRACKEDINTS] = {0}; //The number of 1...3 found in the input "layer"
	int bestZeros = 99999999;
	int bestProduct = -1;
	do
	{
		for(int numRead = 0; numRead < WIDTH*HEIGHT; numRead++)
		{
			scanfReturn = scanf("%1i", &read);
			if(scanfReturn == -1)
				break;
			counts[read]++;
		}
		//printf("%d %d %d\n", counts[0], counts[1], counts[2]);
		if(counts[0] < bestZeros && counts[0] != 0)
		{
			bestProduct = counts[1] * counts[2];
			bestZeros = counts[0];
		}
		zeroArray(counts);
    } while (scanfReturn != -1);
	cout << bestProduct << endl;
	return 0;
}
