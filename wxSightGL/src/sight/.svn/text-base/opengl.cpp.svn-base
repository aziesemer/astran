#include <cmath>
#include "../util/util.h"
#include "opengl.h"

// debug stuffes
int beginCount = 0;
int endCount = 0;
// end debug stuffes

const float PI = 3.14159265358979f;

// -----------------------------------------------------------------------------	

void drawArc( const float vx, const float vy, const float r, const float gamma, const int numPoints ) {
	for ( int i = 0; i < numPoints; i++ ) {
			const float x = r * cos( gamma - ( ( PI / 2.0F ) * i / float( numPoints ) ) );
			const float y = r * sin( gamma - ( ( PI / 2.0F ) * i / float( numPoints ) ) );
			
			glVertex2f( x + vx, y + vy );
	} // end for
}

// -----------------------------------------------------------------------------	

void drawRoundedRectangle( const float x, const float y, const float w, const float h, const GLclampf roundness ) {
	const float l = roundness;
	const int numPoints = 5;

	const float t = y + h;
	const float r = x + w;


	glBegin( GL_POLYGON );
		// Left-Bottom corner.
		drawArc( x + l, y + l, l, -PI/2, numPoints );
		glVertex2f( x, t - l );  
		
		// Left-Top corner.
		drawArc( x + l, t - l, l, PI, numPoints );
		glVertex2f( r - l, t);

		// Right-Top corner.
		drawArc( r - l, t - l, l, PI/2, numPoints );
		glVertex2f( r, y + l );

		// Right-Bottom corner.
		drawArc(  r - l, y + l, l, 0, numPoints );
		glVertex2f( x+l, y );
	glEnd();
	checkGLErrors();	
}

// -----------------------------------------------------------------------------	
