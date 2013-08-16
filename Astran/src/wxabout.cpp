#include "wxabout.h"

WxAbout::WxAbout( wxWindow* parent ):Help_About( parent ){
	
	wxFont font(10, wxROMAN, wxNORMAL, wxNORMAL);
/*
//	wxRichTextCtrl& r = *richtxt_about;

	r.SetFont(font);
	//r.SetBackgroundColour(wxColour(255,255,255));

//	r.BeginAlignment(wxTEXT_ALIGNMENT_CENTRE);
	r.WriteImage(wxBitmap( wxT( "../Images/logo_ufrgs.jpg"), wxBITMAP_TYPE_ANY ));
	r.WriteText(wxT(" \t "));
	r.WriteImage(wxBitmap( wxT( "../Images/logo_inf.png"), wxBITMAP_TYPE_ANY ));

	r.Newline();
	r.Newline();
	r.Newline();

//	r.BeginBold();
//	r.BeginFontSize(12);
	r.WriteText(wxT("ICPD - Integrated Circuit Physical Design Tool v"));
	r.WriteText(wxT(VERSION));	
	r.Newline();
	r.WriteText(wxT("UFRGS - Universidade Federal do Rio Grande do Sul"));
	r.Newline();
	r.WriteText(wxT("GME - Microelectronics Group"));
	r.Newline();
	r.EndAlignment();
//	r.EndFontSize();
//	r.EndBold();

	r.Newline();

//	r.BeginBold();
	r.WriteText(wxT("Project Advisor"));
//	r.EndBold();
	r.Newline();

//	r.BeginLeftIndent(60);
	r.WriteText(wxT("Ricardo Reis (reis@inf.ufrgs.br)"));
	r.Newline();
	r.EndLeftIndent();

	r.Newline();

//	r.BeginBold();
	r.WriteText(wxT("Creator"));
//	r.EndBold();
	r.Newline();

//	r.BeginLeftIndent(60);
	r.WriteText(wxT("Adriel Ziesemer (amziesemerj@inf.ufrgs.br)"));
	r.Newline();
	r.EndLeftIndent();

	r.Newline();

//	r.BeginBold();
	r.WriteText(wxT("Co-Developers"));
//	r.EndBold();
	r.Newline();

//	r.BeginLeftIndent(60);
	r.WriteText(wxT("Charles Capella Leonhardt (ccleonhardt@inf.ufrgs.br)"));
	r.Newline();
	r.WriteText(wxT("Felipe Avila Nesello (felipe.nesello@inf.ufrgs.br)"));
	r.Newline();
	r.EndLeftIndent();

	r.Newline();

//	r.BeginBold();
	r.WriteText(wxT("Acknowledgement"));
//	r.EndBold();
	r.Newline();

//	r.BeginLeftIndent(60);
	r.WriteText(wxT("Cristiano Lazzari (clazz@inf.ufrgs.br)"));
	r.Newline();
	r.WriteText(wxT("Glauco Santos (gbvsantos@inf.ufrgs.br)"));
	r.Newline();
	r.WriteText(wxT("Guilherme Flach (gaflach@inf.ufrgs.br)"));
	r.Newline();
	r.WriteText(wxT("Renato Hentschke (renato@inf.ufrgs.br)"));
	r.Newline();
	r.EndLeftIndent();
	
	r.Newline();

//	r.BeginBold();
	r.WriteText(wxT("Web Site"));
//	r.EndBold();
	r.Newline();

//	r.BeginLeftIndent(60);
//	r.BeginUnderline();
	r.WriteText(wxT("http://www.inf.ufrgs.br/~amziesemerj/icpd"));
	r.Newline();
	r.EndUnderline();
	r.EndLeftIndent();

	r.Newline();

//	r.BeginBold();
	r.WriteText(wxT("Partners"));
//	r.EndBold();
	r.Newline();
	r.Newline();

//	r.BeginLeftIndent(60);
	r.WriteImage(wxBitmap( wxT( "../Images/logo_cnpq.jpg"), wxBITMAP_TYPE_ANY ));
	r.WriteText(wxT(" \t "));
	r.WriteImage(wxBitmap( wxT( "../Images/logo_capes.png"), wxBITMAP_TYPE_ANY ));
	r.Newline();
	r.EndLeftIndent();
*/
	//r.Disable();
}

void WxAbout::hide( wxCloseEvent& event ){
    Show(false);
}