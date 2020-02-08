#include <iostream>
#include <string>
#include <cmath>
#define DEBUG 0
int MEMSIZE = 700;
int MAXOPCODEPARAMS = 4;
int OPCODEPARAMS [] = {-1,3,3,1,1,2,2,3,3};
//int memory[MEMSIZE];
//[NONE,add,multiple,store,print, jumpiftrue, jumpiffalse, lessthan, equal]

//OpCode names
const int ADD = 1;
const int MUL = 2;
const int PRMPT = 3;
const int PRNT = 4;
const int JIT = 5;
const int JIF = 6;
const int LT = 7;
const int EQ = 8;
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

//Read an initial memory state from stdin
void initializeMemory(int* memory)
{
	int curRead;
	int addr = 0;
	char seperator;
	do
	{
		seperator='\0';
		scanf("%d%c",&curRead, &seperator);
		memory[addr] = curRead;
		if(addr >= MEMSIZE)
		{
			cout << "Memory overflow. Please download more RAM.\n";
			exit(-1);
		}
		addr++;
	}
	while (seperator == ',');
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
//C B A are the parameters for the opcode and match with the corresponding mode packed with the opcode
void readOpCode(int* memory, OpCode* opcode, int pc)
{
	int protoOpCode = memory[pc]; //This includes the operation but also parameter modes
	opcode->clearParams();
	opcode->operation = extractDecimalDigit(protoOpCode,1)*10 + extractDecimalDigit(protoOpCode,0);
	
	if(opcode->operation == EXIT) //EXIT is our halt opcode
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
			if(param < MEMSIZE)
				return memory[param];
			else
			{
				cout << "Memory overflow. Please download more RAM.\n";
				exit(-1);
			}
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
			if(param < MEMSIZE)
				memory[param] = data;
			else
			{
				cout << "Memory overflow. Please download more RAM.\n";
				exit(-1);
			}
			break;
		case IMMEDIATE:
			cout << "NONSENSICAL MEMORY MODE: " << mode << "\n";
			exit(-1);
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
		pc += OPCODEPARAMS[opcode->operation] + 1;
		switch(opcode->operation)
		{
			case ADD: //Add two numbers together and store them. [addend, addend, store]
				writeMem(memory, readMem(memory, opcode->parameters[0], opcode->paramModes[0])+
							    readMem(memory, opcode->parameters[1], opcode->paramModes[1]),
								opcode->parameters[2],
								opcode->paramModes[2]);
				break;
			case MUL: //Multiply two numbers together and store them
				writeMem(memory, readMem(memory, opcode->parameters[0], opcode->paramModes[0])*
							    readMem(memory, opcode->parameters[1], opcode->paramModes[1]),
								opcode->parameters[2],
								opcode->paramModes[2]);
				break;
			case PRMPT: //Prompt for an integer input. We're not going to try to catch bad input in this scenario.
				cout << "Please enter a number\n";
				int input;
				scanf("%d",&input);
				writeMem(memory, input, opcode->parameters[0], opcode->paramModes[0]);
				break;
			case PRNT: //Print a number from the system
				cout << readMem(memory, opcode->parameters[0], opcode->paramModes[0]) << "\n";
				break;
			case JIT: //Jump if true. If param 1 != 0, set PC to second. Otherwise NOOP
				break;
			case JIF: //Jump if true. If param 1 == 0, set PC to second. Otherwise NOOP
				break;
			case LT:
				break;
			case EQ:
				break;
			default:
				cout << "INVALID OPCODE ERROR: " << opcode->operation << "\n";
				exit(-1);
		}
	}
}

int main()
{
	int memory[MEMSIZE];
	initializeMemory(memory);
	intCodeInterpreter(memory);
}
