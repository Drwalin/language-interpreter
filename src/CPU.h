
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
#include <cstdlib>

#include "GetSthFromString.cpp"
#include "Array.cpp"

typedef unsigned char byte;
typedef unsigned long long int uint64;
typedef long long int int64;

class MyAssemblyLang
{
public:
	
	int64 temp, temp1, temp2;
	double dtemp, dtemp1, dtemp2, dtemp3;
	uint64 utemp, utemp1, utemp2, utemp3;
	std::string stemp, stemp1, stemp2;
	char ctemp;
	char * stempclear;
	
	Array < byte > data;
	Array < byte > cache;
	
	uint64 localVariableOffset;
	uint64 pointer;
	std::map < uint64, std::fstream > file;
	uint64 counterActions;		// counter of iterations
	
	Array < uint64 > freeMemoryMap;
	Array < uint64 > functionCache;
	
	bool debug;
	
public:
	
	void SetDebug( bool src );
	
	static int64 GetInt64FromString( std::string str, int & err );
	
	void End();
	
	void SetIntAt( uint64 var, uint64 ptr );
	uint64 GetIntFrom( uint64 ptr );
	
	void PushBytes( Array < byte > & src );
	void PopBytes( Array < byte > & src, uint64 count );
	void PushValue( uint64 val );
	void PopValue( uint64 & val );
	uint64 PopValue();
	
	uint64 AllocateMemory( uint64 size );
	void FreeMemory( uint64 beg, uint64 size );
	void ReserveMemory( uint64 size );
	void FreeReservedMemory();

	void Clear();
	void PrimitiveCompiler( const char * fileName );
	void CompileComplexToPrimitive( const char * fileName, const char * fileNameAsm /*out*/ );
	void Preprocessor( const char * fileName, const char * fileNameComplex /*out*/ );
	void Load( const char * fileName );
	void Save( const char * fileName );

	int DoOnce();
	void Do( const uint64 count );

	MyAssemblyLang();
	~MyAssemblyLang();
};

enum MAIN_COMMANDS
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

	
	PRINTCHAR,
	PRINTSTRING,
	PRINTINT,
	GETCHAR,
	GETSTRING,
	GETINT,
	GOTOXY,			// first push x, second push y
	CLEARSCR,		// clear screen
//	GETKEYBOARDBUFFERCHAR,
	
	CALL,
	RET,
	
	ALLOCATEMEMORY,
	FREEMEMORY,
	RESERVEMEMORY,
	FREERESERVEDMEMORY,
	
	DATACOPY
	/*
	STRINGLENGTH,
	*/
};

enum ALU_COMMANDS
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
	
	STRINGEQUAL,
	STRINGNOTEQUAL,
	STRINGLESS,
	STRINGGRATER,
	STRINGLESSEQUAL,
	STRINGGRATEREQUAL,
	//STRINGTOUPPERCASE,
	//STRINGTOLOWERCASE,
	
	TOBOOLEAN
};



