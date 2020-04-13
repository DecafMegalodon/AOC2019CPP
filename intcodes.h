#ifndef INTCODES_H
#define INTCODES_H

//OpCode names
const int ADD = 1;
const int MUL = 2;
const int STOR = 3;
const int PRNT = 4;
const int JIT = 5;
const int JIF = 6;
const int LT = 7;
const int EQ = 8;
const int ARBO = 9; //Adjust the releative base offset
const int EXIT = 99;

//Address modes
const int POSITION = 0;
const int IMMEDIATE = 1;
const int RELATIVE = 2;

const int MAXOPCODEPARAMS = 4;

const int OPCODEPARAMS [] = {-1,3,3,1,1,2,2,3,3,1};
static const char* OPCODENAMES[] = {"NONE", "ADD", "MULT", "STOR", "PRNT", "JIT", "JIF", "LT", "EQ", "ARBO"};
//[NONE,add,multiple,store,print, jumpiftrue, jumpiffalse, lessthan, equals, AdjustRelBaseOffset]

const int MEMSIZE = 1024;

//Memory offsets for storing various things in RAM. This is from the last memory location working towards 0
const int PCOFFSET = 1; //Program counter, updated on suspended execution
const int RBOSTORE = 2; //Relative base offset

struct OpCode
{
	long operation;
	long* parameters = new long[MAXOPCODEPARAMS];
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

//Reads an initial memory state from standard in
void initializeMemory(long* memory, int size);

//Extracts a single decimal digit from number, in the digit-th spot. Right to left, starting at 0
//If there is no such decimal digit it will be treated as being padded with zeros
int extractDecimalDigit(int number, int digit);

//Reads an opcode starting from the current program counter
//OPCODE format: ABCDE, C, B, A, [start of next opcode].
//               |||^^ Two digit operation
//               ^^^   Parameter modes. May be more or less of them depending on the operation. Leftmost 0-mode (position) may be omitted in the spec
//C B A are the parameters for the opcode and match with the corresponding mode packed with the opcode
bool readOpCode(long* memory, OpCode* opcode, int pc);

//Prints RAM in slightly more human readable chunks of integers
void printRAM(long* memory, int size);

//Reads a specified piece of memory (param) in the specified parameter mode (mode)
long readMem(long* memory, const int param, const int mode);

//Ditto, but writing to memory instead
void writeMem(long* memory, const long data, const int param, const int mode);

#endif