//---------------------------------------------------------------------------
//
// Name:        icpdApp.cpp
// Author:      Adriel Mota Ziesemer Junior
// Created:     20/9/2007 19:13:08
// Description: 
//
//---------------------------------------------------------------------------

#include <wx/wx.h>

#include "icpdApp.h"
#include "icpdfrm.h"

using namespace std;

IMPLEMENT_APP(icpdFrmApp)

bool icpdFrmApp::OnInit(){
	//frame->SetIcon(wxIcon(help_xpm));
    setlocale(LC_ALL,"C");
	if (argc == 2 && string(wxString(argv[1]).mb_str())=="--shell") {
		DesignMng designmng;
        string cmd;
        while (true) {
            cout << "astran> ";
            getline (cin, cmd);
            designmng.readCommand(cmd);            
        }
    }else if (argc == 2 ){
		DesignMng designmng;
		string cmd;
        cmd= string("read ") + string(wxString(argv[1]).mb_str());
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