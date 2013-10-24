#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
	using std::vector; 
#include <iostream>
	using std::cout;

#include "../util/util.h"

#include "vertex.h"
#include "opengl.h"

enum ObjectType { OBJECT_RECTANGLE, OBJECT_WIRE, OBJECT_LINE, OBJECT_LABEL, OBJECT_COMPONENT };

class Object {
	private:	
		ObjectType clsType;
		int clsGroup;

	public:

		Object( const ObjectType type ): clsType( type ), clsGroup( 0 ) {}
		virtual ~Object(){}

	
		virtual void draw( const float z ) const = 0;
		virtual void drawEdges( const float z ) const = 0;
		virtual void drawEditMode( const int z, const int x1, const int y1) const = 0;
		
		virtual void resize( const int x1, const int y1, const int x2, const int y2)  = 0;	

		virtual Object* clone()const=0;
		
		virtual Vertex getMin()const = 0;
		virtual Vertex getMax()const  = 0;
		
		virtual string getText(){ return "";}
				
		virtual	void setGroup( int g ){ clsGroup = g; }
		virtual int getGroup() const { return clsGroup; }		
				
		ObjectType getType() const { return clsType; }
				
		virtual void translate(const int x, const int y) = 0;
		virtual void rotate(const int x, const int y) = 0;
		virtual void mirroring(const bool inX) = 0;

		virtual bool contains( const int x, const int y ) const = 0;// return true if the object contains the vertex
		virtual bool inn(Vertex min, Vertex max) const = 0;// return true if the object is all into the box
		virtual bool intersected(Vertex min, Vertex max) const = 0;// return true if the object is intersected by a line
		virtual bool intersected(Object * obj) const =0;//by other
		
}; // end class

// -----------------------------------------------------------------------------
class ObjectNode {
	private:
		Object * clsObject;
		ObjectNode * sibling; //brother in the block
		ObjectNode * clsNext;
		
	public:
		ObjectNode( Object *object ): clsObject( object ), clsNext( NULL ) {}
	
		Object * getObject() const { return clsObject; }
		ObjectNode * getNext() const { return clsNext; }

		void setNext( ObjectNode * const next ) { clsNext = next; }
		bool hasNext() const { return clsNext == NULL; }
		

}; // end class

// -----------------------------------------------------------------------------

class ObjectIterator {
	private:
		ObjectNode * clsObjectNode;
		
	public:
		void operator++() {
			clsObjectNode = clsObjectNode->getNext();
		} // end operator

		bool operator!() {
			return !clsObjectNode->hasNext();
		} // end operator
}; // end class 

//________________________________________________________________________________________
class compositeObject {
	private:
		int ID;
		ObjectNode * firstBorn; 
		Vertex clsMin;
		Vertex clsMax;
};

// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------


#endif