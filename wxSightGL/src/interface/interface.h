#ifndef INTERFACE_H
#define INTERFACE_H

#include <iostream>
	using std::cout;
	using std::cerr;

#include "../sight/opengl.h"

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/glcanvas.h>
#include <wx/splitter.h>
#include <wx/filedlg.h>
#include <wx/aboutdlg.h>
#include <wx/toolbar.h> 
#include <wx/tbarbase.h>
#include <wx/minifram.h>
#include <wx/aui/aui.h>
#include <wx/colordlg.h>
#include <wx/combobox.h>


#include "../basicglpane.h"
#include "../sight/sight.h"
#include "layeritem.h"
#include "../sight/stipple.h"

class wxSightFrame: public wxFrame {
	private:

		BasicGLPane * clsBasicGLPane;
		wxString clsFilename;
		
		wxBoxSizer * topLayout;		
		wxToolBar * stateToolBar;
		wxCheckListBox * checkListBox;
		LayerList * clsLayerList;
		wxSplitterWindow * splitter;
		BasicGLPane * basic;
		wxComboBox * clsModelsCombo;
		
		wxAuiManager * clsAuiManager;
		
		vector <wxString> clsModels;
		
		void CreateMenubar();	
		void CreateToolbar();
		void CreateStatusbar();	
		
		void CreateLayerPane();
		void CreateLayerListPane();
		void CreateSightPane();
		void CreateDebugPane();
			
	public:

    wxSightFrame( const wxString& title, const wxPoint& pos, const wxSize& size);

	void Initialize();
	
	// -----------------------------------------------------------------------------
//file menu
  void OnQuit(wxCommandEvent& event);
	void New(wxCommandEvent& event);
	void Open(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);
	void SaveAs(wxCommandEvent& event);
	void Reload(wxCommandEvent& WXUNUSED(event));
// -----------------------------------------------------------------------------
//edit menu
	void OnCut(wxCommandEvent& WXUNUSED(event));
	void OnCopy(wxCommandEvent& WXUNUSED(event));
	void OnPaste(wxCommandEvent& WXUNUSED(event));
	void OnDelete(wxCommandEvent& WXUNUSED(event));
// -----------------------------------------------------------------------------
//object menu
	void OnGroup(wxCommandEvent& WXUNUSED(event));
	void OnUngroup(wxCommandEvent& WXUNUSED(event));
	void OnModel(wxCommandEvent& WXUNUSED(event));
	void OnComponent(wxCommandEvent& WXUNUSED(event));
	void editLabel(wxCommandEvent& WXUNUSED(event));
// -----------------------------------------------------------------------------	
//view menu
	void changeBgColor(wxCommandEvent& event);
	void changeGuideColor(wxCommandEvent& event);
	void showLabel(wxCommandEvent& event);
	void showLayerList(wxCommandEvent& event);
// -----------------------------------------------------------------------------	
    void OnButtonClick(wxCommandEvent& event);
	void clearRulers(wxCommandEvent& event);
// -----------------------------------------------------------------------------
//layer check list
	void OnChangeSelectedLayer( wxLayerListEvent &event );
	void OnShowLayer( wxLayerListEvent &event );
// -----------------------------------------------------------------------------
	void hideTool(wxCommandEvent& event);	
	void setCompletTitle(wxString filename);
	
//state bar	
	void selection(wxCommandEvent& event);
	void wire(wxCommandEvent& event);
	void rectangle(wxCommandEvent& event);
	void line(wxCommandEvent& event);
	void ruler(wxCommandEvent& event);
	void drag(wxCommandEvent& event);
	void insertLabel(wxCommandEvent& event);
// -----------------------------------------------------------------------------
//zoom
	void zoomOut(wxCommandEvent& event);
	void zoomIn(wxCommandEvent& event);
	void fitIn(wxCommandEvent& event);
// -----------------------------------------------------------------------------
	void openFile( const char * const filename );
// -----------------------------------------------------------------------------
	void printSnapShot();
	void saveSnapShot(wxCommandEvent& event);
	
	void CreateModelList();
	void CreateModelsCombo();
	
	void OnModelChoice(wxCommandEvent& event);
	void SaveModel(wxCommandEvent& WXUNUSED(event));
	void SaveModelAs(wxCommandEvent& WXUNUSED(event));
	
	//void keyPressed(wxKeyEvent& event);
// -----------------------------------------------------------------------------

void Ask(wxCommandEvent& WXUNUSED(event));

	
    DECLARE_EVENT_TABLE()
};

enum
{
	ID_SaveSS = 100,
	ID_Cut,
	ID_Copy,
	ID_Paste,
	ID_Delete,
	ID_Group,
	ID_Ungroup,
	ID_NW_Component,
	ID_NW_Model,
	ID_Lbl,
	ID_CloseComponent,
	E_topTool,
	ID_BGCOLOR,
	ID_GCOLOR,
	ID_SLABEL,
	ID_SLAYER,
	ID_Save_Model_As,
	LL,
	HELP,
	ID_Save_Model,
	ID_Reload,
	ID_ASK,
 
	TOOL_BAR_TOP = 50,
	SPLITTER,
	CHECK_LIST_BOX_LAYERS,
	SELECTION = 501,
	RECTANGLE,
	WIRE,
	LINE,
	RULER,
	DRAG,
	LABEL,	
	ZOOMIN,
	ZOOMOUT,
	ZOOM,
	MDL,
	
	CLEAR_RULERS

};



#endif