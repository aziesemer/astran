#include "rectangle.h"
#include <math.h>
#include <iostream>
	using namespace std;

// -----------------------------------------------------------------------------	

void SightRectangle::draw( const float z ) const {
	glBegin( GL_QUADS );
		glVertex3f( clsMin.x, clsMin.y, z );
		glVertex3f( clsMax.x, clsMin.y, z );
		glVertex3f( clsMax.x, clsMax.y, z );
		glVertex3f( clsMin.x, clsMax.y, z );
	glEnd();
} // end method

// -----------------------------------------------------------------------------

void SightRectangle::drawEdges( const float z ) const {
	glBegin( GL_QUADS );
		glVertex3f( clsMin.x, clsMin.y, z );
		glVertex3f( clsMax.x, clsMin.y, z );
		glVertex3f( clsMax.x, clsMax.y, z );
		glVertex3f( clsMin.x, clsMax.y, z );
	glEnd();
} // end method

// -----------------------------------------------------------------------------

void SightRectangle::drawEditMode(const int z,const int x,const int y) const{
	glBegin( GL_QUADS );
		glVertex3f( clsMin.x + x, clsMin.y + y, z );
		glVertex3f( clsMax.x + x, clsMin.y + y, z );
		glVertex3f( clsMax.x + x, clsMax.y + y, z );
		glVertex3f( clsMin.x + x, clsMax.y + y, z );
	glEnd();	
}

// -----------------------------------------------------------------------------

void SightRectangle::resize( const int x1, const int y1, const int x2, const int y2) {

	clsMin.x = clsMin.x + x1;
	clsMin.y = clsMin.y + y1;
	clsMax.x = clsMax.x + x2;
	clsMax.y = clsMax.y + y2;

}//end methodd

// -----------------------------------------------------------------------------

bool SightRectangle::contains( const int x, const int y ) const {
	return
		( x >= clsMin.x && x <= clsMax.x ) &&
		( y >= clsMin.y && y <= clsMax.y );
} // end method

// -----------------------------------------------------------------------------

bool SightRectangle::inn(Vertex min, Vertex max) const{
	return
	(clsMax.x <= max.x && clsMax.y <= max.y) &&
	(clsMin.x >= min.x && clsMin.y >= min.y);
}

// -----------------------------------------------------------------------------

bool SightRectangle::intersected(Vertex min, Vertex max) const { 
	return //REVER
	
	(clsMax.y <= max.y && clsMax.y >= min.y) && (max.x <= clsMax.x && max.x >= clsMin.x) 
	||
	(clsMax.x <= max.x && clsMax.x >= min.x) && (max.y <= clsMax.y && max.y >= clsMin.y);//*/

}//end method
// -----------------------------------------------------------------------------

bool SightRectangle::intersected(Object * obj) const{
	if(obj == this)
		return false;
	else{
		Vertex max = obj->getMax();
		Vertex min = obj->getMin();
		
		return !(	min.x > clsMax.x ||
					max.x < clsMin.x ||
					max.y < clsMin.y ||
					min.y > clsMax.y
		);
	}
}//end method

// -----------------------------------------------------------------------------

void SightRectangle::translate(const int x, const int y) {
	clsMin.x = clsMin.x + x;
	clsMin.y = clsMin.y + y;
	clsMax.x = clsMax.x + x;
	clsMax.y = clsMax.y + y;
}

// -----------------------------------------------------------------------------

void SightRectangle::rotate(const int x, const int y) {

}

// -----------------------------------------------------------------------------

void SightRectangle::mirroring(const bool inX) {
	if( inX ){	
		const int y0 = -clsMin.y;
		const int y1 = -clsMax.y;
		clsMin.y = min(y0, y1);
		clsMax.y = max(y0, y1);
	}else{
		const int x0 = -clsMin.x;
		const int x1 = -clsMax.x;
		clsMin.x = min(x0, x1);
		clsMax.x = max(x0, x1);
	}
}

// -----------------------------------------------------------------------------

Object* SightRectangle::clone()const{
	return new SightRectangle(*this);
}





