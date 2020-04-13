#include <iostream>
#include <string>
#include "intcodes.h"

using namespace std;

//Disassemble and print RAM opcodes, in a somewhat dumb but hopefully helpful way
//For example, conditional jumps are ignored
void disassembleRAM(int* RAM, int size)
{
	OpCode* opcode = new OpCode();
	bool success;
	
	for(int addr = 0; addr < size; ++addr)
	{
		success = readOpCode(RAM, opcode, addr);
		if(success)
		{
			printf("[%5d] %4s\n", addr, OPCODENAMES[opcode->operation]);
			addr += OPCODEPARAMS[opcode->operation] + 1;
		}
		else
			addr += 1;
	}
}
