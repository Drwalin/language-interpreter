
#pragma once

#include "Compiler.cpp"
#include "MethodsCPU.cpp"

int MyAssemblyLang::DoOnce()
{
	++counterActions;
	int64 temp1, temp2, temp;
	double dtemp1, dtemp2, dtemp3;
	uint64 utemp1, utemp2, utemp3;
	switch( data[pointer++] )
	{
	case END:
		return 0;
		break;
	case JUMP:
		pointer = GetIntFrom( pointer );
		break;
	case JUMPTRUE:
		if( PopValue() != 0 )
		{
			pointer = GetIntFrom( pointer );
		}
		else
		{
			pointer += 8;
		}
		break;
	case JUMPFALSE:
		if( PopValue() == 0 )
		{
			pointer = GetIntFrom( pointer );
		}
		else
		{
			pointer += 8;
		}
		break;
		
		
		
	case CALL:		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		PushValue( pointer );	// pointer
		pointer = GetIntFrom( pointer );
		PushValue( localVariableOffset );	// utemp2
		utemp1 = GetIntFrom( pointer );
		localVariableOffset = AllocateMemory( utemp1 );
		PushValue( utemp1 );	// utemp1
		pointer += 8;
		break;
	case RET:		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		utemp1 = PopValue();
		FreeMemory( localVariableOffset, utemp1 );
		localVariableOffset = PopValue();
		pointer = PopValue();
		break;
		
		
		
	case PUSHADRESSVALUEGLOBAL:
		PushValue( GetIntFrom( GetIntFrom( GetIntFrom( pointer ) ) ) );
		pointer += 8;
		break;
	case PUSHGLOBAL:
		PushValue( GetIntFrom( GetIntFrom( pointer ) ) );
		pointer += 8;
		break;
	case PUSHADRESSGLOBAL:
		PushValue( GetIntFrom( pointer ) );
		pointer += 8;
		break;
	case POPGLOBAL:
		SetIntAt( PopValue(), GetIntFrom( pointer ) );
		pointer += 8;
		break;
	case POPGLOBALADRESS:
		SetIntAt( PopValue(), GetIntFrom( GetIntFrom( pointer ) ) );
		pointer += 8;
		break;
	case PUSHADRESSVALUELOCAL:
		PushValue( GetIntFrom( GetIntFrom( GetIntFrom( localVariableOffset + pointer ) ) ) );
		pointer += 8;
		break;
	case PUSHLOCAL:
		PushValue( GetIntFrom( GetIntFrom( localVariableOffset + pointer ) ) );
		pointer += 8;
		break;
	case PUSHADRESSLOCAL:
		PushValue( GetIntFrom( localVariableOffset + pointer ) );
		pointer += 8;
		break;
	case POPLOCAL:
		SetIntAt( PopValue(), GetIntFrom( localVariableOffset + pointer ) );
		pointer += 8;
		break;
	case POPLOCALADRESS:
		SetIntAt( PopValue(), GetIntFrom( GetIntFrom( pointer ) ) );
		pointer += 8;
		break;
	case PUSHCONST:
		PushValue( GetIntFrom( pointer ) );
		pointer += 8;
		break;
	case PRINTINTNEWLINE:
		printf( "\n " );
		std::cout << (int64)PopValue();
		break;
	case SCANINTKEYBOARD:
		printf( "\n " );
		temp = 0;
		std::cin >> temp;
		PushValue( (uint64)temp );
		break;
	case ALLOCATEMEMORY:
		PushValue( AllocateMemory( PopValue() ) );
		break;
	case FREEMEMORY:
		temp1 = PopValue(); temp2 = PopValue();
		FreeMemory( temp2, temp1 );
		break;
	case RESERVEMEMORY:
		ReserveMemory( PopValue() );
		break;
	case FREERESERVEDMEMORY:
		FreeReservedMemory();
		break;
	case ALU:
		switch( data[pointer++] )
		{
		case ADD:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2+temp1) );
			break;
		case SUB:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2-temp1) );
			break;
		case DIV:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2/temp1) );
			break;
		case MOD:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2%temp1) );
			break;
		case MUL:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2*temp1) );
			break;
		case AND:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2&temp1) );
			break;
		case NAND:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( ~uint64(temp2&temp1) );
			break;
		case OR:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2|temp1) );
			break;
		case NOR:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( ~uint64(temp2|temp1) );
			break;
		case XOR:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2^temp1) );
			break;
		case XNOR:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( ~uint64(temp2^temp1) );
			break;
		case NOT:
			PushValue( ~PopValue() );
			break;
		case POW:
			temp1 = PopValue(); temp2 = PopValue();
			temp = pow( (double)temp2, (double)temp1 );
			PushValue( uint64(temp) );
			break;
		case SQRT:
			temp = PopValue();
			temp = sqrt( (double)temp );
			PushValue( uint64(temp) );
			break;
		case SHIFTLEFT:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( temp2<<temp1 );
			break;
		case SHIFTRIGHT:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( temp2>>temp1 );
			break;
		case EQUAL:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2==temp1) );
			break;
		case NOTEQUAL:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2!=temp1) );
			break;
		case LESS:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2<temp1) );
			break;
		case GRATER:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2>temp1) );
			break;
		case LESSEQUAL:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2<=temp1) );
			break;
		case GRATEREQUAL:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2>=temp1) );
			break;
		case TOBOOLEAN:
			PushValue( uint64(PopValue()!=0) );
			break;
		default:
			return 0;
		}
		break;
	default:
		return 0;
	}
	return 1;
}



