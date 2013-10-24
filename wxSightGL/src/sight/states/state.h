#ifndef SIGHT_STATE
#define SIGHT_STATE

#include "../vertex.h"
#include "../ruler.h"
#include "../object.h"

#include <iostream>
	using std::ostream;

enum SightStateName { SIGHT_SELECT, SIGHT_RECTANGLE, SIGHT_WIRE, SIGHT_RULER, SIGHT_LINE, SIGHT_DRAG, SIGHT_LABEL, SIGHT_NUM_STATES };
enum SightStateStatus { SIGHT_IDLE, SIGHT_READY, SIGHT_RUNNING };

class SightGL;
class SightState {
	protected:
	
		SightGL &clsSight;
		SightStateStatus clsStatus;	
	
	public:
	
		SightState( SightGL &sight ) : clsSight( sight ) {}
			
		virtual ~SightState() {}
	
		virtual void onTouch( const Vertex cursorPos, const bool stop = false ) = 0;
		virtual void onCursorMove( const Vertex cursorPos ) = 0;
		virtual void onReset( const bool clearScreen ) = 0;

		virtual void render( const bool flush = false ) {};
		
		// Gets.
		SightStateStatus getStatus() const { return clsStatus; }

}; // end class

#include "../sight.h" // GUILHERME FLACH: ISSO TEM QUE FICAR AQUI.

#endif