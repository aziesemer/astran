///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __WXINTERFACE_H__
#define __WXINTERFACE_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/toolbar.h>
#include <wx/stattext.h>
#include <wx/listbox.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/statline.h>
#include <wx/checkbox.h>
#include <wx/slider.h>

///////////////////////////////////////////////////////////////////////////

#define ID_readscript 1000
#define ID_opentech 1001
#define ID_opennetlist 1002
#define ID_openlibrary 1003
#define ID_openplacement 1004
#define ID_savenetlist 1005
#define ID_savelayout 1006
#define ID_savepl 1007
#define ID_rules 1008
#define ID_circuit 1009
#define ID_cellgen 1010
#define ID_preferences 1011
#define ID_refresh 1012
#define ID_help 1013

///////////////////////////////////////////////////////////////////////////////
/// Class ICPD_frm
///////////////////////////////////////////////////////////////////////////////
class ICPD_frm : public wxFrame 
{
	private:
	
	protected:
		wxMenuBar* barra_menu;
		wxMenu* menu_file;
		wxMenu* file_new;
		wxMenu* file_load;
		wxMenu* file_save;
		wxMenu* file_import;
		wxMenu* file_export;
		wxMenu* menu_view;
		wxMenu* menu_size;
		wxMenu* menu_cells;
		wxMenu* menu_place;
		wxMenu* menu_route;
		wxMenu* menu_help;
		wxToolBar* toolbar;
		wxStaticText* m_staticText6;
		wxListBox* wx_layouts;
		wxStaticText* m_staticText7;
		wxListBox* wx_instances;
		wxStaticText* m_staticText8;
		wxListBox* wx_cells;
		wxStaticText* m_staticText9;
		wxListBox* wx_nets;
		wxStaticText* m_staticText10;
		wxListBox* wx_interfaces;
		wxTextCtrl* wx_log;
		wxTextCtrl* wxCommand;
		wxStatusBar* statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void PressedKey( wxKeyEvent& event ) { event.Skip(); }
		virtual void newDesign( wxCommandEvent& event ) { event.Skip(); }
		virtual void newCell( wxCommandEvent& event ) { event.Skip(); }
		virtual void loadProject( wxCommandEvent& event ) { event.Skip(); }
		virtual void loadTech( wxCommandEvent& event ) { event.Skip(); }
		virtual void loadSpiceNetlist( wxCommandEvent& event ) { event.Skip(); }
		virtual void loadLayouts( wxCommandEvent& event ) { event.Skip(); }
		virtual void loadPlacement( wxCommandEvent& event ) { event.Skip(); }
		virtual void loadRouting( wxCommandEvent& event ) { event.Skip(); }
		virtual void saveProject( wxCommandEvent& event ) { event.Skip(); }
		virtual void saveTech( wxCommandEvent& event ) { event.Skip(); }
		virtual void saveSpiceNetlist( wxCommandEvent& event ) { event.Skip(); }
		virtual void saveLayouts( wxCommandEvent& event ) { event.Skip(); }
		virtual void savePlacement( wxCommandEvent& event ) { event.Skip(); }
		virtual void saveRouting( wxCommandEvent& event ) { event.Skip(); }
		virtual void importSpiceNetlist( wxCommandEvent& event ) { event.Skip(); }
		virtual void importLEF( wxCommandEvent& event ) { event.Skip(); }
		virtual void exportLayout( wxCommandEvent& event ) { event.Skip(); }
		virtual void exportCellSizes( wxCommandEvent& event ) { event.Skip(); }
		virtual void exportPlacement( wxCommandEvent& event ) { event.Skip(); }
		virtual void flReadScript( wxCommandEvent& event ) { event.Skip(); }
		virtual void saveHistory( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnFileExit( wxCommandEvent& event ) { event.Skip(); }
		virtual void showTech( wxCommandEvent& event ) { event.Skip(); }
		virtual void showCircuit( wxCommandEvent& event ) { event.Skip(); }
		virtual void showPreferences( wxCommandEvent& event ) { event.Skip(); }
		virtual void sizeFO4( wxCommandEvent& event ) { event.Skip(); }
		virtual void sizeGP( wxCommandEvent& event ) { event.Skip(); }
		virtual void elmoreDelay( wxCommandEvent& event ) { event.Skip(); }
		virtual void showCellgen( wxCommandEvent& event ) { event.Skip(); }
		virtual void calcPinsPos( wxCommandEvent& event ) { event.Skip(); }
		virtual void showFP( wxCommandEvent& event ) { event.Skip(); }
		virtual void plTerminals( wxCommandEvent& event ) { event.Skip(); }
		virtual void fastp( wxCommandEvent& event ) { event.Skip(); }
		virtual void plVerify( wxCommandEvent& event ) { event.Skip(); }
		virtual void plAutoFlip( wxCommandEvent& event ) { event.Skip(); }
		virtual void plIncremental( wxCommandEvent& event ) { event.Skip(); }
		virtual void plWL( wxCommandEvent& event ) { event.Skip(); }
		virtual void rotdl( wxCommandEvent& event ) { event.Skip(); }
		virtual void pfinder( wxCommandEvent& event ) { event.Skip(); }
		virtual void rtOptimize( wxCommandEvent& event ) { event.Skip(); }
		virtual void rtCompact( wxCommandEvent& event ) { event.Skip(); }
		virtual void rtClear( wxCommandEvent& event ) { event.Skip(); }
		virtual void HelpaboutClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void helpHP( wxCommandEvent& event ) { event.Skip(); }
		virtual void refreshIcon( wxCommandEvent& event ) { event.Skip(); }
		virtual void layoutListEvent( wxCommandEvent& event ) { event.Skip(); }
		virtual void viewLayout( wxCommandEvent& event ) { event.Skip(); }
		virtual void openInstancewindow( wxCommandEvent& event ) { event.Skip(); }
		virtual void printInstance( wxCommandEvent& event ) { event.Skip(); }
		virtual void printCell( wxCommandEvent& event ) { event.Skip(); }
		virtual void printNet( wxCommandEvent& event ) { event.Skip(); }
		virtual void printInterface( wxCommandEvent& event ) { event.Skip(); }
		virtual void CmdKeyDown( wxKeyEvent& event ) { event.Skip(); }
		virtual void command( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ICPD_frm( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("ASTRAN - GME/UFRGS"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 700,550 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~ICPD_frm();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Specify_FP
///////////////////////////////////////////////////////////////////////////////
class Specify_FP : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* static_topcell;
		wxChoice* cell_choice;
		wxButton* ok_button;
		wxStaticText* static_rows;
		wxTextCtrl* wxNrRows;
		wxButton* cancel_button;
		wxStaticText* static_coreutil;
		wxTextCtrl* wxUtilization;
		wxStaticLine* m_staticline26;
		wxStaticLine* m_staticline27;
		wxStaticLine* m_staticline28;
		wxStaticLine* m_staticline29;
		wxStaticText* static_margin;
		wxStaticText* static_cleft;
		wxTextCtrl* wxC2L;
		wxStaticText* static_ctop;
		wxTextCtrl* wxC2T;
		wxStaticText* static_cright;
		wxTextCtrl* wxC2R;
		wxStaticText* static_cbottom;
		wxTextCtrl* wxC2B;
		
		// Virtual event handlers, overide them in your derived class
		virtual void hide( wxCloseEvent& event ) { event.Skip(); }
		virtual void PressedKey( wxKeyEvent& event ) { event.Skip(); }
		virtual void okButtonEvt( wxCommandEvent& event ) { event.Skip(); }
		virtual void cancelButtonEvt( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		Specify_FP( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Specify Floorplan"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~Specify_FP();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CellGen
///////////////////////////////////////////////////////////////////////////////
class CellGen : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* static_cellnet;
		wxChoice* cellnet_choice;
		wxButton* generate_button;
		wxButton* select_button;
		wxStaticLine* m_staticline16;
		wxStaticLine* m_staticline17;
		wxStaticLine* m_staticline18;
		wxStaticLine* m_staticline19;
		wxStaticLine* m_staticline20;
		wxStaticText* static_trnum;
		wxTextCtrl* s_RN;
		wxButton* fold_button;
		wxStaticLine* m_staticline11;
		wxStaticLine* m_staticline12;
		wxStaticLine* m_staticline13;
		wxStaticLine* m_staticline14;
		wxStaticLine* m_staticline15;
		wxStaticText* static_widcost;
		wxTextCtrl* p_WC;
		wxStaticText* static_gatemmcost;
		wxTextCtrl* p_GMC;
		wxStaticText* static_rtcost;
		wxTextCtrl* p_RC;
		wxStaticText* static_rtdenscost;
		wxTextCtrl* p_RTDC;
		wxStaticText* static_gapcost;
		wxTextCtrl* p_NGC;
		wxStaticText* static_taqual;
		wxTextCtrl* p_TAQ;
		wxStaticText* static_numatt;
		wxTextCtrl* p_NRA;
		wxButton* place_button;
		wxStaticLine* m_staticline6;
		wxStaticLine* m_staticline7;
		wxStaticLine* m_staticline8;
		wxStaticLine* m_staticline9;
		wxStaticLine* m_staticline10;
		wxStaticText* static_metal;
		wxCheckBox* hPoly;
		wxButton* route_button;
		wxStaticLine* m_staticline1;
		wxStaticLine* m_staticline2;
		wxStaticLine* m_staticline3;
		wxStaticLine* m_staticline4;
		wxStaticLine* m_staticline5;
		wxStaticText* static_ds;
		wxCheckBox* diffStretching;
		wxStaticText* static_gp;
		wxCheckBox* griddedPolly;
		wxStaticText* static_ddc;
		wxSlider* rdCntsCost;
		wxStaticText* m_staticText64;
		wxSlider* maxDiffCnts;
		wxStaticText* static_adc;
		wxCheckBox* alignDiffCnts;
		wxStaticText* m_staticText70;
		wxCheckBox* enableDFM;
		wxStaticText* m_staticText65;
		wxCheckBox* debug;
		wxStaticText* m_staticText71;
		wxTextCtrl* timeLimit;
		wxButton* compact_button;
		wxStaticLine* m_staticline21;
		wxStaticLine* m_staticline22;
		wxStaticLine* m_staticline23;
		wxStaticLine* m_staticline24;
		wxStaticLine* m_staticline25;
		wxButton* view_button;
		
		// Virtual event handlers, overide them in your derived class
		virtual void hide( wxCloseEvent& event ) { event.Skip(); }
		virtual void PressedKey( wxKeyEvent& event ) { event.Skip(); }
		virtual void generateCell( wxCommandEvent& event ) { event.Skip(); }
		virtual void select( wxCommandEvent& event ) { event.Skip(); }
		virtual void fold( wxCommandEvent& event ) { event.Skip(); }
		virtual void place( wxCommandEvent& event ) { event.Skip(); }
		virtual void route( wxCommandEvent& event ) { event.Skip(); }
		virtual void compact( wxCommandEvent& event ) { event.Skip(); }
		virtual void view( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		CellGen( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Cellgen"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~CellGen();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class View_Circuit
///////////////////////////////////////////////////////////////////////////////
class View_Circuit : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* static_design;
		wxTextCtrl* design_name;
		wxStaticText* static_horizontal;
		wxTextCtrl* horizontalgrid;
		wxStaticText* static_vertical;
		wxTextCtrl* verticalgrid;
		wxStaticText* m_staticText60;
		wxCheckBox* hGridOffset;
		wxStaticText* m_staticText61;
		wxCheckBox* vGridOffset;
		wxStaticText* static_vdd;
		wxTextCtrl* vddnetname;
		wxStaticText* static_gnd;
		wxTextCtrl* gndnetname;
		wxStaticText* static_row;
		wxTextCtrl* rowheight;
		wxStaticText* static_supply;
		wxTextCtrl* supplysize;
		wxStaticText* static_supply1;
		wxTextCtrl* nWellPos;
		wxStaticText* static_supply12;
		wxTextCtrl* nWellBorder;
		wxStaticText* static_supply121;
		wxTextCtrl* pnSelBorder;
		wxStaticText* static_supply11;
		wxChoice* cellTemplate;
		wxButton* ok_button;
		wxButton* cancel_button;
		
		// Virtual event handlers, overide them in your derived class
		virtual void hide( wxCloseEvent& event ) { event.Skip(); }
		virtual void PressedKey( wxKeyEvent& event ) { event.Skip(); }
		virtual void applyButtonEvt( wxCommandEvent& event ) { event.Skip(); }
		virtual void okButtonEvt( wxCommandEvent& event ) { event.Skip(); }
		virtual void cancelButtonEvt( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		View_Circuit( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Circuit"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~View_Circuit();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class View_Preferences
///////////////////////////////////////////////////////////////////////////////
class View_Preferences : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* static_plPath;
		wxTextCtrl* plPath;
		wxButton* plPath_button;
		wxStaticText* static_rotdl;
		wxTextCtrl* rotdlPath;
		wxButton* rotdlPath_button;
		wxStaticText* static_viewer;
		wxTextCtrl* viewerPath;
		wxButton* viewerPath_button;
		wxStaticText* static_lpsolver;
		wxTextCtrl* lpsolverPath;
		wxButton* lpsolverPath_button;
		wxStaticText* static_logf;
		wxTextCtrl* logPath;
		wxButton* logPath_button;
		wxButton* ok_button;
		wxButton* cancel_button;
		
		// Virtual event handlers, overide them in your derived class
		virtual void hide( wxCloseEvent& event ) { event.Skip(); }
		virtual void PressedKey( wxKeyEvent& event ) { event.Skip(); }
		virtual void plButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void rotdlButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void viewerButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void lpsolverButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void logButton( wxCommandEvent& event ) { event.Skip(); }
		virtual void okButtonEvt( wxCommandEvent& event ) { event.Skip(); }
		virtual void cancelButtonEvt( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		View_Preferences( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~View_Preferences();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Design_Rules
///////////////////////////////////////////////////////////////////////////////
class Design_Rules : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* static_technology;
		wxTextCtrl* wxTechName;
		wxStaticText* static_soi;
		wxCheckBox* wxSOI;
		wxStaticText* static_mlayers;
		wxTextCtrl* wxMLayers;
		wxStaticText* static_resolution;
		wxTextCtrl* wxResolution;
		wxStaticText* static_rule;
		wxChoice* wxSelect_Rule;
		wxStaticText* static_RuleDesc;
		wxStaticText* static_value;
		wxTextCtrl* wxRule_Value;
		wxStaticText* static_layerlabel;
		wxChoice* wxSelect_Layer;
		wxStaticText* static_LayerDesc;
		wxStaticText* static_cif;
		wxTextCtrl* wxCIF;
		wxStaticText* static_gds;
		wxTextCtrl* wxGDSII;
		wxStaticText* static_tech;
		wxTextCtrl* wxTechVal;
		wxButton* save_button;
		wxButton* close_button;
		
		// Virtual event handlers, overide them in your derived class
		virtual void hide( wxCloseEvent& event ) { event.Skip(); }
		virtual void PressedKey( wxKeyEvent& event ) { event.Skip(); }
		virtual void selectRule( wxCommandEvent& event ) { event.Skip(); }
		virtual void selectLayer( wxCommandEvent& event ) { event.Skip(); }
		virtual void saveButtonEvt( wxCommandEvent& event ) { event.Skip(); }
		virtual void closeButtonEvt( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		Design_Rules( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("Design Rules"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~Design_Rules();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class Help_About
///////////////////////////////////////////////////////////////////////////////
class Help_About : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* credits;
		
		// Virtual event handlers, overide them in your derived class
		virtual void hide( wxCloseEvent& event ) { event.Skip(); }
		
	
	public:
		
		Help_About( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("About"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 520,319 ), long style = wxDEFAULT_FRAME_STYLE|wxALWAYS_SHOW_SB|wxTAB_TRAVERSAL );
		
		~Help_About();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class View_PrintGP
///////////////////////////////////////////////////////////////////////////////
class View_PrintGP : public wxFrame 
{
	private:
	
	protected:
		wxStaticText* static_design1;
		wxChoice* optimize_choice;
		wxStaticText* static_design13;
		wxChoice* sizingtype_choice;
		wxStaticText* static_design12;
		wxChoice* technology_choice;
		wxStaticText* static_design11;
		wxStaticText* static_design112;
		wxTextCtrl* parameterCload;
		wxStaticText* static_design111;
		wxTextCtrl* parameterMaxArea;
		wxStaticText* static_design1111;
		wxTextCtrl* parameterDelay;
		wxStaticText* static_design11111;
		wxTextCtrl* parameterMaxCin;
		wxButton* ok_button;
		wxButton* cancel_button;
		
		// Virtual event handlers, overide them in your derived class
		virtual void PressedKey( wxKeyEvent& event ) { event.Skip(); }
		virtual void applyButtonEvt( wxCommandEvent& event ) { event.Skip(); }
		virtual void okButtonEvt( wxCommandEvent& event ) { event.Skip(); }
		virtual void cancelButtonEvt( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		View_PrintGP( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("GP-Geometric Program"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( -1,-1 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~View_PrintGP();
	
};

#endif //__WXINTERFACE_H__
