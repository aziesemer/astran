#include "basicglpane.h"

BEGIN_EVENT_TABLE(BasicGLPane, wxGLCanvas)
	EVT_LEFT_DCLICK(BasicGLPane::doubleLClick)
	EVT_MOTION(BasicGLPane::mouseMoved)
	EVT_LEFT_DOWN(BasicGLPane::mouseDown)
	EVT_LEFT_UP(BasicGLPane::mouseReleased)		
	EVT_RIGHT_DOWN(BasicGLPane::rightClick)
	EVT_RIGHT_UP(BasicGLPane::rightClick)
	EVT_LEAVE_WINDOW(BasicGLPane::mouseLeftWindow)
	EVT_SIZE(BasicGLPane::resized)
	EVT_KEY_DOWN(BasicGLPane::keyPressed)
	//EVT_KEY_UP(BasicGLPane::keyReleased)
	EVT_MOUSEWHEEL(BasicGLPane::mouseWheelMoved)
	//EVT_MIDDLE_DCLICK(BasicGLPane::middleClick)
	EVT_MIDDLE_DOWN(BasicGLPane::middleDown)
	EVT_MIDDLE_UP(BasicGLPane::middleUp)
	EVT_PAINT( BasicGLPane::OnPaint )
	
	EVT_MENU(popCONNECTlayer,  BasicGLPane::connect)
	EVT_MENU(popCONNECTlayerUP,  BasicGLPane::connect)
	EVT_MENU(popCONNECTlayerDOWN,  BasicGLPane::connect)
	EVT_MENU(popCONNECTlayerUD,  BasicGLPane::connect)
	
	EVT_MENU(popMIRRORx, BasicGLPane::MirrorX)
	EVT_MENU(popMIRRORy, BasicGLPane::MirrorY)
END_EVENT_TABLE()

// ----------------------------------------------------------------------------- 
 
void wxGLCanvasSwapBuffers( void * data ) {
	cerr << "Swapping buffers...\n";
	( ( BasicGLPane * ) data)->SwapBuffers();
} // end function

// ----------------------------------------------------------------------------- 

BasicGLPane::BasicGLPane(wxWindow* parent, int* args) :
	wxGLCanvas(parent, wxID_ANY,  wxDefaultPosition,  wxDefaultSize, 0, wxT("GLCanvas"),  args)
{
	doubleFlag = false;
	SetMinSize(wxSize(300,300));
		
	sight.setup();
	sight.setSwapBuffersFunction( wxGLCanvasSwapBuffers );
	sight.setSwapBuffersFunctionData( this );
	
	createPopUp();
	
} // end method

// ----------------------------------------------------------------------------- 

void BasicGLPane::createSubMenuConnect(){
	clsConnect = new wxMenu(wxEmptyString, 0);
	clsConnect->Append(popCONNECTlayer, _("This layer"), wxEmptyString );
	clsConnect->Append(popCONNECTlayerUP, _("This & Up"), wxEmptyString );
	clsConnect->Append(popCONNECTlayerDOWN, _("This & Down"), wxEmptyString );
	clsConnect->Append(popCONNECTlayerUD, _("This, Up & Down"), wxEmptyString );
}

// ----------------------------------------------------------------------------- 
void BasicGLPane::createPopUp(){
	clsPopUp = new wxMenu(wxT("SightGL"), 0);
	//clsPopUp->Append(popCONNECTION, _("Connection"), _("encontra conex�es"));
	createSubMenuConnect();
	clsPopUp->AppendSubMenu(clsConnect, _("Connection"));
	clsPopUp->AppendSeparator( );
	clsPopUp->Append(popMIRRORx, _("Mirror X"), _("espelhamento em x"));
	clsPopUp->Append(popMIRRORy, _("Mirror Y"), _("espelhamento em y"));
	
}

// ----------------------------------------------------------------------------- 
//+++MOUSE
// -----------------------------------------------------------------------------
void BasicGLPane::mouseMoved(wxMouseEvent& event) { 
	if(event.MiddleIsDown() ){
		cout<<"midddle button event\n";
	}
/*
	if( event.Moving() ){
		sight.movePassiveCursor( event.GetX(), event.GetY() ); 
	} else {
		sight.moveMotionCursor( event.GetX(), event.GetY() );
	}
*/
	sight.moveCursor( event.GetX(), event.GetY() );
}
// -----------------------------------------------------------------------------
void BasicGLPane::mouseLeftWindow(wxMouseEvent& event) {}
// -----------------------------------------------------------------------------
//+++LEFT
// -----------------------------------------------------------------------------
void BasicGLPane::mouseDown(wxMouseEvent& event) {
	sight.touch();
	SetFocus();	
}//end event
// -----------------------------------------------------------------------------
void BasicGLPane::mouseReleased(wxMouseEvent& event) { 
	if( sight.getCurrentStateName() == SIGHT_SELECT ) sight.touch();
	
	if( sight.getCurrentStateName() == SIGHT_RECTANGLE )
		if(sight.getCurrentStateStatus() == SIGHT_READY )
			sight.touch();
	 
}//end event
// -----------------------------------------------------------------------------
void BasicGLPane::doubleLClick(wxMouseEvent& event){
	/*cout<<"DOUBLE CLICK****************\n";
	doubleFlag = true;
	sight.doubleL(event.GetX(), event.GetY());*/
}//end event
// -----------------------------------------------------------------------------
//+++RIGHT
// -----------------------------------------------------------------------------
void BasicGLPane::rightClick(wxMouseEvent& event) {
	if( event.RightDown() ){
		sight.beginZooming();
	} else {
		if( !sight.touch() ){		
			PopupMenu(clsPopUp, event.GetPosition());
		}
		sight.end();	
	}//end else
}//end method

// -----------------------------------------------------------------------------
//+++SCROLL
// -----------------------------------------------------------------------------
void BasicGLPane::mouseWheelMoved(wxMouseEvent& event) { 
	sight.zoom(event.GetWheelRotation() > 0); //>0 = wheel rotation up
}//end event
// -----------------------------------------------------------------------------
void BasicGLPane::middleDown(wxMouseEvent& event){
	sight.beginDrag();
 }//end event
// ----------------------------------------------------------------------------- 
  void BasicGLPane::middleUp(wxMouseEvent& event){
	sight.touch();
	sight.end();
 }//end event
 
// ----------------------------------------------------------------------------- 
//+++KEYBOARD
// -----------------------------------------------------------------------------
void BasicGLPane::keyPressed(wxKeyEvent& event) {

	switch ( event.GetKeyCode() ) {
		case WXK_ESCAPE:
			if ( sight.getCurrentStateStatus() == SIGHT_IDLE )
				sight.setState( SIGHT_SELECT );
			else
				sight.resetState();
			break;		
		case WXK_RETURN:
			sight.touch( true );
			break;
		case wxID_DELETE:
		case WXK_DELETE:
			sight.deleteSelectedObject();
			cout<<"delete\n";
			checkGLErrors();
			break;
		case WXK_F4:
			cout<<"f4\n";
		//	sight.hideRotingLayers(true);
		break;
		case WXK_F5:
			//sight.hideRotingLayers(false);
		break;
		case WXK_ADD:
		case '=':
			sight.zoom( true );
			break;
		case '-':
			sight.zoom( false );
		break;
		case WXK_UP:
			sight.toUp();
		break;
		case WXK_DOWN:
			sight.toDown();
		break;
		case WXK_LEFT:
			sight.toLeft();
		break;		
		case WXK_RIGHT:
			sight.toRight();
		break;
		
		case WXK_PAGEDOWN:
			sight.hierarchy(true);//bool down?
		break;		
		case WXK_PAGEUP:
			sight.hierarchy(false);//bool down?
		break;		
			
		// BEGIN COMPABILITY
		default:
			sight.handleKeyEvent( event.GetKeyCode() ,event.m_controlDown );	
			break;	
		// END
	} // end switch
	
}//end event
// -----------------------------------------------------------------------------
void BasicGLPane::keyReleased(wxKeyEvent& event) {}
// ----------------------------------------------------------------------------- 


//+++OTHER
// ----------------------------------------------------------------------------- 

// -----------------------------------------------------------------------------
void BasicGLPane::resized(wxSizeEvent& evt) {
    wxGLCanvas::OnSize(evt);
	sight.reshapeWindow( getWidth(), getHeight() );
    //Update(); // Refresh(); isn't enough. If you need to update the window immediately you should use Update instead of Refresh().
}
// ----------------------------------------------------------------------------- 
void BasicGLPane::render() {
}

// -----------------------------------------------------------------------------

void BasicGLPane::OnPaint( wxPaintEvent &event ) {
	wxPaintDC dc( this );
	cerr << "Painting...\n";
	sight.refreshScreen();
} // end event

// -----------------------------------------------------------------------------

void BasicGLPane::MirrorX(wxCommandEvent& WXUNUSED(event)){
	sight.mirror(true);
}

// -----------------------------------------------------------------------------

void BasicGLPane::MirrorY(wxCommandEvent& WXUNUSED(event)){
	sight.mirror(false);
}

// -----------------------------------------------------------------------------

void BasicGLPane::connect(wxCommandEvent& event){
	sight.connection( event.GetId());
}

// -----------------------------------------------------------------------------
















// -----------------------------------------------------------------------------