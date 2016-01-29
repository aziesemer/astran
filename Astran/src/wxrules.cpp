#include "wxrules.h"

WxRules::WxRules( IcpdFrm* frmwork ):Design_Rules( frmwork ){
	currentFrmwork=frmwork;
	refresh();
}

void WxRules::PressedKey( wxKeyEvent& event ){
	if (event.GetKeyCode() == WXK_ESCAPE)
		close();
	event.Skip();
}

void WxRules::hide( wxCloseEvent& event ){
	Show(false);
}

void WxRules::saveButtonEvt( wxCommandEvent& event ){
	save();
}

void WxRules::closeButtonEvt( wxCommandEvent& event ){
	close();
}

void WxRules::selectRule( wxCommandEvent& event ){
	// select the rule in selection box
	wxRule_Value->SetValue(wxString::Format(_T("%f"), static_cast<float>(currentFrmwork->getDesign()->getRules()->getRulef((rule_name)wxSelect_Rule->GetSelection()))));
	// update window according to the the selected rule
    static_RuleDesc->SetLabel(wxString::From8BitData(currentFrmwork->getDesign()->getRules()->getRuleDesc((rule_name)wxSelect_Rule->GetSelection()).c_str()));
}

void WxRules::selectLayer( wxCommandEvent& event ){
	// select the layer label in selection box
	static_LayerDesc->SetLabel(wxString::From8BitData(currentFrmwork->getDesign()->getRules()->getLayerDesc((layer_name)wxSelect_Layer->GetSelection()).c_str()));
	// update window according to the the selected layer label
	wxCIF->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getRules()->getCIFVal((layer_name)wxSelect_Layer->GetSelection()).c_str()));
	wxGDSII->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getRules()->getGDSIIVal((layer_name)wxSelect_Layer->GetSelection()).c_str()));
	wxTechVal->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getRules()->getTechVal((layer_name)wxSelect_Layer->GetSelection()).c_str()));
}

void WxRules::save(){
	wxString cmd;

	// compare if current technology name is different from textbox value
	if(string(wxTechName->GetValue().mb_str()) != currentFrmwork->getDesign()->getRules()->getCIFVal((layer_name)TECHNAME)){
		cmd=wxT("set technology name ") + wxTechName->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}

	// compare if current number of metal layers is different from textbox value
	if(string(wxMLayers->GetValue().mb_str()) != currentFrmwork->getDesign()->getRules()->getCIFVal((layer_name)MLAYERS)){
		cmd=wxT("set technology MLayers ") + wxMLayers->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}

	// compare if SOI value changed to YES
	if(wxSOI->GetValue() && currentFrmwork->getDesign()->getRules()->getCIFVal((layer_name)SOI) != "YES"){
		cmd=wxT("set technology SOI YES");
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}

	// compare if SOI value changed to NO
	if(!wxSOI->GetValue() && currentFrmwork->getDesign()->getRules()->getCIFVal((layer_name)SOI) == "YES"){
		cmd=wxT("set technology SOI NO");
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}

	// compare if current resolution is different from textbox value
	if(wxResolution->GetValue() != wxString::Format(_T("%d"), currentFrmwork->getDesign()->getRules()->getScale())){
		cmd=wxT("set technology resolution ") + wxResolution->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
		refresh();
	}

	// compare if current rule value is different from textbox value
	if(wxRule_Value->GetValue() != wxString::Format(_T("%f"), currentFrmwork->getDesign()->getRules()->getRulef((rule_name)wxSelect_Rule->GetSelection()))){
		cmd=wxT("set technology rule ") +  wxSelect_Rule->GetStringSelection() + wxT(" ") + wxRule_Value->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}

	// compare if current layers values are different from textboxes values
	if(string(wxCIF->GetValue().mb_str()) != currentFrmwork->getDesign()->getRules()->getCIFVal((layer_name)wxSelect_Layer->GetSelection())){
		cmd=wxT("set technology CIF ") +  wxSelect_Layer->GetStringSelection() + wxT(" ") + wxCIF->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}
	if(string(wxGDSII->GetValue().mb_str()) != currentFrmwork->getDesign()->getRules()->getGDSIIVal((layer_name)wxSelect_Layer->GetSelection())){
		cmd=wxT("set technology GDSII ") +  wxSelect_Layer->GetStringSelection() + wxT(" ") + wxGDSII->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}
	if(string(wxTechVal->GetValue().mb_str()) != currentFrmwork->getDesign()->getRules()->getTechVal((layer_name)wxSelect_Layer->GetSelection())){
		cmd=wxT("set technology val_tech ") +  wxSelect_Layer->GetStringSelection() + wxT(" ") + wxTechVal->GetValue();
		currentFrmwork->executeCommand(string(cmd.mb_str()));
	}
}

void WxRules::close(){
	refresh();
    Show(false);
}

void WxRules::refresh(){
	// set the current values for the textboxes
	wxTechName->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getRules()->getCIFVal((layer_name)TECHNAME).c_str()));
	wxMLayers->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getRules()->getCIFVal((layer_name)MLAYERS).c_str()));
	wxResolution->SetValue(wxString::Format(_T("%d"), currentFrmwork->getDesign()->getRules()->getScale()));
	if (currentFrmwork->getDesign()->getRules()->getCIFVal((layer_name)SOI) == "YES")
		wxSOI->SetValue(true);
	else
		wxSOI->SetValue(false);

	wxSelect_Rule->Clear();
	wxSelect_Layer->Clear();

	if(currentFrmwork->getDesign()){
		// append the itens (rules) for the selection box
		for(int i=0; i<currentFrmwork->getDesign()->getRules()->getRulesSize() ; i++)
			wxSelect_Rule->Append(wxString::From8BitData(currentFrmwork->getDesign()->getRules()->getRuleName(i).c_str()));
		wxSelect_Rule->Select(0);

		// append the itens (layer labels) for the selection box
		for(int i=0; i<currentFrmwork->getDesign()->getRules()->getLayersSize() ; i++)
			wxSelect_Layer->Append(wxString::From8BitData(currentFrmwork->getDesign()->getRules()->getLayerName(i).c_str()));
		wxSelect_Layer->Select(0);

		// set the current values according to the the selected rule
		wxRule_Value->SetValue(wxString::Format(_T("%f"), currentFrmwork->getDesign()->getRules()->getRulef((rule_name)wxSelect_Rule->GetSelection())));
		static_RuleDesc->SetLabel(wxString::From8BitData(currentFrmwork->getDesign()->getRules()->getRuleDesc((rule_name)wxSelect_Rule->GetSelection()).c_str()));

		// set the current values according to the the selected layer label
		static_LayerDesc->SetLabel(wxString::From8BitData(currentFrmwork->getDesign()->getRules()->getLayerDesc((layer_name)wxSelect_Rule->GetSelection()).c_str()));
		wxCIF->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getRules()->getCIFVal((layer_name)wxSelect_Layer->GetSelection()).c_str()));
		wxGDSII->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getRules()->getGDSIIVal((layer_name)wxSelect_Layer->GetSelection()).c_str()));
		wxTechVal->SetValue(wxString::From8BitData(currentFrmwork->getDesign()->getRules()->getTechVal((layer_name)wxSelect_Layer->GetSelection()).c_str()));
	}
}