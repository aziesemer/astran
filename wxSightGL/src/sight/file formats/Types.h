#ifndef TYPES_H
#define TYPES_H

#include <cassert>

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef char int8;
typedef short int16;
typedef int int32;

inline void AssertTypes() {
	assert( sizeof( uint8 ) == 1 );
	assert( sizeof( uint16 ) == 2 );
	assert( sizeof( int8 ) == 1 );
	assert( sizeof( int16 ) == 2 );
	assert( sizeof( int32 ) == 4 );
} // end function

#endif