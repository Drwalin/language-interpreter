
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
#include "Array.cpp"

typedef unsigned char byte;
typedef unsigned long long int uint64;
typedef long long int int64;

class MyAssemblyLang
{
public:
	
	Array < byte > data;
	Array < byte > cache;
	
	uint64 cacheOffset;
	uint64 localVariableOffset;
	uint64 pointer;
	std::map < uint64, std::fstream > file;
	uint64 counterActions;		// counter of iterations
	
	std::vector < uint64 > freeMemoryMap;
	std::vector < uint64 > functionCache;
	
	bool debug;
	
public:
	
	void SetDebug( bool src );
	
	static inline int64 GetInt64FromString( std::string str, int & err );
	
	void End();
	
	inline void SetIntAt( uint64 var, uint64 ptr );
	inline uint64 GetIntFrom( uint64 ptr );
	
	// push -  allocate   4096 bytes blocks (if needed)
	// pop - deallocate n 4096 bytes blocks (if cache.size()-cacheOffset > n*1024)
	inline void PushBytes( std::vector < byte > & src );
	inline void PopBytes( std::vector < byte > & src, uint64 count );
	inline void PushValue( uint64 val );
	inline void PopValue( uint64 & val );
	inline uint64 PopValue();
	
	uint64 AllocateMemory( uint64 size );
	void FreeMemory( uint64 beg, uint64 size );
	inline void ReserveMemory( uint64 size );
	inline void FreeReservedMemory();

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
	FREERESERVEDMEMORY
	
	/*
	STRINGLENGTH,
	DATACOPY
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



