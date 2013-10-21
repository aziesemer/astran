#ifndef LAYER_H
#define LAYER_H

#include <string>
	using std::string;

#include "../util/util.h"

#include "vertex.h"
#include "object.h"
#include "opengl.h"
#include "stipple.h"

class Layer {
	private:
		Color clsFillColor;
		Color clsBorderColor;
		StippleMask clsStippleMask;
		
		float clsZ;
		int clsLayer;
		string clsName;
		
		//stiple
		bool clsVisible; //visible
		vector <ObjectNode> clsInLayer;
		
	public:
	
		Layer() { clsVisible = true; }
		
		
		float getZ()const{return clsZ;}
		
		void clearObj(){ clsInLayer.clear(); }
	// -----------------------------------------------------------------------------	
		bool isVisible() const { return clsVisible; }
	// -----------------------------------------------------------------------------
		void addObject( Object * object ){ clsInLayer.push_back( ObjectNode( object ) ); }
		Object *getObject( const int i ) const { return clsInLayer[ i ].getObject(); }
		void gimmeObjects(vector<Object*> &vec);
		
	// -----------------------------------------------------------------------------	
		void deleteObj(const int i );
	// -----------------------------------------------------------------------------	
		int getNumObjects() const { return clsInLayer.size(); }		
	// -----------------------------------------------------------------------------	
		void drawLayer() const;
		void drawLayer(vector <Object *>  &deleteObj);
		void drawLayerIf(Vertex min, Vertex max) const ;
	// -----------------------------------------------------------------------------	
		void setLayerFillProperties() const;
		void setLayerEdgeProperties() const;
		
		
		void setVisible( bool visible) { clsVisible = visible; }
		void setName( const string name ) { clsName = name; }
		void setLayer( const int layer ) { clsLayer = layer; clsZ = 2.0f - 2.0f/(layer+1.0f); }
		void setFillColor( const Color &c ) { clsFillColor = c; }
		void setBorderColor( const Color &c ) { clsBorderColor = c; }
		void setStippleMask( const StippleMask stippleMask ) { clsStippleMask = stippleMask; }
		
		StippleMask getStippleMask()const{ return clsStippleMask; }
		Color getFillColor() const{ return clsFillColor; }
		Color getBorderColor() const{ return clsBorderColor; }
		
		void gimmeConnectedObjects(vector <Object*> &object, size_t index, size_t incremento);
		
		const string &getName() const { return clsName; }
		
		void configure( const string name, const int layer, const Color &fillColor, const Color &borderColor, const StippleMask stippleMask );
		
		template< class T >
		inline void foreachObject( T * const t, void (T::*f)( const Object * ) const ) const;
};
/*
template< class T >
inline void Layer::foreachObject(  T * const t, void (T::*f)( const Object * ) const ) const {
	for( size_t i=0; i < clsInLayer.size(); i++)
		(t->*f)( clsInLayer[i].getObject() );
} // end method
*/
#endif