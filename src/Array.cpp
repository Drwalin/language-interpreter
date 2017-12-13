
#pragma once

#include <cstdio>
#include <cstring>
#include <cstdlib>

#define ARRAY_SHIFT_BIT_FULL_SIZE 12

template < class T >
class Array
{
private:
	T * ptr;
	unsigned long int ptrsize;
	unsigned long int fullsize;		// includes reserved memory
	
public:
	
	inline long long int size()
	{
		return ptrsize;
	}
	
	inline T * begin()
	{
		return ptr;
	}
	
	inline T * end()
	{
		return ptr+ptrsize;
	}
	
	inline T & operator[]( const long long int id )
	{
		return *(ptr+id);
	}
	
	inline T & at( const long long int id )
	{
		return *(ptr+id);
	}
	
	inline T & front()
	{
		return *ptr;
	}
	
	inline T & back()
	{
		return *(ptr+ptrsize-1);
	}
	
	inline void shrink_to_fit()
	{
		register long long int temp = ( ( ptrsize >> ARRAY_SHIFT_BIT_FULL_SIZE ) + 1 ) << ARRAY_SHIFT_BIT_FULL_SIZE;
		if( temp < fullsize )
		{
			fullsize = temp;
			ptr = (T*) realloc( ptr, fullsize );
		}
	}
	
	inline void resize( const long long int size )
	{
		if( size >= 0 )
		{
			register long long int temp = ( ( size >> ARRAY_SHIFT_BIT_FULL_SIZE ) + 1 ) << ARRAY_SHIFT_BIT_FULL_SIZE;
			
			if( temp > fullsize )
			{
				fullsize = temp;
				ptr = (T*) realloc( ptr, fullsize );
			}
			this->ptrsize = size;
		}
		else
		{
			register long long int temp = 1 << ARRAY_SHIFT_BIT_FULL_SIZE;
			
			if( temp > fullsize )
			{
				fullsize = temp;
				ptr = (T*) realloc( ptr, fullsize );
			}
			this->ptrsize = 0;
		}
	}
	
	inline void push_back( const T src )
	{
		resize( ptrsize + 1 );
		ptr[ptrsize-1] = src;
	}
	
	inline void clear()
	{
		ptr = (T*) realloc( ptr, sizeof(T) << ARRAY_SHIFT_BIT_FULL_SIZE );
		ptrsize = 0;
		fullsize = 1 << ARRAY_SHIFT_BIT_FULL_SIZE;
	}
	
	inline void insert( long long int point, const T src )
	{
		register long long int bsize = ptrsize - point;
		resize( ptrsize + 1 );
		
		memmove( ptr + point + 1, ptr + point, bsize * sizeof(T) );
		ptr[point] = src;
	}
	
	inline void insert( long long int point, T * beg, T * end )
	{
		register long long int bsize = ptrsize - point;
		resize( ptrsize + end - beg );
		
		memmove( ptr + point + (end - beg), ptr + point, bsize * sizeof(T) );
		memcpy( ptr + point, beg, (end - beg) * sizeof(T) );
	}
	
	inline void insert( long long int point, T * beg, long long int count )
	{
		register long long int bsize = ptrsize - point;
		resize( ptrsize + count );
		
		memmove( ptr + point + count, ptr + point, bsize * sizeof(T) );
		memcpy( ptr + point, beg, count * sizeof(T) );
	}
	
	inline void erase( long long int beg, long long int end )
	{
		if( end > beg )
		{
			if( end < ptrsize )
				memmove( ptr + beg, ptr + end, (ptrsize - end) * sizeof(T) );
			
			resize( beg + ( ptrsize - end ) );
		}
	}
	
	inline long long int capacity()
	{
		return fullsize;
	}
	
	inline void reserve_mem( long long int size /*size>ptrsize*/ )
	{
		register long long int temp = ( ( size >> ARRAY_SHIFT_BIT_FULL_SIZE ) + 1 ) << ARRAY_SHIFT_BIT_FULL_SIZE;
		if( temp > fullsize )
		{
			fullsize = temp;
			ptr = (T*) realloc( ptr, fullsize );
		}
	}
	
	Array()
	{
		ptr = (T*) malloc( sizeof(T) * ( 1 << ARRAY_SHIFT_BIT_FULL_SIZE ) );
		ptrsize = 0;
		fullsize = 1 << ARRAY_SHIFT_BIT_FULL_SIZE;
	}
	
	~Array()
	{
		if( ptr )
			free( ptr );
		ptr = NULL;
		ptrsize = 0;
		fullsize = 0;
	}
};



