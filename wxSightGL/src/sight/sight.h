/*
 *  sight.h
 *  SightGL
 *
 *  Created by Lenna on 7/17/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIGHT_H
#define SIGHT_H

#include <string>
	using std::string;
#include <iostream>
	using std::cerr;

#include "./states/state.h"
#include "./states/select.h"
#include "./states/line.h"
#include "./states/wire.h"
#include "./states/sqr.h"
#include "./states/rulerst.h"
#include "./states/drag.h"
#include "./states/lbl.h"

#include "camera.h"
#include "repository.h"
#include "io_files.h"
#include "opengl.h"

#include "fileformats/gds.h"

void openFile( const string fileName );
void zoomL(const bool down, const int xScreen, const int yScreen);
void finalise(const int x, const int y);

void restartDraw();

enum SightExcept { SIGHT_NONE, SIGHT_ZOOM, SIGHT_MOVE };

enum SightCursorDirection { SIGHT_FREE, SIGHT_DIAGONAL, SIGHT_ORTHOGONAL };

class SightGL {
	private:

		Vertex cameraMin;	
		Vertex cameraMax;
// -----------------------------------------------------------------------------
		// Window Dimensions
		int clsWindowWidth;
		int clsWindowHeight;
// -----------------------------------------------------------------------------	
		// Cursor position is in discretised space coordinate system.
		Vertex clsPreviousCursorPos, clsCursorPos;		
		Vertex clsPrevTouch;		
		SightExcept clsException;
		SightStateName clsCurrentStateName;
		vector< SightState * > clsStates;
// -----------------------------------------------------------------------------
		Color clsColorGuide;
// -----------------------------------------------------------------------------
		float clsCurrentLayerZ;
// -----------------------------------------------------------------------------
		inline void drawObject( const Object *obj ) const;
		inline void drawObjectEdge( const Object *obj ) const;
// -----------------------------------------------------------------------------
		string clsCurrentLabel;
// -----------------------------------------------------------------------------

		void (*clsSwapBuffersFunction)( void *data );
		void *clsSwapBuffersFunctionData;
		
	protected:
	
// -----------------------------------------------------------------------------
		void window_pos( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
		void copyPixels( GLint x, GLint y, GLint width, GLint height, GLenum sourceBuffer, GLenum targetBuffer );
// -----------------------------------------------------------------------------
		void doMove(const int direction);
// -----------------------------------------------------------------------------
		void renderGrid();
		void renderAxes();
// -----------------------------------------------------------------------------
		int discretise( const float f, const int step);
// -----------------------------------------------------------------------------
		int scale;
		int stepScale;
		Camera camera;
		Repository repository;
		File clsFile;
		Object * selectedObject;
		Object * previousSelectedObject;
		Ruler * selectedRuler;
		Ruler * previousRuler;
		int   clsCurrentLayer;
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------	
	
	
	public:
	
		void setSwapBuffersFunction( void (*f)(void *data) ) { clsSwapBuffersFunction = f; }
		void setSwapBuffersFunctionData( void *data ) { clsSwapBuffersFunctionData = data; }



// -----------------------------------------------------------------------------		
		void setup();
		void restartAll();
		void handleReshape( int width, int height );
		void reshapeWindow( const int width, const int height );
		void refreshScreen();
// -----------------------------------------------------------------------------
	//io methods ->files
		void newFile();
		void loadCIF( const char * const filename );
		void loadEDFII( const char * const filename );
		void loadGDSII( const char* const filename );
		void saveFile( const char * const filename);
// -----------------------------------------------------------------------------		
	//others
		int getScale() const { return scale; }
		int getZ() const { return maximo; }
		void restartDraw();
		void deleteSelectedObject();
// -----------------------------------------------------------------------------
	//coisas q talvez possivelmente n devessem estar aqui...
		void mouseMove( const float x, const float y);
		void addObject( Object * newObj);
		void addLabel( Object * newObj);
		
		void drawCurrentLayer();
		void addRuler(Ruler ruler);
		bool findSelectedBoundingBox(Vertex min, Vertex max);
		void drawSelectedBoundingBox(const int maximo);	
		Object * objectAtPosition( Vertex pos );
		Ruler * rulerAtPosition(Vertex pos);
		bool existsSelection();		
		
		void drawLabelLayer();		
		void setLabel(string label);
		string getCurrentLabel();
// -----------------------------------------------------------------------------
		bool isClickInSelection();
		void moveSelection(Vertex click);
		void moveObjects(Vertex click);
		void move();
// -----------------------------------------------------------------------------		
		void handleKeyEvent(unsigned char key, bool ctrl);	
// -----------------------------------------------------------------------------
	//layers methods
		string getLayerName( const int index ) ;
		void setLayerVisibility( const int layer, const bool visible );		
		int getNumLayers() { return repository.getNumLayers(); }
		int getCurrentLayer() const { return clsCurrentLayer; }				
		void setCurrentLayer( const int layer ) { clsCurrentLayer = layer; }	
		StippleMask getLayerMask( const int index )  ;	
		Color getLayerFillColor(const int index);
		Color getLayerLineColor(const int index);
// -----------------------------------------------------------------------------
	   void changeBlackgroundColor(const float r, const float b, const float g);
		void changeGuideColor(const float r, const float b, const float g);
		void applyColorGuide();
// -----------------------------------------------------------------------------
	//edity methods
		void copy();
		void cut();
		void paste();
		void deleteObj();
		void clearRulers();
		void mirror(bool Xmirror);
// -----------------------------------------------------------------------------
		void group();
		void ungroup();
// -----------------------------------------------------------------------------
	//visualization methods
		void renderBlendingBox(Vertex first, Vertex last);
		
		void zoom(const bool up);
		void fitIn( );	
		bool zoomIn();
		
		float getZoom(){ return camera.getZoom(); }
		
		void beginZooming(){ clsException = SIGHT_ZOOM; clsPrevTouch = clsCursorPos; }
		void beginDrag()   { clsException = SIGHT_MOVE; clsPrevTouch = clsCursorPos; }
		void end(){ clsException = SIGHT_NONE; }
		
		void connection(const int ID);
// -----------------------------------------------------------------------------
		unsigned char* getPixels() const;
// -----------------------------------------------------------------------------
		int getWidth() { return clsWindowWidth;  }
		int getHeight(){ return clsWindowHeight; }

// -----------------------------------------------------------------------------
		void toUp();
		void toDown();
		void toLeft();
		void toRight();
		
// -----------------------------------------------------------------------------
	//hierarchy
		void hierarchy(bool down);
		
		int getNumModels();
		string getModelName(int i);
		void setCurrentModel(string name);
		
		bool createNewComponent(const char* name);
		void createComponent();
		bool createNewModel(const char* name);
		void saveModel();
		void saveModelAs(const char* name);
		
		bool dirtyModel(){return repository.dirty();}
// -----------------------------------------------------------------------------


		// =========================================================================
		// New stuff
		// =========================================================================

		SightGL();

		// State.
		void setState( const SightStateName sightState );
		void resetState();

		// Touches.
		bool touch( const bool stop = false );
		
		// Cursor.
		void moveCursor( const int xScreen, const int yScreen );
		void displaceCursor( const int dxScreen, const int dyScreen );

		// Gets.
		SightStateName getCurrentStateName() const { return clsCurrentStateName; }
		SightStateStatus getCurrentStateStatus() const { return clsStates[ clsCurrentStateName ]->getStatus(); }

		// Render.
		void render();

		void restoreScreen();
		void saveScreen();
		void restoreScreen( const int xSpace, const int ySpace, const int wSpace, const int hSpace );
		void saveScreen( const int xSpace, const int ySpace, const int wSpace, const int hSpace );
		
}; // end class

// -----------------------------------------------------------------------------

void SightGL::drawObject( const Object *obj ) const {
	switch ( obj->getType() ) {
		case OBJECT_RECTANGLE: (*( SightRectangle * )( obj )).draw( clsCurrentLayerZ );	break;
		case OBJECT_WIRE:      (*( SightWire      * )( obj )).draw( clsCurrentLayerZ ); break;
		case OBJECT_LINE:      (*( SightLine      * )( obj )).draw( clsCurrentLayerZ ); break;
		case OBJECT_LABEL:     (*( SightLabel     * )( obj )).draw( clsCurrentLayerZ ); break;
		default: break;
	} // end switch
} // end method

// -----------------------------------------------------------------------------

void SightGL::drawObjectEdge( const Object *obj ) const {
	switch ( obj->getType() ) {
		case OBJECT_RECTANGLE: (*( SightRectangle * )( obj )).drawEdges( clsCurrentLayerZ ); break;
		case OBJECT_WIRE:      (*( SightWire      * )( obj )).drawEdges( clsCurrentLayerZ ); break;
		case OBJECT_LINE:      (*( SightLine      * )( obj )).drawEdges( clsCurrentLayerZ ); break;
		case OBJECT_LABEL:     (*( SightLabel     * )( obj )).drawEdges( clsCurrentLayerZ ); break;
		default: break;
	} // end switch
} // end method


#endif