//--- Day 4: Secure Container ---
/*
You arrive at the Venus fuel depot only to discover it's protected by a password. The Elves had written the password on a sticky note, but someone threw it out.

However, they do remember a few key facts about the password:

    It is a six-digit number.
    The value is within the range given in your puzzle input.
    Two adjacent digits are the same (like 22 in 122345).
    Going from left to right, the digits never decrease; they only ever increase or stay the same (like 111123 or 135679).
*/
#include <iostream>
#include <string>
int PASSWORDMIN = 254032;
int PASSWORDMAX = 789860;

using namespace std;

bool passwordIsValid(int a, int b, int c, int d, int e, int f)
{
	int builtPassword = a*1e5 + b*1e4 + c*1e3 + d*1e2 + e*1e1 + f;
	bool passwordOK = (a==b || b==c || c==d || d==e || e==f) && builtPassword >= PASSWORDMIN && builtPassword <= PASSWORDMAX;
	// if(passwordOK)
		// cout<<builtPassword<<"\n";
	// else
		// cout<<"REJECTED "<<builtPassword<<"\n";
	return passwordOK;
}

int main()
{
	int validPasswords = 0;
	for(int a=0; a<=9; a++)
	{
	for(int b=a; b<=9; b++)
	{
	for(int c=b; c<=9; c++)
	{
	for(int d=c; d<=9; d++)
	{
	for(int e=d; e<=9; e++)
	{
	for(int f=e; f<=9; f++)
	{
		validPasswords += passwordIsValid(a,b,c,d,e,f) ? 1 : 0;
	}
	}
	}
	}
	}
	}
	cout << validPasswords << "\n";
	return 0;
}
