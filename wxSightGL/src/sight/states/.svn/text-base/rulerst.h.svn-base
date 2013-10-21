#ifndef SIGHT_STATE_RULER
#define SIGHT_STATE_RULER

#include "state.h"

class SightRulerState : public SightState {
	private:
		Vertex clsBeginPoint, clsEndPoint;
		
		Ruler * previousRuler;
		Ruler * selectedRuler;
		
	public:
	
		SightRulerState( SightGL &sight ) : SightState( sight ) {}
		virtual ~SightRulerState() {}
	
		virtual void onTouch( const Vertex cursorPos, const bool stop = false );
		virtual void onCursorMove( const Vertex cursorPos );
		virtual void onReset( const bool clearScreen );

		virtual void render( const bool flush = false );		
}; // end class


#endif