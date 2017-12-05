
#pragma once

#include "ComplexCompiler.cpp"
#include "Compiler.cpp"
#include "MethodsCPU.cpp"
#include "SaveLoad.cpp"

#include "Gotoxy.cpp"

int MyAssemblyLang::DoOnce()
{
	++counterActions;
	switch( data[pointer++] )
	{
	case GOTOXY:
		temp1 = PopValue()/*y*/; temp2 = PopValue()/*x*/;
#ifdef DEBUG
		printf( ":gotoxy:" );
		getch();
#endif
		Gotoxy( temp2, temp1 );
#ifdef DEBUG
		printf( ":gotoxy-after:" );
		getch();
#endif
		break;
	case CLEARSCR:
#ifdef DEBUG
		printf( ":clearscr:" );
		getch();
#endif
		Gotoxy( 0, 0 );
		stempclear = new char[512];
		memset( stempclear, ' ', 311+1 );
		*((unsigned long long int*)(&(stempclear[311]))) = 0;
		for( temp = 0; temp < 311; ++temp )
			printf( stempclear );
		Gotoxy( 0, 0 );
		delete[] stempclear;
		break;
		
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
		functionCache.push_back( pointer );	// pointer
		pointer = GetIntFrom( pointer );
		functionCache.push_back( localVariableOffset );	// utemp2
		utemp1 = GetIntFrom( pointer );
		localVariableOffset = AllocateMemory( utemp1 );
		functionCache.push_back( utemp1 );	// utemp1
		pointer += 8;
#ifdef DEBUG
		printf( "\n call" );
#endif
		break;
	case RET:		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		utemp1 = functionCache.back();functionCache.resize(functionCache.size()-1);
		FreeMemory( localVariableOffset, utemp1 );
		localVariableOffset = functionCache.back();functionCache.resize(functionCache.size()-1);
		pointer = functionCache.back();functionCache.resize(functionCache.size()-1);
#ifdef DEBUG
		printf( "\n ret" );
#endif
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
		PushValue( GetIntFrom( GetIntFrom( localVariableOffset + GetIntFrom( pointer ) ) ) );
		pointer += 8;
		break;
	case PUSHLOCAL:
		PushValue( GetIntFrom( localVariableOffset + GetIntFrom( pointer ) ) );
		pointer += 8;
		break;
	case PUSHADRESSLOCAL:
		PushValue( localVariableOffset + GetIntFrom( pointer ) );
		pointer += 8;
		break;
	case POPLOCAL:
		SetIntAt( PopValue(), localVariableOffset + GetIntFrom( pointer ) );
		pointer += 8;
		break;
	case POPLOCALADRESS:
		SetIntAt( PopValue(), GetIntFrom( localVariableOffset + GetIntFrom( pointer ) ) );
		pointer += 8;
		break;
	case PUSHCONST:
		PushValue( GetIntFrom( pointer ) );
		pointer += 8;
		break;
	case PRINTCHAR:
		printf( "%c", (char)PopValue() );
		break;
	case PRINTSTRING:
		temp = PopValue();
#ifdef DEBUG
		printf( ":printstring:" );
		std::cout << temp << ":";
#endif
		printf( "%s", (char*)(&(data[temp])) );
		break;
	case PRINTINT:
		printf( "%lli", (int64)PopValue() );
		break;
	case GETCHAR:
		ctemp = ' ';
		scanf( "%c", &ctemp );
		PushValue( (uint64)ctemp );
		break;
	case GETSTRING:
#ifdef DEBUG
		printf( ":getstring:" );
#endif
		scanf( "%s", (char*)(&(data[PopValue()])) );
		break;
	case GETINT:
		temp = 0;
		scanf( "%lli", (int64*)(&temp) );
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
		
		
		
		
	case DATACOPY:
		utemp3 = PopValue();
		utemp1 = PopValue();	// destiny
		utemp2 = PopValue();	// source
		memmove( data.begin() + utemp1, data.begin() + utemp2, utemp3 );
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
			utemp1 = PopValue(); utemp2 = PopValue();
			PushValue( uint64(utemp2&utemp1) );
			break;
		case NAND:
			utemp1 = PopValue(); utemp2 = PopValue();
			PushValue( ~uint64(utemp2&utemp1) );
			break;
		case OR:
			utemp1 = PopValue(); utemp2 = PopValue();
			PushValue( uint64(utemp2|utemp1) );
			break;
		case NOR:
			utemp1 = PopValue(); utemp2 = PopValue();
			PushValue( ~uint64(utemp2|utemp1) );
			break;
		case XOR:
			utemp1 = PopValue(); utemp2 = PopValue();
			PushValue( uint64(utemp2^utemp1) );
			break;
		case XNOR:
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( ~uint64(utemp2^utemp1) );
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
			
		case STRINGEQUAL:
			stemp1 = stemp2 = "";
			stemp1 = (char*)(&data[PopValue()]);
			stemp2 = (char*)(&data[PopValue()]);
			PushValue( uint64(stemp2==stemp1) );
			break;
		case STRINGNOTEQUAL:
			stemp1 = stemp2 = "";
			stemp1 = (char*)(&data[PopValue()]);
			stemp2 = (char*)(&data[PopValue()]);
			PushValue( uint64(stemp2!=stemp1) );
			break;
		case STRINGLESS:
			stemp1 = stemp2 = "";
			stemp1 = (char*)(&data[PopValue()]);
			stemp2 = (char*)(&data[PopValue()]);
			PushValue( uint64(stemp2<stemp1) );
			break;
		case STRINGGRATER:
			stemp1 = stemp2 = "";
			stemp1 = (char*)(&data[PopValue()]);
			stemp2 = (char*)(&data[PopValue()]);
			PushValue( uint64(stemp2>stemp1) );
			break;
		case STRINGLESSEQUAL:
			stemp1 = stemp2 = "";
			stemp1 = (char*)(&data[PopValue()]);
			stemp2 = (char*)(&data[PopValue()]);
			PushValue( uint64(stemp2<=stemp1) );
			break;
		case STRINGGRATEREQUAL:
			stemp1 = stemp2 = "";
			stemp1 = (char*)(&data[PopValue()]);
			stemp2 = (char*)(&data[PopValue()]);
			PushValue( uint64(stemp2>=stemp1) );
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



