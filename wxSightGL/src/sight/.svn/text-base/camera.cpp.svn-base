#include "camera.h"
#include "../util/util.h"

Camera::Camera() {
	clsZoom = 3;
	clsInitialZoom = 3;
} // end constructor

// -----------------------------------------------------------------------------

GLvoid Camera::setCameraPosition( const GLfloat cameraCenterX, const GLfloat cameraCenterY ) {
	clsCameraCenterX = cameraCenterX;
	clsCameraCenterY = cameraCenterY;
	clsInitialCameraCenterX = cameraCenterX;
	clsInitialCameraCenterY = cameraCenterY;
} // set the position with a new center

// -----------------------------------------------------------------------------

GLvoid Camera::setViewportDimension( const GLint viewportWidth, const GLint viewportHeight ) {
	clsViewportWidth = viewportWidth;
	clsViewportHeight = viewportHeight;
	
	applyChanges();
} //set the dimensions of the viewport

// -----------------------------------------------------------------------------

GLvoid Camera::applyChanges() {
	const GLfloat windownWidth  = clsViewportWidth  / clsZoom;
	const GLfloat windownHeight = clsViewportHeight / clsZoom;
	
	const GLfloat halfWindownWidth = windownWidth / 2.0f;
	const GLfloat halfWindownHeight = windownHeight / 2.0f;
	
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();

	glViewport( 0, 0, clsViewportWidth, clsViewportHeight );

	glOrtho( 
		clsCameraCenterX - halfWindownWidth, clsCameraCenterX + halfWindownWidth,
		clsCameraCenterY - halfWindownHeight, clsCameraCenterY + halfWindownHeight, -3, 0 );

	glMatrixMode( GL_MODELVIEW );
		
} // apply the geometric transformations

// -----------------------------------------------------------------------------

GLfloat Camera::xScreenToCamera( const GLfloat screenX ) {
	return ( screenX / clsZoom ) + clsCameraCenterX -
		( ( clsViewportWidth / clsZoom ) / 2.0f );
} // convert a screen coordinate to camera coordinate

// -----------------------------------------------------------------------------

GLfloat Camera::distScreenToCamera( const int screenDX ) {
	return ( screenDX / clsZoom );
} // discretize the screen coordinate into a camera coordinate

// -----------------------------------------------------------------------------

int Camera::distCameraToScreen( const GLfloat cameraDX ) {
	return (int) round( cameraDX * clsZoom );
} // discretize the camera coordinate into a screen coordinate

// -----------------------------------------------------------------------------

GLfloat Camera::yScreenToCamera( const GLfloat screenY ) {
	return ( ( clsViewportHeight - screenY ) / clsZoom ) + clsCameraCenterY - 
		( ( clsViewportHeight / clsZoom ) / 2.0f );
} // convert the y from screen to a camera y

//______________________________________________________________________________

GLint Camera::yCameraToScreen( const GLfloat spaceY ) {
	return (int) round(clsZoom * ( spaceY - clsCameraCenterY) + clsViewportHeight/2.0f); 
} // end method

//______________________________________________________________________________

GLint Camera::xCameraToScreen( const GLfloat spaceX ) {
	return (int) round(clsZoom * ( spaceX - clsCameraCenterX) + clsViewportWidth/2.0f); 
} // end method

// -----------------------------------------------------------------------------

GLvoid Camera::toUp( const GLfloat percentage ) {
	clsCameraCenterY += ( clsViewportHeight / clsZoom ) * percentage;
	applyChanges();
} // increase the center y to move to up in y axis

// -----------------------------------------------------------------------------

GLvoid Camera::toDown ( const GLfloat percentage ) {
	clsCameraCenterY -= ( clsViewportWidth / clsZoom ) * percentage;
	applyChanges();
} // decrease the center y to move to down in y axis

// -----------------------------------------------------------------------------

GLvoid Camera::toLeft ( const GLfloat percentage ) {
	clsCameraCenterX -=( clsViewportHeight / clsZoom ) * percentage;
	applyChanges();
} //decrease the center x to move to left

// -----------------------------------------------------------------------------

GLvoid Camera::toRight( const GLfloat percentage ) {
	clsCameraCenterX +=( clsViewportWidth / clsZoom ) * percentage;
	applyChanges();
} // increase the center x to move to right

//______________________________________________________________________________

GLvoid Camera::mouseZoomIn( const float mouseWidth, const float mouseHeight, const float newCenterX, const float newCenterY ){
	clsCameraCenterX = newCenterX;
	clsCameraCenterY = newCenterY;
	
	float percentage;
	if(mouseWidth > mouseHeight) { percentage =(float) (mouseWidth * clsZoom/ clsViewportWidth); }	
	else {  percentage = (float) (mouseHeight * clsZoom / clsViewportHeight); }
	
	clsZoom =  1.0f / percentage * clsZoom;

	applyChanges();
}//change the viewport dimensions

//______________________________________________________________________________

GLvoid Camera::mouseZoomOut( const float mouseWidth, const float mouseHeight, const float newCenterX, const float newCenterY){
	clsCameraCenterX = newCenterX;
	clsCameraCenterY = newCenterY;
	
	float percentage;
	if(mouseWidth > mouseHeight) { percentage =(float) (mouseWidth * clsZoom/ clsViewportWidth); }	
	else {  percentage = (float) (mouseHeight * clsZoom / clsViewportHeight); }
	
	clsZoom =  1.0f * percentage * clsZoom;
	zoomOut(1.0f);
	applyChanges();
}//change the viewport dimensions 

//______________________________________________________________________________

GLvoid Camera::mouseMove( const float mouseX, const float mouseY){
		clsCameraCenterX -= mouseX;
		clsCameraCenterY -= mouseY;
		applyChanges();
}//end method

//______________________________________________________________________________

GLvoid Camera::resetZoomAndPosition( float W, float H ){
	clsCameraCenterX = clsInitialCameraCenterX;
	clsCameraCenterY = clsInitialCameraCenterY;
	
	float percentage;
	W = ceil(W) * 1.1f;
	H = ceil(H) * 1.1f;
	
	if(W > H) { percentage =(float) (W * clsInitialZoom /  clsViewportWidth); }
	else {  percentage = (float) (H * clsInitialZoom / clsViewportHeight); }
	
	clsZoom = 1.0f / percentage * clsInitialZoom;
	
	applyChanges();
}//end method

//______________________________________________________________________________

GLfloat Camera::getBeginGridX(){
	const GLfloat windownWidth  = clsViewportWidth  / clsZoom;
	return clsCameraCenterX - windownWidth/2.0f;
 }//end function

//______________________________________________________________________________

GLfloat Camera::getBeginGridY(){
	const GLfloat windownHeight  = clsViewportHeight  / clsZoom;
	return clsCameraCenterY - windownHeight/2.0f;
}//end function

//______________________________________________________________________________

GLfloat Camera::getEndGridX(){
	const GLfloat windownWidth  = clsViewportWidth  / clsZoom;
	return clsCameraCenterX + windownWidth/2.0f;
 }//end function

//______________________________________________________________________________

GLfloat Camera::getEndGridY(){
	const GLfloat windownHeight  = clsViewportHeight  / clsZoom;
	return clsCameraCenterY + windownHeight/2.0f;
}//end function









