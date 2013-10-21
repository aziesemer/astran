
#include "rulerst.h"


// -----------------------------------------------------------------------------

void SightRulerState::onTouch( const Vertex cursorPos, const bool stop ) {
	switch ( clsStatus ) {
		case SIGHT_IDLE:
			clsStatus = SIGHT_READY;
			clsBeginPoint = cursorPos;
			break;
		case SIGHT_READY:{
			clsStatus = SIGHT_IDLE;
			clsEndPoint = cursorPos;
			
			Ruler ruler = Ruler(clsBeginPoint, clsEndPoint);
			ruler.lock();
			clsSight.addRuler(ruler);

			clsSight.saveScreen();
			break;
		}
		default: break;
	} // end switch
} // end method

// -----------------------------------------------------------------------------

void SightRulerState::onCursorMove( const Vertex cursorPos ) {
	switch (clsStatus){
		case SIGHT_READY :
			clsEndPoint = cursorPos;
			render( true );
		break;
		case SIGHT_IDLE :
			previousRuler = selectedRuler;
			selectedRuler = clsSight.rulerAtPosition(cursorPos);
			if(selectedRuler != NULL){
				if(selectedRuler != previousRuler){
					clsSight.restoreScreen();
				}//end if
				selectedRuler->drawBB(clsSight.getZ());
			}else{
				clsSight.restoreScreen();				
			}//end else			
		break;
		case SIGHT_RUNNING :
		
		break;
	} //end switch
} // end method

// -----------------------------------------------------------------------------

void SightRulerState::render( const bool flush ) {
	if ( clsStatus == SIGHT_IDLE ) return;
	
	clsSight.restoreScreen();
	
	glDisable(GL_DEPTH_TEST);

	Ruler ruler = Ruler( clsBeginPoint, clsEndPoint );

	ruler.lock();
	
	ruler.drawRuler( clsSight.getZ() , clsSight.getScale(), clsSight.getZoom() );

	glEnable(GL_DEPTH_TEST);			
	
	if ( flush )
		glFlush();
} // end method

// -----------------------------------------------------------------------------

void SightRulerState::onReset( const bool clearScreen ) {
	clsStatus = SIGHT_IDLE;
	if ( clearScreen )
		clsSight.restoreScreen();
} // end method
