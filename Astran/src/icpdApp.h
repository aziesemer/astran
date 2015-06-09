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

#ifndef WX_PRECOMP
	#include <wx/wx.h>
#else
	#include <wx/wxprec.h>
#endif
#include "icpdfrm.h"
#include <wx/wx.h>

class icpdFrmApp : public wxApp
{
	public:
		bool OnInit();
		int OnExit();
};

#endif
