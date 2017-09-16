
#pragma once

#include <cstdio>
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <stack>
#include <map>
#include <string>
#include <cstring>

#include "GetSthFromString.cpp"

typedef unsigned char byte;
typedef unsigned long long int uint64;
typedef long long int int64;

class MyAssemblyLang
{
private:

	std::vector < byte > data;
	std::vector < byte > cache;
	uint64 cacheOffset, localVariableOffset;
	uint64 pointer;
	std::map < byte, fstream > file;

	std::map < std::string, std::vector < byte > > commands;

public:
	
	static int64 GetInt64FromString( std::string str, int & err );
	
	void End();
	
	void SetIntAt( uint64 var, uint64 ptr );
	uint64 GetIntFrom( uint64 ptr );
	
	// push -  allocate   1024 bytes blocks (if needed)
	// pop - reallocate n 1024 bytes blocks (if cache.size()-cacheOffset > n*1024)
	void PushBytes( std::vector < byte > & src );
	void PopBytes( std::vector < byte > & src, uint64 count );
	void PushValue( uint64 val, uint64 count );
	void PopValue( uint64 & val, uint64 count );
	uint64 PopValue( uint64 count );

	void Clear();
	void PrimitiveCompiler( const char * fileName );

	int DoOnce();
	void Do( const uint64 count );

	MyAssemblyLang();
	~MyAssemblyLang();
};

enum MAIN_COMMENDS
{
	END = 0,
	ALU = 1,

	JUMP = 2,				// pointer = adress
	//JUMPTRUE = 3,			// if( cache.top()->val != 0 ) JUMP else ++pointer
	//JUMPFALSE = 4,		// if( cache.top()->val == 0 ) JUMP else ++pointer

	/*
	FILEOPEN = 5,			// std::ios::binary
	FILECLOSE = 6,
	FILEREADCHAR = 7,
	FILEREADSTRING = 8,
	FILEREADINT = 9,
	FILESAVECHAR = 10,
	FILESAVESTRING = 11,
	FILESAVEINT = 12,
	FILEGETSIZE = 13,
	FILEJUMP = 14,
	FILEGETPOS = 15,
	*/

	PUSHADRESSGLOBAL = 16,			// push( adress )
//	PUSHADRESSLOCAL = 17,			// push( adress )
	PUSHGLOBAL = 18,
//	PUSHLOCAL = 19,
	PUSHCONST = 20,
//	POPLOCAL = 21,
	POPGLOBAL = 22,

	/*
	PRINTCHAR = 23,
	PRINTSTRING = 24,
	PRINTINT = 25,
	GETCHAR = 26,
    GETSTRING = 27,
    GETINT = 28,
	GOTOXY = 29,
	*/

//	GETKEYBOARDBUFFERCHAR = 30,
	
	/*
	CALL = 31,
	RET = 32,
	
	ALLOCATE = 33,
	FDEALLOCATE = 34,
	*/
	
	PRINTINTNEWLINE,		/////////////////////////////////////////////////////
	SCANINTKEYBOARD			/////////////////////////////////////////////////////
};

enum ALU_COMMENDS
{
	ADD = 0,
	SUB = 1,
	DIV = 2,
	MOD = 3,
	MUL = 4,

	AND = 5,
	NAND = 6,
	OR = 7,
	NOR = 8,
	XOR = 9,
	XNOR = 10
	NOT = 11,

	POW = 12,
	SQRT = 13,
	LOG = 14,

	SHIFTLEFT = 15,
	SHIFTRIGHT = 16,

	EQUAL = 17,
	NOTEQUAL = 18,
	LESS = 19,
	GRATER = 20,
	LESSEQUAL = 21,
	GRATEREQUAL = 22,
	
	/*
	STRINGEQUAL = 23,
	STRINGNOTEQUAL = 24,
	STRINGLESS = 25,
	STRINGGRATER = 26,
	STRINGLESSEQUAL = 27,
	STRINGGRATEREQUAL = 28,
	*/
	
	TOBOOLEAN = 29
};



