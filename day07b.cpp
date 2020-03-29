#include <iostream>
#include <string>
#include <cmath>
#include <cstring>
#include <algorithm>
#define DEBUG 0
const int MEMSIZE = 40960;
const int MAXOPCODEPARAMS = 4;
const int OPCODEPARAMS [] = {-1,3,3,1,1,2,2,3,3};

//[NONE,add,multiple,store,print, jumpiftrue, jumpiffalse, lessthan, equal]

const int NUMAMPS=5;

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
bool readOpCode(int* memory, OpCode* opcode, int pc)
{
	int protoOpCode = memory[pc]; //This includes the operation but also parameter modes
	opcode->clearParams();
	opcode->operation = extractDecimalDigit(protoOpCode,1)*10 + extractDecimalDigit(protoOpCode,0);
	
	if(opcode->operation == EXIT) //EXIT is our halt opcode
		return false;
	
	for(int opcodeParam = 0; opcodeParam < OPCODEPARAMS[opcode->operation]; opcodeParam++)
	{
		opcode->paramModes[opcodeParam]=extractDecimalDigit(protoOpCode,opcodeParam+2);
		opcode->parameters[opcodeParam]=memory[pc+opcodeParam+1];
	}
	return true;
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

//Our intcode computer. Immediately returns the first value returned by PRNT and halts
int intCodeInterpreter(int* memory, int thrusterSetting, int thrusterInput)
{
	bool isOnFirstInput = true;
	int pc = 0; //The program counter
	OpCode* opcode = new OpCode();
	while(true)
	{
		if (!readOpCode(memory, opcode, pc)) return -1;
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
				if(isOnFirstInput)
				{
					writeMem(memory, thrusterSetting, opcode->parameters[0], opcode->paramModes[0]);
					isOnFirstInput = false;
				}
				else
					writeMem(memory, thrusterInput, opcode->parameters[0], opcode->paramModes[0]);
				break;
				
			case PRNT: //Returns a memory value from the system
				return readMem(memory, opcode->parameters[0], opcode->paramModes[0]);
				break;
			case JIT: //Jump if true. If param 1 != 0, set PC to second. Otherwise NOOP
				if(readMem(memory, opcode->parameters[0], opcode->paramModes[0]) != 0)
					pc = readMem(memory, opcode->parameters[1], opcode->paramModes[1]);
				break;
			case JIF: //Jump if true. If param 1 == 0, set PC to second. Otherwise NOOP
				if(readMem(memory, opcode->parameters[0], opcode->paramModes[0]) == 0)
					pc = readMem(memory, opcode->parameters[1], opcode->paramModes[1]);
				break;
			case LT: //If param 1 < param 2, store 1 in param 3
				if(readMem(memory, opcode->parameters[0], opcode->paramModes[0]) <
						readMem(memory, opcode->parameters[1], opcode->paramModes[1]))
					writeMem(memory, 1, opcode->parameters[2], opcode->paramModes[2]);
				else
					writeMem(memory, 0, opcode->parameters[2], opcode->paramModes[2]);
				break;
			case EQ: //If param 1 == param 2, store 1 in param 3
				if(readMem(memory, opcode->parameters[0], opcode->paramModes[0]) ==
						readMem(memory, opcode->parameters[1], opcode->paramModes[1]))
					writeMem(memory, 1, opcode->parameters[2], opcode->paramModes[2]);
				else
					writeMem(memory, 0, opcode->parameters[2], opcode->paramModes[2]);
				break;
			default:
				cout << "INVALID OPCODE ERROR: " << opcode->operation << "\n";
				exit(-1);
		}
	}
}

void resetMemories(int* refmem, int* memory)
{
	for(int membank = 0; membank < NUMAMPS; membank++)
	{
		std::memcpy(memory+MEMSIZE*membank,refmem,MEMSIZE); //Reset the RAM for the AMP
	}
}

int main()
{
	int* refMem = new int[MEMSIZE]; //The starting memory for all the amplifiers
	int* memory = new int[NUMAMPS*MEMSIZE];
	initializeMemory(refMem);
	int maxAmpSoFar= -1;
	int ampSettings [NUMAMPS] = {5,6,7,8,9};
	int ampIO;
    do {
		ampIO=0; //The input to the first amp is always zero
		resetMemories(refMem, memory);
        for(int amp=0; amp<NUMAMPS; amp++)
		{
			ampIO = intCodeInterpreter(memory+amp*MEMSIZE, ampSettings[amp], ampIO);
		}
		if(maxAmpSoFar < ampIO)
		{
			maxAmpSoFar = ampIO;
		}
    } while(std::next_permutation(ampSettings, ampSettings+NUMAMPS));
	std::cout << maxAmpSoFar << endl;
}