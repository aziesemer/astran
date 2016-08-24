#include "icpdfrm.h"

/** Constructor */
IcpdFrm::IcpdFrm( wxWindow* parent ):ICPD_frm( parent ){
	wxLog::SetActiveTarget( new wxLogTextCtrl(wx_log));
	
    new Redirector( wx_log, cout, false);
    new Redirector( wx_log, cerr, true);
    
	printHeader(cout, "", "");
    
	string astran_cfg;
    astran_cfg = "astran.cfg";

    wxString astran_path;
    ::wxGetEnv(wxT("ASTRAN_PATH"), &astran_path);
	
	if (wxDirExists(astran_path)) {
		astran_cfg = string(wxString(astran_path).mb_str()) + "/bin/astran.cfg";
	}

	executeCommand(string("read \"" + astran_cfg + "\""));
    wxabout = new WxAbout(this);
    wxrules = new WxRules(this);
    wxautocell = new WxAutoCell(this);
    wxcircuit = new WxCircuit(this);
    wxfp = new WxFP(this);
    wxpreferences = new WxPreferences(this);
    refresh();
}

// functions that implement events

// Frame events
void IcpdFrm::OnClose(wxCloseEvent& event){
	executeCommand("exit");
}

void IcpdFrm::CmdKeyDown( wxKeyEvent& event ){
	pos = designmng.getPosCmdLog();
    
	if (event.GetKeyCode() == WXK_UP && pos != 0){
		--pos;
		designmng.setPosCmdLog(pos);
		wxCommand->SetValue(wxString::From8BitData(designmng.getCommandLog()[pos].c_str()));
		wxCommand->SetInsertionPointEnd();
	}
	else if (event.GetKeyCode() == WXK_DOWN && pos != designmng.getCommandLog().size()){
        ++pos;
        if (pos == designmng.getCommandLog().size())
            wxCommand->SetValue(wxT(""));
        else
            wxCommand->SetValue(wxString::From8BitData(designmng.getCommandLog()[pos].c_str()));
        designmng.setPosCmdLog(pos);
        wxCommand->SetInsertionPointEnd();
	}else if (event.GetKeyCode() == WXK_ESCAPE)
		wxCommand->SetValue(wxT(""));
	else if (event.GetKeyCode() == WXK_F5)
		refresh();
	else if (event.GetKeyCode() == WXK_F1)
		wxabout->Show();
	else event.Skip();
}

void IcpdFrm::PressedKey( wxKeyEvent& event ){
	if (event.GetKeyCode() == WXK_F5)
		refresh();
	else if (event.GetKeyCode() == WXK_F1)
		wxabout->Show();
	event.Skip();
}

// File->New
void IcpdFrm::newDesign( wxCommandEvent& event ){
	wxString cmd = wxGetTextFromUser(wxT("Enter the name: "),wxT("New Design"),wxT(""));
	if(cmd!=wxT("")){
		cmd=wxT("new design \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
        
		instanceLevel.clear();
		refresh();
	}
}

void IcpdFrm::newCell( wxCommandEvent& event ){
	wxString cmd = wxGetTextFromUser(wxT("Enter the cell name: "),wxT("Add a new cell netlist"),wxT(""));
	if(cmd!=wxT("")){
		cmd=wxT("new cell ") + cmd;
		executeCommand(string(cmd.mb_str()));
		refresh();
	}
}

// File->Load
void IcpdFrm::loadProject( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Open Project"), wxT(""), wxT(""), wxT("ICPD Project File (*.icp)|*.icp|All Files (*)|*"), wxFD_OPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("load project \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
		refresh();
    }
}

void IcpdFrm::loadTech( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Open Technology"), wxT(""), wxT(""), wxT("Rules File (*.rul)|*.rul|All Files (*)|*"), wxFD_OPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("load technology \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
    }
}

void IcpdFrm::loadSpiceNetlist( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Open Netlist"), wxT(""), wxT(""), wxT("Spice (*.sp)|*.sp|Structural Verilog (*.v)|*.v|All Files (*)|*"), wxFD_OPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("load netlist \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
		refresh();
	}
}

void IcpdFrm::loadLayouts( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Open Layouts"), wxT(""), wxT(""), wxT("ICPD Layouts Format (*.lay)|*.lay|All Files (*)|*"), wxFD_OPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("load layouts \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
		refresh();
	}
}

void IcpdFrm::loadPlacement( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Open Placement"), wxT(""), wxT(""), wxT("Book Shelf (*.pl)|*.pl|Mango Parrot (*.mpp)|*.mpp|All Files (*)|*"), wxFD_OPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("load placement \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
		refresh();
	}
}

void IcpdFrm::loadRouting( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Open Routing"), wxT(""), wxT(""), wxT("Rotdl Routing Format (*.rot)|*.rot|All Files (*)|*"), wxFD_OPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("load routing \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
		refresh();
	}
}

// File->Save
void IcpdFrm::saveProject( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Project"), wxT(""), wxT(""), wxT("ICPD Project File (*.icp)|*.icp"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("save project \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
	}
}

void IcpdFrm::saveTech( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Rules"), wxT(""), wxT(""), wxT("Rules File (*.rul)|*.rul"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("save technology \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
	}
}

void IcpdFrm::saveSpiceNetlist( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Netlist"), wxT(""), wxT(""), wxT("Spice (*.sp)|*.sp"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("save netlist \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
	}
}

void IcpdFrm::saveLayouts( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Layouts"), wxT(""), wxT("project.lay"), wxT("ICPD Layouts File (*.lay)|*.lay"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("save layouts \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
	}
}

void IcpdFrm::savePlacement( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Placement"), wxT(""), wxT(""), wxT("Book Shelf (*.pl)|*.pl"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("save placement \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
	}
}

void IcpdFrm::saveRouting( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Routing"), wxT(""), wxT(""), wxT("Rotdl Routing Format (*.rot)|*.rot"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("save routing \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
	}
}

// File->Import
void IcpdFrm::importSpiceNetlist( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Import Netlist"), wxT(""), wxT(""), wxT("All Files (*)|*|Spice (*.sp)|*.sp"), wxFD_OPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("import netlist \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
		refresh();
	}
}

void IcpdFrm::importLEF( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Import Cell Library"), wxT(""), wxT(""), wxT("Library Exchange Format (*.lef)|*.lef|All Files (*)|*"), wxFD_OPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("import lef \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
		refresh();
	}
}

// File->Export
void IcpdFrm::exportLayout( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Layout"), wxT(""), wx_layouts->GetStringSelection()+wxT(".cif"), wxT("Calltech Intermediate Format (*.cif)|*.cif|Graphic Data System II(*.gds)|*.gds|CIF2Cadende Convertion File (*.c2c)|*.c2c"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("export layout ") + wx_layouts->GetStringSelection() + wxT(" \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
	}
}

void IcpdFrm::exportCellSizes( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Library Dimensions"), wxT(""), wxT(""), wxT("Mango Parrot Cell estimative File (*.est)|*.est"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("export cellsizes \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
	}
}

void IcpdFrm::exportPlacement( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save Placement"), wxT(""), wxT(""), wxT("Cadence Placement File (*.place)|*.place"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("export placement \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
	}
}

// File Script
void IcpdFrm::flReadScript( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Read Script"), wxT(""), wxT(""), wxT("Script File (*.run)|*.run|All Files (*)|*"), wxFD_OPEN);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("read \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
		refresh();
	}
}

void IcpdFrm::saveHistory( wxCommandEvent& event ){
	wxFileDialog* tmp =  new wxFileDialog(this, wxT("Save History"), wxT(""), wxT(""), wxT("Script File (*.run)|*.run"), wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
	if ( tmp->ShowModal() == wxID_OK ){
		wxString cmd = tmp->GetPath();
		cmd = wxT("save history \"") + cmd + wxT("\"");
		executeCommand(string(cmd.mb_str()));
	}
}

// File Exit
void IcpdFrm::OnFileExit( wxCommandEvent& event ){
	executeCommand("exit");
}

// View
void IcpdFrm::showTech( wxCommandEvent& event ){
	wxrules->Show(true);
    wxrules->Iconize(false);
    wxrules->SetFocus();
    wxrules->Raise();
}

void IcpdFrm::showCircuit( wxCommandEvent& event ){
	wxcircuit->Show(true);
    wxcircuit->Iconize(false);
    wxcircuit->SetFocus();
    wxcircuit->Raise();
}

void IcpdFrm::showPreferences( wxCommandEvent& event ){
	wxpreferences->Show(true);
    wxpreferences->Iconize(false);
    wxpreferences->SetFocus();
    wxpreferences->Raise();
}

// Cells
void IcpdFrm::showCellgen( wxCommandEvent& event ){
	wxautocell->Show(true);
    wxautocell->Iconize(false);
    wxautocell->SetFocus();
    wxautocell->Raise();
}

// Place
void IcpdFrm::showFP( wxCommandEvent& event ){
	wxfp->Show(true);
    wxfp->Iconize(false);
    wxfp->SetFocus();
    wxfp->Raise();
}

void IcpdFrm::calcPinsPos( wxCommandEvent& event ){
	executeCommand("calcPinsPos");
}

void IcpdFrm::plTerminals( wxCommandEvent& event ){
	executeCommand("place terminals");
}

void IcpdFrm::fastp( wxCommandEvent& event ){
	executeCommand("place fplace");
}

void IcpdFrm::plVerify( wxCommandEvent& event ){
	executeCommand("place check");
}

void IcpdFrm::plAutoFlip( wxCommandEvent& event ){
	executeCommand("place autoflip");
}

void IcpdFrm::plIncremental( wxCommandEvent& event ){
	executeCommand("place incremental");
}

void IcpdFrm::plWL( wxCommandEvent& event ){
	executeCommand("place getwl");
}

// Route
void IcpdFrm::rotdl( wxCommandEvent& event ){
	executeCommand("route rotdl");
	refresh();
}

void IcpdFrm::pfinder( wxCommandEvent& event ){
	wxString cmd = wxGetTextFromUser(wxT("Enter the number of iterations (ex. 1000): "),wxT("Negociation Based Router"),wxT(""));
	if(cmd!=wxT("")){
		cmd=wxT("route pfinder ") + cmd;
		executeCommand(string(cmd.mb_str()));
		refresh();
	}
}

void IcpdFrm::rtOptimize( wxCommandEvent& event ){
	executeCommand("route optimize");
	refresh();
}

void IcpdFrm::rtCompact( wxCommandEvent& event ){
	executeCommand("route compact");
	refresh();
}

void IcpdFrm::rtClear( wxCommandEvent& event ){
	executeCommand("route clear");
	refresh();
}

// Help
void IcpdFrm::HelpaboutClick( wxCommandEvent& event ){
	wxabout->Show();
}

void IcpdFrm::helpHP( wxCommandEvent& event ){
	wxLaunchDefaultBrowser(wxT("http://www.inf.ufrgs.br/~amziesemerj/icpd"));
}

// Tools
void IcpdFrm::refreshIcon( wxCommandEvent& event ){
	refresh();
}

// List Boxes
void IcpdFrm::layoutListEvent( wxCommandEvent& event ){
	string selectedLayout=string(wx_layouts->GetStringSelection().mb_str());
	instanceLevel.clear();
	instanceLevel.push_back(designmng.getCircuit()->getLayout(selectedLayout));
	refreshInstanceList();
}

void IcpdFrm::viewLayout( wxCommandEvent& event ){
	viewLayout(string(wx_layouts->GetStringSelection().mb_str()));
}

void IcpdFrm::viewLayout( string cellName ){
	if(designmng.getViewer()!=""){
		Cif *cif=new Cif("temp.cif", *(designmng.getRules()));
		cif->cellCif(*(designmng.getCircuit()->getLayouts()), cellName);
		delete(cif);
		string cmd = designmng.getViewer() + " temp.cif";
		cout << "-> Opening viewer: " << cmd << endl;
		icpdExecute(cmd);
	}
}

void IcpdFrm::openInstancewindow( wxCommandEvent& event ){
	//wxinstance->open(wx_layouts->GetStringSelection(), wx_instances->GetStringSelection());
}

void IcpdFrm::printInstance( wxCommandEvent& event ){
	if(wx_instances->GetStringSelection()==wxT("..")){
		instanceLevel.pop_back();
		refreshInstanceList();
	}else{
		string cmd = "print instance ";
		cmd += instanceLevel.back()->getName();
		cmd += " " + string(wx_instances->GetStringSelection().mb_str());
		executeCommand(cmd);
		CLayout *tmp=designmng.getCircuit()->getLayout(instanceLevel.back()->getInstance(string(wx_instances->GetStringSelection().mb_str()))->getTargetCell());
		if(tmp){
			instanceLevel.push_back(tmp);
			refreshInstanceList();
		}
		else cout << "-> No further hierarchy" << endl;
	}
	wx_layouts->Select(wx_layouts->FindString(wxString::From8BitData(instanceLevel.back()->getName().c_str())));
}

void IcpdFrm::printCell( wxCommandEvent& event ){
	wxString cmd = wxT("print cell ") + wx_cells->GetStringSelection(); 
	executeCommand(string(cmd.mb_str()));
}

void IcpdFrm::printNet( wxCommandEvent& event ){
	wxString cmd = wxT("print net ") + wx_nets->GetStringSelection();
	executeCommand(string(cmd.mb_str()));
}

void IcpdFrm::printInterface( wxCommandEvent& event ){
	wxString cmd = wxT("print interface ") + wx_interfaces->GetStringSelection();
	executeCommand(string(cmd.mb_str()));
}

// Shell
void IcpdFrm::command( wxCommandEvent& event ){
	string cmd=string(wxCommand->GetValue().mb_str());
	wxCommand->SetValue(_T(""));
	executeCommand(cmd.c_str());
}

void IcpdFrm::executeCommand(string cmd){
	statusBar->SetStatusText(wxT("Executing command..."));
    statusBar->Refresh();
    Update();
	designmng.readCommand(cmd);
	statusBar->SetStatusText(wxT(""));
}

// functions that don't implement events
void IcpdFrm::refresh(){
    wx_layouts->DeselectAll();
	wx_layouts->Clear();
	map<string, CLayout >::iterator layouts_it;
	for(layouts_it=designmng.getCircuit()->getLayouts()->begin(); layouts_it!=designmng.getCircuit()->getLayouts()->end(); layouts_it++)
		wx_layouts->Append(wxString::From8BitData(layouts_it->first.c_str()));
	if (instanceLevel.size())
		wx_layouts->Select(wx_layouts->FindString(wxString::From8BitData(instanceLevel.back()->getName().c_str())));
    
    wx_cells->DeselectAll();
	wx_cells->Clear();
	map<string, CellNetlst>::iterator cells_it;
	for(cells_it=designmng.getCircuit()->getCellNetlsts()->begin(); cells_it!=designmng.getCircuit()->getCellNetlsts()->end(); cells_it++)
		wx_cells->Append(wxString::From8BitData(cells_it->first.c_str()));
    
    instanceLevel.clear();
	refreshInstanceList();
    
    /*	wx_nets->Clear();
     map<string, list<Net> >::iterator tmp3_it;
     for(tmp3_it=designmng.getCircuit()->getNetList()->begin(); tmp3_it!=designmng.getCircuit()->getNetList()->end(); tmp3_it++)
     wx_nets->Append(tmp3_it->first.c_str());
     //wx_nets->Select(0);
     */
	wx_interfaces->DeselectAll();
	wx_interfaces->Clear();
	map<string, Circuit::Interface>::iterator tmp4_it;
	for(tmp4_it=designmng.getCircuit()->getInterfaces()->begin(); tmp4_it!=designmng.getCircuit()->getInterfaces()->end(); tmp4_it++)
		wx_interfaces->Append(wxString::From8BitData(tmp4_it->first.c_str()));
    
	wxautocell->refresh();
	wxcircuit->refresh();
	wxrules->refresh();
	wxfp->refresh();
	wxpreferences->refresh();
}

void IcpdFrm::refreshInstanceList(){
	wx_instances->Clear();
	if(instanceLevel.size()){
        if(instanceLevel.size()>1) wx_instances->Append(wxT(".."));
        map<string, Instance>::iterator tmp2_it;
        for(tmp2_it=instanceLevel.back()->getInstances()->begin(); tmp2_it!=instanceLevel.back()->getInstances()->end(); tmp2_it++)
            wx_instances->Append(wxString::From8BitData(tmp2_it->first.c_str()));
//        wx_instances->Select(0);
	}
}