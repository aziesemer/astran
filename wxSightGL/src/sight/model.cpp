#include "model.h"

#include <climits>

//------------------------------------------------------------------------------

bool Model::addContent( Object *newObject, int layer )	{
	clsContents.push_back( ObjectReference( newObject, layer ) );
	return true;
}	// add any object to the model

//------------------------------------------------------------------------------

void Model::draw(){
	Vertex zero = Vertex(0,0);
	cerr<<"model draw\n";
	drawFillFromModel(zero, zero, false, false);
	drawEdgeFromModel(zero, zero, false, false);	

}

//------------------------------------------------------------------------------

void Model::drawFillFromModel( const Vertex translation , const Vertex rotation, const bool mirrorX, bool hasmirror ){
	glPushMatrix();
	glTranslatef(translation.x, translation.y, 0);
	//glRotatef(rotation.x, rotation.y);
	
	if(hasmirror)
		if(!mirrorX){
			glScalef(-1, 1, 0);
		}else{
			glScalef(1, -1, 0);
		}//*/	

	for(int i = 0; i < clsContents.size(); i++){
		Layer layer = (*clsLayers)[clsContents[i].getLayer()];
		layer.setLayerFillProperties();
		(clsContents[i].getObject())->draw(layer.getZ()); 
	} //end for
	
	glPopMatrix();
	
}//draw the edges of the component

//------------------------------------------------------------------------------

void Model::drawEdgeFromModel( const Vertex translation , const Vertex rotation, const bool mirrorX, bool hasmirror ){
	glPushMatrix();
		cerr<<"Edge from MODEL\n\n";
	glTranslatef(translation.x, translation.y, 0);
	//glRotatef(rotation.x, rotation.y);
	if(hasmirror)
		if(!mirrorX){
			glScalef(-1, 1, 0);
		}else{
			glScalef(1, -1, 0);
		}//*/	
	printme(clsContents.size());
	for(int i = 0; i < clsContents.size(); i++){
		Layer layer = (*clsLayers)[clsContents[i].getLayer()];
		layer.setLayerEdgeProperties();
		(clsContents[i].getObject())->draw(layer.getZ());
	//	cerr<<"drawing edge from model\n";
	} //end for
	cerr<<"finish edge from model\n\n";
	glPopMatrix();
	
}//draw the edges of the component

//------------------------------------------------------------------------------

void Model::drawFromModel( const Vertex translation , const Vertex rotation, const bool mirrorX, bool hasmirror ){
	glPushMatrix();
	glColor3f(1,0,0);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	
	glTranslatef(translation.x, translation.y, 0);
	//glRotatef(rotation.x, rotation.y);
	
	if(hasmirror)
		if(!mirrorX){
			glScalef(-1, 1, 0);
		}else{
			glScalef(1, -1, 0);
		}//*/	

	for(size_t i =0; i < clsContents.size(); i++){ 
		(clsContents[i].getObject())->draw(clsContents[i].getLayer()); 
	} 
	glPopMatrix();
	
}//not right
//------------------------------------------------------------------------------

void Model::drawLayerFromModel( const Vertex translation, const Vertex Rotation, const bool mirrorX, const bool hasmirror, int layer ){

	glPushMatrix();
	
	glTranslatef(translation.x, translation.y, 0);
	//glRotatef(rotation.x, rotation.y);
	
	if(hasmirror)
		if(!mirrorX){
			glScalef(-1, 1, 0);
		}else{
			glScalef(1, -1, 0);
		}//*/	

	if(clsLayerIndex < clsContents.size() ){
		printme(layer);
		printme(clsLayerIndex);
		while( clsLayerIndex == layer ){		
			(clsContents[clsLayerIndex].getObject())->draw(clsContents[clsLayerIndex++].getLayer()); 
		} // end while
	} else {
		clsLayerIndex = 0;
	}
	glPopMatrix();

}//wrong

//------------------------------------------------------------------------------

void Model::drawFromPicture( const Vertex translation , const Vertex rotation, const bool mirrorX, bool hasmirror ){
	glPushMatrix();
	
	glTranslatef(translation.x, translation.y, 0);
	//glRotatef(rotation.x, rotation.y);
	
	if(hasmirror)
		if(!mirrorX){
			glScalef(-1, 1, 0);
		}else{
			glScalef(1, -1, 0);
		}//*/	

	glBindTexture(textureType , clsPicture.getReference());

	const int w = clsMax.x - clsMin.x;
	const int h = clsMax.y - clsMin.y;

	glBegin(GL_QUADS);
		glTexCoord2f( clsMin.x, clsMin.y ); glVertex2f( clsMin.x, clsMin.y );//glTextCoord2f:seta as coordenadas do valor p/operar
		glTexCoord2f( clsMax.x, clsMin.y ); glVertex2f( clsMax.x, clsMin.y );//glVertex: manda calcular
		glTexCoord2f( clsMax.x, clsMax.y ); glVertex2f( clsMax.x, clsMax.y );
		glTexCoord2f( clsMin.x, clsMax.y ); glVertex2f( clsMin.x, clsMax.y ); 
	glEnd();
	glFlush();

	glPopMatrix();
	
}//draw the model from its texture

//------------------------------------------------------------------------------

void Model::setMinMax(){
	cerr<<"\nSET MIN MAX \t";
	printme(clsName);

	clsMin.x = clsMin.y = INT_MAX;
	clsMax.x = clsMax.y = INT_MIN;
	for(size_t i = 0; i < clsContents.size(); i++){
		if(clsContents[i].getObject()->getType() == OBJECT_COMPONENT){
			((sgtComponent*)clsContents[i].getObject())->setMinMax();
		}
		Vertex mn = (clsContents[i].getObject())->getMin();
		Vertex mx = (clsContents[i].getObject())->getMax();
		clsMin.x = min( min(clsMin.x, mn.x), mx.x);
		clsMin.y = min( min(clsMin.y, mn.y), mx.y);
		clsMax.x = max( max(clsMax.x, mn.x), mx.x);
		clsMax.y = max( max(clsMax.y, mn.y), mx.y); 
	}//end for
	printme(clsMax);
	printme(clsMin);
}//set the model's boudingbox


//------------------------------------------------------------------------------

void Model::picMe(){
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, clsPicture.getFBid() );
	
	for(size_t i =0; i < clsContents.size(); i++){ 
		(*clsLayers)[clsContents[i].getLayer()].setLayerFillProperties();
		(clsContents[i].getObject())->draw(clsContents[i].getLayer()); 
	} 
	
	for(size_t i =0; i < clsContents.size(); i++){ 
		(*clsLayers)[clsContents[i].getLayer()].setLayerEdgeProperties();
		(clsContents[i].getObject())->drawEdges(clsContents[i].getLayer()); 
	} 
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}//draw the model into a texture

//------------------------------------------------------------------------------

void Model::gimmeObjectsOnly( vector <ObjectReference> & vec ){
	ObjectReference obj;
	printme(clsName);
	printme(clsContents.size());
	for(size_t i = 0 ; i < clsContents.size(); i++){
		if(!(clsContents[i].getObject()->getType() == OBJECT_COMPONENT) ){
			obj = ObjectReference(clsContents[i]->clone(), clsContents[i].getLayer());
			vec.push_back(obj);
		}
	}//end for
}//set the vector with clones of the objects

//------------------------------------------------------------------------------

void Model::saveCallsOnly( ofstream &file ){

	printme(clsName);
	printme(clsContents.size());
	for(size_t i = 0 ; i < clsContents.size(); i++){
		if(clsContents[i].getObject()->getType() == OBJECT_COMPONENT){
			sgtComponent * call =  (sgtComponent*)clsContents[i].getObject();
			// C id transf args transf args..
			file <<  "C " << call->getModelId() ;
			if(call->hMirror() ) {
				file << " M";
				if(call->mirrorX() )	file << "X";
				else					file << "Y";
			}//end if
			if(call->hasTranslation()) {
				file << " T " << call->getTranslation().x << " "<< call->getTranslation().y;
			}//end if translation
			file << ";\n";
		} //end if component
	}//end for

}//set the vector with clones of the objects

//------------------------------------------------------------------------------

void Model::gimmeObjects( vector <ObjectReference> & vec ){
	ObjectReference obj;
	printme(clsName);
	printme(clsContents.size());
	for(size_t i = 0 ; i < clsContents.size(); i++){
		if(clsContents[i].getObject()->getType() == OBJECT_COMPONENT){
			((sgtComponent*)clsContents[i].getObject())->gimmeObjects(vec);
		} else {
			obj = ObjectReference(clsContents[i]->clone(), clsContents[i].getLayer());
			vec.push_back(obj);
		}
	}//end for
}//set the vector with clones of the objects

//------------------------------------------------------------------------------

void Model::saveChanges(){
	clsContents.clear();
	cerr<<"saving changes\n\n";	
	for(int i = 0; i <  clsLayers->size(); i++){
		vector <Object*> news;
		(*clsLayers)[i].gimmeObjects(news);
		for(int j = 0; j < news.size(); j++){
			clsContents.push_back(ObjectReference(news[j], i));
		}//end for
	}//end for
}

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

sgtComponent::sgtComponent(Vertex t, Model* m): Object( OBJECT_COMPONENT ){ 
	clsTranslation = t; 
	clsModelIndex = m; 
	hasMirror = false; 
	clsDirty = false; 
	clsMin = m->getMin() + t;
	clsMax = m->getMax() + t;	
	clsActiveHierarchy = SGT_IDLE;

}//constructor

//------------------------------------------------------------------------------

sgtComponent::sgtComponent(): Object( OBJECT_COMPONENT ){ 
	clsTranslation = Vertex(0,0); 
	clsRotation = Vertex(0,0); 
	hasMirror = false; 
	clsDirty = false; 
	clsMin = Vertex(0,0);
	clsMax = Vertex(0,0);
	clsActiveHierarchy = SGT_IDLE;
}//constructor

//------------------------------------------------------------------------------

void sgtComponent::setModelPt( Model * model ){ 
	clsModelIndex = model; 
	clsMin = model->getMin();
	clsMax = model->getMax();
}//set the model of the component

//------------------------------------------------------------------------------

Object* sgtComponent::contains( Vertex point ) {	
/*	switch ( clsActiveHierarchy ) {
		case SGT_IDLE: 
			return NULL;
		break;
		case SGT_ACTIVE: 
			if(	clsMax.x > point.x && clsMin.x < point.x &&
				clsMax.y > point.y && clsMin.y < point.y){ 
					return this; 
			} else { return NULL;}
		break;
		case SGT_OPEN: //*/
			cerr<<"OPEN and FIND\n\n";
			if(clsObjects.empty()){cerr<<"\nerror - empty comps\n";}
			for(int i = 0; i <  clsObjects.size(); i++){
				if(clsObjects[i]->getType() == OBJECT_COMPONENT){
					if(((sgtComponent*)clsObjects[i])->contains(point)){
						return clsObjects[i]; 
					} 
				} else {
					if(clsObjects[i]->contains(point.x, point.y)){
						return clsObjects[i]; 
					} 
				}//end if
			}//end for
			return NULL;		
/*		break;
	}//end switch */
}//return true if the component contains the click in its boudingbox

//------------------------------------------------------------------------------

bool sgtComponent::contains( const int x, const int y ) const{
	/*switch ( clsActiveHierarchy ) {
		case SGT_IDLE: 
			return false;
		break;
		case SGT_ACTIVE: //*/
			return	clsMax.x > x && clsMin.x < x &&
				clsMax.y > y && clsMin.y < y;
/*		break;
		case SGT_OPEN:
			return false;			
		break;
	}//end switch*/
}//return true if the component contains the click in its boudingbox

//------------------------------------------------------------------------------

bool sgtComponent::inn(Vertex min, Vertex max) const {
/*	switch ( clsActiveHierarchy ) {
		case SGT_IDLE: 
			return false;
		break;
		case SGT_ACTIVE: //*/
			return	min.x < clsMin.x && min.y < clsMin.y &&
				max.x > clsMax.x && max.y > clsMax.y;
/*		break;
		case SGT_OPEN: 
			return	false;
		break;
	}//end switch */
}// return true if the object is all into the selection box

//------------------------------------------------------------------------------

bool sgtComponent::intersected(Vertex min, Vertex max) const {
	return true;		
}// return true if the object is intersected by the line

//------------------------------------------------------------------------------

void sgtComponent::gimmeObjects( vector <ObjectReference> & vec ){
	clsObjects.clear();
	vector <ObjectReference> v = clsModelIndex->getContents();
	printme(v.size());
	ObjectReference obj;
	for(size_t i = 0 ; i < v.size(); i++){
		obj = ObjectReference(v[i]->clone(), v[i].getLayer());
		if(hasMirror){obj->mirroring(clsMirrorX);}
		obj->translate(clsTranslation.x, clsTranslation.y);
		vec.push_back(obj);
		clsObjects.push_back(obj.getObject());
	}//end for
}//set the vector with clones of the objects

//------------------------------------------------------------------------------

 void sgtComponent::drawEdges( const float z ) const {
	cerr<<"compdrawedg \n\n";
	
	for(int i = 0; i <  clsObjects.size(); i++){
		clsObjects[i]->drawEdges(3);
	}
	
//	clsModelIndex->drawEdgeFromModel( clsTranslation, clsRotation, clsMirrorX, hasMirror );	  
 }//draw the edges of the component from the model
 
//------------------------------------------------------------------------------

void sgtComponent::draw( const float z ) const{
	cerr<<"call draw component \n\n\n";
	/*for(int i = 0; i <  clsObjects.size(); i++){
		clsObjects[i]->draw(3);
	}//*/
	clsModelIndex->drawFillFromModel( clsTranslation, clsRotation, clsMirrorX, hasMirror ); 
	//clsModelIndex->drawEdgeFromModel( clsTranslation, clsRotation, clsMirrorX, hasMirror ); //*/																																																									clsModelIndex->drawFillFromModel( clsTranslation, clsRotation, clsMirrorX, hasMirror );  
}//draw the fill component from the model

//------------------------------------------------------------------------------

void sgtComponent::setTranslation(int x, int y){ 
	clsTranslation = Vertex(x,y); 
	clsMin.x = clsMin.x + x;
	clsMin.y = clsMin.y + y;
	clsMax.x = clsMax.x + x;
	clsMax.y = clsMax.y + y;
}//set the values to teh translatiom from model and atualize the bounding box

//------------------------------------------------------------------------------

void sgtComponent::setMirror(bool X){ 
	clsMirrorX = X; 
	if(X){
		const int y0 = -clsMin.y;
		const int y1 = -clsMax.y;
		clsMin.y = min(y0, y1);
		clsMax.y = max(y0, y1);
	}else{
		const int x0 = -clsMin.x;
		const int x1 = -clsMax.x;
		clsMin.x = min(x0, x1);
		clsMax.x = max(x0, x1);	
	}//end if
}//set the mirroring of the component

//------------------------------------------------------------------------------

void sgtComponent::drawEditMode( const int z, const int x1, const int y1) const {
	drawEdges(3.0);
	/*int x = x1; int y = y1;
	Vertex t = Vertex(x,y) + clsTranslation ;
	clsModelIndex->drawEdgeFromModel( t, clsRotation, clsMirrorX, hasMirror );//*/
}//

//------------------------------------------------------------------------------

void sgtComponent::translate(const int x, const int y) {
	clsTranslation = clsTranslation + Vertex(x,y);
	
	for(int i = 0; i <  clsObjects.size(); i++){
		clsObjects[i]->translate(x, y);	
	}//end for
	
	clsMin.x = clsMin.x + x;
	clsMin.y = clsMin.y + y;
	clsMax.x = clsMax.x + x;
	clsMax.y = clsMax.y + y;
	printme(clsTranslation);
}

//------------------------------------------------------------------------------
void sgtComponent::setHierarchy(sgtHierarchy h){ 
	clsActiveHierarchy = h; 
	
	if( clsActiveHierarchy == SGT_OPEN ){ 
		for(int i =0; i < clsObjects.size(); i++){
			((sgtComponent*) clsObjects[i])->setHierarchy(SGT_ACTIVE);
		}//end for
	}//end if
	cerr<<"hierarchy\n";
/*	if( clsActiveHierarchy == SGT_IDLE || clsActiveHierarchy == SGT_ACTIVE){ 
		if(clsObjects[0]->getType() == OBJECT_COMPONENT) {
			if(((sgtComponent*)clsObjects[0])->getHierarchyStatus() != SGT_IDLE){
				for(int i = 0; i < clsObjects.size(); i++){
					if(clsObjects[i]->getType() == OBJECT_COMPONENT)
						((sgtComponent*) clsObjects[i])->setHierarchy(SGT_IDLE);
				}//end for
			}//end if
		}//end if
	}//end if	*/
	
}//set the hierarchy of the component and its sons

//------------------------------------------------------------------------------

void sgtComponent::setMinMax(){
	clsMin = clsModelIndex->getMin() + clsTranslation;
	clsMax = clsModelIndex->getMax() + clsTranslation;
	if(	hasMirror ){
		if( clsMirrorX ){
			clsMin.y = - clsMin.y;
			clsMax.y = - clsMax.y;
		} else {
			clsMin.x = - clsMin.x;
			clsMax.x = - clsMax.x;		
		}//end else
	}//end if
}//end mthod

//------------------------------------------------------------------------------


//------------------------------------------------------------------------------



