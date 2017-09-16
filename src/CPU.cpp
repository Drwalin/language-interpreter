
#pragma once

#include "Compiler.cpp"
#include "MethodsCPU.cpp"

int MyAssemblyLang::DoOnce()
{
	++counterActions;
//	printf( "\n Current action number: %lli \n Pointer: %lli \n", (int64)counterActions, (int64)pointer );
	int64 temp1, temp2, temp;
	switch( data[pointer++] )
	{
	case END:
//		printf( " end\n" );
		return 0;
		break;
	case JUMP:
//		printf( " jump\n" );
		pointer = GetIntFrom( pointer );
		break;
	case PUSHADRESSGLOBAL:
//		printf( " pushadressglobal\n" );
		PushValue( GetIntFrom( pointer ), 8 );
		pointer += 8;
		break;
	case PUSHGLOBAL:
//		printf( " pushglobal\n" );
		PushValue( GetIntFrom( GetIntFrom( pointer ) ), 8 );
		pointer += 8;
		break;
	case PUSHCONST:
//		printf( " pushconst\n" );
		PushValue( GetIntFrom( pointer ), 8 );
		pointer += 8;
		break;
	case POPGLOBAL:
//		printf( " popglobal\n" );
		SetIntAt( PopValue( 8 ), GetIntFrom( pointer ) );
		pointer += 8;
		break;
	case PRINTINTNEWLINE:
//		printf( " printf\n" );
		printf( "\n " );
		std::cout << (int64)PopValue( 8 );
		break;
	case SCANINTKEYBOARD:
//		printf( " scanf\n" );
		printf( "\n " );
		temp = 0;
		std::cin >> temp;
		PushValue( (uint64)temp, 8 );
		break;
	case ALU:
//		printf( " alu\n" );
#define uint64
		switch( data[pointer++] )
		{
		case ADD:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2+temp1), 8 );
			break;
		case SUB:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2-temp1), 8 );
			break;
		case DIV:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2/temp1), 8 );
			break;
		case MOD:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2%temp1), 8 );
			break;
		case MUL:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2*temp1), 8 );
			break;
		case AND:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2&temp1), 8 );
			break;
		case NAND:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( ~uint64(temp2&temp1), 8 );
			break;
		case OR:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2|temp1), 8 );
			break;
		case NOR:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( ~uint64(temp2|temp1), 8 );
			break;
		case XOR:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2^temp1), 8 );
			break;
		case XNOR:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( ~uint64(temp2^temp1), 8 );
			break;
		case NOT:
			PushValue( ~PopValue( 8 ), 8 );
			break;
		case POW:////////////////////////////////////////////////////
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2+temp1), 8 );
			break;
		case SQRT:///////////////////////////////////////////////////
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2+temp1), 8 );
			break;
		case LOG:////////////////////////////////////////////////////
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2+temp1), 8 );
			break;
		case SHIFTLEFT:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( temp2<<temp1, 8 );
			break;
		case SHIFTRIGHT:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( temp2>>temp1, 8 );
			break;
		case EQUAL:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2==temp1), 8 );
			break;
		case NOTEQUAL:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2!=temp1), 8 );
			break;
		case LESS:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2<temp1), 8 );
			break;
		case GRATER:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2>temp1), 8 );
			break;
		case LESSEQUAL:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2<=temp1), 8 );
			break;
		case GRATEREQUAL:
			temp1 = PopValue( 8 ); temp2 = PopValue( 8 );
			PushValue( uint64(temp2>=temp1), 8 );
			break;
		case TOBOOLEAN:
			PushValue( uint64(PopValue(8)!=0), 8 );
			break;
		default:
			return 0;
		}
#undef uint64
//		printf( "\n = %lli ", (int64)cache[cache.size()-8] );
		break;
	default:
		return 0;
	}
	return 1;
}



