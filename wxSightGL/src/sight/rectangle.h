#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "../util/util.h"

#include "opengl.h"
#include "object.h"
#include "vertex.h"

class SightRectangle : public Object {
	private:
		Vertex clsMin;
		Vertex clsMax;
		
	public:
		SightRectangle() : Object( OBJECT_RECTANGLE ) {}
		SightRectangle( const Vertex v0, const Vertex v1 ) : Object( OBJECT_RECTANGLE ) {
			clsMin.x = min( v0.x, v1.x ); 
			clsMin.y = min( v0.y, v1.y );
			clsMax.x = max( v0.x, v1.x );
			clsMax.y = max( v0.y, v1.y );
		} // end constructor
	
		virtual ~SightRectangle(){}
	
		virtual void draw( const float z ) const;
		virtual void drawEdges( const float z ) const;
		virtual void drawEditMode( const int z, const int x1, const int y1) const;
		
		virtual void resize( const int x1, const int y1, const int x2, const int y2) ;
		
		virtual void translate(const int x, const int y) ;
		virtual void rotate(const int x, const int y);
		virtual void mirroring(const bool inX) ;
		
		virtual Object* clone()const; 
		
		virtual Vertex getMin()const { return clsMin; }
		virtual Vertex getMax()const { return clsMax; }
		
	//	virtual void drawEditMode(const int x, const int y);

		virtual bool contains( const int x, const int y ) const;	
		virtual bool inn(Vertex min, Vertex max) const;
		virtual bool intersected(Vertex min, Vertex max) const;
		virtual bool intersected(Object * obj) const;

}; // end class

#endif