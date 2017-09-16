
#pragma once

#include "Compiler.cpp"
#include "MethodsCPU.cpp"


class MyAssemblyLang
{
private:

	std::vector < byte > data;
	std::vector < byte > cache;
	uint64 cacheOffset, localVariableOffset;
	uint64 pointer;
	std::map < byte, fstream > file;

public:
	
	void End();
	
	void SetIntAt( uint64 var, uint64 ptr );
	uint64 GetIntFrom( uint64 ptr );
	
	void PushBytes( std::vector < byte > & src );
	void PopBytes( std::vector < byte > & src, uint64 count );
	void PushValue( uint64 val, uint64 count );
	void PopValue( uint64 & val, uint64 count );
	uint64 PopValue( uint64 count );
};

int MyAssemblyLang::DoOnce()
{
	switch( data[pointer++] )
	{
	case END:
		return 1;
		break;
	case JUMP:
		pointer = GetIntFrom( pointer );
		break;
	case PUSHADRESSGLOBAL:
		PushValue( GetIntFrom( pointer ), 8 );
		pointer += 8;
		break;
	case PUSHGLOBAL:
		PushValue( GetIntFrom( GetIntFrom( pointer ) ), 8 );
		pointer += 8;
		break;
	case PUSHCONST:
		PushValue( GetIntFrom( pointer ), 8 );
		pointer += 8;
		break;
	case POPGLOBAL:
		SetIntAt( PopValue( 8 ), GetIntFrom( pointer ) );
		pointer += 8;
		break;
	case PRINTINTNEWLINE:
		printf( "\n %lli", (int64)PopValue( 8 ) );
		break;
	case SCANINTKEYBOARD:
		int64 temp = 0;
		scanf( "%lli", &temp );
		PushValue( (uint64)temp, 8 );
		break;
	case ALU:
		switch( data[pointer++] )
		{
		case ADD:
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2+temp1), 8 );
			break;
		case SUB:
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2-temp1), 8 );
			break;
		case DIV:
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2/temp1), 8 );
			break;
		case MOD:
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2%temp1), 8 );
			break;
		case MUL:
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2*temp1), 8 );
			break;
		case AND:
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2&temp1), 8 );
			break;
		case NAND:
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( ~uint64(temp2&temp1), 8 );
			break;
		case OR:
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2|temp1), 8 );
			break;
		case NOR:
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( ~uint64(temp2|temp1), 8 );
			break;
		case XOR:
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2^temp1), 8 );
			break;
		case XNOR:
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( ~uint64(temp2^temp1), 8 );
			break;
		case NOT:
			PushValue( ~PopValue( 8 ), 8 );
			break;
		case POW:////////////////////////////////////////////////////
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2+temp1), 8 );
			break;
		case SQRT:///////////////////////////////////////////////////
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2+temp1), 8 );
			break;
		case LOG:////////////////////////////////////////////////////
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2+temp1), 8 );
			break;
		case SHIFTLEFT:
			uint64 temp1 = PopValue( 8 ); uint64 temp2 = PopValue( 8 );
			PushValue( temp2<<temp1, 8 );
			break;
		case SHIFTRIGHT:
			uint64 temp1 = PopValue( 8 ); uint64 temp2 = PopValue( 8 );
			PushValue( temp2>>temp1, 8 );
			break;
		case EQUAL:
			uint64 temp1 = PopValue( 8 ); uint64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2==temp1), 8 );
			break;
		case NOTEQUAL:
			uint64 temp1 = PopValue( 8 ); uint64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2!=temp1), 8 );
			break;
		case LESS:
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2<temp1), 8 );
			break;
		case GRATER:
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2>temp1), 8 );
			break;
		case LESSEQUAL:
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2<=temp1), 8 );
			break;
		case GRATEREQUAL:
			int64 temp1 = PopValue( 8 ); int64 temp2 = PopValue( 8 );
			PushValue( uint64(temp2>=temp1), 8 );
			break;
		case TOBOOLEAN:
			PushValue( uint64(PopValue(8)!=0), 8 )
			break;
		}
		break;
	}
	return 0;
}

enum MAIN_COMMENDS
{
	END = 0,
	ALU = 1,

	JUMP = 2,

	PUSHADRESSGLOBAL = 16,			// push( adress )
	PUSHGLOBAL = 19,
	PUSHCONST = 20,
	POPGLOBAL = 22,
	
	PRINTINTNEWLINE,
	SCANINTKEYBOARD
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
	
	TOBOOLEAN = 29
};






