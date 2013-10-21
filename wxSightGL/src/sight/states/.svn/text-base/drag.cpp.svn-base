
#include "drag.h"

// -----------------------------------------------------------------------------

void SightDragState::onTouch( const Vertex cursorPos, const bool stop ) {
	switch ( clsStatus ) {
		case SIGHT_IDLE:
			clsStatus = SIGHT_READY;
			clsBeginPoint = cursorPos;
			printme("click drag");
			//clsSight.beginDrag();
			break;
		case SIGHT_READY:
			clsStatus = SIGHT_IDLE;
			clsEndPoint = cursorPos;
			printme("click ready to iddle")
			//clsSight.end();
			//render(true);
		break;
		default: break;
	} // end switch
} // end method

// -----------------------------------------------------------------------------

void SightDragState::onCursorMove( const Vertex cursorPos ) {
	switch (clsStatus){
		case SIGHT_READY :
			clsEndPoint = cursorPos;
			render( true );
			printme("drga running");
		break;
		case SIGHT_IDLE :

		break;
		case SIGHT_RUNNING :
		
		break;
	} // end switch
} // end method

// -----------------------------------------------------------------------------

void SightDragState::render( const bool flush ) {
	if ( clsStatus == SIGHT_IDLE ) return;
	
	//clsSight.restoreScreen();
	
	//glDisable(GL_DEPTH_TEST);


	clsSight.mouseMove(clsEndPoint.x - clsBeginPoint.x, clsEndPoint.y - clsBeginPoint.y ); //move camera
	clsSight.render();
	//glEnable(GL_DEPTH_TEST);			
	
	if ( flush )
		glFlush();
} // end method

// -----------------------------------------------------------------------------

void SightDragState::onReset( const bool clearScreen ) {
	clsStatus = SIGHT_IDLE;
	if ( clearScreen )
		clsSight.restoreScreen();
} // end method

