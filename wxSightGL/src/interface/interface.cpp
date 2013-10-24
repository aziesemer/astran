/*
 * hworld.cpp
 * Hello world sample by Robert Roebling
 */

#include <wx/stdpaths.h>
#include <wx/file.h>
#include <wx/artprov.h>
#include <wx/mstream.h>
#include <wx/image.h>

#include "../art/cif-png.h"
#include "../art/drag-png.h"
#include "../art/label-png.h"
#include "../art/line-png.h"
#include "../art/minus-png.h"
#include "../art/plus-png.h"
#include "../art/select-png.h"
#include "../art/rectangle-png.h"
#include "../art/ruler-png.h"
#include "../art/select-png.h"
#include "../art/sight-png.h"
#include "../art/wire-png.h"
#include "../art/zoom-png.h"

#define wxGetImageFromMemory(name) _wxGetImageFromMemory(name ## _img, sizeof(name ## _img))
inline wxImage _wxGetImageFromMemory(const unsigned char *data, int length) {
	wxMemoryInputStream is(data, length);
	return wxImage(is, wxBITMAP_TYPE_PNG);
} // end method

#include "interface.h"

SightGL sight;

BEGIN_EVENT_TABLE(wxSightFrame, wxFrame)
	//EVT_KEY_UP(wxSightFrame::keyPressed)
		
	EVT_MENU(wxID_EXIT, wxSightFrame::OnQuit)
	EVT_MENU(wxID_OPEN,wxSightFrame::Open)	
	EVT_MENU(wxID_SAVEAS,wxSightFrame::SaveAs)
	
	EVT_MENU(ID_Reload,wxSightFrame::Reload)
	
	EVT_MENU(ID_ASK,wxSightFrame::Ask)
	
	EVT_MENU(ID_SaveSS,wxSightFrame::saveSnapShot)
	
	EVT_MENU(ID_Cut, wxSightFrame::OnCut)
	EVT_MENU(ID_Copy, wxSightFrame::OnCopy)
	EVT_MENU(ID_Paste, wxSightFrame::OnPaste)
	EVT_MENU(ID_Delete, wxSightFrame::OnDelete)
	
	EVT_MENU(ID_Group, wxSightFrame::OnGroup)
	EVT_MENU(ID_Ungroup, wxSightFrame::OnUngroup)
	EVT_MENU(ID_NW_Model, wxSightFrame::OnModel)
	EVT_MENU(ID_NW_Component, wxSightFrame::OnComponent)
	EVT_MENU(ID_Lbl, wxSightFrame::editLabel)
	
	EVT_MENU(ID_BGCOLOR, wxSightFrame::changeBgColor)
	EVT_MENU(ID_GCOLOR, wxSightFrame::changeGuideColor)	
	EVT_MENU(ID_SLABEL, wxSightFrame::showLabel)
	EVT_MENU(ID_SLAYER, wxSightFrame::showLayerList)
	
	EVT_TOOL( SELECTION , wxSightFrame::selection )
	EVT_TOOL( RECTANGLE, wxSightFrame::rectangle )
	EVT_TOOL( LINE, wxSightFrame::line )
	EVT_TOOL( WIRE, wxSightFrame::wire )
	EVT_TOOL( RULER, wxSightFrame::ruler )
	EVT_TOOL( DRAG, wxSightFrame::drag )
	EVT_TOOL( LABEL, wxSightFrame::insertLabel)
	
	EVT_TOOL( 100, wxSightFrame::hideTool )
	
	EVT_TOOL( ZOOM, wxSightFrame::fitIn )	
	EVT_TOOL( ZOOMIN, wxSightFrame::zoomIn )
	EVT_TOOL( ZOOMOUT, wxSightFrame::zoomOut )
	
	EVT_TOOL( CLEAR_RULERS, wxSightFrame::clearRulers )
	
	EVT_LAYER_LIST_CHECK(LL, wxSightFrame::OnShowLayer )
	EVT_LAYER_LIST_SELECTION_CHANGED( wxID_ANY, wxSightFrame::OnChangeSelectedLayer) 
	
	EVT_COMBOBOX(MDL, wxSightFrame::OnModelChoice)
	
	EVT_MENU(ID_Save_Model, wxSightFrame::SaveModel)
	EVT_MENU(ID_Save_Model_As, wxSightFrame::SaveModelAs)
	
	

END_EVENT_TABLE()

class Redirector : public std::streambuf {
	private:
		wxTextCtrl *clsTextCtrl;

	protected:
		int overflow(int i) {
			clsTextCtrl->WriteText( wxString( wxChar( i ) ) );
			return i;
		}

	public:
		Redirector( wxTextCtrl *text, ostream &out ) : clsTextCtrl( text ) {
			out.rdbuf( this );
		}

}; // end class



wxSightFrame::wxSightFrame( const wxString& title, const wxPoint& pos, const wxSize& size)
: wxFrame( NULL, wxID_ANY, title, pos, size )
{
} // end constructor

// -----------------------------------------------------------------------------

void wxSightFrame::Initialize() {
	// Create a new wxAuiManager and tell it to manage this window.
	clsAuiManager = new wxAuiManager( this,wxAUI_MGR_ALLOW_FLOATING | wxAUI_MGR_RECTANGLE_HINT );
	
	// Crete menubar, toolbar and statusbar.
	CreateMenubar();
	CreateToolbar();
	CreateStatusbar();

	// Create panes;
	//CreateLayerPane();
	CreateLayerListPane();
	CreateSightPane();
	//CreateDebugPane();
	
	clsModelsCombo = NULL;

	// Define the window min size.
	SetMinSize(wxSize(450,450));

	// Comitt.
	clsAuiManager->Update();
} // end method

// -----------------------------------------------------------------------------

void wxSightFrame::CreateToolbar() {	
	wxString resourcePath = wxStandardPaths::Get().GetResourcesDir() + _T("/"); // [TODO] How do get the correct slash on each plataform?
	
	wxBitmap bitmaps[] = {
		wxGetImageFromMemory(select),
		wxGetImageFromMemory(rectangle),
		wxGetImageFromMemory(wire),
		wxGetImageFromMemory(line),
		wxGetImageFromMemory(ruler),
		wxGetImageFromMemory(drag),
		wxGetImageFromMemory(label),
		wxGetImageFromMemory(zoom),
		wxGetImageFromMemory(plus),
		wxGetImageFromMemory(minus)
	
	};
	
	const wxSize bitmapSize( 32, 32 );
		
	// Create tool bar
	stateToolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_NODIVIDER);
	stateToolBar->SetToolBitmapSize( bitmapSize );
					 	
	stateToolBar->AddRadioTool(SELECTION, wxT("Select"   ), bitmaps[ 0 ], bitmaps[ 0 ], _T("Select"   ));
	stateToolBar->AddRadioTool(RECTANGLE, wxT("Rectangle"), bitmaps[ 1 ], bitmaps[ 1 ], _T("Rectangle"));
	stateToolBar->AddRadioTool(WIRE     , wxT("Wire"     ), bitmaps[ 2 ], bitmaps[ 2 ], _T("Wire"     ));
	stateToolBar->AddRadioTool(LINE     , wxT("Line"     ), bitmaps[ 3 ], bitmaps[ 3 ], _T("Line"     ));
	stateToolBar->AddRadioTool(RULER    , wxT("Ruler"    ), bitmaps[ 4 ], bitmaps[ 4 ], _T("Ruler"    ));		
	stateToolBar->AddRadioTool(DRAG     , wxT("Drag"     ), bitmaps[ 5 ], bitmaps[ 5 ], _T("Drag"     ));	
	stateToolBar->AddRadioTool(LABEL    , wxT("Label"    ), bitmaps[ 6 ], bitmaps[ 6 ], _T("Label"    ));	
	stateToolBar->AddSeparator();
	stateToolBar->AddTool(ZOOM   , wxT("Fit in"  ), bitmaps[ 7 ], wxT("Fit in"  ));
	stateToolBar->AddTool(ZOOMIN , wxT("Zoom in" ), bitmaps[ 8 ], wxT("Zoom in" ));
	stateToolBar->AddTool(ZOOMOUT, wxT("Zoom out"), bitmaps[ 9 ], wxT("Zoom out"));
	stateToolBar->AddSeparator();														
	stateToolBar->Realize();
	
	clsAuiManager->AddPane(stateToolBar, wxAuiPaneInfo().
							Name(wxT("ToolBar")).Caption(wxT("ToolBar")).
							ToolbarPane().Top().
							LeftDockable(false).RightDockable(false));
} // end method



// -----------------------------------------------------------------------------

void wxSightFrame::CreateModelsCombo(){
	const int size = clsModels.size();
	//wxString choices[size];
	wxString *choices = new wxString[clsModels.size()];

	for(int i =0; i <  size; i++){
		choices[i] = clsModels[i];
	}
	
	if(!clsModelsCombo){
		clsModelsCombo = new wxComboBox(this, MDL, choices[size-1],wxDefaultPosition, wxDefaultSize, size, choices, wxCB_SORT|wxCB_READONLY, wxDefaultValidator);
		clsAuiManager->AddPane( clsModelsCombo, wxAuiPaneInfo().Top().ToolbarPane().Position(1) );//*/			
	} else {
		clsModelsCombo->Clear();
		for(int i =0; i <  size; i++){
			clsModelsCombo->Append(choices[i]);
		}
		clsModelsCombo->SetValue(choices[size-1]);
	}//end else
	clsAuiManager->Update();
}

// -----------------------------------------------------------------------------
void wxSightFrame::CreateMenubar(){
	wxMenuBar *menuBar = new wxMenuBar;
	//menu file++++++++++++++++++++++++++++++++++
	wxMenu *menuFile = new wxMenu;
	menuFile->Append( wxID_ABOUT, _T("About..\tCtrl+A") );
	menuFile->AppendSeparator();
	menuFile->Append( wxID_NEW, _T("New\tCtrl+N") );
	menuFile->Append( wxID_OPEN, _T("Open..\tCTRL+O") );
	menuFile->AppendSeparator();	
	menuFile->Append( ID_Reload, _T("Reload..\tCTRL+R") );
	menuFile->AppendSeparator();
//	menuFile->Append( ID_Save, _T("&Save") );
	menuFile->Append( wxID_SAVEAS, _T("Save As..\tCtrl+S") );
	menuFile->Append( ID_SaveSS, wxT("Save Snapshot\tCtrl+D") );
	menuFile->AppendSeparator();
	menuFile->Append( wxID_EXIT, _T("Exit\tCtrl+Q") );    
	menuBar->Append( menuFile, _T("&File") );    


//[TODO]passar dados p/ area de transferencia do SO e habilitar copy/cut/paste 
	//menu edit++++++++++++++++++++++++++++++++++
	wxMenu *menuEdit = new wxMenu;
	menuEdit->Append(ID_Cut, _T("Cut\tCtrl+X"));
	//menuEdit->AppendSeparator();
	menuEdit->Append(ID_Copy, _T("Copy\tCtrl+C"));
	//menuEdit->AppendSeparator();
	menuEdit->Append(ID_Paste, _T("Paste\tCtrl+V"));
	menuEdit->AppendSeparator();
	menuEdit->Append(ID_Delete, _T("Delete"));	
	menuBar->Append(menuEdit, _T("&Edit"));

	//menu object+++++++++++++++++++++++++++++++
	wxMenu *menuObject = new wxMenu;
	menuObject->Append(ID_Group, _T("Group\tCtrl+G"));
	menuObject->Append(ID_Ungroup, _T("Ungroup\tCtrl+U"));
	menuObject->AppendSeparator();
	menuObject->Append(ID_NW_Component, _T("New Component\tCtrl+K"));
	menuObject->AppendSeparator();
	menuObject->Append(ID_Lbl, _T("Label\tCtrl+L"));	
	menuBar->Append(menuObject, _T("&Object"));
	
	//menu model+++++++++++++++++++++++++++++++
	wxMenu *menuModel = new wxMenu;
	menuModel->Append(ID_NW_Model, _T("New Model\tCtrl+M"));
	menuModel->Append(ID_Save_Model, _T("Save Model"));
	menuModel->Append(ID_Save_Model_As, _T("Save Model As.."));
	menuModel->AppendSeparator();
	menuBar->Append(menuModel, _T("&Model"));	
	

	//menu view+++++++++++++++++++++++++++++++++
	wxMenu *menuView = new wxMenu;
	//( menuView->AppendCheckItem(LL, _("Main tool"),_("shows/hide the main tool bar")) )->Check(true);
	//( menuView->AppendCheckItem(CHECK_LIST_BOX_LAYERS, _("Layers List"),_("shows/hide the list of layers")) )->Check(true);
	menuView->Append(ID_ASK, _("DoubleBuffer Support.."));
	menuEdit->AppendSeparator();	
	menuView->Append(ID_BGCOLOR, _("Change Background.."));
	menuView->Append(ID_GCOLOR, _("Change Guides color.."));
	menuView->AppendSeparator();
	(menuView->AppendCheckItem(ID_SLABEL, _("Show Labels"), _("shows/hide labels")) )->Check(true);
	menuView->Append(ID_SLAYER, _("Show Layer List"), _("shows/hide layer list")) ;
	menuBar->Append(menuView, _T("&View"));
//	wxMenu *menuHelp = new wxMenu;
//	menuHelp->Append(wxID_HELP, _T("SightGL Help"));
	//menuBar->Append(menuHelp, _T("Help"));
	SetMenuBar( menuBar );

} // end method
// -----------------------------------------------------------------------------

void wxSightFrame::CreateStatusbar() {
//configure status bar
  	CreateStatusBar();
    SetStatusText( _T("Selection") );
} // end method

// -----------------------------------------------------------------------------

void wxSightFrame::CreateLayerPane() {
	checkListBox =  new wxCheckListBox( this, CHECK_LIST_BOX_LAYERS, wxPoint( 0, 0 ), wxSize( 110, 100 ) );
	const int numLayers = sight.getNumLayers() - 1;
	for ( int l = 0; l < numLayers; l++ ) {
		wxString name =  wxString::FromAscii( sight.getLayerName(l).c_str() ); 	
		checkListBox->Append(name);			
		checkListBox->Check(l,true);
	} // end for	
	// Add panes.
	clsAuiManager->AddPane( checkListBox, wxAuiPaneInfo().Name( wxT( "LayerList") ).Caption( wxT("Layers") ).Left() );
} // end method

// -----------------------------------------------------------------------------

void wxSightFrame::CreateLayerListPane() {
	clsLayerList = new LayerList( this, LL );
	const int numLayers = sight.getNumLayers() - 1;
	for ( int l = 0; l < numLayers; l++ ) {
		wxString name =  wxString::FromAscii( sight.getLayerName(l).c_str() );
		StippleMask msk = sight.getLayerMask(l);
			unsigned char * mask = (unsigned char *) STIPPLE_MASKS[msk];		
		Color fll = sight.getLayerFillColor(l);
			wxColor fill = wxColor(fll.r, fll.g, fll.b);		
		Color ln = sight.getLayerFillColor(l);
			wxColor line = wxColor(ln.r, ln.g, ln.b);		
		clsLayerList->addItem( new LayerItem( clsLayerList, l, mask, name, fill, line ) );
	}					
	// Add panes.
	clsAuiManager->AddPane( clsLayerList, wxAuiPaneInfo().Name( wxT( "LayerList") ).Caption( wxT("Layers") ).Left() );
} // end method

// -----------------------------------------------------------------------------

void wxSightFrame::CreateDebugPane() {
	wxTextCtrl *txtDebug = new wxTextCtrl( this, wxID_ANY, wxT( "" ), wxDefaultPosition,
	wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE );

	clsAuiManager->AddPane( txtDebug, wxAuiPaneInfo().Name( wxT( "Debug") ).Caption( wxT("Debug") ).Bottom() );

	new Redirector( txtDebug, cout );
	new Redirector( txtDebug, cerr );
}

// -----------------------------------------------------------------------------

void wxSightFrame::CreateSightPane() {
#ifndef __linux__
        int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, WX_GL_LEVEL, 1, 0};
#else                
        int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
#endif
	basic = new BasicGLPane( this, args );		
	// Add panes.
	clsAuiManager->AddPane( basic, wxAuiPaneInfo().Name( wxT( "Sight") ).CenterPane() );

	
	basic->SetCurrent();
} // end method

// -----------------------------------------------------------------------------

void wxSightFrame::CreateModelList() {
	clsModels.clear();
	const int numModels = sight.getNumModels();
	for ( int l = 0; l < numModels; l++ ) {
		wxString name =  wxString::FromAscii( sight.getModelName(l).c_str() );
		clsModels.push_back(name);
	}			
	CreateModelsCombo();		
} // end method


// -----------------------------------------------------------------------------

void wxSightFrame::saveSnapShot(wxCommandEvent& event){
	wxString filename = wxFileSelector(_("File name"), _(""), _(""), _(""),
			_("PNG files (*.png)|*.png"), wxSAVE);
	
	const int w = sight.getWidth()   ;  // {TODO} verificar bug maior 660width
	const int h = sight.getHeight()   ; 
	
	Color * source = ( Color * ) sight.getPixels();
	Color * target = ( Color * ) malloc( w * h * sizeof( Color ) );
	
	for( int i = 0; i < h ; i++ ){
			for( int j = 0; j < w; j++){
				target[( (( h - 1  ) - i)*w + j ) ] = source[ ( (i * w) + j ) ];
			}//end for row
	}//end for line	

	wxImage image;
	image.SetOption(_("wxIMAGE_OPTION_PNG_FORMAT"),_("wxPNG_TYPE_COLOUR"));
	image.Create(w, h);
	image.SetData( (unsigned char*) target);
	image.SaveFile(filename + _(".png"), wxBITMAP_TYPE_PNG);
		
	delete [] source;
	delete [] target;
} // end method

// -----------------------------------------------------------------------------

void wxSightFrame::changeBgColor(wxCommandEvent& event){
	wxColour color = wxGetColourFromUser(this);
	sight.changeBlackgroundColor( color.Red()/255.0f, color.Green()/255.0f, color.Blue()/255.0f );
}

// -----------------------------------------------------------------------------

void wxSightFrame::changeGuideColor(wxCommandEvent& event){
	wxColour color = wxGetColourFromUser(this);
	sight.changeGuideColor( color.Red()/255.0f, color.Green()/255.0f, color.Blue()/255.0f );
}
// -----------------------------------------------------------------------------

void wxSightFrame::showLabel(wxCommandEvent& event){
	const int lbl = sight.getNumLayers()-1;
	sight.setLayerVisibility( lbl, event.IsChecked( ) );	
	sight.render();
}
// -----------------------------------------------------------------------------

void wxSightFrame::showLayerList(wxCommandEvent& event){
	clsAuiManager->RestorePane( wxAuiPaneInfo().Name( wxT( "LayerList") ).Caption( wxT("Layers") ).Left() );
	
	clsAuiManager->Update(); //*/
}

// -----------------------------------------------------------------------------




//==============================================================================
//EVENT FUNCTIONS:
//==============================================================================

//LAYERS
// -----------------------------------------------------------------------------
void wxSightFrame::OnChangeSelectedLayer( wxLayerListEvent &event ) {
	sight.setCurrentLayer(clsLayerList->getLayerID());
}
// -----------------------------------------------------------------------------
void wxSightFrame::OnShowLayer( wxLayerListEvent &event ) {
	int layer = event.GetInt();
	wxCheckBox *check = ( wxCheckBox * ) event.GetEventObject();
	sight.setLayerVisibility( layer, check->IsChecked( ) );	
	sight.render();
}
// -----------------------------------------------------------------------------


//STATES
// -----------------------------------------------------------------------------
void wxSightFrame::selection(wxCommandEvent& event){
	sight.setState( SIGHT_SELECT );
	SetStatusText( _T("Selection") );
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::rectangle(wxCommandEvent& event){
	sight.setState( SIGHT_RECTANGLE );
	SetStatusText( _T("Rectangle") );
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::ruler(wxCommandEvent& event){
	sight.setState( SIGHT_RULER );
	SetStatusText( _T("Ruler") );
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::line(wxCommandEvent& event){
	sight.setState( SIGHT_LINE );
	SetStatusText( _T("Line") );
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::wire(wxCommandEvent& event){
	sight.setState( SIGHT_WIRE );
	SetStatusText( _T("Wire") );
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::drag(wxCommandEvent& event){
	sight.setState( SIGHT_DRAG );
	SetStatusText( _("Drag") );
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::insertLabel(wxCommandEvent& event){
	sight.setState( SIGHT_LABEL );
	SetStatusText( _("Label") );	
}
// -----------------------------------------------------------------------------
void wxSightFrame::editLabel(wxCommandEvent& WXUNUSED(event)){
	wxString label = wxGetTextFromUser(_(""), _("Choose Label")).c_str();
	const char* lbl = (const char*)label.mb_str(wxConvUTF8);
	if( strcmp(lbl,"") ) {
		sight.setLabel(lbl);
	}//end if
}//end method
// -----------------------------------------------------------------------------


//MENUS
// -----------------------------------------------------------------------------
void wxSightFrame::OnQuit(wxCommandEvent& WXUNUSED(event)) {
    Close(TRUE);
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::OnAbout(wxCommandEvent& WXUNUSED(event)) {
    wxMessageBox(_T("This is a wxWindows Hello world sample"),
        _T("About Hello World"), wxOK | wxICON_INFORMATION, this);
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::OnCut(wxCommandEvent& WXUNUSED(event)){
	sight.cut();
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::OnCopy(wxCommandEvent& WXUNUSED(event)){
	sight.copy();
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::OnPaste(wxCommandEvent& WXUNUSED(event)){
	sight.paste();
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::OnDelete(wxCommandEvent& WXUNUSED(event)){
	sight.deleteObj();
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::OnGroup(wxCommandEvent& WXUNUSED(event)){
	sight.group();
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::OnUngroup(wxCommandEvent& WXUNUSED(event)){
	sight.ungroup();
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::New(wxCommandEvent& event){
	sight.newFile();
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::Open(wxCommandEvent& event){
	wxString filename = wxFileSelector(_T("Choose a file to open"), _(""), _(""), _(""), _("CIF files (*.cif)|*.cif|GDSII files (*.gds)|*.gds"));
	if ( !filename.empty() ){
		clsFilename = filename;
		if(filename.Contains(_(".cif"))){
			cerr<<"OPEN CIF\n";
			sight.loadCIF( filename.ToAscii() );
		} else {
			sight.loadGDSII( filename.ToAscii());
			cerr<<"OPEN GDS2\n";
		}
		setCompletTitle(filename);
		sight.render();		
		CreateModelList();
	}//else: cancelled by user
	
	
		
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::SaveAs(wxCommandEvent& event){
	wxString filename = wxFileSelector(_("File name"), _(""), _(""), _("cif"),_("*.cif"), wxSAVE);
	if( !filename.empty() ){
		sight.saveFile( filename.ToAscii() );
	}
		
}//end event
// -----------------------------------------------------------------------------
/*void wxSightFrame::enableTool(wxCommandEvent& event){
	//clsLayerList->EnableTool(event.GetId(), event.IsChecked() );
	clsLayerList->Show(event.IsChecked());
	topLayout->Show(2, event.IsChecked());
	topLayout->Layout();
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::enableList(wxCommandEvent& event){
	checkListBox->Show(event.IsChecked());
	
	if( event.IsChecked()){	
		splitter->SplitVertically( checkListBox, basic, 10 );
	}else{
		splitter->Unsplit(checkListBox);
	}

}//end event*/
// -----------------------------------------------------------------------------


//ZOOM
// -----------------------------------------------------------------------------
void wxSightFrame::zoomIn(wxCommandEvent& event){
	sight.zoom(true); //true=zoomIn
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::zoomOut(wxCommandEvent& event){
	sight.zoom(false);//false=zoomOut
}//end event
// -----------------------------------------------------------------------------
void wxSightFrame::fitIn(wxCommandEvent& event){	
	sight.fitIn();		
}//end event


// -----------------------------------------------------------------------------
void wxSightFrame::clearRulers(wxCommandEvent& event){
	sight.clearRulers();
}//end event
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
void wxSightFrame::hideTool(wxCommandEvent& event){
	//stateToolBar.Show(false);
}//end method
// -----------------------------------------------------------------------------
void wxSightFrame::setCompletTitle(wxString filename){
	/*
	wxString title = _("");
	size_t size = filename.Len()-1; 
	for( size_t i = size; i >=0; i--){
		if( filename[i] == '/' ) { 
			break; 
		} else {
			title = filename[i] + title;
		}//end else
	}//end for

	title = _("SightGL - ") + title;

	SetTitle(title);
	*/

	SetTitle(_("SightGL"));

}//end method
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void wxSightFrame::openFile( const char * const filename ) {
		sight.loadCIF( filename );
		setCompletTitle( wxString::FromAscii(filename) );
		sight.render();	
		CreateModelList();
		clsFilename = wxString::FromAscii(filename);
/*
	sight.loadCIF( filename );	
	sight.render();	 //*/	 
} // end method
// -----------------------------------------------------------------------------

void wxSightFrame::OnModelChoice(wxCommandEvent& event){
	if( !sight.dirtyModel() ){//if no changes
		sight.setCurrentModel( string(event.GetString().mb_str()) );
	} else {//save?
		int what = wxMessageBox(_(""), _("Save Changes?"), wxYES_NO, this);	
		if(what == wxYES){
			sight.saveModel();
		}
		sight.setCurrentModel( string(event.GetString().mb_str()));
	}//end else
}

// -----------------------------------------------------------------------------

void wxSightFrame::OnComponent(wxCommandEvent& WXUNUSED(event)){
	wxString label = wxGetTextFromUser(_(""), _("Choose Model Name")).c_str();
	const char* lbl = (const char*)label.mb_str(wxConvUTF8);
	if( strcmp(lbl,"") ) {
		sight.createNewModel(lbl);
	}//end if
	if( !sight.createNewComponent(lbl) ){
		 wxMessageBox(_T("This is a wxWindows Hello world sample"),
        _T("About Hello World"), wxOK | wxICON_INFORMATION, this);
	}
}
// -----------------------------------------------------------------------------

void wxSightFrame::OnModel(wxCommandEvent& WXUNUSED(event)){
	wxString label = wxGetTextFromUser(_(""), _("Choose Model Name")).c_str();
	const char* lbl = (const char*)label.mb_str(wxConvUTF8);
	if( strcmp(lbl,"") ) {
		if( sight.createNewModel(lbl) ){
			CreateModelList();
			sight.setCurrentModel( lbl );
		}//end iff
	}//end if
	
}

// -----------------------------------------------------------------------------

void wxSightFrame::SaveModel(wxCommandEvent& WXUNUSED(event)){
	sight.saveModel();//save changes on current model
}


// -----------------------------------------------------------------------------

void wxSightFrame::Reload(wxCommandEvent& WXUNUSED(event)){
	
	if ( !clsFilename.empty() ){
		if(clsFilename.Contains(_(".cif"))){
			cerr<<"OPEN CIF\n";
			sight.loadCIF( clsFilename.ToAscii() );
		} else {
			sight.loadGDSII( clsFilename.ToAscii());
			cerr<<"OPEN GDS2\n";
		}
		sight.render();		
		CreateModelList();
	}//else: cancelled by user	
	
}

// -----------------------------------------------------------------------------

void wxSightFrame::SaveModelAs(wxCommandEvent& WXUNUSED(event)){
	wxString label = wxGetTextFromUser(_(""), _("Choose Model Name")).c_str();
		const char* lbl = (const char*)label.mb_str(wxConvUTF8);
		if( strcmp(lbl,"") ) {
			sight.saveModelAs(lbl);//save new Model
			CreateModelList();
			sight.setCurrentModel( lbl );
		}//end if
}

// -----------------------------------------------------------------------------

void wxSightFrame::Ask(wxCommandEvent& WXUNUSED(event)){
	GLboolean params[1];
	
	glGetBooleanv( GL_DOUBLEBUFFER, params);
	
	if(params[0])     wxMessageBox(_T("DoubleBuffer Supported"), _T("Yupeee \\o/"), wxOK | wxICON_INFORMATION, this);
	else			wxMessageBox(_T("DoubleBuffer Not Supported"), _T("HELL"), wxOK | wxICON_INFORMATION, this);
	
}//end method





