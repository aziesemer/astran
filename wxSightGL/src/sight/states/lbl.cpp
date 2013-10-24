#include "lbl.h"

// -----------------------------------------------------------------------------

void SightLabelState::onTouch( const Vertex cursorPos, const bool stop ) {
	switch ( clsStatus ) {
		case SIGHT_IDLE:{
			cout<<"passando\n";
			clsPoint = cursorPos;
			SightLabel * newLabel = new SightLabel(clsLabel, clsPoint.x, clsPoint.y);			
			clsSight.addLabel( newLabel );
			clsSight.drawLabelLayer();
			glFlush();
			clsSight.saveScreen();
		} break;
		case SIGHT_READY:			
		break;
		default: break;
	} // end switch
} // end method

// -----------------------------------------------------------------------------

void SightLabelState::onCursorMove( const Vertex cursorPos ) {
	switch (clsStatus){
		case SIGHT_READY :

		break;
		case SIGHT_IDLE :

		break;
		case SIGHT_RUNNING:

		break;
	} // end switch
} // end method

// -----------------------------------------------------------------------------

void SightLabelState::render( const bool flush ) {
	
} // end method

// -----------------------------------------------------------------------------

void SightLabelState::onReset( const bool clearScreen ) {
	clsStatus = SIGHT_IDLE;	
	clsLabel = clsSight.getCurrentLabel();
	
	if ( clearScreen )
		clsSight.restoreScreen();
} // end method
