///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Nov  6 2013)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wxinterface.h"
#include "wxicons.h"

///////////////////////////////////////////////////////////////////////////

ICPD_frm::ICPD_frm( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	barra_menu = new wxMenuBar( 0 );
	menu_file = new wxMenu();
	file_new = new wxMenu();
	wxMenuItem* new_design;
	new_design = new wxMenuItem( file_new, wxID_ANY, wxString( wxT("Design") ) , wxEmptyString, wxITEM_NORMAL );
	file_new->Append( new_design );
	
	wxMenuItem* new_cellnetlist;
	new_cellnetlist = new wxMenuItem( file_new, wxID_ANY, wxString( wxT("Cell Netlist") ) , wxEmptyString, wxITEM_NORMAL );
	file_new->Append( new_cellnetlist );
	
	wxMenuItem* new_equation;
	new_equation = new wxMenuItem( file_new, wxID_ANY, wxString( wxT("Equation") ) , wxEmptyString, wxITEM_NORMAL );
	file_new->Append( new_equation );
	
	wxMenuItem* new_layout;
	new_layout = new wxMenuItem( file_new, wxID_ANY, wxString( wxT("Layout") ) , wxEmptyString, wxITEM_NORMAL );
	file_new->Append( new_layout );
	
	menu_file->Append( -1, wxT("New"), file_new );
	
	file_load = new wxMenu();
	wxMenuItem* load_project;
	load_project = new wxMenuItem( file_load, wxID_ANY, wxString( wxT("Project") ) , wxEmptyString, wxITEM_NORMAL );
	file_load->Append( load_project );
	
	file_load->AppendSeparator();
	
	wxMenuItem* load_technology;
	load_technology = new wxMenuItem( file_load, wxID_ANY, wxString( wxT("Technology") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	load_technology->SetBitmaps( wxGetBitmapFromMemory(load_tec));
	#elif defined( __WXGTK__ )
	load_technology->SetBitmap( wxGetBitmapFromMemory(load_tec));
	#endif
	file_load->Append( load_technology );
	
	wxMenuItem* load_netlist;
	load_netlist = new wxMenuItem( file_load, wxID_ANY, wxString( wxT("Netlist") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	load_netlist->SetBitmaps( wxGetBitmapFromMemory(load_net));
	#elif defined( __WXGTK__ )
	load_netlist->SetBitmap( wxGetBitmapFromMemory(load_net));
	#endif
	file_load->Append( load_netlist );
	
	wxMenuItem* load_layouts;
	load_layouts = new wxMenuItem( file_load, wxID_ANY, wxString( wxT("Layouts") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	load_layouts->SetBitmaps( wxGetBitmapFromMemory(load_lay));
	#elif defined( __WXGTK__ )
	load_layouts->SetBitmap( wxGetBitmapFromMemory(load_lay));
	#endif
	file_load->Append( load_layouts );
	
	wxMenuItem* load_placement;
	load_placement = new wxMenuItem( file_load, wxID_ANY, wxString( wxT("Placement") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	load_placement->SetBitmaps( wxGetBitmapFromMemory(load_pl));
	#elif defined( __WXGTK__ )
	load_placement->SetBitmap( wxGetBitmapFromMemory(load_pl));
	#endif
	file_load->Append( load_placement );
	
	wxMenuItem* load_routing;
	load_routing = new wxMenuItem( file_load, wxID_ANY, wxString( wxT("Routing Rotdl") ) , wxEmptyString, wxITEM_NORMAL );
	file_load->Append( load_routing );
	
	menu_file->Append( -1, wxT("Load"), file_load );
	
	file_save = new wxMenu();
	wxMenuItem* save_project;
	save_project = new wxMenuItem( file_save, wxID_ANY, wxString( wxT("Project") ) , wxEmptyString, wxITEM_NORMAL );
	file_save->Append( save_project );
	
	file_save->AppendSeparator();
	
	wxMenuItem* save_tech;
	save_tech = new wxMenuItem( file_save, wxID_ANY, wxString( wxT("Technology") ) , wxEmptyString, wxITEM_NORMAL );
	file_save->Append( save_tech );
	
	wxMenuItem* save_netlist;
	save_netlist = new wxMenuItem( file_save, wxID_ANY, wxString( wxT("Netlist") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	save_netlist->SetBitmaps( wxGetBitmapFromMemory(save_net));
	#elif defined( __WXGTK__ )
	save_netlist->SetBitmap( wxGetBitmapFromMemory(save_net));
	#endif
	file_save->Append( save_netlist );
	
	wxMenuItem* save_layouts;
	save_layouts = new wxMenuItem( file_save, wxID_ANY, wxString( wxT("Layouts") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	save_layouts->SetBitmaps(wxGetBitmapFromMemory(save_lay));
	#elif defined( __WXGTK__ )
	save_layouts->SetBitmap(wxGetBitmapFromMemory(save_lay));
	#endif
	file_save->Append( save_layouts );
	
	wxMenuItem* save_placement;
	save_placement = new wxMenuItem( file_save, wxID_ANY, wxString( wxT("Placement") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	save_placement->SetBitmaps(wxGetBitmapFromMemory(save_pl));
	#elif defined( __WXGTK__ )
	save_placement->SetBitmap(wxGetBitmapFromMemory(save_pl));
	#endif
	file_save->Append( save_placement );
	
	wxMenuItem* save_routing;
	save_routing = new wxMenuItem( file_save, wxID_ANY, wxString( wxT("Routing Rotdl") ) , wxEmptyString, wxITEM_NORMAL );
	file_save->Append( save_routing );
	
	menu_file->Append( -1, wxT("Save"), file_save );
	
	file_import = new wxMenu();
	wxMenuItem* import_netlist;
	import_netlist = new wxMenuItem( file_import, wxID_ANY, wxString( wxT("Cadence Netlist") ) , wxEmptyString, wxITEM_NORMAL );
	file_import->Append( import_netlist );
	
	wxMenuItem* import_lef;
	import_lef = new wxMenuItem( file_import, wxID_ANY, wxString( wxT("LEF") ) , wxEmptyString, wxITEM_NORMAL );
	file_import->Append( import_lef );
	
	menu_file->Append( -1, wxT("Import"), file_import );
	
	file_export = new wxMenu();
	wxMenuItem* export_cellLayout;
	export_cellLayout = new wxMenuItem( file_export, wxID_ANY, wxString( wxT("Layout") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	export_cellLayout->SetBitmaps( wxNullBitmap );
	#elif defined( __WXGTK__ )
	export_cellLayout->SetBitmap( wxNullBitmap );
	#endif
	file_export->Append( export_cellLayout );
	
	wxMenuItem* export_cellsizes;
	export_cellsizes = new wxMenuItem( file_export, wxID_ANY, wxString( wxT("Cell Sizes") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	export_cellsizes->SetBitmaps( wxNullBitmap );
	#elif defined( __WXGTK__ )
	export_cellsizes->SetBitmap( wxNullBitmap );
	#endif
	file_export->Append( export_cellsizes );
	
	wxMenuItem* export_placement;
	export_placement = new wxMenuItem( file_export, wxID_ANY, wxString( wxT("Placement") ) , wxEmptyString, wxITEM_NORMAL );
	file_export->Append( export_placement );
	
	menu_file->Append( -1, wxT("Export"), file_export );
	
	menu_file->AppendSeparator();
	
	wxMenuItem* read_script;
	read_script = new wxMenuItem( menu_file, wxID_ANY, wxString( wxT("Read Script") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	read_script->SetBitmaps(wxGetBitmapFromMemory(read_script));
	#elif defined( __WXGTK__ )
	read_script->SetBitmap(wxGetBitmapFromMemory(read_script));
	#endif
	menu_file->Append( read_script );
	
	wxMenuItem* save_history;
	save_history = new wxMenuItem( menu_file, wxID_ANY, wxString( wxT("Save Script") ) , wxEmptyString, wxITEM_NORMAL );
	menu_file->Append( save_history );
	
	menu_file->AppendSeparator();
	
	wxMenuItem* exit;
	exit = new wxMenuItem( menu_file, wxID_ANY, wxString( wxT("Exit") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	exit->SetBitmaps( wxGetBitmapFromMemory(close));
	#elif defined( __WXGTK__ )
	exit->SetBitmap( wxGetBitmapFromMemory(close));
	#endif
	menu_file->Append( exit );
	
	barra_menu->Append( menu_file, wxT("File") ); 
	
	menu_view = new wxMenu();
	wxMenuItem* view_rules;
	view_rules = new wxMenuItem( menu_view, wxID_ANY, wxString( wxT("Rules...") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	view_rules->SetBitmaps( wxGetBitmapFromMemory(view_rules));
	#elif defined( __WXGTK__ )
	view_rules->SetBitmap( wxGetBitmapFromMemory(view_rules));
	#endif
	menu_view->Append( view_rules );
	
	wxMenuItem* view_circuit;
	view_circuit = new wxMenuItem( menu_view, wxID_ANY, wxString( wxT("Circuit...") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	view_circuit->SetBitmaps( wxGetBitmapFromMemory(view_cir));
	#elif defined( __WXGTK__ )
	view_circuit->SetBitmap( wxGetBitmapFromMemory(view_cir));
	#endif
	menu_view->Append( view_circuit );
	
	wxMenuItem* view_preferences;
	view_preferences = new wxMenuItem( menu_view, wxID_ANY, wxString( wxT("Preferences...") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	view_preferences->SetBitmaps( wxGetBitmapFromMemory(view_pref));
	#elif defined( __WXGTK__ )
	view_preferences->SetBitmap( wxGetBitmapFromMemory(view_pref));
	#endif
	menu_view->Append( view_preferences );
	
	menu_size = new wxMenu();
	wxMenuItem* size_fo4;
	size_fo4 = new wxMenuItem( menu_size, wxID_ANY, wxString( wxT("FO4 Method") ) , wxEmptyString, wxITEM_NORMAL );
	menu_size->Append( size_fo4 );
	
	wxMenuItem* size_gp;
	size_gp = new wxMenuItem( menu_size, wxID_ANY, wxString( wxT("GP Method") ) , wxEmptyString, wxITEM_NORMAL );
	menu_size->Append( size_gp );
	
	wxMenuItem* m_menuItem47;
	m_menuItem47 = new wxMenuItem( menu_size, wxID_ANY, wxString( wxT("Elmore Delay") ) , wxEmptyString, wxITEM_NORMAL );
	menu_size->Append( m_menuItem47 );
	
	menu_view->Append( -1, wxT("Size"), menu_size );
	
	barra_menu->Append( menu_view, wxT("View") ); 
	
	menu_cells = new wxMenu();
	wxMenuItem* netlist_cellgen;
	netlist_cellgen = new wxMenuItem( menu_cells, wxID_ANY, wxString( wxT("CellGen...") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	netlist_cellgen->SetBitmaps( wxGetBitmapFromMemory(view_cellgen));
	#elif defined( __WXGTK__ )
	netlist_cellgen->SetBitmap( wxGetBitmapFromMemory(view_cellgen));
	#endif
	menu_cells->Append( netlist_cellgen );
	
	wxMenuItem* netlist_calcPinsPos;
	netlist_calcPinsPos = new wxMenuItem( menu_cells, wxID_ANY, wxString( wxT("Calculate Pins' Position") ) , wxEmptyString, wxITEM_NORMAL );
	menu_cells->Append( netlist_calcPinsPos );
	
	barra_menu->Append( menu_cells, wxT("Cells") ); 
	
	menu_place = new wxMenu();
	wxMenuItem* place_spec_fp;
	place_spec_fp = new wxMenuItem( menu_place, wxID_ANY, wxString( wxT("Specify Floorplan...") ) , wxEmptyString, wxITEM_NORMAL );
	menu_place->Append( place_spec_fp );
	
	wxMenuItem* place_terminals;
	place_terminals = new wxMenuItem( menu_place, wxID_ANY, wxString( wxT("Terminals") ) , wxEmptyString, wxITEM_NORMAL );
	menu_place->Append( place_terminals );
	
	wxMenuItem* place_mobral;
	place_mobral = new wxMenuItem( menu_place, wxID_ANY, wxString( wxT("MobralPlace") ) , wxEmptyString, wxITEM_NORMAL );
	menu_place->Append( place_mobral );
	
	wxMenuItem* place_verify;
	place_verify = new wxMenuItem( menu_place, wxID_ANY, wxString( wxT("Verify") ) , wxEmptyString, wxITEM_NORMAL );
	menu_place->Append( place_verify );
	
	wxMenuItem* place_autoflip;
	place_autoflip = new wxMenuItem( menu_place, wxID_ANY, wxString( wxT("Auto Flip") ) , wxEmptyString, wxITEM_NORMAL );
	menu_place->Append( place_autoflip );
	
	wxMenuItem* place_inc_placement;
	place_inc_placement = new wxMenuItem( menu_place, wxID_ANY, wxString( wxT("Incremental Placement") ) , wxEmptyString, wxITEM_NORMAL );
	menu_place->Append( place_inc_placement );
	
	wxMenuItem* place_checkwl;
	place_checkwl = new wxMenuItem( menu_place, wxID_ANY, wxString( wxT("Check WL") ) , wxEmptyString, wxITEM_NORMAL );
	menu_place->Append( place_checkwl );
	
	barra_menu->Append( menu_place, wxT("Place") ); 
	
	menu_route = new wxMenu();
	wxMenuItem* route_rotdl;
	route_rotdl = new wxMenuItem( menu_route, wxID_ANY, wxString( wxT("Rotdl") ) , wxEmptyString, wxITEM_NORMAL );
	menu_route->Append( route_rotdl );
	
	wxMenuItem* route_nbrouter;
	route_nbrouter = new wxMenuItem( menu_route, wxID_ANY, wxString( wxT("NBRouter") ) , wxEmptyString, wxITEM_NORMAL );
	menu_route->Append( route_nbrouter );
	
	wxMenuItem* route_optimize;
	route_optimize = new wxMenuItem( menu_route, wxID_ANY, wxString( wxT("Optimize") ) , wxEmptyString, wxITEM_NORMAL );
	menu_route->Append( route_optimize );
	
	wxMenuItem* route_compact;
	route_compact = new wxMenuItem( menu_route, wxID_ANY, wxString( wxT("Compact") ) , wxEmptyString, wxITEM_NORMAL );
	menu_route->Append( route_compact );
	
	wxMenuItem* route_clear;
	route_clear = new wxMenuItem( menu_route, wxID_ANY, wxString( wxT("Clear") ) , wxEmptyString, wxITEM_NORMAL );
	menu_route->Append( route_clear );
	
	barra_menu->Append( menu_route, wxT("Route") ); 
	
	menu_help = new wxMenu();
	wxMenuItem* help_about;
	help_about = new wxMenuItem( menu_help, wxID_ANY, wxString( wxT("About ICPD") ) , wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	help_about->SetBitmaps( wxGetBitmapFromMemory(help));
	#elif defined( __WXGTK__ )
	help_about->SetBitmap( wxGetBitmapFromMemory(help));
	#endif
	menu_help->Append( help_about );
	
	wxMenuItem* help_homepage;
	help_homepage = new wxMenuItem( menu_help, wxID_ANY, wxString( wxT("Home Page") ) , wxEmptyString, wxITEM_NORMAL );
	menu_help->Append( help_homepage );
	
	barra_menu->Append( menu_help, wxT("Help") ); 
	
	this->SetMenuBar( barra_menu );
	
	toolbar = this->CreateToolBar( wxTB_FLAT|wxTB_HORIZONTAL, wxID_ANY ); 
	toolbar->AddTool( ID_readscript, wxT("tool_readscript"), wxGetBitmapFromMemory(read_script), wxNullBitmap, wxITEM_NORMAL, wxT("Read Script"), wxEmptyString, NULL );
	
	toolbar->AddSeparator(); 
	
	toolbar->AddTool( ID_opentech, wxT("tool_opentech"), wxGetBitmapFromMemory(load_tec), wxNullBitmap, wxITEM_NORMAL, wxT("Load Technology"), wxEmptyString, NULL );
	
	toolbar->AddTool( ID_opennetlist, wxT("tool_opennetlist"), wxGetBitmapFromMemory(load_net), wxNullBitmap, wxITEM_NORMAL, wxT("Load Netlist"), wxEmptyString, NULL );
	
	toolbar->AddTool( ID_openlibrary, wxT("tool_openlibrary"), wxGetBitmapFromMemory(load_lay), wxNullBitmap, wxITEM_NORMAL, wxT("Load Layouts"), wxEmptyString, NULL );
	
	toolbar->AddTool( ID_openplacement, wxT("tool_openplacement"), wxGetBitmapFromMemory(load_pl), wxNullBitmap, wxITEM_NORMAL, wxT("Load Placement"), wxEmptyString, NULL );
	
	toolbar->AddSeparator(); 
	
	toolbar->AddTool( ID_savenetlist, wxT("tool_savenetlist"), wxGetBitmapFromMemory(save_net), wxNullBitmap, wxITEM_NORMAL, wxT("Save Netlist"), wxEmptyString, NULL );
	
	toolbar->AddTool( ID_savelayout, wxT("tool_savelayouts"), wxGetBitmapFromMemory(save_lay), wxNullBitmap, wxITEM_NORMAL, wxT("Save Layouts"), wxEmptyString, NULL );
	
	toolbar->AddTool( ID_savepl, wxT("tool_savepl"), wxGetBitmapFromMemory(save_pl), wxNullBitmap, wxITEM_NORMAL, wxT("Save Placement"), wxEmptyString, NULL );
	
	toolbar->AddSeparator(); 
	
	toolbar->AddTool( ID_rules, wxT("tool_rules"), wxGetBitmapFromMemory(view_rules), wxNullBitmap, wxITEM_NORMAL, wxT("View Rules"), wxEmptyString, NULL );
	
	toolbar->AddTool( ID_circuit, wxT("tool_circuit"), wxGetBitmapFromMemory(view_cir), wxNullBitmap, wxITEM_NORMAL, wxT("View Circuit"), wxEmptyString, NULL );
	
	toolbar->AddTool( ID_cellgen, wxT("tool_cellgen"), wxGetBitmapFromMemory(view_cellgen), wxNullBitmap, wxITEM_NORMAL, wxT("View CellGen"), wxEmptyString, NULL );
	
	toolbar->AddTool( ID_preferences, wxT("tool_preferences"), wxGetBitmapFromMemory(view_pref), wxNullBitmap, wxITEM_NORMAL, wxT("View Preferences"), wxEmptyString, NULL );
	
	toolbar->AddSeparator(); 
	
	toolbar->AddTool( ID_refresh, wxT("tool_refresh"), wxGetBitmapFromMemory(refresh), wxNullBitmap, wxITEM_NORMAL, wxT("Refresh"), wxEmptyString, NULL );
	
	toolbar->AddSeparator(); 
	
	toolbar->AddTool( ID_help, wxT("tool_help"), wxGetBitmapFromMemory(help), wxNullBitmap, wxITEM_NORMAL, wxT("Help"), wxEmptyString, NULL );
	
	toolbar->Realize(); 
	
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );
	
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText6 = new wxStaticText( this, wxID_ANY, wxT("Layouts:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	bSizer4->Add( m_staticText6, 0, wxALL, 5 );
	
	wx_layouts = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer4->Add( wx_layouts, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer3->Add( bSizer4, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer5;
	bSizer5 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText7 = new wxStaticText( this, wxID_ANY, wxT("Instances Tree:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	bSizer5->Add( m_staticText7, 0, wxALL, 5 );
	
	wx_instances = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer5->Add( wx_instances, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer3->Add( bSizer5, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer6;
	bSizer6 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText8 = new wxStaticText( this, wxID_ANY, wxT("Cells:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText8->Wrap( -1 );
	bSizer6->Add( m_staticText8, 0, wxALL, 5 );
	
	wx_cells = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer6->Add( wx_cells, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer3->Add( bSizer6, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer7;
	bSizer7 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText9 = new wxStaticText( this, wxID_ANY, wxT("Nets:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	bSizer7->Add( m_staticText9, 0, wxALL, 5 );
	
	wx_nets = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer7->Add( wx_nets, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer3->Add( bSizer7, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizer8;
	bSizer8 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText10 = new wxStaticText( this, wxID_ANY, wxT("Interfaces:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText10->Wrap( -1 );
	bSizer8->Add( m_staticText10, 0, wxALL, 5 );
	
	wx_interfaces = new wxListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	bSizer8->Add( wx_interfaces, 1, wxALL|wxEXPAND, 5 );
	
	
	bSizer3->Add( bSizer8, 1, wxEXPAND, 5 );
	
	
	bSizer2->Add( bSizer3, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxBoxSizer* bSizer9;
	bSizer9 = new wxBoxSizer( wxVERTICAL );
	
	// wxTE_MULTILINE|
	wx_log = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	// wx_log->SetMaxLength( 0 ); 
	bSizer9->Add( wx_log, 1, wxALL|wxEXPAND, 5 );
	bSizer2->Add( bSizer9, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	wxCommand = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER );
	wxCommand->SetMaxLength( 0 ); 
	bSizer2->Add( wxCommand, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 0 );
	
	
	this->SetSizer( bSizer2 );
	this->Layout();
	statusBar = this->CreateStatusBar( 2, wxST_SIZEGRIP, wxID_ANY );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ICPD_frm::OnClose ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ) );
	barra_menu->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	this->Connect( new_design->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::newDesign ) );
	this->Connect( new_cellnetlist->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::newCell ) );
	this->Connect( new_layout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::newCell ) );
	this->Connect( load_project->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadProject ) );
	this->Connect( load_technology->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadTech ) );
	this->Connect( load_netlist->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadSpiceNetlist ) );
	this->Connect( load_layouts->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadLayouts ) );
	this->Connect( load_placement->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadPlacement ) );
	this->Connect( load_routing->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadRouting ) );
	this->Connect( save_project->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveProject ) );
	this->Connect( save_tech->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveTech ) );
	this->Connect( save_netlist->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveSpiceNetlist ) );
	this->Connect( save_layouts->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveLayouts ) );
	this->Connect( save_placement->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::savePlacement ) );
	this->Connect( save_routing->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveRouting ) );
	this->Connect( import_netlist->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::importSpiceNetlist ) );
	this->Connect( import_lef->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::importLEF ) );
	this->Connect( export_cellLayout->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::exportLayout ) );
	this->Connect( export_cellsizes->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::exportCellSizes ) );
	this->Connect( export_placement->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::exportPlacement ) );
	this->Connect( read_script->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::flReadScript ) );
	this->Connect( save_history->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveHistory ) );
	this->Connect( exit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::OnFileExit ) );
	this->Connect( view_rules->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showTech ) );
	this->Connect( view_circuit->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showCircuit ) );
	this->Connect( view_preferences->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showPreferences ) );
	this->Connect( size_fo4->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::sizeFO4 ) );
	this->Connect( size_gp->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::sizeGP ) );
	this->Connect( m_menuItem47->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::elmoreDelay ) );
	this->Connect( netlist_cellgen->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showCellgen ) );
	this->Connect( netlist_calcPinsPos->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::calcPinsPos ) );
	this->Connect( place_spec_fp->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showFP ) );
	this->Connect( place_terminals->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plTerminals ) );
	this->Connect( place_mobral->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::fastp ) );
	this->Connect( place_verify->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plVerify ) );
	this->Connect( place_autoflip->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plAutoFlip ) );
	this->Connect( place_inc_placement->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plIncremental ) );
	this->Connect( place_checkwl->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plWL ) );
	this->Connect( route_rotdl->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rotdl ) );
	this->Connect( route_nbrouter->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::pfinder ) );
	this->Connect( route_optimize->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rtOptimize ) );
	this->Connect( route_compact->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rtCompact ) );
	this->Connect( route_clear->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rtClear ) );
	this->Connect( help_about->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::HelpaboutClick ) );
	this->Connect( help_homepage->GetId(), wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::helpHP ) );
	toolbar->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	this->Connect( ID_readscript, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::flReadScript ) );
	this->Connect( ID_opentech, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadTech ) );
	this->Connect( ID_opennetlist, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadSpiceNetlist ) );
	this->Connect( ID_openlibrary, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadLayouts ) );
	this->Connect( ID_openplacement, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadPlacement ) );
	this->Connect( ID_savenetlist, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::saveSpiceNetlist ) );
	this->Connect( ID_savelayout, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::saveLayouts ) );
	this->Connect( ID_savepl, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::savePlacement ) );
	this->Connect( ID_rules, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showTech ) );
	this->Connect( ID_circuit, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showCircuit ) );
	this->Connect( ID_cellgen, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showCellgen ) );
	this->Connect( ID_preferences, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showPreferences ) );
	this->Connect( ID_refresh, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::refreshIcon ) );
	this->Connect( ID_help, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::HelpaboutClick ) );
	wx_layouts->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_layouts->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ICPD_frm::layoutListEvent ), NULL, this );
	wx_layouts->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::viewLayout ), NULL, this );
	wx_instances->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_instances->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ICPD_frm::openInstancewindow ), NULL, this );
	wx_instances->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printInstance ), NULL, this );
	wx_cells->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_cells->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printCell ), NULL, this );
	wx_nets->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_nets->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printNet ), NULL, this );
	wx_interfaces->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_interfaces->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printInterface ), NULL, this );
	wx_log->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wxCommand->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::CmdKeyDown ), NULL, this );
	wxCommand->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ICPD_frm::command ), NULL, this );
	statusBar->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
}

ICPD_frm::~ICPD_frm()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( ICPD_frm::OnClose ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ) );
	barra_menu->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::newDesign ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::newCell ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::newCell ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadProject ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadTech ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadSpiceNetlist ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadLayouts ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadPlacement ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::loadRouting ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveProject ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveTech ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveSpiceNetlist ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveLayouts ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::savePlacement ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveRouting ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::importSpiceNetlist ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::importLEF ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::exportLayout ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::exportCellSizes ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::exportPlacement ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::flReadScript ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::saveHistory ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::OnFileExit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showTech ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showCircuit ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showPreferences ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::sizeFO4 ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::sizeGP ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::elmoreDelay ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showCellgen ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::calcPinsPos ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::showFP ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plTerminals ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::fastp ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plVerify ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plAutoFlip ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plIncremental ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::plWL ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rotdl ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::pfinder ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rtOptimize ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rtCompact ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::rtClear ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::HelpaboutClick ) );
	this->Disconnect( wxID_ANY, wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( ICPD_frm::helpHP ) );
	toolbar->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	this->Disconnect( ID_readscript, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::flReadScript ) );
	this->Disconnect( ID_opentech, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadTech ) );
	this->Disconnect( ID_opennetlist, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadSpiceNetlist ) );
	this->Disconnect( ID_openlibrary, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadLayouts ) );
	this->Disconnect( ID_openplacement, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::loadPlacement ) );
	this->Disconnect( ID_savenetlist, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::saveSpiceNetlist ) );
	this->Disconnect( ID_savelayout, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::saveLayouts ) );
	this->Disconnect( ID_savepl, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::savePlacement ) );
	this->Disconnect( ID_rules, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showTech ) );
	this->Disconnect( ID_circuit, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showCircuit ) );
	this->Disconnect( ID_cellgen, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showCellgen ) );
	this->Disconnect( ID_preferences, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::showPreferences ) );
	this->Disconnect( ID_refresh, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::refreshIcon ) );
	this->Disconnect( ID_help, wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( ICPD_frm::HelpaboutClick ) );
	wx_layouts->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_layouts->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ICPD_frm::layoutListEvent ), NULL, this );
	wx_layouts->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::viewLayout ), NULL, this );
	wx_instances->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_instances->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( ICPD_frm::openInstancewindow ), NULL, this );
	wx_instances->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printInstance ), NULL, this );
	wx_cells->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_cells->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printCell ), NULL, this );
	wx_nets->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_nets->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printNet ), NULL, this );
	wx_interfaces->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wx_interfaces->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( ICPD_frm::printInterface ), NULL, this );
	wx_log->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	wxCommand->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::CmdKeyDown ), NULL, this );
	wxCommand->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( ICPD_frm::command ), NULL, this );
	statusBar->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( ICPD_frm::PressedKey ), NULL, this );
	
}

Specify_FP::Specify_FP( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer8;
	fgSizer8 = new wxFlexGridSizer( 7, 4, 0, 0 );
	fgSizer8->SetFlexibleDirection( wxBOTH );
	fgSizer8->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	static_topcell = new wxStaticText( this, wxID_ANY, wxT("Top Cell:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_topcell->Wrap( -1 );
	fgSizer8->Add( static_topcell, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxArrayString cell_choiceChoices;
	cell_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 100,-1 ), cell_choiceChoices, 0 );
	cell_choice->SetSelection( 0 );
	fgSizer8->Add( cell_choice, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	ok_button = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( ok_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	static_rows = new wxStaticText( this, wxID_ANY, wxT("# Rows:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_rows->Wrap( -1 );
	fgSizer8->Add( static_rows, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxNrRows = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	wxNrRows->SetMaxLength( 0 ); 
	fgSizer8->Add( wxNrRows, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	fgSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	cancel_button = new wxButton( this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer8->Add( cancel_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	static_coreutil = new wxStaticText( this, wxID_ANY, wxT("Core Utilization:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_coreutil->Wrap( -1 );
	fgSizer8->Add( static_coreutil, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxUtilization = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	wxUtilization->SetMaxLength( 0 ); 
	fgSizer8->Add( wxUtilization, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	fgSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticline26 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer8->Add( m_staticline26, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline27 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer8->Add( m_staticline27, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline28 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer8->Add( m_staticline28, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline29 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer8->Add( m_staticline29, 0, wxEXPAND | wxALL, 5 );
	
	static_margin = new wxStaticText( this, wxID_ANY, wxT("Margins:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_margin->Wrap( -1 );
	fgSizer8->Add( static_margin, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	
	fgSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer8->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_cleft = new wxStaticText( this, wxID_ANY, wxT("Core to Left:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_cleft->Wrap( -1 );
	fgSizer8->Add( static_cleft, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxC2L = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	wxC2L->SetMaxLength( 0 ); 
	fgSizer8->Add( wxC2L, 0, wxALL|wxEXPAND, 5 );
	
	static_ctop = new wxStaticText( this, wxID_ANY, wxT("Core to Top:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_ctop->Wrap( -1 );
	fgSizer8->Add( static_ctop, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxC2T = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	wxC2T->SetMaxLength( 0 ); 
	fgSizer8->Add( wxC2T, 0, wxALL|wxEXPAND, 5 );
	
	static_cright = new wxStaticText( this, wxID_ANY, wxT("Core to Right:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_cright->Wrap( -1 );
	fgSizer8->Add( static_cright, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxC2R = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	wxC2R->SetMaxLength( 0 ); 
	fgSizer8->Add( wxC2R, 0, wxALL|wxEXPAND, 5 );
	
	static_cbottom = new wxStaticText( this, wxID_ANY, wxT("Core to Bottom:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_cbottom->Wrap( -1 );
	fgSizer8->Add( static_cbottom, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxC2B = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), 0 );
	wxC2B->SetMaxLength( 0 ); 
	fgSizer8->Add( wxC2B, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( fgSizer8 );
	this->Layout();
	fgSizer8->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Specify_FP::hide ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ) );
	ok_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Specify_FP::okButtonEvt ), NULL, this );
	ok_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxNrRows->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	cancel_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Specify_FP::cancelButtonEvt ), NULL, this );
	cancel_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxUtilization->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2L->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2T->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2R->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2B->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
}

Specify_FP::~Specify_FP()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Specify_FP::hide ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ) );
	ok_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Specify_FP::okButtonEvt ), NULL, this );
	ok_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxNrRows->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	cancel_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Specify_FP::cancelButtonEvt ), NULL, this );
	cancel_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxUtilization->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2L->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2T->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2R->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	wxC2B->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Specify_FP::PressedKey ), NULL, this );
	
}

CellGen::CellGen( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 19, 5, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	static_cellnet = new wxStaticText( this, wxID_ANY, wxT("Cell Netlist:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_cellnet->Wrap( -1 );
	fgSizer6->Add( static_cellnet, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString cellnet_choiceChoices;
	cellnet_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 100,-1 ), cellnet_choiceChoices, 0 );
	cellnet_choice->SetSelection( 0 );
	fgSizer6->Add( cellnet_choice, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	generate_button = new wxButton( this, wxID_ANY, wxT("&Generate Cell"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( generate_button, 0, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	select_button = new wxButton( this, wxID_ANY, wxT("&Select"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( select_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticline16 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline16, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline17 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline17, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline18 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline18, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline19 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline19, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline20 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline20, 0, wxEXPAND | wxALL, 5 );
	
	m_staticText75 = new wxStaticText( this, wxID_ANY, wxT("Conservative Gen.:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText75->Wrap( -1 );
	fgSizer6->Add( m_staticText75, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	reduceMetTracks = new wxSlider( this, wxID_ANY, 0, 0, 4, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer6->Add( reduceMetTracks, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText50 = new wxStaticText( this, wxID_ANY, wxT("Nr. of Internal Tracks"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText50->Wrap( -1 );
	fgSizer6->Add( m_staticText50, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	nrIntTracks = new wxTextCtrl( this, wxID_ANY, wxT("2"), wxDefaultPosition, wxDefaultSize, 0 );
	nrIntTracks->SetMaxLength( 0 ); 
	fgSizer6->Add( nrIntTracks, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	fold_button = new wxButton( this, wxID_ANY, wxT("&Fold Tr."), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( fold_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticline11 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline11, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline12 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline12, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline13 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline13, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline14 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline14, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline15 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline15, 0, wxEXPAND | wxALL, 5 );
	
	static_widcost = new wxStaticText( this, wxID_ANY, wxT("Width Cost:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_widcost->Wrap( -1 );
	fgSizer6->Add( static_widcost, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	p_WC = new wxTextCtrl( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxDefaultSize, 0 );
	p_WC->SetMaxLength( 0 ); 
	fgSizer6->Add( p_WC, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	static_gatemmcost = new wxStaticText( this, wxID_ANY, wxT("Gate Missmatch Cost:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_gatemmcost->Wrap( -1 );
	fgSizer6->Add( static_gatemmcost, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	p_GMC = new wxTextCtrl( this, wxID_ANY, wxT("4"), wxDefaultPosition, wxDefaultSize, 0 );
	p_GMC->SetMaxLength( 0 ); 
	fgSizer6->Add( p_GMC, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_rtcost = new wxStaticText( this, wxID_ANY, wxT("Routing Cost:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_rtcost->Wrap( -1 );
	fgSizer6->Add( static_rtcost, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	p_RC = new wxTextCtrl( this, wxID_ANY, wxT("1"), wxDefaultPosition, wxDefaultSize, 0 );
	p_RC->SetMaxLength( 0 ); 
	fgSizer6->Add( p_RC, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	static_rtdenscost = new wxStaticText( this, wxID_ANY, wxT("Routing Density Cost:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_rtdenscost->Wrap( -1 );
	fgSizer6->Add( static_rtdenscost, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	p_RTDC = new wxTextCtrl( this, wxID_ANY, wxT("4"), wxDefaultPosition, wxDefaultSize, 0 );
	p_RTDC->SetMaxLength( 0 ); 
	fgSizer6->Add( p_RTDC, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_gapcost = new wxStaticText( this, wxID_ANY, wxT("Nr. of Gaps Cost:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_gapcost->Wrap( -1 );
	fgSizer6->Add( static_gapcost, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	p_NGC = new wxTextCtrl( this, wxID_ANY, wxT("2"), wxDefaultPosition, wxDefaultSize, 0 );
	p_NGC->SetMaxLength( 0 ); 
	fgSizer6->Add( p_NGC, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_taqual = new wxStaticText( this, wxID_ANY, wxT("Nr. of iterations:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_taqual->Wrap( -1 );
	fgSizer6->Add( static_taqual, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	p_TAQ = new wxTextCtrl( this, wxID_ANY, wxT("150"), wxDefaultPosition, wxDefaultSize, 0 );
	p_TAQ->SetMaxLength( 0 ); 
	fgSizer6->Add( p_TAQ, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	static_numatt = new wxStaticText( this, wxID_ANY, wxT("Nr. of attempts:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_numatt->Wrap( -1 );
	fgSizer6->Add( static_numatt, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	p_NRA = new wxTextCtrl( this, wxID_ANY, wxT("3"), wxDefaultPosition, wxDefaultSize, 0 );
	p_NRA->SetMaxLength( 0 ); 
	fgSizer6->Add( p_NRA, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	place_button = new wxButton( this, wxID_ANY, wxT("&Place Tr."), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( place_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticline6 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline6, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline7 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline7, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline8 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline8, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline9 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline9, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline10 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline10, 0, wxEXPAND | wxALL, 5 );
	
	static_metal = new wxStaticText( this, wxID_ANY, wxT("Horizontal Poly:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_metal->Wrap( -1 );
	fgSizer6->Add( static_metal, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	hPoly = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	hPoly->SetValue(true); 
	fgSizer6->Add( hPoly, 0, wxALL, 5 );
	
	m_staticText73 = new wxStaticText( this, wxID_ANY, wxT("Align diffs top/bottom"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText73->Wrap( -1 );
	fgSizer6->Add( m_staticText73, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	increaseIntTracks = new wxCheckBox( this, wxID_ANY, wxT("(more internal tracks)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( increaseIntTracks, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText76 = new wxStaticText( this, wxID_ANY, wxT("Reduce Vertical Routing:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText76->Wrap( -1 );
	fgSizer6->Add( m_staticText76, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	reduceVRt = new wxCheckBox( this, wxID_ANY, wxT("(- width/+ congest.)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( reduceVRt, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText74 = new wxStaticText( this, wxID_ANY, wxT("Optimize:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText74->Wrap( -1 );
	fgSizer6->Add( m_staticText74, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	optimize = new wxCheckBox( this, wxID_ANY, wxT("(add steiner nodes)"), wxDefaultPosition, wxDefaultSize, 0 );
	optimize->SetValue(true); 
	fgSizer6->Add( optimize, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	route_button = new wxButton( this, wxID_ANY, wxT("&Route"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( route_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticline1 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline1, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline2 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline2, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline3 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline3, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline4 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline4, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline5 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline5, 0, wxEXPAND | wxALL, 5 );
	
	static_ds = new wxStaticText( this, wxID_ANY, wxT("Diffusion Stretching:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_ds->Wrap( -1 );
	fgSizer6->Add( static_ds, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	diffStretching = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	diffStretching->SetValue(true); 
	fgSizer6->Add( diffStretching, 0, wxALL, 5 );
	
	static_gp = new wxStaticText( this, wxID_ANY, wxT("Gridded Polly:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_gp->Wrap( -1 );
	fgSizer6->Add( static_gp, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	griddedPolly = new wxCheckBox( this, wxID_ANY, wxT("(align polys vertically)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( griddedPolly, 0, wxALL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_ddc = new wxStaticText( this, wxID_ANY, wxT("Redundant Diff. Cnts.:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_ddc->Wrap( -1 );
	fgSizer6->Add( static_ddc, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	rdCntsCost = new wxSlider( this, wxID_ANY, 50, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_LABELS );
	fgSizer6->Add( rdCntsCost, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText64 = new wxStaticText( this, wxID_ANY, wxT("# Max. Diff. Cnts.:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText64->Wrap( -1 );
	fgSizer6->Add( m_staticText64, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	maxDiffCnts = new wxSlider( this, wxID_ANY, 2, 1, 10, wxDefaultPosition, wxDefaultSize, wxSL_LABELS );
	fgSizer6->Add( maxDiffCnts, 0, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_adc = new wxStaticText( this, wxID_ANY, wxT("Align Diffusion Cnts.:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_adc->Wrap( -1 );
	fgSizer6->Add( static_adc, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	alignDiffCnts = new wxCheckBox( this, wxID_ANY, wxT("(reduce distance)"), wxDefaultPosition, wxDefaultSize, 0 );
	alignDiffCnts->SetValue(true); 
	fgSizer6->Add( alignDiffCnts, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText70 = new wxStaticText( this, wxID_ANY, wxT("Reduce L turns:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText70->Wrap( -1 );
	fgSizer6->Add( m_staticText70, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	reduceLturns = new wxCheckBox( this, wxID_ANY, wxT("(better quality)"), wxDefaultPosition, wxDefaultSize, 0 );
	reduceLturns->SetValue(true); 
	fgSizer6->Add( reduceLturns, 0, wxALL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText731 = new wxStaticText( this, wxID_ANY, wxT("Enable DFM:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText731->Wrap( -1 );
	fgSizer6->Add( m_staticText731, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	enableDFM = new wxCheckBox( this, wxID_ANY, wxT("(10% extra dist)"), wxDefaultPosition, wxDefaultSize, 0 );
	enableDFM->SetValue(true); 
	fgSizer6->Add( enableDFM, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText741 = new wxStaticText( this, wxID_ANY, wxT("Experimental:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText741->Wrap( -1 );
	fgSizer6->Add( m_staticText741, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	experimental = new wxCheckBox( this, wxID_ANY, wxT("(untested feat.)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( experimental, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	m_staticText65 = new wxStaticText( this, wxID_ANY, wxT("Debug:"), wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE );
	m_staticText65->Wrap( -1 );
	fgSizer6->Add( m_staticText65, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	debug = new wxCheckBox( this, wxID_ANY, wxT("(variable cell height)"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( debug, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	m_staticText71 = new wxStaticText( this, wxID_ANY, wxT("Time Limit (s):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText71->Wrap( -1 );
	fgSizer6->Add( m_staticText71, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	timeLimit = new wxTextCtrl( this, wxID_ANY, wxT("3600"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( timeLimit, 0, wxALL|wxEXPAND, 5 );
	
	compact_button = new wxButton( this, wxID_ANY, wxT("&Compact Layout"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( compact_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	m_staticline21 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline21, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline22 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline22, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline23 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline23, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline24 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline24, 0, wxEXPAND | wxALL, 5 );
	
	m_staticline25 = new wxStaticLine( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLI_HORIZONTAL );
	fgSizer6->Add( m_staticline25, 0, wxEXPAND | wxALL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	view_button = new wxButton( this, wxID_ANY, wxT("&View Layout"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( view_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( fgSizer6 );
	this->Layout();
	fgSizer6->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CellGen::hide ) );
	cellnet_choice->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	generate_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::generateCell ), NULL, this );
	select_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::select ), NULL, this );
	select_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	nrIntTracks->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	fold_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::fold ), NULL, this );
	fold_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_WC->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_GMC->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_RC->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_RTDC->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_NGC->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_TAQ->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_NRA->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	place_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::place ), NULL, this );
	place_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	route_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::route ), NULL, this );
	route_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	compact_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::compact ), NULL, this );
	compact_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	view_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::view ), NULL, this );
	view_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
}

CellGen::~CellGen()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( CellGen::hide ) );
	cellnet_choice->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	generate_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::generateCell ), NULL, this );
	select_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::select ), NULL, this );
	select_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	nrIntTracks->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	fold_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::fold ), NULL, this );
	fold_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_WC->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_GMC->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_RC->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_RTDC->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_NGC->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_TAQ->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	p_NRA->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	place_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::place ), NULL, this );
	place_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	route_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::route ), NULL, this );
	route_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	compact_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::compact ), NULL, this );
	compact_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	view_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CellGen::view ), NULL, this );
	view_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( CellGen::PressedKey ), NULL, this );
	
}

View_Circuit::View_Circuit( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer3;
	fgSizer3 = new wxFlexGridSizer( 14, 2, 0, 0 );
	fgSizer3->SetFlexibleDirection( wxVERTICAL );
	fgSizer3->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	static_design = new wxStaticText( this, wxID_ANY, wxT("Design Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design->Wrap( -1 );
	fgSizer3->Add( static_design, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	design_name = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( -1,-1 ), 0 );
	design_name->SetMaxLength( 0 ); 
	fgSizer3->Add( design_name, 0, wxALL|wxEXPAND, 5 );
	
	static_horizontal = new wxStaticText( this, wxID_ANY, wxT("Horizontal Grid:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_horizontal->Wrap( -1 );
	fgSizer3->Add( static_horizontal, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	horizontalgrid = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	horizontalgrid->SetMaxLength( 0 ); 
	fgSizer3->Add( horizontalgrid, 0, wxALL|wxEXPAND, 5 );
	
	static_vertical = new wxStaticText( this, wxID_ANY, wxT("Vertical Grid:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_vertical->Wrap( -1 );
	fgSizer3->Add( static_vertical, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	verticalgrid = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	verticalgrid->SetMaxLength( 0 ); 
	fgSizer3->Add( verticalgrid, 0, wxALL|wxEXPAND, 5 );
	
	m_staticText60 = new wxStaticText( this, wxID_ANY, wxT("H Grid Offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText60->Wrap( -1 );
	fgSizer3->Add( m_staticText60, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	hGridOffset = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( hGridOffset, 0, wxALL, 5 );
	
	m_staticText61 = new wxStaticText( this, wxID_ANY, wxT("V Grid Offset:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText61->Wrap( -1 );
	fgSizer3->Add( m_staticText61, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	vGridOffset = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( vGridOffset, 0, wxALL, 5 );
	
	static_vdd = new wxStaticText( this, wxID_ANY, wxT("Vdd Net Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_vdd->Wrap( -1 );
	fgSizer3->Add( static_vdd, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	vddnetname = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	vddnetname->SetMaxLength( 0 ); 
	fgSizer3->Add( vddnetname, 0, wxALL|wxEXPAND, 5 );
	
	static_gnd = new wxStaticText( this, wxID_ANY, wxT("Gnd Net Name:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_gnd->Wrap( -1 );
	fgSizer3->Add( static_gnd, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	gndnetname = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	gndnetname->SetMaxLength( 0 ); 
	fgSizer3->Add( gndnetname, 0, wxALL|wxEXPAND, 5 );
	
	static_row = new wxStaticText( this, wxID_ANY, wxT("Row Height:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_row->Wrap( -1 );
	fgSizer3->Add( static_row, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	rowheight = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	rowheight->SetMaxLength( 0 ); 
	fgSizer3->Add( rowheight, 0, wxALL|wxEXPAND, 5 );
	
	static_supply = new wxStaticText( this, wxID_ANY, wxT("Supply Size:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_supply->Wrap( -1 );
	fgSizer3->Add( static_supply, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	supplysize = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	supplysize->SetMaxLength( 0 ); 
	fgSizer3->Add( supplysize, 0, wxALL|wxEXPAND, 5 );
	
	static_supply1 = new wxStaticText( this, wxID_ANY, wxT("nWell Position:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_supply1->Wrap( -1 );
	fgSizer3->Add( static_supply1, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	nWellPos = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	nWellPos->SetMaxLength( 0 ); 
	fgSizer3->Add( nWellPos, 0, wxALL|wxEXPAND, 5 );
	
	static_supply12 = new wxStaticText( this, wxID_ANY, wxT("nWell Border:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_supply12->Wrap( -1 );
	fgSizer3->Add( static_supply12, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	nWellBorder = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	nWellBorder->SetMaxLength( 0 ); 
	fgSizer3->Add( nWellBorder, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	static_supply121 = new wxStaticText( this, wxID_ANY, wxT("pnSel Border:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_supply121->Wrap( -1 );
	fgSizer3->Add( static_supply121, 0, wxALL|wxALIGN_RIGHT, 5 );
	
	pnSelBorder = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	pnSelBorder->SetMaxLength( 0 ); 
	fgSizer3->Add( pnSelBorder, 0, wxALL|wxEXPAND, 5 );
	
	static_supply11 = new wxStaticText( this, wxID_ANY, wxT("StdCell Template:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_supply11->Wrap( -1 );
	fgSizer3->Add( static_supply11, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxArrayString cellTemplateChoices;
	cellTemplate = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 130,-1 ), cellTemplateChoices, 0 );
	cellTemplate->SetSelection( 0 );
	fgSizer3->Add( cellTemplate, 0, wxALL|wxEXPAND, 5 );
	
	ok_button = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( ok_button, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	cancel_button = new wxButton( this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer3->Add( cancel_button, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( fgSizer3 );
	this->Layout();
	fgSizer3->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( View_Circuit::hide ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ) );
	design_name->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	design_name->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_Circuit::applyButtonEvt ), NULL, this );
	horizontalgrid->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	verticalgrid->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	vddnetname->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	gndnetname->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	rowheight->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	supplysize->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	nWellPos->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	nWellBorder->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	pnSelBorder->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	ok_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Circuit::okButtonEvt ), NULL, this );
	ok_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	cancel_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Circuit::cancelButtonEvt ), NULL, this );
	cancel_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
}

View_Circuit::~View_Circuit()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( View_Circuit::hide ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ) );
	design_name->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	design_name->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_Circuit::applyButtonEvt ), NULL, this );
	horizontalgrid->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	verticalgrid->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	vddnetname->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	gndnetname->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	rowheight->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	supplysize->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	nWellPos->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	nWellBorder->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	pnSelBorder->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	ok_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Circuit::okButtonEvt ), NULL, this );
	ok_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	cancel_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Circuit::cancelButtonEvt ), NULL, this );
	cancel_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Circuit::PressedKey ), NULL, this );
	
}

View_Preferences::View_Preferences( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer4;
	fgSizer4 = new wxFlexGridSizer( 6, 3, 0, 0 );
	fgSizer4->SetFlexibleDirection( wxBOTH );
	fgSizer4->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	static_plPath = new wxStaticText( this, wxID_ANY, wxT("Placement Tool:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_plPath->Wrap( -1 );
	fgSizer4->Add( static_plPath, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	plPath = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 300,-1 ), 0 );
	plPath->SetMaxLength( 0 ); 
	fgSizer4->Add( plPath, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	plPath_button = new wxButton( this, wxID_ANY, wxT("Choose"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( plPath_button, 0, wxALL, 5 );
	
	static_rotdl = new wxStaticText( this, wxID_ANY, wxT("Rotdl path:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_rotdl->Wrap( -1 );
	fgSizer4->Add( static_rotdl, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	rotdlPath = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	rotdlPath->SetMaxLength( 0 ); 
	fgSizer4->Add( rotdlPath, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	rotdlPath_button = new wxButton( this, wxID_ANY, wxT("Choose"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( rotdlPath_button, 0, wxALL, 5 );
	
	static_viewer = new wxStaticText( this, wxID_ANY, wxT("CIF viewer path:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_viewer->Wrap( -1 );
	fgSizer4->Add( static_viewer, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	viewerPath = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	viewerPath->SetMaxLength( 0 ); 
	fgSizer4->Add( viewerPath, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	viewerPath_button = new wxButton( this, wxID_ANY, wxT("Choose"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( viewerPath_button, 0, wxALL, 5 );
	
	static_lpsolver = new wxStaticText( this, wxID_ANY, wxT("LPSolver path:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_lpsolver->Wrap( -1 );
	fgSizer4->Add( static_lpsolver, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	lpsolverPath = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	lpsolverPath->SetMaxLength( 0 ); 
	fgSizer4->Add( lpsolverPath, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	lpsolverPath_button = new wxButton( this, wxID_ANY, wxT("Choose"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( lpsolverPath_button, 0, wxALL, 5 );
	
	static_logf = new wxStaticText( this, wxID_ANY, wxT("Log File:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_logf->Wrap( -1 );
	fgSizer4->Add( static_logf, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	logPath = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	logPath->SetMaxLength( 0 ); 
	fgSizer4->Add( logPath, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	logPath_button = new wxButton( this, wxID_ANY, wxT("Choose"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( logPath_button, 0, wxALL, 5 );
	
	ok_button = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( ok_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	fgSizer4->Add( 0, 0, 1, wxEXPAND, 5 );
	
	cancel_button = new wxButton( this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer4->Add( cancel_button, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( fgSizer4 );
	this->Layout();
	fgSizer4->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( View_Preferences::hide ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ) );
	plPath->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	plPath_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::plButton ), NULL, this );
	plPath_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	rotdlPath->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	rotdlPath_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::rotdlButton ), NULL, this );
	rotdlPath_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	viewerPath->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	viewerPath_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::viewerButton ), NULL, this );
	viewerPath_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	lpsolverPath->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	lpsolverPath_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::lpsolverButton ), NULL, this );
	lpsolverPath_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	logPath->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	logPath_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::logButton ), NULL, this );
	logPath_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	ok_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::okButtonEvt ), NULL, this );
	ok_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	cancel_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::cancelButtonEvt ), NULL, this );
	cancel_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
}

View_Preferences::~View_Preferences()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( View_Preferences::hide ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ) );
	plPath->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	plPath_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::plButton ), NULL, this );
	plPath_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	rotdlPath->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	rotdlPath_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::rotdlButton ), NULL, this );
	rotdlPath_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	viewerPath->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	viewerPath_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::viewerButton ), NULL, this );
	viewerPath_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	lpsolverPath->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	lpsolverPath_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::lpsolverButton ), NULL, this );
	lpsolverPath_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	logPath->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	logPath_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::logButton ), NULL, this );
	logPath_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	ok_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::okButtonEvt ), NULL, this );
	ok_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	cancel_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_Preferences::cancelButtonEvt ), NULL, this );
	cancel_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_Preferences::PressedKey ), NULL, this );
	
}

Design_Rules::Design_Rules( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 8, 4, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	static_technology = new wxStaticText( this, wxID_ANY, wxT("Technology:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_technology->Wrap( -1 );
	fgSizer2->Add( static_technology, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxTechName = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), 0 );
	wxTechName->SetMaxLength( 0 ); 
	fgSizer2->Add( wxTechName, 0, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	static_soi = new wxStaticText( this, wxID_ANY, wxT("SOI:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_soi->Wrap( -1 );
	fgSizer2->Add( static_soi, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxSOI = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( wxSOI, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	static_mlayers = new wxStaticText( this, wxID_ANY, wxT("Metal Layers:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_mlayers->Wrap( -1 );
	fgSizer2->Add( static_mlayers, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxMLayers = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	wxMLayers->SetMaxLength( 0 ); 
	fgSizer2->Add( wxMLayers, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	static_resolution = new wxStaticText( this, wxID_ANY, wxT("Resolution:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_resolution->Wrap( -1 );
	fgSizer2->Add( static_resolution, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxResolution = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 100,-1 ), 0 );
	wxResolution->SetMaxLength( 0 ); 
	fgSizer2->Add( wxResolution, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	static_rule = new wxStaticText( this, wxID_ANY, wxT("Rule:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_rule->Wrap( -1 );
	fgSizer2->Add( static_rule, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxArrayString wxSelect_RuleChoices;
	wxSelect_Rule = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSelect_RuleChoices, 0 );
	wxSelect_Rule->SetSelection( 0 );
	fgSizer2->Add( wxSelect_Rule, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	static_RuleDesc = new wxStaticText( this, wxID_ANY, wxT("Rule Description"), wxDefaultPosition, wxDefaultSize, 0 );
	static_RuleDesc->Wrap( -1 );
	fgSizer2->Add( static_RuleDesc, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_value = new wxStaticText( this, wxID_ANY, wxT("Value:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_value->Wrap( -1 );
	fgSizer2->Add( static_value, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxRule_Value = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	wxRule_Value->SetMaxLength( 0 ); 
	fgSizer2->Add( wxRule_Value, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_layerlabel = new wxStaticText( this, wxID_ANY, wxT("Layer Label:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_layerlabel->Wrap( -1 );
	fgSizer2->Add( static_layerlabel, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxArrayString wxSelect_LayerChoices;
	wxSelect_Layer = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSelect_LayerChoices, 0 );
	wxSelect_Layer->SetSelection( 0 );
	fgSizer2->Add( wxSelect_Layer, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	static_LayerDesc = new wxStaticText( this, wxID_ANY, wxT("Layer Description"), wxDefaultPosition, wxDefaultSize, 0 );
	static_LayerDesc->Wrap( -1 );
	fgSizer2->Add( static_LayerDesc, 0, wxALL|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_cif = new wxStaticText( this, wxID_ANY, wxT("CIF:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_cif->Wrap( -1 );
	fgSizer2->Add( static_cif, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxCIF = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	wxCIF->SetMaxLength( 0 ); 
	fgSizer2->Add( wxCIF, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	static_gds = new wxStaticText( this, wxID_ANY, wxT("GDSII:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_gds->Wrap( -1 );
	fgSizer2->Add( static_gds, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxGDSII = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	wxGDSII->SetMaxLength( 0 ); 
	fgSizer2->Add( wxGDSII, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	static_tech = new wxStaticText( this, wxID_ANY, wxT("Tech:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_tech->Wrap( -1 );
	fgSizer2->Add( static_tech, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );
	
	wxTechVal = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	wxTechVal->SetMaxLength( 0 ); 
	fgSizer2->Add( wxTechVal, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	save_button = new wxButton( this, wxID_ANY, wxT("&Save"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( save_button, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer2->Add( 0, 0, 1, wxEXPAND, 5 );
	
	close_button = new wxButton( this, wxID_ANY, wxT("&Close"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( close_button, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( fgSizer2 );
	this->Layout();
	fgSizer2->Fit( this );
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Design_Rules::hide ) );
	this->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ) );
	wxTechName->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxSOI->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxMLayers->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxResolution->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxSelect_Rule->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Design_Rules::selectRule ), NULL, this );
	wxSelect_Rule->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxRule_Value->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxSelect_Layer->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Design_Rules::selectLayer ), NULL, this );
	wxSelect_Layer->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxCIF->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxGDSII->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxTechVal->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	save_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Design_Rules::saveButtonEvt ), NULL, this );
	save_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	close_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Design_Rules::closeButtonEvt ), NULL, this );
	close_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
}

Design_Rules::~Design_Rules()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Design_Rules::hide ) );
	this->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ) );
	wxTechName->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxSOI->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxMLayers->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxResolution->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxSelect_Rule->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Design_Rules::selectRule ), NULL, this );
	wxSelect_Rule->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxRule_Value->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxSelect_Layer->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( Design_Rules::selectLayer ), NULL, this );
	wxSelect_Layer->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxCIF->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxGDSII->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	wxTechVal->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	save_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Design_Rules::saveButtonEvt ), NULL, this );
	save_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	close_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( Design_Rules::closeButtonEvt ), NULL, this );
	close_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( Design_Rules::PressedKey ), NULL, this );
	
}

Help_About::Help_About( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxSize( -1,-1 ) );
	
	wxBoxSizer* bSizer10;
	bSizer10 = new wxBoxSizer( wxVERTICAL );
	
	credits = new wxStaticText( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	credits->Wrap( -1 );
	bSizer10->Add( credits, 0, wxALL|wxEXPAND, 5 );
	
	
	this->SetSizer( bSizer10 );
	this->Layout();
	
	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Help_About::hide ) );
}

Help_About::~Help_About()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( Help_About::hide ) );
	
}

View_PrintGP::View_PrintGP( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxFlexGridSizer* fgSizer6;
	fgSizer6 = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizer6->SetFlexibleDirection( wxBOTH );
	fgSizer6->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	static_design1 = new wxStaticText( this, wxID_ANY, wxT("Optimize:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design1->Wrap( -1 );
	fgSizer6->Add( static_design1, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString optimize_choiceChoices[] = { wxT("delay"), wxT("area") };
	int optimize_choiceNChoices = sizeof( optimize_choiceChoices ) / sizeof( wxString );
	optimize_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxSize( 100,-1 ), optimize_choiceNChoices, optimize_choiceChoices, 0 );
	optimize_choice->SetSelection( 0 );
	fgSizer6->Add( optimize_choice, 0, wxALL|wxEXPAND, 5 );
	
	static_design13 = new wxStaticText( this, wxID_ANY, wxT("Sizing Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design13->Wrap( -1 );
	fgSizer6->Add( static_design13, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString sizingtype_choiceChoices[] = { wxT("transistor"), wxT("gate") };
	int sizingtype_choiceNChoices = sizeof( sizingtype_choiceChoices ) / sizeof( wxString );
	sizingtype_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, sizingtype_choiceNChoices, sizingtype_choiceChoices, 0 );
	sizingtype_choice->SetSelection( 0 );
	fgSizer6->Add( sizingtype_choice, 0, wxALL|wxEXPAND, 5 );
	
	static_design12 = new wxStaticText( this, wxID_ANY, wxT("Technology:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design12->Wrap( -1 );
	fgSizer6->Add( static_design12, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	wxString technology_choiceChoices[] = { wxT("45nm"), wxT("350nm") };
	int technology_choiceNChoices = sizeof( technology_choiceChoices ) / sizeof( wxString );
	technology_choice = new wxChoice( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, technology_choiceNChoices, technology_choiceChoices, 0 );
	technology_choice->SetSelection( 0 );
	fgSizer6->Add( technology_choice, 0, wxALL|wxEXPAND, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_design11 = new wxStaticText( this, wxID_ANY, wxT("Parameters:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design11->Wrap( -1 );
	fgSizer6->Add( static_design11, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	
	fgSizer6->Add( 0, 0, 1, wxEXPAND, 5 );
	
	static_design112 = new wxStaticText( this, wxID_ANY, wxT("Cload:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design112->Wrap( -1 );
	fgSizer6->Add( static_design112, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	parameterCload = new wxTextCtrl( this, wxID_ANY, wxT("4*1.434879e-16"), wxDefaultPosition, wxSize( 150,-1 ), 0 );
	parameterCload->SetMaxLength( 0 ); 
	fgSizer6->Add( parameterCload, 0, wxALL|wxEXPAND, 5 );
	
	static_design111 = new wxStaticText( this, wxID_ANY, wxT("Max Area:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design111->Wrap( -1 );
	fgSizer6->Add( static_design111, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	parameterMaxArea = new wxTextCtrl( this, wxID_ANY, wxT("3.255"), wxDefaultPosition, wxDefaultSize, 0 );
	parameterMaxArea->SetMaxLength( 0 ); 
	fgSizer6->Add( parameterMaxArea, 0, wxALL|wxEXPAND, 5 );
	
	static_design1111 = new wxStaticText( this, wxID_ANY, wxT("Max Delay:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design1111->Wrap( -1 );
	fgSizer6->Add( static_design1111, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	parameterDelay = new wxTextCtrl( this, wxID_ANY, wxT("4.29113e-10"), wxDefaultPosition, wxDefaultSize, 0 );
	parameterDelay->SetMaxLength( 0 ); 
	fgSizer6->Add( parameterDelay, 0, wxALL|wxEXPAND, 5 );
	
	static_design11111 = new wxStaticText( this, wxID_ANY, wxT("Max Cin:"), wxDefaultPosition, wxDefaultSize, 0 );
	static_design11111->Wrap( -1 );
	fgSizer6->Add( static_design11111, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );
	
	parameterMaxCin = new wxTextCtrl( this, wxID_ANY, wxT("1.0001"), wxDefaultPosition, wxDefaultSize, 0 );
	parameterMaxCin->SetMaxLength( 0 ); 
	fgSizer6->Add( parameterMaxCin, 0, wxALL|wxEXPAND|wxALIGN_CENTER_VERTICAL, 5 );
	
	ok_button = new wxButton( this, wxID_ANY, wxT("&OK"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( ok_button, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	cancel_button = new wxButton( this, wxID_ANY, wxT("&Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer6->Add( cancel_button, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );
	
	
	this->SetSizer( fgSizer6 );
	this->Layout();
	fgSizer6->Fit( this );
	
	// Connect Events
	parameterCload->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterCload->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	parameterMaxArea->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterMaxArea->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	parameterDelay->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterDelay->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	parameterMaxCin->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterMaxCin->Connect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	ok_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_PrintGP::okButtonEvt ), NULL, this );
	ok_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	cancel_button->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_PrintGP::cancelButtonEvt ), NULL, this );
	cancel_button->Connect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
}

View_PrintGP::~View_PrintGP()
{
	// Disconnect Events
	parameterCload->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterCload->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	parameterMaxArea->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterMaxArea->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	parameterDelay->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterDelay->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	parameterMaxCin->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	parameterMaxCin->Disconnect( wxEVT_COMMAND_TEXT_ENTER, wxCommandEventHandler( View_PrintGP::applyButtonEvt ), NULL, this );
	ok_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_PrintGP::okButtonEvt ), NULL, this );
	ok_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	cancel_button->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( View_PrintGP::cancelButtonEvt ), NULL, this );
	cancel_button->Disconnect( wxEVT_KEY_DOWN, wxKeyEventHandler( View_PrintGP::PressedKey ), NULL, this );
	
}
