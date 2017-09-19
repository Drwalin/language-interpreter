
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
public:

	std::vector < byte > data;
	std::vector < byte > cache;
	uint64 cacheOffset, localVariableOffset;
	uint64 pointer;
	std::map < byte, std::fstream > file;
	uint64 counterActions;		// counter of iterations

	std::vector < uint64 > freeMemoryMap;

public:
	
	static int64 GetInt64FromString( std::string str, int & err );
	
	void End();
	
	void SetIntAt( uint64 var, uint64 ptr );
	uint64 GetIntFrom( uint64 ptr );
	
	// push -  allocate   4096 bytes blocks (if needed)
	// pop - reallocate n 4096 bytes blocks (if cache.size()-cacheOffset > n*1024)
	void PushBytes( std::vector < byte > & src );
	void PopBytes( std::vector < byte > & src, uint64 count );
	void PushValue( uint64 val );
	void PopValue( uint64 & val );
	uint64 PopValue();
	
	uint64 AllocateMemory( uint64 size );
	void FreeMemory( uint64 beg, uint64 size );
	void ReserveMemory( uint64 size );
	void FreeReservedMemory();

	void Clear();
	void PrimitiveCompiler( const char * fileName );
	void CompileComplexToPrimitive( const char * fileName, const char * fileNameAsm /*OUT*/ );
	void Load( const char * fileName );
	void Save( const char * fileName );

	int DoOnce();
	void Do( const uint64 count );

	MyAssemblyLang();
	~MyAssemblyLang();
};

enum MAIN_COMMENDS
{
	END,
	ALU,

	JUMP,				// pointer = adress
	JUMPTRUE,			// if( cache.top()->val != 0 ) JUMP else ++pointer
	JUMPFALSE,			// if( cache.top()->val == 0 ) JUMP else ++pointer

	/*
	FILEOPEN,			// std::ios::binary
	FILECLOSE,
	FILEREADBYTE,
	FILEREADSTRING,
	FILEREADINT,
	FILESAVEBYTE,
	FILESAVESTRING,
	FILESAVEINT,
	FILEGETSIZE,
	FILEJUMP,
	FILEGETPOS,
	*/

	PUSHADRESSGLOBAL,			// push( adress )
	PUSHADRESSVALUEGLOBAL,		// push( value( adress ) )
	PUSHGLOBAL,
	POPGLOBAL,
	POPGLOBALADRESS,
	PUSHADRESSLOCAL,			// push( adress )
	PUSHADRESSVALUELOCAL,		// push( value( adress ) )
	PUSHLOCAL,
	POPLOCAL,
	POPLOCALADRESS,
	PUSHCONST,

	/*
	PRINTCHAR,
	PRINTSTRING,
	PRINTINT,
	GETCHAR,
    GETSTRING,
    GETINT,
	GOTOXY,
	*/

//	GETKEYBOARDBUFFERCHAR,
	
	CALL,
	RET,
	
	ALLOCATEMEMORY,
	FREEMEMORY,
	RESERVEMEMORY,
	FREERESERVEDMEMORY,
	
	PRINTINTNEWLINE,		/////////////////////////////////////////////////////
	SCANINTKEYBOARD			/////////////////////////////////////////////////////
	
	/*
	STRINGLENGTH,
	DATACOPY
	*/
};

enum ALU_COMMENDS
{
	ADD,
	SUB,
	DIV,
	MOD,
	MUL,

	AND,
	NAND,
	OR,
	NOR,
	XOR,
	XNOR,
	NOT,

	POW,
	SQRT,

	SHIFTLEFT,
	SHIFTRIGHT,

	EQUAL,
	NOTEQUAL,
	LESS,
	GRATER,
	LESSEQUAL,
	GRATEREQUAL,
	
	/*
	STRINGEQUAL,
	STRINGNOTEQUAL,
	STRINGLESS,
	STRINGGRATER,
	STRINGLESSEQUAL,
	STRINGGRATEREQUAL,
	*/
	
	TOBOOLEAN
};



