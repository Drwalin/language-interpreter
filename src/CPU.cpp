
#pragma once

#include "Preprocessor.cpp"
#include "ComplexCompiler.cpp"
#include "Compiler.cpp"
#include "MethodsCPU.cpp"
#include "SaveLoad.cpp"

#include "Gotoxy.cpp"

inline int MyAssemblyLang::DoOnce()
{
	++counterActions;
	BEGIN:
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
#if defined __WIN__
		system( "cls" );
#else
		system( "clear" );
#endif
		
	case END:
		return 0;
		break;
	case JUMP:
		pointer = GetIntFrom( pointer );
		break;
	case JUMPTRUE:
#ifdef DEBUG
		printf( ":jumptrue:" );
#endif
		if( PopValue() != 0 )
		{
			pointer = GetIntFrom( pointer );
		}
		else
		{
			pointer += 8;
		}
#ifdef DEBUG
		printf( ":jumped:" );
#endif
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
		utemp1 = functionCache.back();
		functionCache.resize( functionCache.size() - 1 );
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
#ifdef DEBUG
		printf( ":alu:" );
		if( cache.size() >= 8 )
			printf( "val=%lli", (int64*)(cache.begin() + (cache.size() - 8) ) );
		printf( ":alu:" );
		getch();
#endif
		switch( data[pointer++] )
		{
		case ADD:
#ifdef DEBUG
		printf( "add:" );
		printf( "::%lld-%lld::", cache.size(), cache.capacity() );
#endif
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2+temp1) );
			break;
		case SUB:
#ifdef DEBUG
		printf( "sub:" );
#endif
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2-temp1) );
			break;
		case DIV:
#ifdef DEBUG
		printf( "div:" );
#endif
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2/temp1) );
			break;
		case MOD:
#ifdef DEBUG
		printf( "mod:" );
#endif
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2%temp1) );
			break;
		case MUL:
#ifdef DEBUG
		printf( "mul:" );
#endif
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2*temp1) );
			break;
		case AND:
#ifdef DEBUG
		printf( "and:" );
#endif
			utemp1 = PopValue(); utemp2 = PopValue();
			PushValue( uint64(utemp2&utemp1) );
			break;
		case NAND:
#ifdef DEBUG
		printf( "nand:" );
#endif
			utemp1 = PopValue(); utemp2 = PopValue();
			PushValue( ~uint64(utemp2&utemp1) );
			break;
		case OR:
#ifdef DEBUG
		printf( "or:" );
#endif
			utemp1 = PopValue(); utemp2 = PopValue();
			PushValue( uint64(utemp2|utemp1) );
			break;
		case NOR:
#ifdef DEBUG
		printf( "nor:" );
#endif
			utemp1 = PopValue(); utemp2 = PopValue();
			PushValue( ~uint64(utemp2|utemp1) );
			break;
		case XOR:
#ifdef DEBUG
		printf( "xor:" );
#endif
			utemp1 = PopValue(); utemp2 = PopValue();
			PushValue( uint64(utemp2^utemp1) );
			break;
		case XNOR:
#ifdef DEBUG
		printf( "xnor:" );
#endif
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( ~uint64(utemp2^utemp1) );
			break;
		case NOT:
#ifdef DEBUG
		printf( "not:" );
#endif
			PushValue( ~PopValue() );
			break;
		case POW:
#ifdef DEBUG
		printf( "pow:" );
#endif
			temp1 = PopValue(); temp2 = PopValue();
			temp = pow( (double)temp2, (double)temp1 );
			PushValue( uint64(temp) );
			break;
		case SQRT:
#ifdef DEBUG
		printf( "sqrt:" );
#endif
			temp = PopValue();
			temp = sqrt( (double)temp );
			PushValue( uint64(temp) );
			break;
		case SHIFTLEFT:
#ifdef DEBUG
		printf( "shiftleft:" );
#endif
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( temp2<<temp1 );
			break;
		case SHIFTRIGHT:
#ifdef DEBUG
		printf( "shiftright:" );
#endif
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( temp2>>temp1 );
			break;
		case EQUAL:
#ifdef DEBUG
		printf( "equal:" );
#endif
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2==temp1) );
			break;
		case NOTEQUAL:
#ifdef DEBUG
		printf( "notequal:" );
#endif
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2!=temp1) );
			break;
		case LESS:
#ifdef DEBUG
		printf( "less:" );
#endif
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2<temp1) );
			break;
		case GRATER:
#ifdef DEBUG
		printf( "grater:" );
#endif
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2>temp1) );
			break;
		case LESSEQUAL:
#ifdef DEBUG
		printf( "lessequal:" );
#endif
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2<=temp1) );
			break;
		case GRATEREQUAL:
#ifdef DEBUG
		printf( "graterequal:" );
#endif
			temp1 = PopValue(); temp2 = PopValue();
			PushValue( uint64(temp2>=temp1) );
			break;
			
		case STRINGEQUAL:
#ifdef DEBUG
		printf( "stringequal:" );
#endif
			stemp1 = stemp2 = "";
			stemp1 = (char*)(&data[PopValue()]);
			stemp2 = (char*)(&data[PopValue()]);
			PushValue( uint64(stemp2==stemp1) );
			break;
		case STRINGNOTEQUAL:
#ifdef DEBUG
		printf( "stringnotequal:" );
#endif
			stemp1 = stemp2 = "";
			stemp1 = (char*)(&data[PopValue()]);
			stemp2 = (char*)(&data[PopValue()]);
			PushValue( uint64(stemp2!=stemp1) );
			break;
		case STRINGLESS:
#ifdef DEBUG
		printf( "stringless:" );
		getch();
#endif
			stemp1 = stemp2 = "";
			stemp1 = (char*)(&data[PopValue()]);
			stemp2 = (char*)(&data[PopValue()]);
			PushValue( uint64(stemp2<stemp1) );
			break;
		case STRINGGRATER:
#ifdef DEBUG
		printf( "stringgrater:" );
#endif
			stemp1 = stemp2 = "";
			stemp1 = (char*)(&data[PopValue()]);
			stemp2 = (char*)(&data[PopValue()]);
			PushValue( uint64(stemp2>stemp1) );
			break;
		case STRINGLESSEQUAL:
#ifdef DEBUG
		printf( "stringlessequal:" );
#endif
			stemp1 = stemp2 = "";
			stemp1 = (char*)(&data[PopValue()]);
			stemp2 = (char*)(&data[PopValue()]);
			PushValue( uint64(stemp2<=stemp1) );
			break;
		case STRINGGRATEREQUAL:
#ifdef DEBUG
		printf( "stringgraterequal:" );
#endif
			stemp1 = stemp2 = "";
			stemp1 = (char*)(&data[PopValue()]);
			stemp2 = (char*)(&data[PopValue()]);
			PushValue( uint64(stemp2>=stemp1) );
			break;
			
		case TOBOOLEAN:
#ifdef DEBUG
		printf( "toboolean:" );
#endif
			PushValue( uint64(PopValue()!=0) );
			break;
		default:
			return 0;
		}
#ifdef DEBUG
		printf( "done:" );
#endif
		break;
	default:
		return 0;
	}
	
	goto BEGIN;
	
	return 1;
}



