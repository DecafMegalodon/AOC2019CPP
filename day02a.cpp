#include <iostream>
#include <string>
#define DEBUG 0
int MEMSIZE = 117;

using namespace std;


void initializeMemory(int* memory)
{
	int curRead;
	for(int addr = 0; addr < MEMSIZE; addr++)
	{
		scanf("%d,",&curRead);
		memory[addr] = curRead;
	}
	memory[1] = 12;
	memory[2] = 2;
}

void printRAM(int* memory)
{
	for(int addr = 0; addr < MEMSIZE; addr++)
	{
		cout << memory[addr] << " ";
	}
	cout << "\n";
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
	int memory[MEMSIZE];
	initializeMemory(memory);
	intCodeInterpreter(memory);
	printf("%d\n", memory[0]);
}
