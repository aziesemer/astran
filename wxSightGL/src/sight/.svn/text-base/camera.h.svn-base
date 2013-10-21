#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
	using std::cout;
#include <math.h>
#include "opengl.h"

const int maximo = 3;//z

class Camera {
	private:
		GLint clsViewportHeight;
		GLint clsViewportWidth;
						
		GLfloat clsCameraCenterX;
		GLfloat clsCameraCenterY;
		GLfloat clsInitialCameraCenterX;
		GLfloat clsInitialCameraCenterY;

		GLfloat clsZoom;
		GLfloat clsInitialZoom;
		GLvoid applyChanges();


	public:
	
		Camera();
		
		//retur the initial point to start to draw the guide grid
		GLfloat getBeginGridX();
		GLfloat getBeginGridY();
		GLfloat getEndGridX();//precisamos disso????
		GLfloat getEndGridY();
		
		GLint getViewportHeight(){ return clsViewportHeight; }
		GLint getViewportWidth() { return clsViewportWidth; }
		
		GLfloat getWindowWidth(){ return clsViewportWidth / clsZoom; }
		GLfloat getWindowHeight(){ return clsViewportHeight / clsZoom; }
		
		GLfloat getCenterX() { return clsCameraCenterX; }
		GLfloat getCenterY() { return clsCameraCenterY; }

		GLvoid setViewportDimension( const GLint viewportWidth, const GLint viewportHeight );
		GLvoid setCameraPosition( const GLfloat cameraCenterX, const GLfloat cameraCenterY );
		GLvoid resetZoomAndPosition( float W, float H);
		
		GLvoid setZoom( const GLfloat zoom ) { clsZoom = zoom; clsInitialZoom = zoom; applyChanges(); }
		GLfloat getZoom() const { return clsZoom; }
		GLvoid setInitialZoom(const GLfloat zoom ) { clsInitialZoom = zoom; }
		
		GLvoid setInitialCameraCenterX( const GLfloat posx ){ clsInitialCameraCenterX = posx; }
		GLvoid setInitialCameraCenterY( const GLfloat posy ){ clsInitialCameraCenterY = posy; }
		
		GLvoid zoomOut( const GLfloat percentage) { clsZoom *= 1.0f - percentage; applyChanges(); }
		GLvoid zoomIn ( const GLfloat percentage) { clsZoom /= 1.0f - percentage; applyChanges(); }
		
		GLvoid mouseZoomIn( const float mouseWidth, const float mouseHeight, const float newCenterX, const float newCenterY );
		GLvoid mouseZoomOut( const float mouseWidth, const float mouseHeight, const float newCenterX, const float newCenterY );
		
		GLvoid toUp   ( const GLfloat percentage );
		GLvoid toDown ( const GLfloat percentage );
		GLvoid toLeft ( const GLfloat percentage );
		GLvoid toRight( const GLfloat percentage );

		GLfloat xScreenToCamera( const GLfloat screenX );
		GLfloat yScreenToCamera( const GLfloat screenY );
		GLfloat distScreenToCamera( const int screenDx ); // convert distance (in pixel) from the screen to the space coordinate system
		int     distCameraToScreen( const GLfloat cameraDx );
		
		GLint yCameraToScreen( const GLfloat spaceY );
		GLint xCameraToScreen( const GLfloat spaceX );		
		
		GLvoid mouseMove( const float mouseX, const float mouseY);

		
}; // end class

#endif