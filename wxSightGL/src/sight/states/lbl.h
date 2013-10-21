#ifndef SIGHT_STATE_LABEL
#define SIGHT_STATE_LABEL

#include "state.h"

class SightLabelState : public SightState {
	private:
		Vertex clsPoint;
		string clsLabel;
		
	public:
	
		SightLabelState( SightGL &sight ) : SightState( sight ) {}
		virtual ~SightLabelState() {}
	
		virtual void onTouch( const Vertex cursorPos, const bool stop = false );
		virtual void onCursorMove( const Vertex cursorPos );
		virtual void onReset( const bool clearScreen );

		virtual void render( const bool flush = false );	
			
}; // end class


#endif

