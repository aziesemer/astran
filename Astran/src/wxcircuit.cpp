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
	if(string(design_name->GetValue().mb_str()) != currentFrmwork->getDesign()->getName()){
		cmd = wxT("set designName ") + design_name->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}

	// compare if current horizontal grid is different from textbox value
	if(horizontalgrid->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getHPitch())){
		cmd = wxT("set hgrid ") + horizontalgrid->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}
    
	// compare if current vertical grid is different from textbox value
	if(verticalgrid->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getVPitch())){
		cmd = wxT("set vgrid ") + verticalgrid->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}
    
	// compare if current horizontal grid offset is different from textbox value
	if(hGridOffset->GetValue() != currentFrmwork->getDesign()->getCircuit()->getHGridOffset()){
		wxString cmd;
        if(hGridOffset->GetValue()==true)
            cmd=wxT("set hgrid_offset YES");
        else
            cmd=wxT("set hgrid_offset NO");
        currentFrmwork->executeCommand(string(cmd.mb_str()));
	}
    
	// compare if current vertical grid offset is different from textbox value
	if(vGridOffset->GetValue() != currentFrmwork->getDesign()->getCircuit()->getVGridOffset()){
		wxString cmd;
        if(vGridOffset->GetValue()==true)
            cmd=wxT("set vgrid_offset YES");
        else
            cmd=wxT("set vgrid_offset NO");
        currentFrmwork->executeCommand(string(cmd.mb_str()));
	}
    
	// compare if current Vdd Net Name is different from textbox value
	if(string(vddnetname->GetValue().mb_str()) != currentFrmwork->getDesign()->getCircuit()->getVddNet()){
		wxString cmd=wxT("set vddnet ") + vddnetname->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}

	// compare if current Gnd Net Name is different from textbox value
	if(string(gndnetname->GetValue().mb_str()) != currentFrmwork->getDesign()->getCircuit()->getGndNet()){
		wxString cmd=wxT("set gndnet ") + gndnetname->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}

	// compare if current Row Height is different from textbox value
	if(string(rowheight->GetValue().mb_str()) != to_string(currentFrmwork->getDesign()->getCircuit()->getRowHeight())){
		wxString cmd=wxT("set rowheight ") + rowheight->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}

	// compare if current Supply Size is different from textbox value
	if(supplysize->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getSupplyVSize())){
		wxString cmd=wxT("set supplysize ") + supplysize->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}

    // compare if current nWell position is  different from textbox value
	if(nWellPos->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getnWellPos())){
		wxString cmd=wxT("set nWellPos ") + nWellPos->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}
    
    // compare if current nWell position is  different from textbox value
	if(nWellBorder->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getnWellBorder())){
		wxString cmd=wxT("set nWellBorder ") + nWellBorder->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}
    
    // compare if current nWell position is  different from textbox value
	if(pnSelBorder->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getpnSelBorder())){
		wxString cmd=wxT("set pnSelBorder ") + pnSelBorder->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}
    
    // compare if current cell template  is  different from textbox value
	if(string(cellTemplate->GetString(cellTemplate->GetSelection()).mb_str()) != currentFrmwork->getDesign()->getCircuit()->getCellTemplate()){
		wxString cmd=wxT("set celltemplate \"") + cellTemplate->GetString(cellTemplate->GetSelection())+wxT("\"");
		currentFrmwork->executeCommand(string(cmd.mb_str()));
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
	hGridOffset->SetValue(currentFrmwork->getDesign()->getCircuit()->getHGridOffset());
	vGridOffset->SetValue(currentFrmwork->getDesign()->getCircuit()->getVGridOffset());
	vddnetname->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getCircuit()->getVddNet().c_str()));
	gndnetname->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getCircuit()->getGndNet().c_str()));
	rowheight->SetValue(wxString::Format(_T("%d"), currentFrmwork->getDesign()->getCircuit()->getRowHeight()));
	supplysize->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getSupplyVSize()));
	nWellPos->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getnWellPos()));
	nWellBorder->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getnWellBorder()));
	pnSelBorder->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getpnSelBorder()));
    
    cellTemplate->Clear();
    vector<string> lista = currentFrmwork->getDesign()->getCircuit()->getCellTemplatesList();
    for(vector<string>::iterator atual=lista.begin(); atual!=lista.end(); atual++)
        cellTemplate->Append(wxString::From8BitData((*atual).c_str()));

    cellTemplate->SetSelection(cellTemplate->FindString(wxString::From8BitData(currentFrmwork->getDesign()->getCircuit()->getCellTemplate().c_str()),TRUE));
}

