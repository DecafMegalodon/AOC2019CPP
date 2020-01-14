#include <iostream>

using namespace std;

int calcFuel(int mass)
{
	return mass/3 - 2;
}

int calcFuelFuel(int fuel)
{
	int fuelFuel = calcFuel(fuel);
	if(fuelFuel <= 0)
		return 0;
	else
		return fuelFuel+calcFuelFuel(fuelFuel); //Does "Fuel" look funny yet?
}

int main()
{
	int moduleFuel = 0;
	int mass, fuelForThisModule;
	while(scanf("%d", &mass) != EOF)
	{
		fuelForThisModule = calcFuel(mass);
		moduleFuel +=  fuelForThisModule + calcFuelFuel(fuelForThisModule);
	}
	cout << moduleFuel << "\n";
	return 0;
}
