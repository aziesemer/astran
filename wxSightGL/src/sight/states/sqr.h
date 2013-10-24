#ifndef SIGHT_STATE_RECTANGLE
#define SIGHT_STATE_RECTANGLE

#include "state.h"

class SightRectangleState : public SightState {
	private:
		Vertex clsBeginPoint, clsEndPoint;
		
	public:
	
		SightRectangleState( SightGL &sight ) : SightState( sight ) {}
		virtual ~SightRectangleState() {}
	
		virtual void onTouch( const Vertex cursorPos, const bool stop = false );
		virtual void onCursorMove( const Vertex cursorPos );
		virtual void onReset( const bool clearScreen );

		virtual void render( const bool flush = false );		
}; // end class


#endif
