#include "sqr.h"

// -----------------------------------------------------------------------------

void SightRectangleState::onTouch( const Vertex cursorPos, const bool stop ) {
	switch ( clsStatus ) {
		case SIGHT_IDLE:
			clsStatus = SIGHT_READY;
			clsBeginPoint = cursorPos;
			break;
		case SIGHT_READY:
			{
			clsStatus = SIGHT_IDLE;
			clsEndPoint = cursorPos;
			clsSight.restoreScreen(); 
			
			if ( clsBeginPoint.x != clsEndPoint.x && clsBeginPoint.y != clsEndPoint.y) { // avoid to create a rectangle with area equals to 0
				SightRectangle * newRectangle = new SightRectangle( clsBeginPoint, clsEndPoint );
				clsSight.addObject( newRectangle );
				
				clsSight.drawCurrentLayer();
			
				glFlush();
				clsSight.saveScreen();
			} // end if

			break;
		}
		default: break;
	} // end switch
} // end method

// -----------------------------------------------------------------------------

void SightRectangleState::onCursorMove( const Vertex cursorPos ) {
	if ( clsStatus == SIGHT_READY ) {
		clsEndPoint = cursorPos;
		render( true );
	} // end if
} // end method

// -----------------------------------------------------------------------------

void SightRectangleState::render( const bool flush ) {
	if ( clsStatus == SIGHT_IDLE ) return;
	
	clsSight.restoreScreen();
	
	glDisable(GL_DEPTH_TEST);
	
	clsSight.applyColorGuide();
	
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glBegin( GL_QUADS );
		glVertex3f( clsBeginPoint.x, clsBeginPoint.y, maximo );
		glVertex3f( clsEndPoint.x, clsBeginPoint.y, maximo );
		glVertex3f( clsEndPoint.x, clsEndPoint.y, maximo );
		glVertex3f( clsBeginPoint.x, clsEndPoint.y, maximo );
	glEnd();	
	
	glEnable(GL_DEPTH_TEST);			
	
	if ( flush )
		glFlush();
} // end method

// -----------------------------------------------------------------------------

void SightRectangleState::onReset( const bool clearScreen ) {
	clsStatus = SIGHT_IDLE;
	if ( clearScreen )
		clsSight.restoreScreen();
} // end method
