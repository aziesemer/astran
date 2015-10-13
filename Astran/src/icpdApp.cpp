//---------------------------------------------------------------------------
//
// Name:        icpdApp.cpp
// Author:      Adriel Mota Ziesemer Junior
// Created:     20/9/2007 19:13:08
// Description: 
//
//---------------------------------------------------------------------------

#include "icpdApp.h"

using namespace std;

IMPLEMENT_APP(icpdFrmApp);

bool icpdFrmApp::OnInit()
{
    setlocale(LC_ALL,"C");

    ifstream ifile(cmdFilename.mb_str());
    if ((!ifile) && (!cmdFilename.empty())) {
        cout << "ERROR: File \'" << cmdFilename.mb_str() << "\' doesn't exist" << endl;
        return false;
    }

    if (!gui_enabled) 
    {
        DesignMng designmng;
        string cmd;

        ifstream ifile(cmdFilename.mb_str());
        if (ifile) 
        {
            cmd = string("read ") + string(wxString(cmdFilename).mb_str());
            cout << "astran> " + cmd;
            designmng.readCommand(cmd);
        } else 
        {
            string astran_cfg;
            astran_cfg = "astran.cfg";

            wxString astran_path;
            ::wxGetEnv(wxT("ASTRAN_PATH"), &astran_path);

            if (wxDirExists(astran_path)) {
                astran_cfg = string(wxString(astran_path).mb_str()) + "/bin/astran.cfg";
            }

            cmd = string("read ") + astran_cfg;
            cout << "astran> " + cmd;
            designmng.readCommand(cmd); 
        }

        while (true) {
            cout << "astran> ";
            getline (cin, cmd);
            designmng.readCommand(cmd);
        }
    } 
    else
    {
		wxInitAllImageHandlers();
		IcpdFrm *frame = new IcpdFrm(NULL);
		SetTopWindow(frame);
        // frame->SetIcon(wxIcon(help));
		frame->Show();
	}

    return true;
}

int icpdFrmApp::OnExit(){
	return EXIT_SUCCESS;
}