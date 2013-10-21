#include "wire.h"

void SightWireState::onTouch( const Vertex cursorPos, const bool stop ) {
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
	
				SightWire *newWire = new SightWire( clsPoints );
				clsSight.addObject( newWire );
				newWire->setThickness( 50 );
				
				clsSight.restoreScreen();
				clsSight.drawCurrentLayer(); //[TODO] desenhar soh o objeto adicionado
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
		default:break;
	} // end switch
} // end method

// -----------------------------------------------------------------------------

void SightWireState::onCursorMove( const Vertex cursorPos ) {
	if ( clsStatus == SIGHT_RUNNING ) {
		clsPoints.back() = cursorPos; // save the current cursor position at the last vector element
		render( true );
	} // end else
} // end method

// -----------------------------------------------------------------------------

void SightWireState::render( const bool flush ) {
	if ( clsStatus == SIGHT_IDLE ) return;
	
	clsSight.restoreScreen();
	
	glDisable(GL_DEPTH_TEST);
	
	glColor3f( 1, 1, 1 );
	SightWire newWire = SightWire( clsPoints ); // [TODO] Nao fica criando a cada instante um novo fio. Deixa um sempre criado, e soh o atualiza.
	newWire.setThickness( 50 );
	newWire.drawEdges( 3.0f );
		
	if ( flush )
		glFlush();
		
	glEnable(GL_DEPTH_TEST);	
} // end method

// -----------------------------------------------------------------------------

void SightWireState::onReset( const bool clearScreen ) {
	clsStatus = SIGHT_IDLE;
	clsPoints.clear();

	if ( clearScreen )
		clsSight.restoreScreen();
} // end method
