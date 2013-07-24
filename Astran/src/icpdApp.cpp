//---------------------------------------------------------------------------
//
// Name:        icpdApp.cpp
// Author:      Adriel Mota Ziesemer Junior
// Created:     20/9/2007 19:13:08
// Description: 
//
//---------------------------------------------------------------------------

#include <wx/wx.h>

#include "icpdfrm.h"
//#include "..\Images\help.xpm"

using namespace std;

class icpdFrmApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

IMPLEMENT_APP(icpdFrmApp)

bool icpdFrmApp::OnInit(){
	//frame->SetIcon(wxIcon(help_xpm));
	if (argc == 2){
		DesignMng designmng;
		string cmd = string("read ") + wxString(argv[1]).ToAscii().data();
		designmng.readCommand(cmd);
		designmng.readCommand("exit");
	} else{
		wxInitAllImageHandlers();
		IcpdFrm *frame = new IcpdFrm(NULL);
		SetTopWindow(frame);
		frame->Show();
	}
    return true;
}

int icpdFrmApp::OnExit(){
	return 0;
}