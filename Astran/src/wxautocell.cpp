#include "wxautocell.h"

WxAutoCell::WxAutoCell( IcpdFrm* frmwork ):CellGen( frmwork ){
	currentFrmwork=frmwork;
	update();
}

void WxAutoCell::PressedKey( wxKeyEvent& event ){
	if (event.GetKeyCode() == WXK_ESCAPE)
		Show(false);
	event.Skip();
}

void WxAutoCell::hide( wxCloseEvent& event ){
    Show(false);
}

/*void WxAutoCell::generateAllCells( wxCommandEvent& event ){
 for(wxArrayString::iterator cells_it=cellnet_choice->GetStrings().begin(); cells_it!=cellnet_choice->GetStrings().end(); cells_it++)
 generateCell(cells_it->fi
 cellnet_choice->Append(cells_it-first.c_str());
 cellnet_choice->Select(0);
 }*/

void WxAutoCell::generateCell( wxCommandEvent& event ){
	cout << "CellGen: Auto generating cell " << cellnet_choice->GetStringSelection() << endl;	
	
	// Select cell
	wxString cmd=wxT("cellgen set pos_tracks ") + s_TP->GetValue();
	currentFrmwork->readCommand(cmd.ToAscii().data());
	cmd=wxT("cellgen set nr_tracks ") + s_RN->GetValue();
	currentFrmwork->readCommand(cmd.ToAscii().data());
	cmd=wxT("cellgen select ") + cellnet_choice->GetStringSelection();
	currentFrmwork->readCommand(cmd.ToAscii().data());
	
	// Fold Transistors
	currentFrmwork->readCommand("cellgen fold");
	
	// Place
	cmd=wxT("cellgen place ") + p_TAQ->GetValue() + wxT(" ") + p_NRA->GetValue() + wxT(" ") + p_WC->GetValue() + wxT(" ") + p_GMC->GetValue() + wxT(" ") + p_RC->GetValue() + wxT(" ") + p_RTDC->GetValue() + wxT(" ") + p_NGC->GetValue();
	currentFrmwork->readCommand(cmd.ToAscii().data());
	
	// Route
	cmd=wxT("cellgen route ") + rc_M->GetValue() + wxT(" ") + rc_P->GetValue() + wxT(" ") + rc_C->GetValue() + wxT(" ") + rc_IOP->GetValue();
	currentFrmwork->readCommand(cmd.ToAscii().data());
	
	// Compact Layout
	cmd=wxT("cellgen compact");
	currentFrmwork->readCommand(cmd.ToAscii().data());
	update();
}

void WxAutoCell::select( wxCommandEvent& event ){
	wxString cmd=wxT("cellgen select ") + cellnet_choice->GetStringSelection();
	currentFrmwork->readCommand(cmd.ToAscii().data());
	update();
}

void WxAutoCell::fold( wxCommandEvent& event ){
	currentFrmwork->readCommand("cellgen fold");
	update();
}

void WxAutoCell::place( wxCommandEvent& event ){
	wxString cmd=wxT("cellgen place ") + p_TAQ->GetValue() + wxT(" ") + p_NRA->GetValue() + wxT(" ") + p_WC->GetValue() + wxT(" ") + p_GMC->GetValue() + wxT(" ") + p_RC->GetValue() + wxT(" ") + p_RTDC->GetValue() + wxT(" ") + p_NGC->GetValue();
	currentFrmwork->readCommand(cmd.ToAscii().data());
	update();
}

void WxAutoCell::route( wxCommandEvent& event ){
	wxString cmd=wxT("cellgen route ") + rc_M->GetValue() + wxT(" ") + rc_P->GetValue() + wxT(" ") + rc_C->GetValue() + wxT(" ") + rc_IOP->GetValue();
	currentFrmwork->readCommand(cmd.ToAscii().data());
	update();
}

void WxAutoCell::compact( wxCommandEvent& event ){
	wxString cmd=wxT("cellgen compact");
	currentFrmwork->readCommand(cmd.ToAscii().data());
	update();
}

void WxAutoCell::view( wxCommandEvent& event ){
	currentFrmwork->viewLayout(cellnet_choice->GetStringSelection().ToAscii().data());
}

void WxAutoCell::refresh(){
	// append the itens (cells) for the selection box
	cellnet_choice->Clear();
	for(map<string, CellNetlst>::iterator cells_it=currentFrmwork->getDesign()->getCircuit()->getCellNetlsts()->begin(); cells_it!=currentFrmwork->getDesign()->getCircuit()->getCellNetlsts()->end(); cells_it++)
		cellnet_choice->Append(wxString::From8BitData(cells_it->first.c_str()));
    cellnet_choice->Select(0);
}

void WxAutoCell::update(){
	// update the states to follow the cellgen's flow
	int state=0;
	if(currentFrmwork->getDesign()->getAutoCell())
		state=currentFrmwork->getDesign()->getAutoCell()->getState();
	cout << state;
	fold_button->Enable(state>=2);
	place_button->Enable(state>=3);
	route_button->Enable(state>=4);
	compact_button->Enable(state>=5);
	view_button->Enable(state>=6);
	Update();
}