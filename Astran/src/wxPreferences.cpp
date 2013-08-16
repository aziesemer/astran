#include "wxpreferences.h"

WxPreferences::WxPreferences( IcpdFrm* frmwork ):View_Preferences( frmwork ){
	currentFrmwork=frmwork;
	refresh();
}

void WxPreferences::PressedKey( wxKeyEvent& event ){
	if (event.GetKeyCode() == WXK_ESCAPE)
		cancel();
	event.Skip();
}

void WxPreferences::hide( wxCloseEvent& event ){
	refresh();
    Show(false);
}

void WxPreferences::okButtonEvt( wxCommandEvent& event ){
	ok();
}

void WxPreferences::cancelButtonEvt( wxCommandEvent& event ){
	cancel();
}

void WxPreferences::plButton( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Set Path"), wxT(""), wxT(""), wxT("*.*"), wxFD_OPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString path=tmp->GetDirectory() + wxT("/") + tmp->GetFilename();
		plPath->SetValue(path);
	}
}

void WxPreferences::rotdlButton( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Set Path"), wxT(""), wxT(""), wxT("*.*"), wxFD_OPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString path=tmp->GetDirectory() + wxT("/") + tmp->GetFilename();
		rotdlPath->SetValue(path);
	}
}

void WxPreferences::viewerButton( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Set Path"), wxT(""), wxT(""), wxT("*.*"), wxFD_OPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString path=tmp->GetDirectory() + wxT("/") + tmp->GetFilename();
		viewerPath->SetValue(path);
	}
}

void WxPreferences::lpsolverButton( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Set Path"), wxT(""), wxT(""), wxT("*.*"), wxFD_OPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString path=tmp->GetDirectory() + wxT("/") + tmp->GetFilename();
		lpsolverPath->SetValue(path);
	}
}

void WxPreferences::logButton( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Set Path"), wxT(""), wxT(""), wxT("*.*"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString path=tmp->GetDirectory() + wxT("/") + tmp->GetFilename();
		logPath->SetValue(path);
	}
}

void WxPreferences::ok(){
	wxString cmd, path;

	// compare if current placer path is different from textbox value
	if(string(plPath->GetValue().ToAscii().data())!=currentFrmwork->getDesign()->getPlacerFile()){
		path = wxT("\"") + plPath->GetValue() + wxT("\"");
		cmd=wxT("set placer ") + path;
		currentFrmwork->readCommand(cmd.ToAscii().data());
	}

	// compare if current rotdl path is different from textbox value
	if(string(rotdlPath->GetValue().ToAscii().data())!=currentFrmwork->getDesign()->getRotDL()){
		path = wxT("\"") + rotdlPath->GetValue() + wxT("\"");
		cmd=wxT("set rotdl ") + path;
		currentFrmwork->readCommand(cmd.ToAscii().data());
	}

	// compare if current viewer path is different from textbox value
	if(string(viewerPath->GetValue().ToAscii().data())!=currentFrmwork->getDesign()->getViewer()){
		path = wxT("\"") + viewerPath->GetValue() + wxT("\"");
		cmd=wxT("set viewer ") + path;
		currentFrmwork->readCommand(cmd.ToAscii().data());
	}

	// compare if current lpsolver path is different from textbox value
	if(string(lpsolverPath->GetValue().ToAscii().data())!=currentFrmwork->getDesign()->getlpSolver()){
		path = wxT("\"") + lpsolverPath->GetValue() + wxT("\"");
		cmd=wxT("set lpsolve ") + path;
		currentFrmwork->readCommand(cmd.ToAscii().data());
	}

	// compare if current log path is different from textbox value
	if(string(logPath->GetValue().ToAscii().data())!=currentFrmwork->getDesign()->getLog()){
		path = wxT("\"") + logPath->GetValue() + wxT("\"");
		cmd=wxT("set log ") + path;
		currentFrmwork->readCommand(cmd.ToAscii().data());
	}

    Show(false);
}

void WxPreferences::cancel(){
	refresh();
    Show(false);
}

void WxPreferences::refresh(){
	// set the current values for the textboxes
	plPath->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getPlacerFile().c_str()));
	rotdlPath->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getRotDL().c_str()));
	viewerPath->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getViewer().c_str()));
	lpsolverPath->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getlpSolver().c_str()));
	logPath->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getLog().c_str()));
}