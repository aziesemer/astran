#ifndef SIGHT_STATE_SELECT
#define SIGHT_STATE_SELECT

#include "state.h"

class SightSelectState : public SightState {
	private:
		Vertex clsBeginPoint, clsEndPoint;
		
		Object * previousSelectedObject;
		Object * selectedObject;
		
	public:
	
		SightSelectState( SightGL &sight ) : SightState( sight ) {
			selectedObject = NULL;
			previousSelectedObject = NULL;
		
		}
		virtual ~SightSelectState() {}
	
		virtual void onTouch( const Vertex cursorPos, const bool stop = false );
		virtual void onCursorMove( const Vertex cursorPos );
		virtual void onReset( const bool clearScreen );

		virtual void render( const bool flush = false );	
		
		void renderSelectedObject();
		void deselectObject();
			
}; // end class


#endif