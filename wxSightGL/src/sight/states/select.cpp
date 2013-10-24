#include "select.h"

// -----------------------------------------------------------------------------

void SightSelectState::onTouch( const Vertex cursorPos, const bool stop ) {
	switch ( clsStatus ) {
		case SIGHT_IDLE:
			clsStatus = SIGHT_READY;
			clsBeginPoint = cursorPos;
			if( clsSight.isClickInSelection() ){
				clsStatus = SIGHT_RUNNING;
			}
		break;
		case SIGHT_READY:			
			clsStatus = SIGHT_IDLE;
			clsEndPoint = cursorPos;
			clsSight.findSelectedBoundingBox(clsBeginPoint, clsEndPoint);
			clsSight.restoreScreen();
			clsSight.drawSelectedBoundingBox(maximo);
		break;
		case SIGHT_RUNNING:
			clsStatus = SIGHT_IDLE;
			clsEndPoint = cursorPos;
			clsSight.moveObjects(clsBeginPoint);
		break;
	} // end switch
} // end method

// -----------------------------------------------------------------------------

void SightSelectState::renderSelectedObject() {
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glLineStipple( 1, 0xf0f0 );
	glEnable( GL_LINE_STIPPLE );
	glLineWidth(2.0f);
	glColor3ub( 255, 255, 0 );
	selectedObject->drawEdges( maximo );
	glDisable( GL_LINE_STIPPLE );
	glLineWidth(1.0f);	
	glFlush();	
} // end function

// -----------------------------------------------------------------------------

void SightSelectState::deselectObject(){
	if( previousSelectedObject != NULL ){
		clsSight.restoreScreen();
	}//end if	
}//end function

// -----------------------------------------------------------------------------

void SightSelectState::onCursorMove( const Vertex cursorPos ) {
	cerr<<"cursor move\n\n";
	switch (clsStatus){
		case SIGHT_READY :
			clsEndPoint = cursorPos;
			render( true );
		break;
		case SIGHT_IDLE :
			if ( !clsSight.existsSelection() ){
				if ( previousSelectedObject != selectedObject){ 	
					deselectObject();	
					previousSelectedObject = selectedObject;
					if( selectedObject != NULL ){
						renderSelectedObject();	
						//sqrStp = false;			
					} 
				}		
			}
			selectedObject = clsSight.objectAtPosition( cursorPos );
		break;
		case SIGHT_RUNNING:
			printme("running");
			clsSight.moveSelection(clsBeginPoint);
		break;
	} // end switch
} // end method

// -----------------------------------------------------------------------------

void SightSelectState::render( const bool flush ) {
	if ( clsStatus == SIGHT_IDLE ) return;
	
	clsSight.renderBlendingBox(clsBeginPoint, clsEndPoint);
	
} // end method

// -----------------------------------------------------------------------------

void SightSelectState::onReset( const bool clearScreen ) {
	clsStatus = SIGHT_IDLE;
	if ( clearScreen )
		clsSight.restoreScreen();
} // end method

