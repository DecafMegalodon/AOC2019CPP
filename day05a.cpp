#include <iostream>
#include <string>
#include <cmath>
#define DEBUG 0
int MEMSIZE = 117;
int MAXOPCODEPARAMS = 4;
int OPCODEPARAMS [5] = {-1,3,3,2,2};
//[NONE,add,multiple,store,print]

//OpCode names
const int ADD = 1;
const int MUL = 2;
const int STOR = 3;
const int PRNT = 4;
const int EXIT = 99;

//Address modes
const int POSITION = 0;
const int IMMEDIATE = 1;

using namespace std;

struct OpCode
{
	int operation;
	int* parameters = new int[MAXOPCODEPARAMS];
	int* paramModes = new int[MAXOPCODEPARAMS];
	
	void clearParams()
	{
		for(int i=0; i<MAXOPCODEPARAMS; i++)
		{
			paramModes[i] = -1;
			parameters[i] = -1;
		}
		
	}
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
//OPCODE format: ABCDE, C, B, A, [start of next opcode].
//               |||^^ Two digit operation
//               ^^^   Parameter modes. May be more or less of them depending on the operation. Leftmost 0-mode (position) may be omitted in the spec
void readOpCode(int* memory, OpCode* opcode, int pc)
{
	int protoOpCode = memory[pc]; //This includes the operation but also parameter modes
	opcode->clearParams();
	opcode->operation = extractDecimalDigit(protoOpCode,1)*10 + extractDecimalDigit(protoOpCode,0);
	
	if(opcode->operation == EXIT) //99 is our stop opcode
		exit(0);
	
	for(int opcodeParam = 0; opcodeParam < OPCODEPARAMS[opcode->operation]; opcodeParam++)
	{
		opcode->paramModes[opcodeParam]=extractDecimalDigit(protoOpCode,opcodeParam+2);
		opcode->parameters[opcodeParam]=memory[pc+opcodeParam+1];
	}
	
}

int readMem(int* memory, int param, const int mode)
{
	switch(mode)
	{
		case POSITION:
			return memory[param];
		case IMMEDIATE:
			return param;
		default:
			cout << "INVALID MEMORY MODE: " << mode << "\n";
			exit(-1);
	}
}

void writeMem(int* memory, const int data, const int param, const int mode)
{
	switch(mode)
	{
		case POSITION:
			memory[param] = data;
		case IMMEDIATE:
			cout << "NONSENSICAL MEMORY MODE: " << mode << "\n";
		default:
			cout << "INVALID MEMORY MODE: " << mode << "\n";
			exit(-1);
	}
}

void intCodeInterpreter(int* memory)
{
	int pc = 0; //The program counter
	OpCode* opcode = new OpCode();
	while(true)
	{
		readOpCode(memory, opcode, pc);
		switch(opcode->operation)
		{
			case ADD:
				//memory[targetAddr] = memory[sourceAddr1] + memory[sourceAddr2];
				break;
			case MUL:
				//memory[targetAddr] = memory[sourceAddr1] * memory[sourceAddr2];
				break;
			case STOR:
				break;
			case PRNT:
				break;
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
