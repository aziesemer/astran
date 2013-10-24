#ifndef SIGHT_STATE_WIRE
#define SIGHT_STATE_WIRE

#include <vector>
	using std::vector;

#include "state.h"

class SightWireState : public SightState {
	private:
		vector< Vertex > clsPoints;
		
	public:
	
		SightWireState( SightGL &sight ) : SightState( sight ) {}
		virtual ~SightWireState() {}
			
		virtual void onTouch( const Vertex cursorPos, const bool stop = false );
		virtual void onCursorMove( const Vertex cursorPos );
		virtual void onReset( const bool clearScreen );

		virtual void render( const bool flush = false );			
}; // end class


#endif
