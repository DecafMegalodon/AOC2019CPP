#include <iostream>
#include <string>
#include <cmath>
#include <cstring>
#include <algorithm>
#include "intcodes.h"
#include "disassembleRAM.h"
#include <climits>

const int NUMAMPS=5;

using namespace std;


//Our intcode computer. Immediately returns the first value returned by PRNT and halts
int intCodeInterpreter(int* memory, int thrusterSetting, int thrusterInput)
{
	//cout << "Booting interpreter\n";
	bool isOnFirstInput = true;
	int pc = memory[MEMSIZE-1]; //The program counter. 
	OpCode* opcode = new OpCode();
	while(true)
	{
		if (!readOpCode(memory, opcode, pc))
			return INT_MIN; //If we encounter the EXIT opcode, stop the interpreter
		printf("[%5d] %4s\n", pc, OPCODENAMES[opcode->operation]);
		pc += OPCODEPARAMS[opcode->operation] + 1;
		printRAM(memory, 320);
		switch(opcode->operation)
		{
			case ADD: //Add two numbers together and store them. [addend, addend, store]
				printf("%d + %d\n", readMem(memory, opcode->parameters[0], opcode->paramModes[0]), 
							readMem(memory, opcode->parameters[1], opcode->paramModes[1]));
				writeMem(memory, readMem(memory, opcode->parameters[0], opcode->paramModes[0])+
							    readMem(memory, opcode->parameters[1], opcode->paramModes[1]),
								opcode->parameters[2],
								opcode->paramModes[2]);

				break;
			case MUL: //Multiply two numbers together and store them
				printf("%d x %d\n", readMem(memory, opcode->parameters[0], opcode->paramModes[0]), 
							readMem(memory, opcode->parameters[1], opcode->paramModes[1]));
				writeMem(memory, readMem(memory, opcode->parameters[0], opcode->paramModes[0])*
							    readMem(memory, opcode->parameters[1], opcode->paramModes[1]),
								opcode->parameters[2],
								opcode->paramModes[2]);
				break;
			case STOR: //Prompt for an integer input. We're not going to try to catch bad input in this scenario.
				if(isOnFirstInput)
				{
					writeMem(memory, thrusterSetting, opcode->parameters[0], opcode->paramModes[0]);
					isOnFirstInput = false;
				}
				else
					writeMem(memory, thrusterInput, opcode->parameters[0], opcode->paramModes[0]);
				break;
				
			case PRNT: //Returns a memory value from the system
				cout<<"Returning memory addr:"<<opcode->parameters[0]<<endl;
				memory[MEMSIZE-1]=pc; //Store our PC to resume execution if we make it back
				return readMem(memory, opcode->parameters[0], opcode->paramModes[0]);
				break;
			case JIT: //Jump if true. If param 1 != 0, set PC to second. Otherwise NOOP
				printf("Examining %d in mode %d for jump to %d in mode %d\n", opcode->parameters[0], opcode->paramModes[0], opcode->parameters[1], opcode->paramModes[1]);
				if(readMem(memory, opcode->parameters[1], opcode->paramModes[1]) != 0)
				{
					pc = readMem(memory, opcode->parameters[0], opcode->paramModes[0]);
					cout << "Jumping to PC=" << pc << endl;
				}
				else
					cout << "Not jumping\n";
				break;
			case JIF: //Jump if true. If param 1 == 0, set PC to second. Otherwise NOOP
				if(readMem(memory, opcode->parameters[0], opcode->paramModes[0]) == 0)
				{
					pc = readMem(memory, opcode->parameters[1], opcode->paramModes[1]);
					cout << "Jumping to PC=" << pc << endl;
				}
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

void printSettings(int amps[5])
{
	cout << "{";
	for(int i = 0; i < 5; ++i)
	{
		printf("%d ", amps[i]);
	}
	cout << "}\n";
}

int main()
{
	int* refMem = new int[MEMSIZE]; //The starting memory for all the amplifiers
	int* memory = new int[NUMAMPS*MEMSIZE];
	initializeMemory(refMem, MEMSIZE);
	int maxAmpSoFar= -1;
	int ampSettings [NUMAMPS] = {5,6,7,8,9};
	int ampIO;
	int maxAmpThisSetting;
    do {
		ampIO=0; //The input to the first amp is always zero
		resetMemories(refMem, memory);
		printSettings(ampSettings);
		
		maxAmpThisSetting = INT_MIN;
		do {
			for(int amp=0; amp<NUMAMPS; amp++)
			{
				//disassembleRAM(memory+amp*MEMSIZE, MEMSIZE);
				cout << "Amp no " << amp << endl;
				ampIO = intCodeInterpreter(memory+amp*MEMSIZE, ampSettings[amp], ampIO);
				cout << "ampIO = " << ampIO << endl;
				if(ampIO == INT_MIN)
				{
					cout << "Done with this setting\n";
					break;
				}
					
			}
			if(ampIO != INT_MIN) //If we completed all the amps
			{
				cout << "Yay!";
			}
			maxAmpThisSetting = max(maxAmpThisSetting, ampIO);
			maxAmpSoFar = max(maxAmpSoFar, maxAmpThisSetting);
			ampIO = INT_MIN;
		} while (ampIO != INT_MIN);
		maxAmpSoFar = max(maxAmpSoFar, maxAmpThisSetting);
    } while(std::next_permutation(ampSettings, ampSettings+NUMAMPS));
	std::cout << maxAmpSoFar << endl;
}