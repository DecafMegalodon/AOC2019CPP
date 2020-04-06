#include <iostream>
#include <string>

const int WIDTH = 25;
const int HEIGHT = 6;

const int TRACKEDINTS = 3; //Starting from 0

using namespace std;

//Clear the image in the resetting AND making transparent sense :)
void clearImage(int image[WIDTH*HEIGHT])
{
	for(int i = 0; i < WIDTH*HEIGHT; i++)
		image[i] = 2;
}

//Designed for a white-on-black terminal but it should be readable regardless
//0=black, 1=white, 2=transparent
void displayImage(int image[WIDTH*HEIGHT])
{
	for(int pixel = 0; pixel < WIDTH*HEIGHT; pixel++)
	{
		if(pixel % WIDTH == 0)
			cout<< endl;
		switch (image[pixel])
		{
			case 0:
				cout<< ".";
				break;
			case 1:
				cout<< "@";
				break;
			case 2:
				break; //It's transparent, do nothing
		}
	}
	cout<< endl;
}

int main()
{
	//printf("Running\n");
	int read;
	int scanfReturn;
	int bestZeros = 99999999;
	int bestProduct = -1;
	int* image = new int[WIDTH*HEIGHT];
	clearImage(image);
	do
	{
		for(int numRead = 0; numRead < WIDTH*HEIGHT; numRead++)
		{
			scanfReturn = scanf("%1i", &read);
			if(scanfReturn == -1)
				break;
			if(image[numRead] == 2) //If it's transparent, we can see stuff under it
				image[numRead] = read;
		}

    } while (scanfReturn != -1);
	displayImage(image);
	return 0;
}
