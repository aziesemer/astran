#ifndef SIGHT_STATE_DRAG
#define SIGHT_STATE_DRAG

#include "state.h"

class SightDragState : public SightState {
	private:
		Vertex clsBeginPoint, clsEndPoint;
		
	public:
	
		SightDragState( SightGL &sight ) : SightState( sight ) {}
		virtual ~SightDragState() {}
	
		virtual void onTouch( const Vertex cursorPos, const bool stop = false );
		virtual void onCursorMove( const Vertex cursorPos );
		virtual void onReset( const bool clearScreen );

		virtual void render( const bool flush = false );	

}; // end class


#endif