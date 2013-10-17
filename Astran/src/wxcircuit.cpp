#include "wxcircuit.h"

WxCircuit::WxCircuit( IcpdFrm* frmwork ):View_Circuit( frmwork ){
	currentFrmwork=frmwork;
	refresh();
}

void WxCircuit::PressedKey( wxKeyEvent& event ){
	if (event.GetKeyCode() == WXK_ESCAPE)
		cancel();
	event.Skip();
}

void WxCircuit::hide( wxCloseEvent& event ){
    Show(false);
}

void WxCircuit::okButtonEvt( wxCommandEvent& event ){
	ok();
}

void WxCircuit::cancelButtonEvt( wxCommandEvent& event ){
	cancel();
}

void WxCircuit::ok(){
	wxString cmd;

	// compare if current Design Name is different from textbox value
	if(design_name->GetValue().ToAscii().data() != currentFrmwork->getDesign()->getName()){
		cmd = wxT("set designName ") + design_name->GetValue();
		currentFrmwork->readCommand(cmd.ToAscii().data());
	}

	// compare if current horizontal grid is different from textbox value
	if(horizontalgrid->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getHPitch())){
		cmd = wxT("set hgrid ") + horizontalgrid->GetValue();
		currentFrmwork->readCommand(cmd.ToAscii().data());
	}

	// compare if current vertical grid is different from textbox value
	if(verticalgrid->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getVPitch())){
		cmd = wxT("set vgrid ") + verticalgrid->GetValue();
		currentFrmwork->readCommand(cmd.ToAscii().data());
	}

	// compare if current Vdd Net Name is different from textbox value
	if(vddnetname->GetValue().ToAscii().data() != currentFrmwork->getDesign()->getCircuit()->getVddNet().c_str()){
		wxString cmd=wxT("set vddnet ") + vddnetname->GetValue();
		currentFrmwork->readCommand(cmd.ToAscii().data());
	}

	// compare if current Gnd Net Name is different from textbox value
	if(gndnetname->GetValue().ToAscii().data() != currentFrmwork->getDesign()->getCircuit()->getGndNet().c_str()){
		wxString cmd=wxT("set gndnet ") + gndnetname->GetValue();
		currentFrmwork->readCommand(cmd.ToAscii().data());
	}

	// compare if current Row Height is different from textbox value
	if(rowheight->GetValue().ToAscii().data() != intToStr(currentFrmwork->getDesign()->getCircuit()->getRowHeight())){
		wxString cmd=wxT("set rowheight ") + rowheight->GetValue();
		currentFrmwork->readCommand(cmd.ToAscii().data());
	}

	// compare if current Supply Size is different from textbox value
	if(supplysize->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getSupplyVSize())){
		wxString cmd=wxT("set supplysize ") + supplysize->GetValue();
		currentFrmwork->readCommand(cmd.ToAscii().data());
	}

    // compare if current nWell position is  different from textbox value
	if(nWellPos->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getnWellPos())){
		wxString cmd=wxT("set nWellPos ") + nWellPos->GetValue();
		currentFrmwork->readCommand(cmd.ToAscii().data());
	}

    // compare if current tapless setting is  different from checked value
	if(tapless->GetValue() != currentFrmwork->getDesign()->getCircuit()->isTapless()){
		wxString cmd;
        if(tapless->GetValue()==true)
            cmd=wxT("set tapless YES");
        else
            cmd=wxT("set tapless NO");
        currentFrmwork->readCommand(cmd.ToAscii().data());
	}

    Show(false);
}

void WxCircuit::cancel(){
	refresh();
    Show(false);
}

void WxCircuit::refresh(){
	// set the current values for the textboxes
	design_name->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getName().c_str()));
	horizontalgrid->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getHPitch()));
	verticalgrid->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getVPitch()));
	vddnetname->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getCircuit()->getVddNet().c_str()));
	gndnetname->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getCircuit()->getGndNet().c_str()));
	rowheight->SetValue(wxString::Format(_T("%d"), currentFrmwork->getDesign()->getCircuit()->getRowHeight()));
	supplysize->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getSupplyVSize()));
	nWellPos->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getnWellPos()));
	tapless->SetValue(currentFrmwork->getDesign()->getCircuit()->isTapless());
}