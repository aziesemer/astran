#ifndef APP_H
#define APP_H

#include <vector>
	using std::vector;

#include "./interface/interface.h"
#include "./sight/sight.h"

class wxSightApp: public wxApp {
	private:

		vector< wxSightFrame * > clsFrames;

    bool OnInit();
    void OnNew(wxCommandEvent& event);

		virtual void MacOpenFile(const wxString &fileName);
		
		void loadFile( const wxString &fileName );
		
    DECLARE_EVENT_TABLE()
		
};

IMPLEMENT_APP(wxSightApp)

#endif