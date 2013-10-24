#include "layer.h"

#define printme( e ) { cout << #e << " = " << ( e ) << "\n"; } 

// -----------------------------------------------------------------------------
	
void Layer::deleteObj(const int i){  
	delete clsInLayer[i].getObject();
	clsInLayer[i] = clsInLayer.back();
	clsInLayer.pop_back();

	//[TODO] ATUALIZAR INDICES DOS NEXT DOS OBJECTNODE QUE APONTAM PRO OBJETO RECEN DELETADO
}

// -----------------------------------------------------------------------------	

void Layer::setLayerFillProperties() const{
		//stipple fill
		glEnable( GL_POLYGON_STIPPLE );
		glPolygonStipple( STIPPLE_MASKS[ clsStippleMask ] );
		glColor4ub( clsFillColor.r, clsFillColor.g, clsFillColor.b, 100);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);		
}

// -----------------------------------------------------------------------------	

void Layer::setLayerEdgeProperties() const {
	glDisable( GL_POLYGON_STIPPLE );
	glColor3ub( clsBorderColor.r, clsBorderColor.g, clsBorderColor.b );
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
} // end method

// -----------------------------------------------------------------------------	

void Layer::drawLayer() const {
		if( !clsVisible || clsInLayer.empty() ) { return; }
		cout<<"draw layer\n";
		setLayerFillProperties();
		for( size_t i=0; i < clsInLayer.size(); i++){
			clsInLayer[i].getObject()->draw( clsZ );}
						
		//line
		setLayerEdgeProperties();
		for( size_t i=0; i < clsInLayer.size(); i++)
			clsInLayer[i].getObject() -> drawEdges( clsZ );
} // end method		

// -----------------------------------------------------------------------------	

void Layer::drawLayer(vector <Object *>  &deleteObj) {
	if( !clsVisible ) { return; }
		
	if( !deleteObj.empty() ){
		vector <ObjectNode> auxiliar;		
		int k = clsInLayer.size() ;		
		for( int i=k-1; i >= 0; i--){
			Object * obj = clsInLayer[i].getObject();			
			if( !deleteObj.empty() && obj == deleteObj.back() ){		
				deleteObj.pop_back();	
				delete obj;				
			}else{	
				auxiliar.push_back(obj);
			}
		}//end for
		clsInLayer.clear();
		clsInLayer = auxiliar;
		auxiliar.clear();
	}//end if
	
	drawLayer();
	
} // end method		*/	

// -----------------------------------------------------------------------------
	
void Layer::drawLayerIf(Vertex min, Vertex max) const {
	if( !clsVisible) { return; }
	
	setLayerFillProperties();
	
	for( size_t i=0; i < clsInLayer.size(); i++){
		Object * obj = clsInLayer[i].getObject();
		
		if( (obj->getMax().x >= min.x && obj->getMax().y >= min.y) 
			&&
			(obj->getMin().x <= max.x && obj->getMin().y <= max.y) )
				{ clsInLayer[i].getObject()->draw( clsZ );	}			
	}//end for
	
	//line
	setLayerEdgeProperties();
	for( size_t i=0; i < clsInLayer.size(); i++)
		clsInLayer[i].getObject() -> draw( clsZ );		
}

// -----------------------------------------------------------------------------	

void Layer::configure( const string name, const int layer, const Color &fillColor, const Color &borderColor, const StippleMask stippleMask ) {
	setName( name );
	setLayer( layer );
	setFillColor( fillColor );
	setBorderColor( borderColor );
	setStippleMask( stippleMask );
} // end method		

// -----------------------------------------------------------------------------	

void Layer::gimmeObjects(vector<Object*> &vec){
	for(int i = 0;  i< clsInLayer.size(); i++){
		vec.push_back(clsInLayer[ i ].getObject()->clone());
	}//end for
}//end method

// -----------------------------------------------------------------------------	
bool match(vector <Object*> &object, Object* obj){
	for(size_t i =0; i < object.size(); i++){
		if(object[i] == obj) return true;
	}//end for
	return false;
}//end func

// -----------------------------------------------------------------------------	

void Layer::gimmeConnectedObjects(vector <Object*> &object, size_t index, size_t incremento){
	bool term = true;

	size_t size = object.size();
	size_t incr = incremento;
	incremento = 0;
	
	for(size_t j = index; j < size; j++){
		for(size_t i = 0; i < clsInLayer.size(); i++){
			if( object[j]->intersected(clsInLayer[i].getObject()) && !match(object,clsInLayer[i].getObject()) ){
				object.push_back(clsInLayer[i].getObject());
				term = false;
				incremento++;
			}//end if
		}//end for i
	}//end for j
	
	if(!term)
		gimmeConnectedObjects(object, index+incr, incremento);

}//end method

// -----------------------------------------------------------------------------	



// -----------------------------------------------------------------------------	


// -----------------------------------------------------------------------------	


// -----------------------------------------------------------------------------	


