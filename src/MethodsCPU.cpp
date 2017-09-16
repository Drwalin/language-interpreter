
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
}

uint64 MyAssemblyLang::GetIntFrom( uint64 ptr )
{
	if( data.size() <= ptr+8 )
		return 0;
	return *((uint64*)&(data[ptr]));
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

void MyAssemblyLang::PushValue( uint64 val )
{
	cache.resize( cache.size()+8 );
	*((uint64*)&(cache[cache.size()-8])) = val;
}

void MyAssemblyLang::PopValue( uint64 & val )
{
	val = *((uint64*)&(cache[cache.size()-8]));
	cache.resize( cache.size()-8 );
}

uint64 MyAssemblyLang::PopValue()
{
	uint64 val = 0;
	val = *((uint64*)&(cache[cache.size()-8]));
	cache.resize( cache.size()-8 );
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
}



