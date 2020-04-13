/*
This file contains some functions common to all intcode challenges.
Some functions may not be included because they are percived as
potentially breaking to future or past challenges if they are updated
*/

#include <iostream>
#include <cmath>
#include "intcodes.h"

using namespace std;
//Read an initial memory state from stdin. 
void initializeMemory(long* memory, int size)
{
	long curRead;
	int addr = 0;
	char seperator;
	do
	{
		seperator='\0';
		scanf("%li%c",&curRead, &seperator);
		memory[addr] = curRead;
		if(addr >= size)
		{
			cout << "Memory overflow. Please download more RAM.\n";
			exit(-1);
		}
		addr++;
	}
	while (seperator == ',');
	for(; addr < size; ++addr)
	{
		memory[addr] = 0;
	}
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
bool readOpCode(long* memory, OpCode* opcode, int pc)
{
	long protoOpCode = memory[pc]; //This includes the operation but also parameter modes
	opcode->clearParams();
	opcode->operation = extractDecimalDigit(protoOpCode,1)*10 + extractDecimalDigit(protoOpCode,0);
	
	if(opcode->operation == EXIT) //EXIT is our halt opcode
		return false;
	
	if(opcode -> operation == 0 || opcode->operation > 10) //Invalid opcode detected
	{
		cout << "Invalid opcode detected: " << opcode->operation << endl;
		return false;	
	}

	
	for(int opcodeParam = 0; opcodeParam < OPCODEPARAMS[opcode->operation]; opcodeParam++)
	{
		opcode->paramModes[opcodeParam]=extractDecimalDigit(protoOpCode,opcodeParam+2);
		opcode->parameters[opcodeParam]=memory[pc+opcodeParam+1];
	}
	return true;
}

void printRAM(long* memory, int size)
{
	for(int addr = 0; addr < size; addr+= 8)
	{
		printf("[%5d]", addr);
		for(int offset = 0; offset < 8; offset++)
		{
			printf("%8li", memory[addr+offset]);
		}
		cout << endl;
	}
	cout << "\n";
}

long readMem(long* memory, const int param, const int mode)
{
	switch(mode)
	{
		int address;
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
		case RELATIVE:
			address = memory[MEMSIZE-RBOSTORE]+param;
			if(address >= 0 && address < MEMSIZE)
				return memory[address];
			else
			{
				cout << "Memory overflow. Please download more RAM.\n";
				exit(-1);
			}
			break;
		default:
			cout << "INVALID MEMORY MODE: " << mode << "\n";
			exit(-1);
	}
}

void writeMem(long* memory, const long data, const int param, const int mode)
{
	switch(mode)
	{
		int address;
		case POSITION:
			if(param < MEMSIZE)
			{
				memory[param] = data;
				return;
				//cout << data << "Write->" << param << endl;
			}
			else
			{
				cout << "Memory overflow. Please download more RAM.\n";
				exit(-1);
			}
		case IMMEDIATE:
			cout << "NONSENSICAL MEMORY MODE: " << mode << "\n";
			exit(-1);
		case RELATIVE:
			address = memory[MEMSIZE-RBOSTORE]+param;
			if(address >= 0 && address < MEMSIZE)
				memory[address]=data;
			else
			{
				cout << "Memory overflow. Please download more RAM.\n";
				exit(-1);
			}
		default:
			cout << "INVALID MEMORY MODE: " << mode << "\n";
			exit(-1);
	}
}