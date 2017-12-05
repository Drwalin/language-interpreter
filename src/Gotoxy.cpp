
#pragma once

#ifdef __WIN__

#include <windows.h> 
inline void Gotoxy( long long int x, long long int y )
{
  COORD coord;
  coord.X = x;
  coord.Y = y;
  SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

#else

#include <cstdio>
inline void Gotoxy( long long int x, long long int y )	
{
	printf( "%c[%d;%df", 0x1B, (int)y, (int)x );
}

#endif
