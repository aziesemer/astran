#ifndef SIGHT_STATE_LINE
#define SIGHT_STATE_LINE

#include <vector>
	using std::vector;

#include "state.h"

class SightLineState : public SightState {
	private:
		vector< Vertex > clsPoints;
		
	public:
	
		SightLineState( SightGL &sight ) : SightState( sight ) {}
		virtual ~SightLineState() {}
			
		virtual void onTouch( const Vertex cursorPos, const bool stop = false );
		virtual void onCursorMove( const Vertex cursorPos );
		virtual void onReset( const bool clearScreen );

		virtual void render( const bool flush = false );			
}; // end class

#endif