#ifndef BASIC_GL_PANE_H
#define BASIC_GL_PANE_H

// include OpenGL
#ifdef __WXMAC__
#include <OpenGL/glu.h>
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#else
#include "./sight/opengl.h"
#endif

#include <wx/wx.h>
#include <wx/glcanvas.h>

#include <iostream>
	using std::cerr;

#include "./sight/sight.h"

extern SightGL sight;

void wxGLCanvasSwapBuffers( void * data );

class BasicGLPane : public wxGLCanvas
{
private:
	bool doubleFlag;
	wxMenu * clsPopUp;
	wxMenu * clsConnect;
// -----------------------------------------------------------------------------		
	void createPopUp();	
	void createSubMenuConnect();	
// -----------------------------------------------------------------------------		
			
public:
	BasicGLPane(wxWindow * parent, int* args);
// -----------------------------------------------------------------------------		
	void resized(wxSizeEvent& evt);
// -----------------------------------------------------------------------------		
	int getWidth() { return GetClientSize().x; }
	int getHeight() { return GetClientSize().y; }
// -----------------------------------------------------------------------------		
	void render();
// -----------------------------------------------------------------------------		
	// events
	void mouseMoved(wxMouseEvent& event);//motion and passive
	void mouseDown(wxMouseEvent& event);//left button
	void doubleLClick(wxMouseEvent& event);//left double click
	void mouseWheelMoved(wxMouseEvent& event);//scroll
	void mouseReleased(wxMouseEvent& event);//:p
	void rightClick(wxMouseEvent& event);//right button
	void rightClickR(wxMouseEvent& event);
	void mouseLeftWindow(wxMouseEvent& event);
	void middleDown(wxMouseEvent& event);//scroll click->drag
	void middleUp(wxMouseEvent& event);//scroll click->~drag
// -----------------------------------------------------------------------------
	void keyPressed(wxKeyEvent& event);
	void keyReleased(wxKeyEvent& event);
// -----------------------------------------------------------------------------	
	void OnPaint( wxPaintEvent &event );
// -----------------------------------------------------------------------------
	void connect(wxCommandEvent& event);
	void MirrorX(wxCommandEvent& WXUNUSED(event));
	void MirrorY(wxCommandEvent& WXUNUSED(event));	
// -----------------------------------------------------------------------------	
		
	DECLARE_EVENT_TABLE()
};

enum {
	popCONNECTlayer = 0,
	popCONNECTlayerUP = 1,
	popCONNECTlayerDOWN = 2,
	popCONNECTlayerUD = 3,
	popMIRRORx,
	popMIRRORy

};







#endif