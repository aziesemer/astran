#include "line.h"


// -----------------------------------------------------------------------------

void SightLineState::onTouch( const Vertex cursorPos, const bool stop ) {
	 // REMARK. Every time the status is SIGGHT_RUNNING there are at last two 
	 // elements in the point vector. The last (back) elements refers to the
	 // current cursor position.

	switch ( clsStatus ) {
		case SIGHT_IDLE:
			clsStatus = SIGHT_RUNNING;
			clsPoints.push_back( cursorPos );
			clsPoints.push_back( cursorPos );
			break;
		case SIGHT_RUNNING:
			if ( stop ) {
				if ( clsPoints[ clsPoints.size() - 2 ] == clsPoints[ clsPoints.size() - 1 ] )
					clsPoints.pop_back();
								
				SightLine *newLine = new SightLine( clsPoints );
				clsSight.addObject( newLine );
				
				clsSight.restoreScreen();
				clsSight.drawCurrentLayer();//[TODO] desenhar soh o putatcho add

				glFlush();
				clsSight.saveScreen();
				
				onReset( false );
			} // end if
			else {
				if ( clsPoints[ clsPoints.size() - 2 ] != cursorPos ) {
					clsPoints.back() = cursorPos;
					clsPoints.push_back( cursorPos );
				} // end if
			} // end else
			break;
			default: break; 
	} // end switch
} // end method

// -----------------------------------------------------------------------------

void SightLineState::onCursorMove( const Vertex cursorPos ) {
	if ( clsStatus == SIGHT_RUNNING ) {
		clsPoints.back() = cursorPos; // save the current cursor position at the last vector element
		render( true );
	} // end else
} // end method

// -----------------------------------------------------------------------------

void SightLineState::render( const bool flush ) {
	if ( clsStatus == SIGHT_IDLE ) return;
	
	clsSight.restoreScreen();
	
	glDisable(GL_DEPTH_TEST);
	
	glColor3f( 1, 1, 1 );
	SightLine *newLine = new SightLine( clsPoints );
	newLine->draw( 3.0f );
		
	if ( flush )
		glFlush();
		
	glEnable(GL_DEPTH_TEST);	
} // end method

// -----------------------------------------------------------------------------

void SightLineState::onReset( const bool clearScreen ) {
	clsStatus = SIGHT_IDLE;
	clsPoints.clear();

	if ( clearScreen )
		clsSight.restoreScreen();
} // end method
