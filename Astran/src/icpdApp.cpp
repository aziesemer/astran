//---------------------------------------------------------------------------
//
// Name:        icpdApp.cpp
// Author:      Adriel Mota Ziesemer Junior
// Created:     20/9/2007 19:13:08
// Description: 
//
//---------------------------------------------------------------------------

#include "icpdApp.h"

DECLARE_APP(icpdFrmApp)
IMPLEMENT_APP(icpdFrmApp);

bool icpdFrmApp::Initialize(int& argc, wchar_t **argv) { 

    static const wxCmdLineEntryDesc desc[] = { 
        { wxCMD_LINE_SWITCH, wxT_2("s"), wxT_2("shell"), wxT_2("Run in shell mode") }, 
        // { wxCMD_LINE_SWITCH, wxT_2("c"), wxT_2("commands"), wxT_2("Show all available commands") },
        { wxCMD_LINE_PARAM, NULL, NULL, wxT_2("FILENAME"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
        { wxCMD_LINE_NONE }
    }; 

    wxCmdLineParser parser(desc, argc, argv); 
    if (parser.Parse(true) != 0) { 
        exit(1); 
    } 
    
    gui_enabled = !parser.Found(wxT_2("shell"));

    // cout << " STAT:" << parser.Parse(true) << endl;
    // cout << " FILE:" << parser.GetParam(0).mb_str() << endl;
    // cout << " CONT:" << parser.GetParamCount() << endl;

    if (parser.GetParamCount() == 1) {
        cmdFilename = parser.GetParam(0);
    }

    if (gui_enabled) { 
        return wxApp::Initialize(argc, argv); 
    } else { 
        return wxAppConsole::Initialize(argc, argv); 
    }
} 

bool icpdFrmApp::OnInit()
{
    setlocale(LC_ALL, "C");

    ifstream ifile(cmdFilename.mb_str());
    if ((!ifile) && (!cmdFilename.empty())) {
        cout << "ERROR: File \'" << cmdFilename.mb_str() << "\' doesn't exist" << endl;
        return false;
    }

    if (!gui_enabled) 
    {
        DesignMng designmng;
        string cmd;

        wxString astran_path;
        ::wxGetEnv(wxT_2("ASTRAN_PATH"), &astran_path);
        string astran_cfg;
        astran_cfg = "astran.cfg";
        astran_cfg = string(wxString(astran_path).mb_str()) + "/bin/astran.cfg";        

        // By default load astran.cfg
        ifstream afile(astran_cfg.c_str());
        if (afile) 
        {
            cmd = string("read ") + astran_cfg;
            cout << "astran> " << cmd << endl;
            designmng.readCommand(cmd); 
        }

        // Read astran_script
        if (ifile) 
        {
            cmd = string("read ") + string(wxString(cmdFilename).mb_str());
            cout << "astran> " << cmd << endl;
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
        frame->Show();
    }

    return true;
}

int icpdFrmApp::OnExit(){
    return EXIT_SUCCESS;
}

void icpdFrmApp::CleanUp()
{ 
    if (gui_enabled) { 
        wxApp::CleanUp(); 
    } else { 
        wxAppConsole::CleanUp(); 
    } 
} 

HybridTraits *icpdFrmApp::CreateTraits()
{ 
    return new HybridTraits(gui_enabled); 
} 
