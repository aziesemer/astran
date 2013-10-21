
#ifndef LABEL_H
#define LABEL_H

#include "vertex.h"
#include "opengl.h"
#include "object.h"
#include <iostream>
#include <algorithm>
#include <string>
	using namespace std;
	

class SightLabel : public Object {
	private:
		string clsText;		
		Vertex clsMin, clsMax;

	public:

		SightLabel(string text, const int minX, const int minY);
		virtual ~SightLabel( ){}
		
		void setText(string text) { clsText = text; }
		string getText() { return clsText; }
		
		//void printMe();
		void printMe(void * font);
		
		virtual void draw( const float z ) const;
		virtual void drawEdges( const float z ) const;
		virtual void drawEditMode( const int z, const int x1, const int y1) const;
		
		virtual void resize( const int x1, const int y1, const int x2, const int y2)  {};	

		virtual Object* clone()const{ return new SightLabel(*this); }
		
		virtual Vertex getMin()const { return clsMin; }
		virtual Vertex getMax()const { return clsMax; }
				
		virtual void translate(const int x, const int y);
		virtual void rotate(const int x, const int y) {}
		virtual void mirroring(const bool inX);

		virtual bool contains( const int x, const int y ) const;// return true if the object contains the vertex
		virtual bool inn(Vertex min, Vertex max) const;// return true if the object is all into the box
		virtual bool intersected(Vertex min, Vertex max) const;// return true if the object is intersected by the line
		
		virtual bool intersected(Object * obj) const{ return false; }

};


#endif
