#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
	using std::ostream;
	using std::cout;

#include <cmath>

#define printme( e ) { cout << #e << " = " << ( e ) << "\n"; } 	

class Vertex {

	friend ostream &operator<<( ostream &out, const Vertex v ){
		return out << "(" << v.x << ", " << v.y << ")";
	} // end operator

	public:
		int x, y;

		Vertex() : x( 0 ), y( 0 ) {}
		Vertex( const int _x, const int _y ) : x( _x ), y( _y ) {}
		
		Vertex operator-( const Vertex v ) {
			return Vertex( x - v.x, y - v.y );
		}

		Vertex operator+( const Vertex v ) {
			return Vertex( x + v.x, y + v.y );
		}
		
		bool operator!=( const Vertex right ) {
			return x != right.x || y != right.y;
		} // end operator

		bool operator==( const Vertex right ) {
			return x == right.x && y == right.y;
		} // end operator	
		
}; // end class



class Color {
	public:
		unsigned char r, g, b;

		Color() : r( 0 ), g( 0 ), b( 0 ) {}
		Color( const unsigned char _r, const unsigned char _g, const unsigned char _b )
			: r( _r ), g( _g ), b( _b ) {}
}; // end class

struct float2 {
	float x;
	float y;
	
	float2() : x(0), y(0) {}
	float2( const float a, const float b ) : x(a), y(b) {}
	float2( const Vertex v ) : x( v.x ), y( v.y ) {}

	float2 operator*( const float2 f )const {
		return float2( x * f.x, y * f.y );
	} // end operator

	float2 operator/( const float2 f )const {
		return float2( x / f.x, y / f.y );
	} // end operator

	float2 operator*( const float scalar ) const {
		return float2( x * scalar, y * scalar );
	} // end operator

	float2 operator/( const float scalar ) const {
		return float2( x / scalar, y / scalar );
	} // end operator

	void operator*=( const float scalar ) {
		x *= scalar;
		y *= scalar;
	} // end operator
	
	float2 operator-( const float scalar ) const{
		return float2( x - scalar, y - scalar );
	} // end operator
	
	float2 operator+( const float scalar ) const {
		return float2( x + scalar, y + scalar );
	} // end operator

	void operator/=( const float scalar )  {
		x /= scalar;
		y /= scalar;
	} // end operator
	
	float length() const {
		return sqrt( x*x + y*y );
	} // end method
	
	bool null() const  {
		return x == 0 && y == 0;
	} // end method
	
	float2 normalize() const {
		return *this / length();
	} // end method
	
	float modulo() const{
		return sqrt( this->x * this->x + this->y * this->y);
	}
	
	float2 perpendicular() const {
		return float2( y, -x );
	} // end method
	
/*	float2 &perpendicular() {
		return (*this) = float2( y, -x );
	} // end method*/
	
	float2 operator+( const float2 f ) const{
		return float2( x + f.x, y + f.y );
	} // end operator

	float2 operator-( const float2 f )const {
		return float2( x - f.x, y - f.y );
	} // end operator
	
	float2 invert( ) {		
		return float2( -x,  -y);
	} // end operator	
	
	operator float *() const { return ( float * ) this; }

}; // end struct



#endif