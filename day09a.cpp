#include <iostream>
#include <string>
#include <cmath>
#include <cstring>
#include <algorithm>
#include "intcodes.h"
#define DEBUG 0

using namespace std;

//Our intcode computer.
int intCodeInterpreter(long* memory)
{
	int pc = 0;
	long result;
	OpCode* opcode = new OpCode();
	while(true)
	{
		if (!readOpCode(memory, opcode, pc)) return -1;
		//printf("[%5d] %4s\n", pc, OPCODENAMES[opcode->operation]);
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
				result = readMem(memory, opcode->parameters[0], opcode->paramModes[0]) *
								readMem(memory, opcode->parameters[1], opcode->paramModes[1]);
				printf("%li x %li = %li\n", readMem(memory, opcode->parameters[0], opcode->paramModes[0]), readMem(memory, opcode->parameters[1], opcode->paramModes[1]), result);
				writeMem(memory, result,
								opcode->parameters[2],
								opcode->paramModes[2]);
				break;
			case STOR: //Prompt for an integer input. We're not going to try to catch bad input in this scenario.
				cout << "Please enter a number\n";
				long input;
				scanf("%li",&input);
				writeMem(memory, input, opcode->parameters[0], opcode->paramModes[0]);				
			case PRNT: //Returns a memory value from the system
				printf("%li\n", readMem(memory, opcode->parameters[0], opcode->paramModes[0]));
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
			case ARBO: //Adjust the relative base offset
				writeMem(memory, 
						readMem(memory, opcode->parameters[0], opcode->paramModes[0])+
							memory[MEMSIZE-RBOSTORE],
						MEMSIZE-RBOSTORE,
						POSITION);
				break;
			default:
				cout << "INVALID OPCODE ERROR: " << opcode->operation << "\n";
				exit(-1);
		}
	}
}

int main()
{
	long* memory = new long[MEMSIZE]; //Out intcode computer's RAM
	initializeMemory(memory,MEMSIZE);
	intCodeInterpreter(memory);
	printRAM(memory,32);
}
