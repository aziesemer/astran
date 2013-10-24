#include "app.h"

#include <wx/wx.h>
#include <wx/msgdlg.h>

#include <string>
using std::string;

BEGIN_EVENT_TABLE(wxSightApp, wxApp)
	EVT_MENU(wxID_NEW,wxSightApp::OnNew)
END_EVENT_TABLE()

// -----------------------------------------------------------------------------

char* narrow( const wstring& str ) {
	ostringstream stm ;
	const ctype<char>& ctfacet =
	use_facet< ctype<char> >( stm.getloc() ) ;
	for( size_t i=0 ; i<str.size() ; ++i )
		stm << ctfacet.narrow( str[i], 0 ) ;
	string s = stm.str();
	char* c = new char [s.size()+1];
	strcpy(c, s.c_str());
	return c;
}

bool wxSightApp::OnInit() {
	char ** args = new char*[argc];
	for ( int i = 0; i < argc; i++ )
		args[i] = narrow( argv[i] );
	
    glutInit( &argc, args );
	#ifdef __WXMAC__
		// wxApp::SetExitOnFrameDelete(false);
		wxMenuBar *menuBar = new wxMenuBar;
		
		wxMenu *menuFile = new wxMenu;
		menuFile->Append( wxID_NEW, wxString(_("New\tCtrl+N") ) );		
		menuBar->Append( menuFile, wxString(_("&File")));

		wxMenuBar::MacSetCommonMenuBar(menuBar);
	#endif

	wxInitAllImageHandlers();

	if ( argc == 2 )
		loadFile( argv[1] );
#ifndef	__WXMAC__
	else
		loadFile( _("") );
#endif	
	
  return true;
}  // end event

// -----------------------------------------------------------------------------

void wxSightApp::loadFile( const wxString &fileName ) {
	if ( clsFrames.size() > 0 ) {
		//wxBell();
		wxMessageBox( _("Currentely SightGL can manipulate just one window at time."), _("Information"), wxOK|wxICON_INFORMATION );
	} // end if
	else {
		wxSightFrame *newFrame = new wxSightFrame( _T("SightGL"), wxDefaultPosition, wxSize(800,600) );
		newFrame->Show(TRUE);
		newFrame->Initialize();

		clsFrames.push_back( newFrame );
		if ( !fileName.IsEmpty() )
			newFrame->openFile( fileName.ToAscii() );
		
	} // end else
} // end method

// -----------------------------------------------------------------------------

void wxSightApp::MacOpenFile(const wxString &fileName) {
	loadFile( fileName );
} // end method

// -----------------------------------------------------------------------------

void wxSightApp::OnNew(wxCommandEvent& event) {
	MacOpenFile( wxString(_("")) );
} // end method
