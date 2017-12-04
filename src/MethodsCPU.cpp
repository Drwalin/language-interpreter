
#pragma once

void MyAssemblyLang::SetDebug( bool src )
{
	debug = src;
}

inline int64 MyAssemblyLang::GetInt64FromString( std::string str, int & err )
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

inline void MyAssemblyLang::SetIntAt( uint64 var, uint64 ptr )
{
	*((uint64*)&(data[ptr])) = var;
}

inline uint64 MyAssemblyLang::GetIntFrom( uint64 ptr )
{
	if( ptr + 8 > data.size() )
		std::cout << "   ::: GetIntFrom-" << ptr << " > " << data.size() << " :::   ";
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

inline void MyAssemblyLang::PushBytes( std::vector < byte > & src )
{
	register uint64 size = (((cacheOffset+1+src.size())>>12)+1)<<12;
	if( size > cache.size() )
		cache.resize( size );
	for( uint64 i = 0; i < src.size(); ++i )
	{
		cache[++cacheOffset] = src[i];
	}
}

inline void MyAssemblyLang::PopBytes( std::vector < byte > & src, uint64 count )
{
	src.resize( count );
	for( uint64 i = count-1; i < count; --i )
	{
		src[i] = cache[cacheOffset];
		cacheOffset--;
	}
	register uint64 size = (((cacheOffset+1)>>12)+1)<<12;
	if( size < cache.size() )
		cache.resize( size );
}

inline void MyAssemblyLang::PushValue( uint64 val )
{
	register uint64 size = (((cacheOffset+9)>>12)+1)<<12;
	if( size > cache.size() )
		cache.resize( size );
	*((uint64*)&(cache[cacheOffset])) = val;
	cacheOffset+=8;
}

inline void MyAssemblyLang::PopValue( uint64 & val )
{
	cacheOffset-=8;
	val = *((uint64*)&(cache[cacheOffset]));
	register uint64 size = (((cacheOffset+1)>>12)+1)<<12;
	if( size < cache.size() )
		cache.resize( size );
}

inline uint64 MyAssemblyLang::PopValue()
{
	uint64 val = 0;
	cacheOffset-=8;
	val = *((uint64*)&(cache[cacheOffset]));
	register uint64 size = (((cacheOffset+1)>>12)+1)<<12;
	if( size < cache.size() )
		cache.resize( size );
	return val;
}

inline void MyAssemblyLang::Clear()
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

uint64 MyAssemblyLang::AllocateMemory( uint64 size )
{
	uint64 temp;
	for( uint64 i = 0; i+1 < freeMemoryMap.size(); i+=2 )
	{
		temp = freeMemoryMap[i+1]+1-freeMemoryMap[i];
		if( temp == size )
		{
			temp = freeMemoryMap[i];
			freeMemoryMap.erase( freeMemoryMap.begin()+i, freeMemoryMap.begin()+i+2 );
			return temp;
		}
		else if( temp > size )
		{
			temp = freeMemoryMap[i];
			freeMemoryMap[i] += size;
			return temp;
		}
	}
	uint64 ptr = data.size();
	data.resize( data.size()+size );
	return ptr;
}

void MyAssemblyLang::FreeMemory( uint64 beg, uint64 size )
{
	uint64 end = beg+size-1, i;
	if( freeMemoryMap.size() > 0 )
	{
		for( i = 0; i < freeMemoryMap.size(); i+=2 )
		{
			if( end < freeMemoryMap[i] )
			{
				freeMemoryMap.insert( freeMemoryMap.begin()+i, end );
				freeMemoryMap.insert( freeMemoryMap.begin()+i, beg );
				break;
			}
		}
		if( i >= freeMemoryMap.size() )
		{
			freeMemoryMap.resize( freeMemoryMap.size() + 2 );
			freeMemoryMap[ freeMemoryMap.size()-1 ] = end;
			freeMemoryMap[ freeMemoryMap.size()-2 ] = beg;
		}
	}
	else
	{
		freeMemoryMap.resize( 2 );
		freeMemoryMap[0] = beg;
		freeMemoryMap[1] = end;
	}
	
	i = 0;
	while( true )
	{
		if( i+2 >= freeMemoryMap.size() )
			break;
		if( freeMemoryMap[i+1]+1 >= freeMemoryMap[i+2] )
		{
			freeMemoryMap.erase( freeMemoryMap.begin()+i+1, freeMemoryMap.begin()+i+3 );
			continue;
		}
		i+=2;
	}
	
	if( freeMemoryMap.size() > 0 )
	{
		if( freeMemoryMap.back() >= data.size()-1 )
		{
			data.resize( freeMemoryMap[ freeMemoryMap.size()-2 ] );
			freeMemoryMap.resize( freeMemoryMap.size()-2 );
		}
	}
}

inline void MyAssemblyLang::ReserveMemory( uint64 size )
{
	uint64 size_d = data.size();
	data.resize( size_d + size );
	data.resize( size_d );
}

inline void MyAssemblyLang::FreeReservedMemory()
{
	data.shrink_to_fit();
}

MyAssemblyLang::~MyAssemblyLang()
{
	Clear();
	debug = false;
}

MyAssemblyLang::MyAssemblyLang()
{
	counterActions = 0;
	cacheOffset = 0;
	localVariableOffset = 0;
	pointer = 0;
	debug = false;
}



