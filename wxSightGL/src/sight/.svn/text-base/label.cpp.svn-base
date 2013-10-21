#include "label.h"

//---------------------------------------------------------------------------------------------------------------

SightLabel::SightLabel(string text, const int minX, const int minY) : Object( OBJECT_LABEL ) { 
	clsText = text; 
	const unsigned char * txt = (const unsigned char *)clsText.c_str();
	const int size = glutBitmapLength(GLUT_BITMAP_HELVETICA_10, txt);	
	clsMin = Vertex(minX, minY);
	clsMax = Vertex(clsMin.x + size, clsMin.y + 10);
}//end constructor

//---------------------------------------------------------------------------------------------------------------

void SightLabel::drawEditMode( const int z, const int x, const int y) const {
	const char * txt = clsText.c_str();
					 
	const int e = 	clsMin.x + x;
	const int u = 	clsMin.y + y;		 

	glEnd();
													 								 				 
	glRasterPos2f(e, u); // Universe text position

	GLboolean validd;
	
	glGetBooleanv( GL_CURRENT_RASTER_POSITION_VALID, &validd);
	if( !validd ){ cout<<"RASTER PANIC*****************\n"; }

	glDisable(GL_DEPTH_TEST);
	while(*txt)// show char by char
		 glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*txt++); 
	glEnable(GL_DEPTH_TEST);
}//draw the label to indicate editable mode

//---------------------------------------------------------------------------------------------------------------

void SightLabel::drawEdges( const float z ) const {
	const  char * txt = clsText.c_str();
		          
	glRasterPos2f(clsMin.x,clsMin.y); // Universe text position

	glDisable(GL_DEPTH_TEST);
	while(*txt)// show char by char
		 glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*txt++); 
	glEnable(GL_DEPTH_TEST);	
}// virtual class compatibility

//---------------------------------------------------------------------------------------------------------------

void SightLabel::draw(const float z) const {	
	const char * txt = clsText.c_str();
		          
	glRasterPos2f(clsMin.x,clsMin.y); // Universe text position

	glDisable(GL_DEPTH_TEST);
	while(*txt)// Show char by char
		 glutBitmapCharacter(GLUT_BITMAP_HELVETICA_10,*txt++); 
	glEnable(GL_DEPTH_TEST);
}// draw the label

//---------------------------------------------------------------------------------------------------------------

void SightLabel::printMe(void * font){

	const char * txt = clsText.c_str();

	glRasterPos2f(clsMin.x,clsMin.y);
	
	glDisable(GL_DEPTH_TEST);
	while(*txt)
		 glutBitmapCharacter(font,*txt++); 
	glEnable(GL_DEPTH_TEST);
}//unused?

//---------------------------------------------------------------------------------------------------------------

void SightLabel::translate(const int x, const int y) {
	clsMin.x = clsMin.x + x;
	clsMin.y = clsMin.y + y;
	clsMax = Vertex(clsMax.x + x, clsMax.y + y) ;
}//translate the point where we draw the label

//---------------------------------------------------------------------------------------------------------------

void SightLabel::mirroring(const bool inX) {
	if( inX ){	
		clsMin.y = -clsMin.y;
		clsMax.y = clsMin.y;
	}else{
		clsMin.x = -clsMin.x;
		clsMax.x = clsMin.x;
	}
}//only invert the values? not sure

//---------------------------------------------------------------------------------------------------------------

bool SightLabel::inn(Vertex min, Vertex max) const{
	return (min.x <= clsMin.x && max.x >= clsMax.x && min.y <= clsMin.y && max.y >= clsMax.y);
}//return true if the label start point is in the selection box

//---------------------------------------------------------------------------------------------------------------

bool SightLabel::contains( const int x, const int y ) const{
	return( clsMin.x <= x && clsMin.y <= y && clsMax.x >= x && clsMax.y >= y );
}//return true if the click on the start point

//---------------------------------------------------------------------------------------------------------------

bool SightLabel::intersected(Vertex min, Vertex max) const {
	return 
	(clsMax.y <= max.y && clsMax.y >= min.y) && (max.x <= clsMax.x && max.x >= clsMin.x) 
	||
	(clsMax.x <= max.x && clsMax.x >= min.x) && (max.y <= clsMax.y && max.y >= clsMin.y);
}//

//---------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------------------------


