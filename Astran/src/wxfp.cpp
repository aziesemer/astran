#include "wxfp.h"

WxFP::WxFP( IcpdFrm* frmwork ):Specify_FP( frmwork ){
	currentFrmwork=frmwork;
    refresh();
}

void WxFP::PressedKey( wxKeyEvent& event ){
	if (event.GetKeyCode() == WXK_ESCAPE)
		cancel();
	else if (event.GetKeyCode() == WXK_RETURN)
		ok();
	event.Skip();
}

void WxFP::hide( wxCloseEvent& event ){
    Show(false);
}

void WxFP::okButtonEvt( wxCommandEvent& event ){
	ok();
}

void WxFP::cancelButtonEvt( wxCommandEvent& event ){
	cancel();
}

void WxFP::ok(){
	wxString cmd;

	// compare if current Top Cell is different from selection box
	if (string(cell_choice->GetStringSelection().mb_str()) != currentFrmwork->getDesign()->getCircuit()->getTopCell()){
		cmd=wxT("set topcell ") + cell_choice->GetStringSelection();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}

	// set new values for margins
	cmd=wxT("set area ") +  wxNrRows->GetValue() + wxT(" ") +  wxUtilization->GetValue();
	currentFrmwork->executeCommand(string(cmd.mb_str()));

	// compare if current margins are different from textboxes values
	if ((wxC2L->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getLMargin() * currentFrmwork->getDesign()->getCircuit()->getHPitch())) || 
		(wxC2R->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getRMargin() * currentFrmwork->getDesign()->getCircuit()->getHPitch())) ||
		(wxC2B->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getBMargin() * currentFrmwork->getDesign()->getCircuit()->getVPitch())) ||
		(wxC2T->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getTMargin() * currentFrmwork->getDesign()->getCircuit()->getVPitch()))) {
			cmd=wxT("set margins ") +  wxC2L->GetValue() + wxT(" ") +  wxC2R->GetValue() + wxT(" ") +  wxC2T->GetValue() + wxT(" ") +  wxC2B->GetValue();
			currentFrmwork->executeCommand(string(cmd.mb_str()));
	}

	Show(false);
}

void WxFP::cancel(){
	refresh();
    Show(false);
}

void WxFP::refresh(){
	// append the itens (cells) for the selection box
	cell_choice->Clear();
	for(map<string, CellNetlst>::iterator cells_it=currentFrmwork->getDesign()->getCircuit()->getCellNetlsts()->begin(); cells_it!=currentFrmwork->getDesign()->getCircuit()->getCellNetlsts()->end(); cells_it++)
		cell_choice->Append(wxString::From8BitData(cells_it->first.c_str()));
    cell_choice->Select(0);

	// set the current values for the textboxes
	wxNrRows->SetValue(wxString::Format(_T("%d"), currentFrmwork->getDesign()->getPlacer()->getNrRows()));
	wxUtilization->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getPlacer()->getUtilization()));
	wxC2L->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getLMargin() * currentFrmwork->getDesign()->getCircuit()->getHPitch()));	
	wxC2R->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getRMargin() * currentFrmwork->getDesign()->getCircuit()->getHPitch()));	
	wxC2B->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getBMargin() * currentFrmwork->getDesign()->getCircuit()->getVPitch()));	
	wxC2T->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getCircuit()->getTMargin() * currentFrmwork->getDesign()->getCircuit()->getVPitch()));	
}