//---------------------------------------------------------------------------
//
// Name:        icpdApp.h
// Author:      Adriel Mota Ziesemer Junior
// Created:     20/9/2007 19:13:08
// Description:
//
//---------------------------------------------------------------------------

#pragma once

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


class HybridTraits : public wxGUIAppTraits{
public:
    HybridTraits(bool gui_enabled) : wxGUIAppTraits(),
    gui_enabled(gui_enabled) {}
    
private:
    bool gui_enabled;
    bool show_commands;
};

class icpdFrmApp : public wxApp{
public:
    wxString cmdFilename;
    
private:
    bool gui_enabled;
    
    bool OnInit();
    int OnExit();
    bool Initialize(int& argc, wchar_t **argv);
    void CleanUp();
    HybridTraits *CreateTraits();
};