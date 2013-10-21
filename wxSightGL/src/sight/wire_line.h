#ifndef WIRE_LINE_H
#define WIRE_LINE_H

#include "opengl.h"
#include "object.h"
#include "vertex.h"
#include "camera.h"
#include <vector>
	using std::vector;

#define printme( e ) { cout << #e << " = " << ( e ) << "\n"; }

class SightWire : public Object {
	private:
		vector<Vertex> clsVecWire;
		//vector<Vertex> clsVecEditLine;
		Color clsColor;
		int clsThickness;

		
	public:
		SightWire() : Object( OBJECT_WIRE ) {  }
		virtual ~SightWire(){}
		SightWire( const vector<Vertex> &mainClicks ) : Object( OBJECT_WIRE ) {
			for( size_t i=0; i< mainClicks.size(); i++){
				clsVecWire.push_back( mainClicks[i] );
			}//end for
		} // end constructor
		
		virtual Vertex getMin() const;
		virtual Vertex getMax() const;
		
		Color getColor() const { return clsColor; }
		void setColor( const Color color ) { clsColor = color; }
		
		int getThickness() const { return clsThickness; }
		void setThickness(const int thickness) { clsThickness = thickness; } 
		
		virtual void draw( const float z ) const;
		virtual void drawEdges( const float z ) const;
		virtual void drawEditMode( const int z, const int x1, const int y1) const {}
		
		virtual void resize( const int x1, const int y1, const int x2, const int y2) {}
		
		virtual Object* clone() const{ return NULL; }
		
		virtual void translate(const int x, const int y) {}
		virtual void rotate(const int x, const int y) {}
		virtual void mirroring(const bool inX) {}
		
		//virtual void drawEditMode(const int x, const int y){}
		virtual bool contains( const int x, const int y ) const { return false; }
		virtual bool inn(Vertex min, Vertex max) const { return false; }
		virtual bool intersected(Vertex min, Vertex max) const { return false; }
		virtual bool intersected(Object * obj) const{ return false; }//[TODO: arrumar]

		void findTwoPoints(float2 const v0, float2 const v1, float2 const v2, const float z)const;
};//end class wire

// -----------------------------------------------------------------------------
class SightLine : public Object {
	private:
		vector<Vertex> clsVecLine;
		vector<Vertex> clsVecEditLine;
		Color clsColor;
		
	public:
		SightLine() : Object( OBJECT_LINE ) {}
		virtual ~SightLine(){}
		SightLine( const vector<Vertex> &mainClicks ) : Object( OBJECT_LINE ) {
			for(size_t i=0; i< mainClicks.size(); i++){
				clsVecLine.push_back( mainClicks[i] );
			}//end for
		} // end constructor
		
		virtual Vertex getMin() const;
		virtual Vertex getMax() const;
		
		Color getColor() const { return clsColor; }
		void setColor( const Color color ) { clsColor = color; }
	
		virtual void draw( const float z ) const;
		virtual void drawEdges( const float z ) const {}
		virtual void drawEditMode( const int z, const int x1, const int y1) const {}
						
		virtual void resize( const int x1, const int y1, const int x2, const int y2) {}
						
		virtual Object* clone() const { return NULL; }
		
		virtual void translate(const int x, const int y) {}
		virtual void rotate(const int x, const int y) {}
		virtual void mirroring(const bool inX) {}
		
	//	virtual void drawEditMode(const int x, const int y){}

		virtual bool contains( const int x, const int y ) const { return false; }
		virtual bool inn(Vertex min, Vertex max) const { return false; }
		virtual bool intersected(Vertex min, Vertex max) const { return false; }
		virtual bool intersected(Object* obj) const{ return false; }//[TODO: arrumar]

};//end class line

// -----------------------------------------------------------------------------
class wireAux{
	public:
		float clsPrevX;	float clsPrevY;
		enum clsDirection { HORIZONTAL, VERTICAL, NONE };
			clsDirection clsCurrentDir;
			clsDirection clsPreviousDir;
			
		void whatIF(int x0, int y0, int x1, int y1);
		void whatthehell(int x0, int y0, int x1, int y1);
};

#endif

