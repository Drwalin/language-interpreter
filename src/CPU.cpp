
#pragma once

#include "Compiler.cpp"
#include "MethodsCPU.cpp"

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



