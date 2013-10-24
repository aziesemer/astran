#include "repository.h"
#include <cstring>
#include <iostream>
using namespace std;

#define printme( e ) { cout << #e << " = " << ( e ) << "\n"; } 

// -----------------------------------------------------------------------------

bool Repository::addModel( Model newModel ){
	clsModelCounter = max(clsModelCounter, newModel.getModelId());
	clsModelCounter++;
	newModel.setLayers(&clsLayers);
	clsModels.push_back( newModel );
	currentModel = clsModels.size() - 1;
	return true;
}//add a model to the list

// -----------------------------------------------------------------------------

bool Repository::addObject( Object * newObject, const int idLayer ) {
	clsLayers[idLayer].addObject( newObject );		
	clsDirty = true;
	return true;
} // add an object / redundancy

// -----------------------------------------------------------------------------

void Repository::copySelection(){
	if( clsSelectedObjects.empty() ) { return; }
	clsTransfer.clear();
	for(size_t i = 0; i < clsSelectedObjects.size() ; i ++){
		clsTransfer.push_back( ObjectReference( clsSelectedObjects[i]->clone(), clsSelectedObjects[i].getLayer()) );
	}//end for 
}// put the selected objects in the transfer area

// -----------------------------------------------------------------------------

void Repository::cuteSelection(){
	clsDirty = true;
	if( clsSelectedObjects.empty() ) { return; }
	clsTransfer.clear();
	for(size_t i = 0; i < clsSelectedObjects.size() ; i ++){
		clsTransfer.push_back( ObjectReference( clsSelectedObjects[i]->clone(), clsSelectedObjects[i].getLayer() ) );
	}//end for 	
	deleteSelecteds();
}//put the selected objects in the transfer area and delete the references in the main struct

// -----------------------------------------------------------------------------

void Repository::drawAll() {
	vector < Object * > del;
	
	if(modelMode){
		//clsComponents[currentModel]->draw(3);
		//clsModels[currentModel].draw();
		printme(clsModels[currentModel].getName());
		//return;
	}
	
	if( deleteSel ){
		for ( size_t i = 0; i < clsLayers.size(); i++ ){			
			del.clear();
			for( size_t y = 0; y < clsSelectedObjects.size(); y++){
				if( i == (size_t)clsSelectedObjects[y].getLayer() ){
					del.push_back( clsSelectedObjects[y].getObject() );
				}//end if
			}//end for
			clsLayers[i].drawLayer( del );			
		}//end for	
		clsSelectedObjects.clear();
		deleteSel = false;
		flatenizeModel();
	} else {
		for ( size_t i = 0; i < clsLayers.size(); i++ )
			clsLayers[i].drawLayer( );	
	} //end else*/	
	
	/*	cerr<<"rep. drawall \n";
		for(int j = 0; j < clsComponents.size(); j++){
			clsComponents[j].drawPic();
		}//*/		
} // draw all visible objects

// -----------------------------------------------------------------------------

void Repository::drawCurrentLayer(const int Layer){
	clsLayers[ Layer ].drawLayer();
}// draw only one layer

// -----------------------------------------------------------------------------

void Repository::drawSelectedBoundingBox(const int z){
	if( clsSelectedObjects.empty() )  {return; }
	
	const float pixelWidth = clsCamera->distScreenToCamera( 1 );
	const float border = pixelWidth * 5;
	const float roundness = pixelWidth * 5;
		
	const float w = maxSelected.x - minSelected.x + border*2;
	const float h = maxSelected.y - minSelected.y + border*2;
	const float x = minSelected.x - border;
	const float y = minSelected.y - border;

	glDisable( GL_DEPTH_TEST );
	
	// Configure OepnGL state.
	glEnable (GL_BLEND);
	glEnable (GL_LINE_SMOOTH);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint (GL_LINE_SMOOTH_HINT, GL_NICEST);

	//glColor4f( 0, 0, 0, 0.75f );
	//glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	//drawRoundedRectangle( x, y, w, h, roundness );
	
	glColor4f( 1, 1, 1, 0.75f );
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glLineWidth ( 2.5f );
	drawRoundedRectangle( x - pixelWidth, y - pixelWidth, w + 2*pixelWidth, h + 2*pixelWidth, roundness );
	glLineWidth( 1.0f );

	glDisable( GL_LINE_SMOOTH );
	glDisable( GL_BLEND );

	glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);
	glColor3ub( 255, 255, 0 );
	for( size_t i = 0; i < clsSelectedObjects.size(); i++){
		clsSelectedObjects[i]->drawEdges( z );
	}

	glEnable( GL_DEPTH_TEST );
		 
	checkGLErrors();
		 
/*
	glLineWidth( 2.0f );
	glColor3f( 1.0f, 1.0f, 1.0f );
	glBegin( GL_QUADS );
		glVertex3f( minSelected.x , minSelected.y , z );
		glVertex3f( maxSelected.x , minSelected.y , z );
		glVertex3f( maxSelected.x , maxSelected.y , z );
		glVertex3f( minSelected.x , maxSelected.y , z );
	glEnd();
	glLineWidth( 1.0f );
*/	
	glFlush();
}//draw the bouding box of the selected objects

// -----------------------------------------------------------------------------

void Repository::drawWiew(Vertex min, Vertex max) const{
	for ( size_t i = 0; i < clsLayers.size(); i++ ){
		clsLayers[i].drawLayerIf( min,  max);	
	}//end for
}//unused / dont draw object out the camera view

// -----------------------------------------------------------------------------

void Repository::deleteObj( const Object *deleteObj , const int layer ){
	if( deleteObj == NULL ){ return; }
	if( !clsLayers[ layer ].isVisible() ) { return; }
	
	const int numObjects = clsLayers[ layer ].getNumObjects();
	for ( int k = 0; k < numObjects; k++ ) {
		Object * obj = clsLayers[ layer ].getObject( k ); 
		if ( obj == deleteObj ){
			clsLayers[layer].deleteObj(k);
			return;
		}//end if			
	}//end for
	clsDirty = true;
}//remove the selecter object permanently

// -----------------------------------------------------------------------------

void Repository::deleteSelecteds(){
	if(!existsSelection()){return;}
	clsDirty = true;
	deleteSel = true;/*
	
	if( clsSelectedObjects.empty() ){ return; }
	
	const int s = clsSelectedObjects.size();
	for(int i = 0; i< s; i++){
		deleteObj( clsSelectedObjects[i].getObject() , clsSelectedObjects[i].getLayer() );
	}//end for
	clsSelectedObjects.clear();*/
}//very complex, dont touch!

// -----------------------------------------------------------------------------

void Repository::editSelecteds(const int x, const int y, const int z){

	glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);
	glLineWidth( 2.0f );
	glColor3f( 1.0f, 1.0f, 1.0f );
	glBegin( GL_QUADS );

		glColor3f( 1, 1, 1 );
		int x1=0;	 int y1=0;
		int x2=0;	 int y2=0;
		switch ( direction ) {
			case RIGHT		: x1 = 0; y1 = 0; x2 = x; y2 = 0; break;
			case LEFT		: x1 = x; y1 = 0; x2 = 0; y2 = 0; break;
			case UP			: x1 = 0; y1 = 0; x2 = 0; y2 = y; break;
			case DOWN		: x1 = 0; y1 = y; x2 = 0; y2 = 0; break;
			case RIGHT_UP	: x1 = 0; y1 = 0; x2 = x; y2 = y; break;
			case RIGHT_DOWN	: x1 = 0; y1 = y; x2 = x; y2 = 0; break;
			case LEFT_UP	: x1 = x; y1 = 0; x2 = 0; y2 = y; break;
			case LEFT_DOWN	: x1 = x; y1 = y; x2 = 0; y2 = 0; break;
			case NONE		: break;
		} // end switch

		for( size_t i = 0; i< clsSelectedObjects.size(); i++){
			clsSelectedObjects[i]->drawEditMode(z, x1, y1);
		}
	
	glEnd();
	glLineWidth( 1.0f );
}//

// -----------------------------------------------------------------------------

bool Repository::isClickInSelection(const int x, const int y){
	for( size_t i = 0; i< clsSelectedObjects.size(); i++){
		if( clsSelectedObjects[i]->contains(x, y) ){ /*cout<< "FOUNDD\n"; */ return true; }
	}
	return false;
}//return true if the selection contains the click

// -----------------------------------------------------------------------------

bool Repository::findSelectedBB(Vertex p1, Vertex p2){
//	if(modelMode){return false;}
	printme(modelMode);
	clsSelectedObjects.clear();
	
	minSelected.x = min( p1.x, p2.x );
	minSelected.y = min( p1.y, p2.y );
	maxSelected.x = max( p1.x, p2.x );
	maxSelected.y = max( p1.y, p2.y );
	
	if( maxSelected.x == minSelected.x && maxSelected.y == minSelected.y){//click
		ObjectReference objReference;
	//	bool hasObject = componentAtPosition( minSelected.x, minSelected.y, objReference );
		bool hasObject = objectAtPosition( minSelected.x, minSelected.y, objReference );
		if( hasObject ){ 	//search group
				minSelected.x = objReference->getMin().x ;
				minSelected.y = objReference->getMin().y ;
				maxSelected.x = objReference->getMax().x ;
				maxSelected.y = objReference->getMax().y ;
				
				clsSelectedObjects.push_back( objReference ); 
		} 					
	} else {	//selection box
			const int numComponents = clsComponents.size();
			for ( int k = 0; k < numComponents; k++ ) {
				sgtComponent * obj = clsComponents[k];				
				if( obj->inn(minSelected, maxSelected) ){
					clsSelectedObjects.push_back( ObjectReference( obj, obj->getLayer() ) ); 
				}//end if	
			}//end for
		setSelectedBoudingBox();
	} //end else

	return clsSelectedObjects.empty();
}//find the selected COMPONENTS - > groups of objects

// -----------------------------------------------------------------------------

bool Repository::findSelectedBoundingBox(Vertex p1, Vertex p2){
//	if(modelMode){	return false; }
	clsSelectedObjects.clear();
	
	minSelected.x = min( p1.x, p2.x );
	minSelected.y = min( p1.y, p2.y );
	maxSelected.x = max( p1.x, p2.x );
	maxSelected.y = max( p1.y, p2.y );
	
	if( maxSelected.x == minSelected.x && maxSelected.y == minSelected.y){//click
		ObjectReference objReference;
		const bool hasObject = objectAtPosition( minSelected.x, minSelected.y, objReference );
		
		if( hasObject ){ 	//search group
			
			int group = objReference.getGroup();
		
			if( group != 0 ){
													
				const int numLayers = clsLayers.size();
			
				for ( int l = numLayers - 1; l >= 0; l-- ) {
					const int numObjects = clsLayers[ l ].getNumObjects();
					for ( int k = 0; k < numObjects; k++ ) {
						Object *obj = clsLayers[ l ].getObject( k );
						if( obj->getGroup() == group ){
							
							clsSelectedObjects.push_back( ObjectReference( obj, l) ); 
						}		
					}//end for
				}//end for
				
				setSelectedBoudingBox();	
			
			} else {					
				clsSelectedObjects.push_back( objReference );

				minSelected.x = objReference->getMin().x ;
				minSelected.y = objReference->getMin().y ;
				maxSelected.x = objReference->getMax().x ;
				maxSelected.y = objReference->getMax().y ;
			}//end else
		}// end if
				
	} else {	//selection box
		
		const int numLayers = clsLayers.size();
		for ( int l = numLayers - 1; l >= 0; l-- ) {
			if( !clsLayers[ l ].isVisible() ) { continue; }
			const int numObjects = clsLayers[ l ].getNumObjects();
			for ( int k = 0; k < numObjects; k++ ) {
				Object *obj = clsLayers[ l ].getObject( k );
				
				if( obj->inn(minSelected, maxSelected) ){
					clsSelectedObjects.push_back( ObjectReference( obj, l ) ); 
				}		
			}//end for
		}//end for
		setSelectedBoudingBox();
	} //end else
	
	return clsSelectedObjects.empty();
}//find the objects in the selection box or click

// -----------------------------------------------------------------------------

Model * Repository::getModel(const int idx ){
	for (size_t i=0; i< clsModels.size(); i++) {
		if( clsModels[i].getModelId() == idx )
			return &clsModels[i];
	}
	return NULL;
}//return a pointer to the model

// -----------------------------------------------------------------------------

Model * Repository::getModelByName(const char* name){
	for(int i = 0; i < clsModels.size(); i++){
		const char * n = clsModels[i].getName().c_str();
		if( !strcmp(name,n) ) {
			return &clsModels[i];
		}//end if
	}//end for
	return NULL;
}//end method

// -----------------------------------------------------------------------------

bool Repository::groupObjects(){
	if( clsSelectedObjects.empty() ){
		return false;
	}else{
		for( size_t i = 0; i < clsSelectedObjects.size(); i ++){
			clsSelectedObjects[i].setGroup( clsNgroup );		
		}	
		clsNgroup++;
		return true;
	}
}//serah descontinuado? formarah modelos?

// -----------------------------------------------------------------------------

bool Repository::letResize(Vertex p){

	bool yeah;
	yeah = false;
	
	if ( minSelected.x == p.x ) { 
		if( p.y <= maxSelected.y && p.y >= minSelected.y ){ 
			yeah = true;
			glutSetCursor( GLUT_CURSOR_LEFT_RIGHT ); 
			setDirection(LEFT);
			if( p.y == minSelected.y ) { setDirection(LEFT_DOWN); glutSetCursor(GLUT_CURSOR_BOTTOM_LEFT_CORNER); }
			else { 
				if( p.y == maxSelected.y ) { setDirection(LEFT_UP); glutSetCursor(GLUT_CURSOR_TOP_LEFT_CORNER); } 
			}			
		}//end if
	} else { 
		if( maxSelected.x == p.x){ 
			if( p.y <= maxSelected.y && p.y >= minSelected.y ){ 
				yeah = true;
				glutSetCursor( GLUT_CURSOR_LEFT_RIGHT ); 
				setDirection(RIGHT);
				if( p.y == minSelected.y ) { setDirection(RIGHT_DOWN); glutSetCursor(GLUT_CURSOR_BOTTOM_RIGHT_CORNER); }
				else { 
					if( p.y == maxSelected.y ) { setDirection(RIGHT_UP); glutSetCursor(GLUT_CURSOR_TOP_RIGHT_CORNER); } 
				}			
			}
		}//end if
	}//end else
	
	if( !yeah ) {
		if( p.y == minSelected.y ){
			if(p.x <= maxSelected.x && p.x >= minSelected.x ){
				yeah = true; 
				setDirection(DOWN); 
				glutSetCursor( GLUT_CURSOR_UP_DOWN );
		}	}
		else {
			if( p.y == maxSelected.y ){
				if( p.x <= maxSelected.x && p.x >= minSelected.x ){
					yeah = true; 
					setDirection(UP); 
					glutSetCursor( GLUT_CURSOR_UP_DOWN );
			}	}
		}//end else
	}
	if( !yeah ){ setDirection(NONE); glutSetCursor(GLUT_CURSOR_LEFT_ARROW);} 
	return yeah;
}

// -----------------------------------------------------------------------------

void Repository::moveSelecteds(const int z, const int difX, const int difY ) {	
	glPolygonMode(GL_FRONT_AND_BACK , GL_LINE);
	glColor3ub( 255, 255, 0 );
	for( size_t i = 0; i< clsSelectedObjects.size(); i++){
		clsSelectedObjects[i]->drawEditMode(z, difX, difY);
	}
}

// -----------------------------------------------------------------------------

Ruler * Repository::rulerAtPosition( Vertex p ) {
	const int numR = clsRulers.size();
	for(int i = 0; i < numR; i++){
		Ruler * ruler = &(clsRulers[i]); 
		if( ruler->contains(p.x, p.y) ){return ruler;}
	}//end for
	return NULL;
}//end method

// -----------------------------------------------------------------------------

Object * Repository::objectAtPosition( Vertex p ) const {
	const int numLayers = clsLayers.size();
	for ( int l = numLayers - 1; l >= 0; l-- ) {
		if( !clsLayers[ l ].isVisible() ) { continue; }
		const int numObjects = clsLayers[ l ].getNumObjects();
		for ( int k = 0; k < numObjects; k++ ) {
			Object *obj = clsLayers[ l ].getObject( k );
			
			if ( obj->contains( p.x, p.y ) ) {
				//cout << "Object: layer=" << clsLayers[ l ].getName() << "(" << l << ")\n";
				return obj;
			} // end if
		} // end for objects
	} // enf for layers
	return NULL;
} // end method 

// -----------------------------------------------------------------------------

Object * Repository::componentAtPosition( Vertex p ) {
	//if(modelMode){return false;}
	cerr<<"comp at pos\n\n";
	const int numComponents = clsComponents.size();
	Object* object;
	for ( int l = numComponents - 1; l >= 0; l-- ) {
		sgtComponent *obj = clsComponents[ l ];				
		if ( (object = obj->contains( p ) ) != NULL  ) {
			if(clsHierarchyStak.empty()){
				return object;
			} else { 
				if( clsComponents[ l ]->getHierarchyStatus() == SGT_OPEN ){ 
					return object;
				} else { cerr<<"NULL\n"; return NULL; }
			}//end else
		}//end if
	} // enf for
	cerr<<"NULL\n"; 
	return NULL;
} // end method 

// -----------------------------------------------------------------------------

bool Repository::componentAtPosition( const int x, const int y, ObjectReference &objReference ) {
	//if(modelMode){return false;}
	cerr<<"COMPONENT AT POS \n";
	objReference = ObjectReference( componentAtPosition(Vertex(x,y)), 1 ); /*
	const int numComponents = clsComponents.size();
	printme(numComponents);
	for ( int l = numComponents - 1; l >= 0; l-- ) {
		sgtComponent *obj = clsComponents[ l ];			
		if ( obj->contains( x, y ) ) {
				objReference = ObjectReference( obj, l);
				return true;
			} // end if
	} // enf for layers
	return false;*/
	if(objReference.getObject() == NULL){ cerr<<"null selected\n\n\n\n\n"; return false; }
	else return true;
} // end method 

// -----------------------------------------------------------------------------

bool Repository::objectAtPosition( const int x, const int y, ObjectReference &objReference ) const {
	cerr<<"OBJ AT POS\n\n";
	const int numLayers = clsLayers.size();
	for ( int l = numLayers - 1; l >= 0; l-- ) {
		if( !clsLayers[ l ].isVisible() ) { continue; }
		const int numObjects = clsLayers[ l ].getNumObjects();
		for ( int k = 0; k < numObjects; k++ ) {
			Object *obj = clsLayers[ l ].getObject( k );
			if ( obj->contains( x, y ) ) {
				printme(k);
				printme( clsLayers[l].getNumObjects() );
				
				objReference = ObjectReference( obj, l);
				return true;
			} // end if
		} // end for objects
	} // enf for layers
	return false;
} // end method 

// -----------------------------------------------------------------------------
/*
Object * Repository::objectByIndex( const int id ) const {
	const int numObjects = clsObjectNodes.size();
	for ( int i = 0; i < numObjects; i++ ) {
		Object *obj = clsObjectNodes[ i ].getObject();
		if ( obj->getId() == id )
			return obj;
	} // end method
	return NULL;
} // end method
*/

// -----------------------------------------------------------------------------

void Repository::pasteSelection(const int x, const int y){
	clsDirty = true;
	if( clsTransfer.empty() ) { return; }
	clsSelectedObjects.clear();
//	bool regroup = false;
	for(size_t i = 0; i < clsTransfer.size() ; i ++){
//		clsTransfer[i]->translate(x, y);
	/*	if( clsTransfer[i]->getGroup() != 0 ){
			clsSelectedObjects.push_back( ObjectReference(clsTransfer[i]->clone(), clsTransfer[i].getLayer()) );
			clsSelectedObjects.back().setGroup(clsNgroup);
//			regroup = true;
		} else {//*/
			clsSelectedObjects.push_back( ObjectReference(clsTransfer[i]->clone(), clsTransfer[i].getLayer()) );
	//	}
			addComponent( ((sgtComponent*)(clsSelectedObjects.back().getObject())) );
	}//end for 	
	setSelectedBoudingBox();
//	if ( regroup ){	clsNgroup++; } 
	
	flatenize();
}//end method

// -----------------------------------------------------------------------------

void Repository::resizeSelecteds(const int x, const int y){
	int x1=0;	 int y1=0;
	int x2=0;	 int y2=0;
	switch ( direction ) {
		case RIGHT		: x1 = 0; y1 = 0; x2 = x; y2 = 0; break;
		case LEFT		: x1 = x; y1 = 0; x2 = 0; y2 = 0; break;
		case UP			: x1 = 0; y1 = 0; x2 = 0; y2 = y; break;
		case DOWN		: x1 = 0; y1 = y; x2 = 0; y2 = 0; break;
		case RIGHT_UP	: x1 = 0; y1 = 0; x2 = x; y2 = y; break;
		case RIGHT_DOWN	: x1 = 0; y1 = y; x2 = x; y2 = 0; break;
		case LEFT_UP	: x1 = x; y1 = 0; x2 = 0; y2 = y; break;
		case LEFT_DOWN	: x1 = x; y1 = y; x2 = 0; y2 = 0; break;
		case NONE		: break;
	};

	for( size_t i = 0; i< clsSelectedObjects.size(); i++){
		clsSelectedObjects[i]->resize(x1, y1, x2, y2);
	}
	
	minSelected.x += x1;
	maxSelected.x += x2;
	minSelected.y += y1;
	maxSelected.y += y2;
	clsDirty = true;
}

// -----------------------------------------------------------------------------

void Repository::reset(){
	for( size_t i = 0; i< clsLayers.size(); i++ ){
		clsLayers[i].clearObj();
	}
	clsModels.clear(); 
	clsTransfer.clear(); 
	clsSelectedObjects.clear(); 
	resetAbsoluteBoudingBox();	
	clsRulers.clear();
	clsComponents.clear();
	
	modelMode = false;
	clsDirty = false;
}

// -----------------------------------------------------------------------------

bool Repository::setAbsoluteBoudingBox(){
	if(modelMode){
		clsMax = clsModels[currentModel].getMax();
		clsMin = clsModels[currentModel].getMin();
	} else {
		const int numLayers = clsLayers.size();
		for ( int l = numLayers - 1; l >= 0; l-- ) {
			if( !clsLayers[ l ].isVisible() ) { continue; }
			const int numObjects = clsLayers[ l ].getNumObjects();
			for ( int k = 0; k < numObjects; k++ ) {
				Object *newObject = clsLayers[ l ].getObject( k );

				clsMax.x = max( clsMax.x, newObject->getMax().x );
				clsMax.y = max( clsMax.y, newObject->getMax().y );
				clsMin.x = min( clsMin.x, newObject->getMin().x );
				clsMin.y = min( clsMin.y, newObject->getMin().y );			

			} // end for objects		
		} // enf for layers	
	}	
	return 
		( clsMax.x == INT_MAX || clsMax.y == INT_MAX ) ||
		( clsMin.y == INT_MIN || clsMin.x == INT_MIN );	
	
}// end method

// -----------------------------------------------------------------------------

void Repository::setSelectedBoudingBox(){
	if( clsSelectedObjects.empty() ){ return; }
	minSelected.x = INT_MAX;
	minSelected.y = INT_MAX;
	maxSelected.x = INT_MIN;
	maxSelected.y = INT_MIN; 
	cout<<"SBOX\n";
	
	for( size_t i = 0; i < clsSelectedObjects.size(); i++ ){
		minSelected.x = min( minSelected.x, clsSelectedObjects[i]->getMin().x );
		minSelected.y = min( minSelected.y, clsSelectedObjects[i]->getMin().y );
		maxSelected.x = max( maxSelected.x, clsSelectedObjects[i]->getMax().x );
		maxSelected.y = max( maxSelected.y, clsSelectedObjects[i]->getMax().y );
	}	
}//end method

// -----------------------------------------------------------------------------

void Repository::translateSelecteds( const int difX, const int difY ){
	clsDirty = true;
	for( size_t i = 0; i< clsSelectedObjects.size(); i++){
		clsSelectedObjects[i]->translate(difX, difY);
	}	
	minSelected.x += difX;
	maxSelected.x += difX;
	minSelected.y += difY;
	maxSelected.y += difY;
	
	clsDirty = true;
}//end method

// -----------------------------------------------------------------------------

bool Repository::ungroupObjects(){
	if( clsSelectedObjects.empty() ){
		return false;
	}else{
		for( size_t i = 0; i < clsSelectedObjects.size(); i ++){
			clsSelectedObjects[i].setGroup( 0 );		
		}	
		return true;
	}//end if
}

// -----------------------------------------------------------------------------

void Repository::drawRulers(const int z, int scale, const float zoom){

	if( clsRulers.empty() || zoom < (float)10/scale ){return;}
	
	glDisable(GL_DEPTH_TEST);   

	for(size_t i = 0; i < clsRulers.size(); i++){
		clsRulers[i].drawRuler(z, scale, zoom);
	}//end for*/	
	glFinish();
	glFlush();
	glEnable(GL_DEPTH_TEST);
}//end method

// -----------------------------------------------------------------------------

float Repository::getCurrentZ(const int layer){
	return clsLayers[layer].getZ();
}

// -----------------------------------------------------------------------------

void Repository::deleteRuler(Ruler * ruler){
	const int numR = clsRulers.size();
	for(int i = 0; i<numR; i++){
		Ruler * r = &(clsRulers[i]);
		if(r == ruler){
			clsRulers[i] = clsRulers.back();
			clsRulers.pop_back();
			return;
		}//end if
	}//end for
}//endd method

// -----------------------------------------------------------------------------

void Repository::flatenize(){
	cerr<<"REFLATENIZANDO\n";

		vector <ObjectReference> news;
		for(size_t l = 0; l < clsLayers.size(); l++){
			clsLayers[l].clearObj();
		}//end for
		
		printme(clsComponents.size());
		for(size_t i = 0; i <  clsComponents.size(); i++){
			news.clear();
			//pega vetor de objetos do componente
			clsComponents[i]->gimmeObjects(news);
			//passa para a estrutura rapida
			for(size_t j = 0; j < news.size(); j++){
				clsLayers[news[j].getLayer()].addObject(news[j].getObject());
			}//end for
		}//end for
	cerr<<"reflatenizado\n";
}//gets the objects from the components and put they in the fast struct

// -----------------------------------------------------------------------------

Model* Repository::FindModelPt( const string &componentName ) {
	const int numModels = clsModels.size();
	for ( int i = 0; i < numModels; i++ ) {
		if ( clsModels[ i ].getName() == componentName )
			return &clsModels[ i ];
	} // end for
	return NULL;
} // end method

// -----------------------------------------------------------------------------

void Repository::downHierarchy(){
	if( clsSelectedObjects.size() == 1 && clsSelectedObjects.back().getObject()->getType() == OBJECT_COMPONENT ){
		clsHierarchyStak.push_back((sgtComponent*)clsSelectedObjects.back().getObject());
		((sgtComponent*)clsSelectedObjects.back().getObject())->setHierarchy(SGT_OPEN);
		clsSelectedObjects.clear();
		flatenize();
	}//end if
}

// -----------------------------------------------------------------------------

void Repository::upHierarchy(){
	if( clsHierarchyStak.empty() ){ return; }
	
	clsHierarchyStak.back()->setHierarchy(SGT_ACTIVE);
	clsHierarchyStak.pop_back();
	
	clsSelectedObjects.clear();
	
	flatenize();
}//

// -----------------------------------------------------------------------------

void Repository::startHierarchy(){
	clsComponents[0]->setHierarchy(SGT_ACTIVE);
}//inicializacao

// -----------------------------------------------------------------------------

string Repository::getModelName(int i){
	return clsModels[i].getName();
}

// -----------------------------------------------------------------------------

bool Repository::createNewComponent(const char* name){
	Model * model = getModelByName(name);
	
	if(model == NULL) {
		return false;
	} else{		
		sgtComponent * c = new sgtComponent();	
		c->setModelPt( model );

		c->setHierarchy(SGT_OPEN);
		addComponent(c);
		clsHierarchyStak.push_back(c);
		flatenize();

		cerr<<"FINISH\n\n";
	}//end else
}//wrong**

// -----------------------------------------------------------------------------

bool Repository::createNewModel(const char* name){
	if( clsSelectedObjects.empty() ) return false;	
	printme(clsSelectedObjects.size());
	Model newModel;
	newModel.setModelId(clsModelCounter++);
	newModel.setScale(1, 1);	
	newModel.setName(name);	 
	newModel.setLayers(&clsLayers); 
	cerr<<"creating new model\n";
	printme(clsSelectedObjects.size());
	
	for(size_t i = 0; i < clsSelectedObjects.size(); i++){
		newModel.addContent( (clsSelectedObjects[i].getObject())->clone(), clsSelectedObjects[i].getLayer() );
	}// end for
	
	newModel.setMinMax();

	addModel( newModel );	
	
	return true;
}

// -----------------------------------------------------------------------------

void Repository::drawModel(string name){
	int index = 0;
	clsDirty = false;
	for(index; index <  clsModels.size(); index++){
		if(clsModels[index].getName() == name){
			break;
		}//end if
	}//end for 
	
	if(index >= clsModels.size()){ 
		modelMode = false;
		flatenize();
	} else {
		currentModel = index;
		modelMode = true;	
		flatenizeModel();
		clsSelectedObjects.clear();
	}//end else
}//end method

// -----------------------------------------------------------------------------

void Repository::flatenizeModel(){
		cerr<<"REFLATENIZANDO pelo modelo\n";
		
	for(size_t l = 0; l < clsLayers.size(); l++){
		clsLayers[l].clearObj();
	}//end for

	vector <ObjectReference> news;
	news.clear();
	//pega vetor de objetos do componente
	clsModels[currentModel].gimmeObjects(news);
	cerr<<"\n\n\n";
	printme(news.size());
	
	for(size_t j = 0; j < news.size(); j++){
		clsLayers[news[j].getLayer()].addObject(news[j].getObject());
	}//end for	
	cerr<<"reflatenizado pelo modelo";
}

// -----------------------------------------------------------------------------

void Repository::start(){
	cerr<<"starting repository\n";
	clsDirty = false;
	printme(clsModels.size());
	int i = 0;
	while( i < clsModels.size() ){
		if(clsModels[i].empty()){
			printme(clsModels[i].getName());
			clsModels[i] = clsModels.back();
			clsModels.pop_back();
		} else {
			clsModels[i].setMinMax();
			i++;
		}
	}
		currentModel = clsModels.size()-1;
		modelMode = true;	
		flatenizeModel();	
		printme(clsModels.size());
	/*clsRepository->flatenize(); 
	clsRepository->startHierarchy(); //*/
}
// -----------------------------------------------------------------------------
		
void Repository::saveCurrentModel(){
	clsModels[currentModel].saveChanges();
	clsModels[currentModel].setMinMax();
	clsDirty = false;
}

// -----------------------------------------------------------------------------

void Repository::saveNewModel(const char* name){
	Model newModel;
	newModel.setModelId(clsModelCounter++);
	newModel.setScale(1, 1);	
	newModel.setName(name);	
	newModel.setLayers(&clsLayers); 
	newModel.saveChanges();	
	newModel.setMinMax();
	addModel( newModel );
}

// -----------------------------------------------------------------------------

bool Repository::mirror(bool Xmirror){
	if (clsSelectedObjects.empty()) return false;
	
	for( size_t i = 0; i <  clsSelectedObjects.size(); i++ ){
		clsSelectedObjects[i]->mirroring(Xmirror);
	}//end for
	clsDirty = true;
	setSelectedBoudingBox();
	setAbsoluteBoudingBox();
	return true;
}//end method

// -----------------------------------------------------------------------------

void Repository::seekAndAdd(int layer, vector <Object*> &object, size_t from){
	clsLayers[layer].gimmeConnectedObjects(object, from, object.size());
	
	for(size_t i = from; i < object.size(); i++){
		clsSelectedObjects.push_back( ObjectReference(object[i], layer ) );
	}//end for	

}

// -----------------------------------------------------------------------------

bool Repository::findConnection(const int ID){
	if( clsSelectedObjects.size() != 1 ) return false;

	vector <Object*> connecteds;
	connecteds.push_back(clsSelectedObjects[0].getObject());
	
	seekAndAdd(clsSelectedObjects.back().getLayer(), connecteds, 0);

	int layer = clsSelectedObjects.back().getLayer();

	switch(ID){
		case 0 : { //this

		} break; 
		case 1 : { //up
				if ( layer+1 > (clsLayers.size() - 1) ) return true;
				
				int size0 = 0;
				int size1 = 0;
				while(size0 != connecteds.size()-1){		
					size0 = connecteds.size()-1;	
					seekAndAdd(layer+1, connecteds, size1);
					size1 = connecteds.size()-1;
					seekAndAdd(layer, connecteds, size0);
				}//end while
		} break;
		case 2: { //down				
				if ( layer-1 < 0 ) return true;
				
				int size0 = 0;
				int size1 = 0;
				while(size0 != connecteds.size()-1){		
					size0 = connecteds.size()-1;	
					seekAndAdd(layer-1, connecteds, size1);
					size1 = connecteds.size()-1;
					seekAndAdd(layer, connecteds, size0);
				}//end while
		} break;
		case 3: { // up down
				int layerD = layer-1;
				int layerU = layer + 1;
				
				if ( layerD < 0 || layerU > (clsLayers.size() - 1) )  return true;
				
				int size0 = 0;
				int size1 = 0;
				int size2 = 0;
				
				while(size0 != connecteds.size()-1){		
					size0 = connecteds.size()-1;	
					seekAndAdd(layerD, connecteds, size2);
					size1 = connecteds.size()-1;
					seekAndAdd(layer, connecteds, size0);
					size2 = connecteds.size()-1;
					seekAndAdd(layerU, connecteds, size1);
				}//end while
		} break;	
	};
	
	return true;
}//end method

// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		
		


