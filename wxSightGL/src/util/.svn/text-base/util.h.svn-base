#ifndef UTIL_H
#define UTIL_H

//
// Algumas versões MS Visual C++ não suportam as funções mim e max da STL
// devido à redefinição delas pela MFC.
//

#if defined(_MSC_VER)
	inline float round( const float f ) {
		const int floored = ( int ) f;
		const float rest = f - floored;
		return ( rest > 0.5f ) ? floored + 1 : floored;
	} // end function

	#if _MSC_VER == 1200
		template< typename T > inline T min( const T &a, const T &b )
		{ return ( a < b ) ? a : b; }

		template< typename T > inline T max( const T &a, const T &b )
		{ return ( a > b ) ? a : b; }

		#define for if( false ); else for // solves the variable redifinition problem on VC6
	#else
		#include <algorithm>
		  using std::max;
		  using std::min;
	#endif
#endif

#endif