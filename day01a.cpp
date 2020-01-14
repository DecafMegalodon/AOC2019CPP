#include <iostream>
#include <string>

using namespace std;

int calcFuel(int mass)
{
	return mass/3 - 2;
}

int main()
{
	int totalFuel = 0;
	int mass;
	while(scanf("%d", &mass) != EOF)
	{
		totalFuel += calcFuel(mass);
	}
	cout << totalFuel << "\n";
	return 0;
}
