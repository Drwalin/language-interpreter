
#pragma once

#include "Compiler.cpp"
#include "CPU.cpp"

static long long int MyAssemblyLang::GetInt64FromString( std::string str, int & err )
{
	err = 0;
	long long int dst = 0;
	bool negate = false;
	for( int i = 0; str[i] != 0; ++i )
	{
		if( i == 0 && str[i] == '-' )
		{
			negate = true;
		}
		else if( str[i] >= '0' && str[i] <= '9' )
		{
			dst *= (long long int)(10);
			dst += (long long int)(src[i]-='0');
		}
		else
		{
			err = 1;
			return 0;
		}
	}
	if( negate )
		dst = -dst;
	return dst;
}

void MyAssemblyLang::End()
{
	pointer = 0;
	for( uint64 i = 0; i < file.size(); ++i )
		if( file[i].good() )
			file[i].close();
	cache.clear();
}

void MyAssemblyLang::PushBytes( std::vector < byte > & src )
{
	register uint64 size = (((cacheOffset+1+src.size())>>10)+1)<<10;
	if( size > cache.size() )
		cache.resize( size );
	for( uint64 i = 0; i < src.size(); ++i )
	{
		cache[++cacheOffset] = src[i];
	}
}

void MyAssemblyLang::PopBytes( std::vector < byte > & src, uint64 count )
{
	src.resize( count );
	for( uint64 i = 0; i < src.size(); ++i )
	{
		src[i] = cache[cacheOffset];
		cacheOffset--;
	}
	register uint64 size = (((cacheOffset+1)>>10)+1)<<10;
	if( size < cache.size() )
		cache.resize( size );
}

void MyAssemblyLang::PushValue( uint64 val, uint64 count )
{
	std::vector < bytes > dat;
	dat.resize( count );
	for( int i = 0; i < count; ++i )
		dat[i] = (val>>(i<<3))&255;
	PushBytes( dat );
}

void MyAssemblyLang::PopValue( uint64 & val, uint64 count )
{
	std::vector < bytes > dat;
	PopBytes( dat, count );
	dat.resize( count );
	val = 0;
	for( int i = 0; i < count; ++i )
		val += dat[i]<<(i<<3);
}

void MyAssemblyLang::Clear()
{
	End();
	data.clear();
}

void MyAssemblyLang::Do( const uint64 count )
{
	for( uint64 i = 0; i < count; ++i )
	{
		if( DoOnce() == 0 )
		{
			break;
		}
	}
}

MyAssemblyLang::~MyAssemblyLang()
{
	Clear();
}

MyAssemblyLang::MyAssemblyLang()
{
	std::vector < byte > elem;

	elem.resize( 2 );
	elem.[0] = ALU;
	elem[1] = ADD;		commands["add"] = elem;
	elem[1] = SUB;		commands["sub"] = elem;
	elem[1] = DIV;		commands["div"] = elem;
	elem[1] = MUL;		commands["mul"] = elem;
	elem[1] = MOD;		commands["mod"] = elem;
	elem[1] = AND;		commands["and"] = elem;
	elem[1] = NAND;		commands["nand"] = elem;
	elem[1] = OR;		commands["or"] = elem;
	elem[1] = NOR;		commands["nor"] = elem;
	elem[1] = XOR;		commands["xor"] = elem;
	elem[1] = XNOR;		commands["xnor"] = elem;
	elem[1] = NOT;		commands["not"] = elem;
	elem[1] = POW;		commands["pow"] = elem;
	elem[1] = SQRT;		commands["sqrt"] = elem;
	elem[1] = LOG;		commands["log"] = elem;

	elem.resize( 1 );
	elem[0] = END;				commands["end"] = elem;
	elem[0] = ALU;				commands["alu"] = elem;
	elem[0] = PUSH;				commands["push"] = elem;
	elem[0] = POP;				commands["pop"] = elem;
	elem[0] = OPENFILE;			commands["openfile"] = elem;
	elem[0] = CLOSEFILE;		commands["closefile"] = elem;
	elem[0] = JUMP;				commands["jump"] = elem;
	elem[0] = JUMPTRUE;			commands["jumptrue"] = elem;
	elem[0] = JUMPFALSE;		commands["jumpfalse"] = elem;
	elem[0] = LOADFROMFILE;		commands["loadfromfile"] = elem;
	elem[0] = SAVETOFILE;		commands["savetofile"] = elem;
	elem[0] = GETFILESIZE;		commands["getfilesize"] = elem;
	elem[0] = JUMPFILE;			commands["jumpfile"] = elem;
	elem[0] = MOVE;				commands["move"] = elem;
	elem[0] = PUSHADRESS;		commands["pushadress"] = elem;
}



