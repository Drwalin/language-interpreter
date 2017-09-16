
#pragma once

int64 MyAssemblyLang::GetInt64FromString( std::string str, int & err )
{
	err = 0;
	int64 dst = 0;
	bool negate = false;
	for( int i = 0; str[i] != 0; ++i )
	{
		if( i == 0 && str[i] == '-' )
		{
			negate = true;
		}
		else if( str[i] >= '0' && str[i] <= '9' )
		{
			dst *= (int64)(10);
			dst += (int64)(str[i]-='0');
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

void MyAssemblyLang::SetIntAt( uint64 var, uint64 ptr )
{
	uint64 a = 0;
	if( data.size() <= ptr+8 )
		data.resize( ptr+8 );
	*((uint64*)&(data[ptr])) = var;
	/*
	for( a = 0; a < 8; ++a )
		data[ptr+a] = (var>>(a<<3))&255;
	*/
}

uint64 MyAssemblyLang::GetIntFrom( uint64 ptr )
{
	if( data.size() <= ptr+8 )
		return 0;
	return *((uint64*)&(data[ptr]));
	/*
	uint64 val = 0;
	for( uint64 i = 0; i < 8; ++i )
		val += data[ptr+i]<<(i<<3);
	return val;
	*/
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
	for( uint64 i = count-1; i < count; --i )
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
//	printf( "\n Val = %lli", (int64)val );
	std::vector < byte > dat;
	dat.resize( count );
	for( int i = 0; i < count; ++i )
		dat[i] = (val>>(i<<3))&255;
	PushBytes( dat );
}

void MyAssemblyLang::PopValue( uint64 & val, uint64 count )
{
	std::vector < byte > dat;
	PopBytes( dat, count );
	val = 0;
	for( int i = 0; i < count; ++i )
		val += dat[i]<<(i<<3);
}

uint64 MyAssemblyLang::PopValue( uint64 count )
{
	uint64 val = 0;
	std::vector < byte > dat;
	PopBytes( dat, count );
	for( int i = 0; i < count; ++i )
		val += dat[i]<<(i<<3);
//	printf( "\n Val = %lli", (int64)val );
	return val;
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
	counterActions = 0;
	cacheOffset = 0;
	localVariableOffset = 0;
	pointer = 0;
	/*
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
	*/
}



