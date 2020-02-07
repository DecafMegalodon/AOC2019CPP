#include <iostream>
#include <string>
#include <cmath>
#define DEBUG 0
int MEMSIZE = 117;
int MAXOPCODEPARAMS = 4;

using namespace std;

struct OpCode
{
	int operation;
	int* parameters = new int[MAXOPCODEPARAMS];
	int* paramModes = new int[MAXOPCODEPARAMS];
	
	
};

void initializeMemory(int* memory)
{
	int curRead;
	for(int addr = 0; addr < MEMSIZE; addr++)
	{
		scanf("%d,",&curRead);
		memory[addr] = curRead;
	}
}

void printRAM(int* memory)
{
	for(int addr = 0; addr < MEMSIZE; addr++)
	{
		printf("[%d] %d\n", addr, memory[addr]);
	}
	cout << "\n";
}

//Extracts a single decimal digit from number, in the digit-th spot. Right to left, starting at 0
//If there is no such decimal digit it will be treated as being padded with zeros
int extractDecimalDigit(int number, int digit)
{
	int trimmed = number % (int)pow(10, digit+1); //Floating point error???
	return (int)(trimmed / pow(10, digit));
}

//Reads an opcode starting from the current program counter
void readOpCode(int* memory, OpCode* opcode, int pc)
{
	int protoOpCode = memory[pc]; //This includes the operation but also parameter modes
	
}

void intCodeInterpreter(int* memory)
{
	int pc = 0; //The program counter
	int operation;
	int sourceAddr1; 
	int sourceAddr2;
	int targetAddr;
	while(true)
	{
		operation = memory[pc];
		sourceAddr1 = memory[pc+1];
		sourceAddr2 = memory[pc+2];
		targetAddr= memory[pc+3];
		//printf("%d, %d, %d, %d\n", operation, sourceAddr1, sourceAddr2, targetAddr);
		switch(operation)
		{
			case 1:
				memory[targetAddr] = memory[sourceAddr1] + memory[sourceAddr2];
				break;
			case 2:
				memory[targetAddr] = memory[sourceAddr1] * memory[sourceAddr2];
				break;
			case 99:
				return;
			default:
				cout << "INVALID OPCODE ERROR\n";
				exit(-1);
		}
		pc += 4;
	}
}

int main()
{
	// int memory[MEMSIZE];
	// initializeMemory(memory);
	// intCodeInterpreter(memory);
	// printf("Memory[0] = %d\n", memory[0]);
}
