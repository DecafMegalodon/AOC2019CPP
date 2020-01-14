#include <iostream>
#include <string>
#define DEBUG 0
int MEMSIZE = 117;
int TARGETOUTPUT = 19690720;

using namespace std;


void initializeMemory(int* memory)
{
	int curRead;
	for(int addr = 0; addr < MEMSIZE; addr++)
	{
		scanf("%d,",&curRead);
		memory[addr] = curRead;
	}
}

void copyMemory(int* sourceMem, int* targetMem)
{
	for (int addr = 0; addr < MEMSIZE; addr++)
	{
		targetMem[addr] = sourceMem[addr];
	}
	
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
			case 1: //Add
				memory[targetAddr] = memory[sourceAddr1] + memory[sourceAddr2];
				break;
			case 2: //Multiply
				memory[targetAddr] = memory[sourceAddr1] * memory[sourceAddr2];
				break;
			case 99: //Terminate
				return;
			default:
				//cout << "INVALID OPCODE ERROR\n";
				return;
		}
		pc += 4;
	}
}

int main()
{
	int memory[MEMSIZE];
	int memoryBackup[MEMSIZE];
	initializeMemory(memory);
	copyMemory(memory, memoryBackup);
	intCodeInterpreter(memory);
	for(int noun = 0; noun <= 99; noun++)
	{
		for(int verb = 0; verb <= 99; verb++)
		{
			//printf("noun=%d,verb=%d\n",noun,verb);
			memory[1] = noun;
			memory[2] = verb;
			intCodeInterpreter(memory);
			if(memory[0] == TARGETOUTPUT)
			{
				printf("Answer = %d\n", 100*noun + verb);
				return 0;
			}
			copyMemory(memoryBackup, memory);
		}
	}
}
