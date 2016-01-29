/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
 *   amziesemer[at]inf.ufrgs.br                                            *
 ***************************************************************************/
#pragma once

/**
 Repository for entire design
 
 @author Adriel Mota Ziesemer Jr.
 */

#include <list>
#include <vector>
#include <string>
#include "clayout.h"
#include "circuit.h"
#include "draw.h"
#include "router.h"
#include "util.h"
#include "placer.h"
#include "spice.h"
#include "rules.h"
#include "autocell2.h"
#include "verilog.h"

#include "cif.h"
#include "lef.h"
#include "gds.h"

class DesignMng{
public:
	DesignMng();
	~DesignMng();
	
	bool readCommand(string cmd);
	int getPosCmdLog(){return poscmdlog;};
	void setPosCmdLog(int pos){poscmdlog = pos;};
	vector<string> getCommandLog(){return commandlog;};
	int getCommandCode(vector<string> shell_cmd);
	string getViewer(){return viewerProgram;};
	string getlpSolver(){return lpSolverFile;};
	string getRotDL(){return rotdlFile;};
	string getPlacerFile(){return placerFile;};
	string getLog(){return historyFile;};
	Rules* getRules(){return rules.get();};
	Router* getRouter(){return router.get();};
	Circuit* getCircuit(){return circuit.get();};
	Placer* getPlacer(){return placer.get();};
	AutoCell* getAutoCell(){return autocell.get();};
	void setName(string s){name=s;};
	string getName(){return name;};
    
private:
    enum command_name {
        // New - 2
        NEW_DESIGN,NEW_CELL,
        // Load - 6
        LOAD_PROJECT,LOAD_TECHNOLOGY,LOAD_NETLIST,LOAD_LAYOUTS,LOAD_PLACEMENT,LOAD_ROUTING,
        // Save - 7
        SAVE_PROJECT,SAVE_TECHNOLOGY,SAVE_NETLIST,SAVE_LAYOUTS,SAVE_PLACEMENT,SAVE_ROUTING,SAVE_HISTORY,
        // Import - 2
        IMPORT_NETLIST,IMPORT_LEF,
        // Export - 3
        EXPORT_LAYOUT,EXPORT_CELLSIZES,EXPORT_PLACEMENT,
        READ,
        // Place - 7
        PLACE_TERMINALS,PLACE_FPLACE,PLACE_GETWL,PLACE_INSTANCE,PLACE_AUTOFLIP,PLACE_INCREMENTAL,PLACE_CHECK,
        // Route - 6
        ROUTE_ROTDL,ROUTE_PFINDER,ROUTE_OPTIMIZE,ROUTE_COMPACT,ROUTE_TEST,ROUTE_CLEAR,
        // Print - 4
        PRINT_INSTANCE,PRINT_CELL,PRINT_NET,PRINT_INTERFACE,
        // Preferences - 6
        SET_PLACER,SET_ROTDL,SET_VIEWER,SET_LPSOLVE,SET_LOG,SET_VERBOSEMODE,
        // Technology - 8
        SET_TECH_NAME,SET_TECH_MLAYERS,SET_TECH_SOI,SET_TECH_RESOLUTION,SET_TECH_RULE,SET_TECH_CIF,SET_TECH_GDSII,SET_TECH_VALTECH,
        // Circuit - 8
        SET_DESIGNNAME,SET_GRID,SET_HGRID,SET_VGRID,SET_HGRID_OFFSET,SET_VGRID_OFFSET,SET_VDDNET,SET_GNDNET,SET_ROWHEIGHT,SET_SUPPLYSIZE, SET_NWELLPOS, SET_NWELLBORDER, SET_PNSELBORDER, SET_CELLTEMPLATE,
        // Floorplan - 3
        SET_TOPCELL,SET_AREA,SET_MARGINS,
        CALCPINSPOS,
        // CellGen - 9
        CELLGEN_SELECT,CELLGEN_AUTOFLOW,CELLGEN_FOLD,CELLGEN_PLACE,CELLGEN_GETARCCOST,CELLGEN_SETARCCOST,CELLGEN_ROUTE,CELLGEN_COMPACT,
        // Help - 2
        HELP, HELP_PARAM,
        
        COMMENT,EXIT,
        NR_COMMANDS
    };
    
    struct Command {
        string name;
        string desc;
    };
    
    const Command commands_lst[NR_COMMANDS] = {
        // New - 2
        {"NEW DESIGN <str_Name>", "Clear current design and start a new one"},
        {"NEW CELL <str_Name>", "Create a new cell, it will appear in the Cells list box"},
        
        // Load - 6
        {"LOAD PROJECT <str_Path>", "Opens a project file previously saved from ICPD. Supported extension: .icp (ICPD Project File)"},
        {"LOAD TECHNOLOGY <str_Path>", "Opens the technology file with the rules to set for the design. Supported extension: .rul (Rules File)"},
        {"LOAD NETLIST <str_Path>", "Opens the netlist file with circuit description. Supported extensions: .sp (Spice), .v (Structural Verilog)"},
        {"LOAD LAYOUTS <str_Path>", "Opens the layouts file previously saved from ICPD. Supported extension: .lay (ICPD Layouts Format)"},
        {"LOAD PLACEMENT <str_Path>", "Opens the placement file. Supported extensions: .pl (Book Shelf), .mpp (Mango Parrot)"},
        {"LOAD ROUTING <str_Path>", "Opens the routing file. Supported extension: .rot (Rotdl Routing Format)"},
        
        // Save - 7
        {"SAVE PROJECT <str_Path>", "Saves the current project and configurations. Supported extension: .icp (ICPD Project File)"},
        {"SAVE TECHNOLOGY <str_Path>", "Saves the current technology rules to file. Supported extension: .rul (Rules File)"},
        {"SAVE NETLIST <str_Path>", "Saves the spice netlist to file. Supported extension: .sp (Spice)"},
        {"SAVE LAYOUTS <str_Path>", "Saves the current layouts to file. Supported extension: .lay (ICPD Layouts Format)"},
        {"SAVE PLACEMENT <str_Path>", "Saves placement to file. Supported extension: .pl (Book Shelf)"},
        {"SAVE ROUTING <str_Path>", "Saves routing to file. Supported extension: .rot (Rotdl Routing Format)"},
        {"SAVE HISTORY <str_Path>", "Saves history log to a script file. Supported extension: .run (Script File)"},
        
        // Import - 2
        {"IMPORT NETLIST <str_Path>", "Imports netlist file generated by Cadence. Supported extensions: .sp (Spice) and Cadence Netlist Files"},
        {"IMPORT LEF <str_Path>", "Imports LEF file for layouts. Supported extension: .lef (Library Exchange Format)"},
        
        // Export - 3
        {"EXPORT LAYOUT <str_Layout> <str_Path>", "Exports layout generated by ICPD to open with other applications. Supported extensions: .cif (Calltech Intermediate Format), .gds (Graphic Data System II), .c2c (CIF to Cadence Convertion File)"},
        {"EXPORT CELLSIZES <str_Path>", "Exports cell sizes file. Supported extension: .est (Mango Parrot Cell Estimative)"},
        {"EXPORT PLACEMENT <str_Path>", "Exports placement generated by ICPD to open with Cadence. Supported extension: .place (Cadence Placement File)"},
        
        {"READ <str_Path>", "Read script file. Supported extension: .run (Script File)"},
        
        // Place - 7
        {"PLACE TERMINALS", "Make placement of interface terminals"},
        {"PLACE FPLACE", ""},
        {"PLACE GETWL", "Check width and length of placement"},
        {"PLACE INSTANCE <str_Layout> <str_Instance> <int_PosX> <int_PosY> <bool_MX> <bool_MY>", "Make placement of instance"},
        {"PLACE AUTOFLIP", "Make cell autoflip"},
        {"PLACE INCREMENTAL", "Make the incremental placement"},
        {"PLACE CHECK", "Check cell's placement"},
        
        // Route - 6
        {"ROUTE ROTDL", "Make routing using Rotdl"},
        {"ROUTE PFINDER <int_Iterations>", "Make routing using Pathfinder"},
        {"ROUTE OPTIMIZE", "Optmize the routing"},
        {"ROUTE COMPACT", "Compact the routing using LPSolver"},
        {"ROUTE TEST", "Make a test of routing to compare"},
        {"ROUTE CLEAR", "Clear the current routing"},
        
        // Print - 4
        {"PRINT INSTANCE <str_Layout> <str_Instance>", "Print the cell's instance's information on the screen"},
        {"PRINT CELL <str_Cell>", "Print the cell's information on the screen"},
        {"PRINT NET <str_Net>", "Print the net's information on the screen"},
        {"PRINT INTERFACE <str_Interface>", "Print the interface's information on the screen"},
        
        // Preferences - 6
        {"SET PLACER <str_Path>", "Set the Mobralplace path"},
        {"SET ROTDL <str_Path>", "Set the Rotdl path"},
        {"SET VIEWER <str_Path>", "Set the viewer path"},
        {"SET LPSOLVE <str_Path>", "Set the LPSolver path"},
        {"SET LOG <str_Path>", "Set the path to salve the log"},
        {"SET VERBOSE_MODE <int_Mode>", "Set the verbose mode to able or disable"},
        
        // Technology - 8
        {"SET TECHNOLOGY NAME <str_Name>", "Set a new name to the current technology rule"},
        {"SET TECHNOLOGY MLAYERS <int_Number>", "Set the number of metal layers supported by the current technology"},
        {"SET TECHNOLOGY SOI <str_YES/NO>", "Set if the current technology supports SOI. Parameter must be \"YES\" or \"NO\""},
        {"SET TECHNOLOGY RESOLUTION <int_Value>", "Set the resolution of the current technology rule"},
        {"SET TECHNOLOGY RULE <str_Rule> <float_Value>", "Set a new value for a specific rule"},
        {"SET TECHNOLOGY CIF <str_Layer> <str_Value>", "Set a new value for a specific CIF layer label"},
        {"SET TECHNOLOGY GDSII <str_Layer> <str_Value>", "Set a new value for a specific GDSII layer label"},
        {"SET TECHNOLOGY VAL_TECH <str_Layer> <str_Value>", "Set a new value for a specific ValTech layer label"},
        
        // Circuit - 8
        {"SET DESIGNNAME <str_Name>", "Set a new name to the current design"},
        {"SET GRID <float_HPitch> <float_VPitch>", "Set new values to the grid (horizontal and vertical pitchs)"},
        {"SET HGRID <float_Value>", "Set a new value to the horizontal pitch grid"},
        {"SET VGRID <float_value>", "Set a new value to the vertical pitch grid"},
        {"SET HGRID_OFFSET <str_YES/NO>", "Set the horizontal grid offset"},
        {"SET VGRID_OFFSET <str_YES/NO>", "Set the vertical grid offset"},
        {"SET VDDNET <str_Name>", "Set a new name to the VDD Net"},
        {"SET GNDNET <str_Name>", "Set a new name to the GND Net"},
        {"SET ROWHEIGHT <int_Value>", "Set a new value to the circuit's row height"},
        {"SET SUPPLYSIZE <float_Value>", "Set a new value to the circuit's supply size"},
        {"SET NWELLPOS <float_Value>", "Set a new value to the nWell position"},
        {"SET NWELLBORDER <float_Value>", "Set a new value to the nWell cell border"},
        {"SET PNSELBORDER <float_Value>", "Set a new value to the P/N Sel/Plus cell border"},
        {"SET CELLTEMPLATE <str_Name>", "Chooses the standard cell template"},
        
        // Floorplan - 3
        {"SET TOPCELL <str_Cell>", "Set a new cell as the top cell"},
        {"SET AREA <int_#Rows> <float_Utilization>", "Set the floorplan area"},
        {"SET MARGINS <float_Left> <float_Right> <float_Top> <float_Bottom>", "Set new values to the floorplan margins"},
        
        {"CALCPINSPOS", "Calculate the pins' position"},
        
        // CellGen - 9
        {"CELLGEN SELECT <str_Cell>", "Select a cell to generate the layout using Cellgen"},
        {"CELLGEN AUTOFLOW", "Try to generate cell layout automatically in the best possible way"},
        {"CELLGEN FOLD <int_nrIntTracks> <int_conservativeFactor>", "Fold the cell's transistors with Cellgen"},
        {"CELLGEN PLACE <int_Saquality> <int_#Attempts> <int_WidthCost> <int_GateMissMatchCost> <int_RoutingCost> <int_RtDensityCost> <int_#GapsCost>", "Place the cell's transistors with Cellgen"},
        {"CELLGEN GETARCCOST <int_Node1> <int_Node2>", "Get the arc cost between two nodes using Cellgen"},
        {"CELLGEN SETARCCOST <int_Node1> <int_Node2> <int_Cost>", "Set the arc cost between two nodes using Cellgen"},
        {"CELLGEN ROUTE <int_hPoly> <int_incrementIntTracks> <int_reduceVRt> <int_optimize>", "Make the routing of the cell with Cellgen"},
        {"CELLGEN COMPACT <int_diffStretching> <int_griddedPolly> <int_rdCntsCost> <int_maxDiffCnts> <int_alignDiffConts> <int_reduceLTurns> <int_enableDFM> <int_experimental> <int_debug> <int_timeLimit>", "Compact the cell's layout with Cellgen"},
        
        // Help - 2
        {"HELP", "Print the commands' usage on the screen"},
        {"HELP <str_Command>", "Prints the specified commands with details on the screen. Use double quotes (\"this is an example\") for words in the same parameter"},
        
        {"*", "Comment"},
        {"EXIT", "Close the program"}
    };
    
    string name;
    std::unique_ptr<Circuit> circuit =  std::make_unique<Circuit>();
    std::unique_ptr<Router> router =  std::make_unique<Router>();
    std::unique_ptr<Rules> rules =  std::make_unique<Rules>();
    std::unique_ptr<Placer> placer =  std::make_unique<Placer>();
    std::unique_ptr<AutoCell> autocell =  std::make_unique<AutoCell>();
    
    int verboseMode, poscmdlog;
    
    string historyFile, placerFile, rotdlFile, viewerProgram, lpSolverFile;
    vector<string> commandlog;
    void addToHistoryLog(string cmd);
    void saveHistory(string filename);
    void saveProjectConfig(string filename, string project_name);
    void run(string filename);
};