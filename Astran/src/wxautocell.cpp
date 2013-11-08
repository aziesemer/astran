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
	cout << "CellGen: Auto generating cell " << cellnet_choice->GetStringSelection().mb_str() << endl;	
	
	// Select cell
	wxString cmd=wxT("cellgen select ") + cellnet_choice->GetStringSelection();
	currentFrmwork->readCommand(string(cmd.mb_str()));
	
	// Fold Transistors
	cmd=wxT("cellgen fold ");
    cmd+= nrIntTracks->GetValue();
	currentFrmwork->readCommand(string(cmd.mb_str()));
	
	// Place
	cmd=wxT("cellgen place ") + p_TAQ->GetValue() + wxT(" ") + p_NRA->GetValue() + wxT(" ") + p_WC->GetValue() + wxT(" ") + p_GMC->GetValue() + wxT(" ") + p_RC->GetValue() + wxT(" ") + p_RTDC->GetValue() + wxT(" ") + p_NGC->GetValue();
	currentFrmwork->readCommand(string(cmd.mb_str()));
	
	// Route
	cmd=wxT("cellgen route ");
    cmd+= hPoly->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= increaseIntTracks->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= optimize->GetValue()==true?wxT("1"):wxT("0");
	currentFrmwork->readCommand(string(cmd.mb_str()));
	
	// Compact Layout
    cmd=wxT("cellgen compact ");
    cmd+= diffStretching->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= griddedPolly->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= wxString::Format(_T("%d"), rdCntsCost->GetValue());
    cmd+= wxT(" ");
    cmd+= wxString::Format(_T("%d"), maxDiffCnts->GetValue());
    cmd+= wxT(" ");
    cmd+= alignDiffCnts->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= reduceLturns->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= enableDFM->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= experimental->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= debug->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= timeLimit->GetValue();
	currentFrmwork->readCommand(string(cmd.mb_str()));
	update();
}

void WxAutoCell::select( wxCommandEvent& event ){
	wxString cmd=wxT("cellgen select ") + cellnet_choice->GetStringSelection();
	currentFrmwork->readCommand(string(cmd.mb_str()));
	update();
}

void WxAutoCell::fold( wxCommandEvent& event ){
	wxString cmd=wxT("cellgen fold ");
    cmd+= nrIntTracks->GetValue();
	currentFrmwork->readCommand(string(cmd.mb_str()));
	update();
}

void WxAutoCell::place( wxCommandEvent& event ){
	wxString cmd=wxT("cellgen place ") + p_TAQ->GetValue() + wxT(" ") + p_NRA->GetValue() + wxT(" ") + p_WC->GetValue() + wxT(" ") + p_GMC->GetValue() + wxT(" ") + p_RC->GetValue() + wxT(" ") + p_RTDC->GetValue() + wxT(" ") + p_NGC->GetValue();
	currentFrmwork->readCommand(string(cmd.mb_str()));
	update();
}

void WxAutoCell::route( wxCommandEvent& event ){
	wxString cmd=wxT("cellgen route ");
    cmd+= hPoly->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= increaseIntTracks->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= optimize->GetValue()==true?wxT("1"):wxT("0");
	currentFrmwork->readCommand(string(cmd.mb_str()));
	update();
}

void WxAutoCell::compact( wxCommandEvent& event ){
	wxString cmd=wxT("cellgen compact ");
    cmd+= diffStretching->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= griddedPolly->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= wxString::Format(_T("%d"), rdCntsCost->GetValue());
    cmd+= wxT(" ");
    cmd+= wxString::Format(_T("%d"), maxDiffCnts->GetValue());
    cmd+= wxT(" ");
    cmd+= alignDiffCnts->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= reduceLturns->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= enableDFM->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= experimental->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= debug->GetValue()==true?wxT("1"):wxT("0");
    cmd+= wxT(" ");
    cmd+= timeLimit->GetValue();
	currentFrmwork->readCommand(string(cmd.mb_str()));
	update();
}

void WxAutoCell::view( wxCommandEvent& event ){
	currentFrmwork->viewLayout(string(cellnet_choice->GetStringSelection().mb_str()));
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
	fold_button->Enable(state>=1);
	place_button->Enable(state>=3);
	route_button->Enable(state>=4);
	compact_button->Enable(state>=5);
	view_button->Enable(state>=6);
	Update();
}