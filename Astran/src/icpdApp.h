//---------------------------------------------------------------------------
//
// Name:        icpdApp.h
// Author:      Adriel Mota Ziesemer Junior
// Created:     20/9/2007 19:13:08
// Description: 
//
//---------------------------------------------------------------------------

#ifndef __ICPDFRMApp_h__
#define __ICPDFRMApp_h__

#include <iostream>
#include <fstream>
#include <wx/apptrait.h> 
#include <wx/cmdline.h> 

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif
#include "icpdfrm.h"
#include <wx/wx.h>


class HybridTraits : public wxGUIAppTraits
{ 
	public: 
		HybridTraits(bool gui_enabled) : wxGUIAppTraits(), 
			gui_enabled(gui_enabled) { 
		} 

	private: 
		bool gui_enabled; 
}; 



class icpdFrmApp : public wxApp
{
	public:
		wxString cmdFilename; 

	private:

		bool OnInit();
		int OnExit();

		bool Initialize(int& argc, wchar_t **argv) { 
 
			static const wxCmdLineEntryDesc desc[] = { 
				{ wxCMD_LINE_SWITCH, wxT("s"), wxT("shell"), wxT("Run in shell mode") }, 
				{ wxCMD_LINE_PARAM, NULL, NULL, wxT("FILENAME"), wxCMD_LINE_VAL_STRING, wxCMD_LINE_PARAM_OPTIONAL },
				{ wxCMD_LINE_NONE }
			}; 

			// wxCmdLineParser parser;
			wxCmdLineParser parser(desc, argc, argv); 
			if (parser.Parse(true) != 0) { 
				exit(1); 
			} 

			gui_enabled = !parser.Found(wxT("shell"));
			cmdFilename = parser.GetParam(0);

			if (gui_enabled) { 
				// printf("Running in GUI\n");
				return wxApp::Initialize(argc, argv); 
			} else { 
				// printf("Running in CLI mode\n");
				return wxAppConsole::Initialize(argc, argv); 
			}
      } 

	void CleanUp() { 
		if (gui_enabled) { 
			wxApp::CleanUp(); 
		} else { 
			wxAppConsole::CleanUp(); 
		} 
	} 

	HybridTraits *CreateTraits() { 
		return new HybridTraits(gui_enabled); 
	} 

	bool gui_enabled; 

};

#endif
