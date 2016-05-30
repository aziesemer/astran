/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
 *   amziesemerj[at]inf.ufrgs.br                                           *
 ***************************************************************************/
#include "designmng.h"

DesignMng::DesignMng():viewerProgram(""),verboseMode(0),poscmdlog(0),historyFile(""){
    circuit->setRules(rules.get());
    placer->setCircuit(circuit.get());
    router->setCircuit(circuit.get());
    setName("astran_project");
}

DesignMng::~DesignMng(){}

// vector shell_cmd: command received from shell
// vector reg_cmd: command registered in commands_lst[]
int DesignMng::getCommandCode(vector<string> shell_cmd){
	string cmd_tmp;
	vector<string> reg_cmd;
	bool same_cmd;
	
    if (shell_cmd[0]==commands_lst[COMMENT].name ) return COMMENT;
        
	// compare all the commands of the array
	for (int i=0; i<NR_COMMANDS; ++i){
		same_cmd = true;
		reg_cmd.clear();
		
		// separate the tokens
		istrstream cmd_stream(commands_lst[i].name.c_str());
		while (cmd_stream>>cmd_tmp)
			reg_cmd.push_back(cmd_tmp);
		
		if (reg_cmd.size() == shell_cmd.size()){
			int j;
			// compare all the tokens between the two commands
			for (j=0; j<reg_cmd.size(); ++j)
				if ((reg_cmd[j][0] != '<') && (reg_cmd[j] != upcase(shell_cmd[j])))
					same_cmd = false;
			
			if(same_cmd){
				// compare the parameters if they are numbers when expected
				for (j=0; j<reg_cmd.size(); ++j)
					if (((reg_cmd[j].substr(0,4) == "<int") || (reg_cmd[j].substr(0,6) == "<float")) && (!isNumber(shell_cmd[j])))
						throw AstranError("Parameters invalid");
				return (i); // OK - command found and valid parameters, return its code
			}
		}
	}
	throw AstranError("Command invalid or wrong number of parameters");
}

void DesignMng::run(string filename) {
	cout << "-> Reading file: " << filename << endl;
	ifstream file(filename.c_str()); // Read
	if (!file)
		throw AstranError("File not found");
    
	int line=1;
	string str_tmp;
	while(getline(file, str_tmp)){
		if((str_tmp[0] != '*') && !readCommand(str_tmp))
            cout << "-> WARNING: Could not execute line " << to_string(line) << " of: " << filename << endl;
		line++;
	}
}

void DesignMng::addToHistoryLog(string cmd){
	if(historyFile!=""){
		ofstream file;
		file.open(historyFile.c_str(),ios_base::app); // Write
		if (!file)
            throw AstranError("Could not save history to file: " + historyFile); 
		else
			if(cmd.size())
				file << cmd << endl;
		file.close();
	}
}

void DesignMng::saveHistory(string filename){
	ofstream file_out;
	file_out.open(filename.c_str());
	
	if (!file_out)
        throw AstranError("Could not save history to file: " + filename);
    
	if (commandlog.size() > 1) {
		for (int i=0; i < commandlog.size()-1; ++i)
			file_out << commandlog[i] << endl;
	} else 
		cout << "-> Command log is empty\n";
	
	file_out.close();
}

void DesignMng::saveProjectConfig(string filename, string project_name){
	ofstream file_out;
	file_out.open(filename.c_str());
	
	if (!file_out)
        throw AstranError("Could not save project to file: " + filename);;
	
	file_out << "new design " << name << "\n";
	file_out << "set hgrid " << circuit->getHPitch() << "\n";
	file_out << "set vgrid " << circuit->getVPitch() << "\n";
	file_out << "set vddnet " << circuit->getVddNet() << "\n";
	file_out << "set gndnet " << circuit->getGndNet() << "\n";
	file_out << "set rowheight " << circuit->getRowHeight() << "\n";
	file_out << "set supplysize " << circuit->getSupplyVSize() << "\n";
	file_out << "set viewer \"" << viewerProgram << "\"\n";
	file_out << "set rotdl \"" << rotdlFile << "\"\n";
	file_out << "set placer \"" << placerFile << "\"\n";
	file_out << "set lpsolve \"" << lpSolverFile << "\"\n";
	file_out << "set log \"" << historyFile << "\"\n";
	
	file_out << "load technology \"" << project_name << ".rul\"\n";
	file_out << "load netlist \"" << project_name << ".sp\"\n";
	file_out << "load layouts \"" << project_name << ".lay\"\n";
	//file_out << "load placement \"" << project_name << ".pl\"\n";
	//file_out << "load placement \"" << project_name << ".place\"\n";
	//file_out << "load routing \"" << project_name << ".rot\"\n";
    
    string file_to_save;
    //save technology
    file_to_save = project_name + ".rul";
    rules->saveRules(file_to_save);
    
    //save netlist
    file_to_save = project_name + ".sp";
    Spice::saveFile(file_to_save, *circuit);
    
    //save cell library
    file_to_save = project_name + ".lay";
    Lef lef;
    lef.saveFile(file_to_save, *circuit);
    
    //save placement
    /*
     ret &= placer->writeBookshelfFiles(project_name, true);
     file_to_save = project_name + ".place";
     ret &= placer->writeCadende(file_to_save);
     
     //save routing
     Router router;
     file_to_save = project_name + ".rot";
     ret &= router.saveRoutingRotdl(file_to_save);
     */
    
    
	file_out.close();
}

bool DesignMng::readCommand(string cmd){
	try {
        
        if(verboseMode){
            cout << "[" << name << "]$ " << cmd << endl;
            addToHistoryLog(cmd);
            if (cmd.size()){
                commandlog.push_back(cmd);
                poscmdlog = static_cast<int>(commandlog.size());
            }
        }
        
        vector<string> words;
        
        // Parser, analyzing tokens
        int inicio=0, fim=0;
        bool insideaspas=false, ok=false;
        
        for (int c=0; c < cmd.size(); ++c){
            
            if (cmd[c] != ' ')
                fim = c;
            
            if (insideaspas)
                fim = fim-1;
            
            if ((c == cmd.size()-1 && cmd[cmd.size()-1] != ' ') || (cmd[c] == ' ' && !insideaspas && fim-inicio >= 0))
                ok = true;
            
            if (ok){
                string tmp = cmd.substr(inicio,fim-inicio+1);
                words.push_back(tmp);
                ok = false;
                inicio = c+1;
            }
            switch (cmd[c]){
                case '"':
                    if (!insideaspas)
                        inicio = c+1;
                    insideaspas = !insideaspas;
                    break;
                case ' ':
                    if (!insideaspas)
                        inicio = c+1;
                    break;
            }
        }
        // tokens are separated and inserted in vector 'words'
        int tmp;
        if (words.size() != 0){
            switch (getCommandCode(words)){
                    
                case NEW_DESIGN:{
                    circuit =  std::unique_ptr<Circuit>(new Circuit());
                    router =  std::unique_ptr<Router>(new Router());
                    rules =  std::unique_ptr<Rules>(new Rules());
                    placer =  std::unique_ptr<Placer>(new Placer());
                    autocell =  std::unique_ptr<AutoCell>(new AutoCell());
                    circuit->setRules(rules.get());
                    placer->setCircuit(circuit.get());
                    router->setCircuit(circuit.get());
                    setName(words[2].c_str());
                }
                    break;
                    
                case NEW_CELL:{
                    CellNetlst tmp;
                    tmp.setName(words[2]);
                    circuit->insertCell(tmp);
                    }
                    break;
                    
                    /****  LOAD - 6  ****/
                case LOAD_PROJECT:
                    cout << "-> Loading project from file: " << words[2] << endl;
                    verboseMode=0;
                    run(words[2]);
                    verboseMode=1;
                    break;
                    
                case LOAD_TECHNOLOGY:
                    cout << "-> Loading technology from file: " << words[2] << endl;
                    rules->readRules(words[2]);
                    break;
                    
                case LOAD_NETLIST:{
                    string tipo=upcase(getExt(words[2]));
                    cout << "-> Loading cells netlist from file: " << words[2] << endl;
                    if(tipo=="V"){
                        Verilog vlog;
                        if(!vlog.readFile(words[2], *circuit))
                            throw AstranError("Could not read verilog file: " + words[2]);
                    }else {//if(tipo=="SP" || tipo="LIB"){
                        Spice::readFile(words[2], *circuit, false);
                    }
                }
                    break;
                    
                case LOAD_LAYOUTS:
                {
                    cout << "-> Loading LEF library: " << words[2] << endl;
                    Lef lef;
                    lef.readFile(words[2], *circuit, false);
                }
                    break;
                    
                case LOAD_PLACEMENT:{
                    string tipo=upcase(getExt(words[2]));
                    cout << "-> Loading placement from file: " << words[2] << endl;
                    if(tipo=="PL"){
                        placer->readBookshelfPlacement(words[2]);
                    }else if(tipo=="MPP")
                        placer->readMangoParrotPlacement(words[2]);
                    else
                        throw AstranError("File extension not supported" + getExt(words[2]));
                }
                    break;
                    
                case LOAD_ROUTING:
                    cout << "-> Loading routing from file: " << words[2] << endl;
                    router->readRoutingRotdl(words[2]);
                    break;
                    
                    /****  SAVE - 7  ****/
                case SAVE_PROJECT:
                    cout << "-> Saving project to file: " << words[2] << endl;
                    saveProjectConfig(words[2], removeExt(words[2]));
                    break;
                    
                case SAVE_TECHNOLOGY:
                    rules->saveRules(words[2]);
                    break;
                    
                case SAVE_NETLIST:{
                    cout << "-> Saving spice netlist to file: " << words[2] << endl;
                    Spice::saveFile(words[2], *circuit);
                }
                    break;
                    
                case SAVE_LAYOUTS:{
                    cout << "-> Saving layouts to file: " << words[2] << endl;
                    Lef lef;
                    lef.saveFile(words[2], *circuit);
                }
                    break;
                    
                case SAVE_PLACEMENT:
                    cout << "-> Saving placement to file: " << words[2] << endl;
                    placer->writeBookshelfFiles(removeExt(getFileName(words[2])), true);
                    break;
                    
                case SAVE_ROUTING:
                    cout << "-> Saving routing to file: " << words[2] << endl;
                    router->saveRoutingRotdl(words[2]);
                    break;
                    
                case SAVE_HISTORY:
                    cout << "-> Saving script to file: " << words[2] << endl;
                    saveHistory(words[2]);
                    break;
                    
                    /****  IMPORT - 2  ****/
                case IMPORT_NETLIST:{
                    cout << "-> Importing netlist from file: " << words[2] << endl;
                    Spice::readFile(words[2], *circuit, true);
                }
                    break;
                    
                case IMPORT_LEF:{
                    cout << "-> Importing LEF library: " << words[2] << endl;
                    Lef lef;
                    lef.readFile(words[2], *circuit, true);
                }
                    break;
                    
                    /****  EXPORT - 3  ****/
                case EXPORT_LAYOUT:{
                    string tipo=upcase(getExt(words[3]));
                    string filename=words[3];
                    if(circuit->getLayout(upcase(words[2]))){
                        if(tipo=="GDS"){
                            rules->saveGDSIILayerTable(getPath(filename)+"GDSIILTable.txt");
                            cout << "-> Saving layout " << words[2] << " to file: " << filename << endl;
                            Gds g(filename);
                            char tmp[20] = "                   ";
                            strcpy(tmp, words[2].substr(0,19).c_str());
                            g.open(); 
                            g.generateHeader(1);
                            g.generateLibrary();
                            g.generateLibname(tmp);
                            g.generateUnits();
                            g.generateStruct();
                            g.generateStructname(tmp);
                            //Insert Boxes
                            list <Box>::iterator layer_it;
                            map <layer_name , list<Box> >::iterator layers_it; // iterador das camadas		
                            for (layers_it = circuit->getLayout(words[2])->layers.begin(); layers_it != circuit->getLayout(words[2])->layers.end(); layers_it++) {
                                if ( !layers_it->second.empty() ) {
                                    int layer = strToInt(rules->getGDSIIVal(layers_it->first));
                                    for ( layer_it = layers_it->second.begin(); layer_it != layers_it->second.end(); layer_it++ ){
                                        long int x1 = 2*layer_it->getX1();
                                        long int y1 = 2*layer_it->getY1();
                                        long int x2 = 2*layer_it->getX2();
                                        long int y2 = 2*layer_it->getY2();
                                        if(x2-x1!=0 & y2-y1!=0){
                                            g.generateBox(layer, x1, y1, x2, y2);
//                                            cout << layer << "-"<< x1 << "-" << y1 << "-" << x2 << "-" << y2 << endl;
                                            g.generateEndelement();
                                        }
                                    }
                                }
                            }
                            
                            list<Label>::iterator labels_it;
                            for (labels_it = circuit->getLayout(words[2])->labels.begin(); labels_it != circuit->getLayout(words[2])->labels.end(); labels_it++){
                                strcpy(tmp, labels_it->text.c_str());
                                g.generateLabel(strToInt(rules->getGDSIIVal(MET1)), 2*labels_it->pt.getX(), 2*labels_it->pt.getY(), tmp);
                                g.generateEndelement();
                            }
                            
                            
                            g.generateEndStruct();
                            g.generateEndLibrary();
                        }else if(tipo=="C2C"){
                            Cif cifOut(removeExt(filename)+".cif", *rules);
                            cifOut.cellCif(*(circuit->getLayouts()), words[2]);
                            cifOut.cif2Cadence(name,  words[2]);
                        }else if(tipo=="CIF"){
                            Cif cifOut(filename, *rules);
                            cifOut.cellCif(*(circuit->getLayouts()), words[2]);
                        }else 
                            throw AstranError("Unknown file type: " + tipo);			
                    }else
                        throw AstranError("Cell not found: " + words[2]);
                }
                    break;
                    
                case EXPORT_CELLSIZES:
                    cout << "-> Writing Cell Sizes to file: " << words[2] << endl;
                    circuit->writeCellSizesFile(words[2]);
                    break;
                    
                case EXPORT_PLACEMENT:
                    cout << "-> Saving placement to file: " << words[2] << endl;
                    placer->writeCadende(words[2]);
                    break;
                    
                case READ:
                    run(words[1]);
                    break;
                    
                    /****  PLACE - 7  ****/
                case PLACE_TERMINALS:
                    placer->placeInterfaceTerminals();
                    break;
                    
                case PLACE_FPLACE:{
                    cout << "-> Writing bookshelf files for placement..." << endl;
                    placer->writeBookshelfFiles("test", false);
                    string cmd = "\"" + placerFile + "\"" + " test > temp.log";
                    cout << "-> Calling placement tool: " << cmd << endl;
                    FILE *x = _popen(cmd.c_str(), "r");
                    if ( x == NULL ) 
                        throw AstranError("Could not execute: " + cmd);
                    _pclose(x);
                    cout << "-> Placing Cells..." << endl;
                    placer->readBookshelfPlacement("test_mp.pl");
                }
                    break;
                    
                case PLACE_GETWL:
                    placer->checkWL();
                    break;
                    
                case PLACE_INSTANCE:{
                    CLayout *tmp=circuit->getLayout(upcase(words[2]));
                    if(!tmp)
                        throw AstranError("Could not find Layout: " + upcase(words[2]));
                    tmp->placeCell(upcase(words[3]), atoi(words[4].c_str()), atoi(words[5].c_str()), atoi(words[6].c_str()), atoi(words[7].c_str()));
                    break;
                }    
                case PLACE_AUTOFLIP:
                    placer->autoFlip();
                    break;
                    
                case PLACE_INCREMENTAL:
                    placer->incrementalPlacement(router.get(), lpSolverFile);
                    break;
                    
                case PLACE_CHECK:
                    placer->checkPlacement();
                    break;
                    
                    /****  ROUTE - 6  ****/
                case ROUTE_ROTDL:
                    router->setup(placer->getHSize(), placer->getVSize(), 3);
                    router->rotdl(rotdlFile);
                    break;
                    
                case ROUTE_PFINDER:
                    router->setup(placer->getHSize(), placer->getVSize(), 3);
                    router->route(atoi(words[2].c_str()));
                    break;
                    
                case ROUTE_OPTIMIZE:
                    router->optimize();
                    break;
                    
                case ROUTE_COMPACT:
                    router->compactLayout(lpSolverFile);
                    break;
                    
                case ROUTE_TEST:
                    router->test(rotdlFile);
                    break;
                    
                case ROUTE_CLEAR:
                    router->getGridRouter()->clear();
                    break;
                    
                    /****  PRINT - 4  ****/
                case PRINT_INSTANCE:
                    circuit->printInstance(circuit->getLayout(upcase(words[2])), upcase(words[3]));
                    break;
                    
                case PRINT_CELL:
                    circuit->getCellNetlst(upcase(words[2]))->print();
                    break;
                    
                case PRINT_NET:
                    circuit->printNet(upcase(words[2]));
                    break;
                    
                case PRINT_INTERFACE:
                    circuit->printInterface(upcase(words[2]));
                    break;
                    
                    /****  PREFERENCES - 6  ****/
                case SET_PLACER:
                    if (!fileExist(words[2]))
                        throw AstranError("Could not find file: " + words[2]);
                    
                    placerFile=words[2];
                    cout << "-> Setting placer executable to: " << placerFile << endl;
                    break;
                    
                case SET_ROTDL:
                    if (!fileExist(words[2]))
                        throw AstranError("Could not find file: " + words[2]);
                    
                    rotdlFile=words[2];
                    cout << "-> Setting rotdl executable path to: " << rotdlFile << endl;
                    break;
                    
                case SET_VIEWER:
                    if (!fileExist(words[2]))
                        throw AstranError("Could not find file: " + words[2]);
                    
                    viewerProgram=words[2];
                    cout << "-> Setting viewer executable path to: " << viewerProgram << endl;
                    break;
                    
                case SET_LPSOLVE:
                    if (!fileExist(words[2]))
                        throw AstranError("Could not find file: " + words[2]);
                    
                    lpSolverFile=words[2];
                    cout << "-> Setting lpsolve executable to: " << lpSolverFile << endl;
                    break;
                    
                case SET_LOG:
                    historyFile = words[2];
                    remove(historyFile.c_str());
                    cout << "-> Saving history log to file: " << historyFile << endl;
                    break;
                    
                case SET_VERBOSEMODE:
                    cout << "-> Setting verbose mode = " << words[2] << endl;
                    verboseMode=atoi(words[2].c_str());
                    break;
                    
                    /****  TECHNOLOGY - 8  ****/
                case SET_TECH_NAME:
                    rules->setTechName(words[3].c_str());
                    break;
                    
                case SET_TECH_MLAYERS:
                    rules->setMLayers(words[3].c_str());
                    break;
                    
                case SET_TECH_SOI:
                    rules->setSOI(words[3].c_str());
                    break;
                    
                case SET_TECH_RESOLUTION:
                    rules->setResolution(atoi(words[3].c_str()));
                    break;
                    
                case SET_TECH_RULE:
                    tmp=rules->findRule(words[3].c_str());
                    if (tmp==-1)
                        throw AstranError("Rule not Found: " + words[3]);
                    
                    rules->setRule((rule_name)tmp, atof(words[4].c_str()));
                    break;
                    
                case SET_TECH_CIF:
                    tmp=rules->findLayerName(words[3].c_str());
                    if (tmp==-1)
                        throw AstranError("Layer not Found: " + words[3]);
                    
                    rules->setCIFVal((layer_name)tmp, words[4].c_str());
                    break;
                    
                case SET_TECH_GDSII:
                    tmp=rules->findLayerName(words[3].c_str());
                    if (tmp==-1)
                        throw AstranError("Layer not Found: " + words[3]);
                    
                    rules->setGDSIIVal((layer_name)tmp, words[4].c_str());
                    break;
                    
                case SET_TECH_VALTECH:
                    tmp=rules->findLayerName(words[3].c_str());
                    if (tmp==-1)
                        throw AstranError("Layer not Found: " + words[3]);
                    
                    rules->setTechVal((layer_name)tmp, words[4].c_str());
                    break;
                    
                    /****  CIRCUIT - 8  ****/
                case SET_DESIGNNAME:
                    setName(words[2].c_str());
                    break;
                    
                case SET_GRID:
                    circuit->setHPitch(atof(words[2].c_str()));
                    circuit->setVPitch(atof(words[3].c_str()));
                    break;
                    
                case SET_HGRID:
                    circuit->setHPitch(atof(words[2].c_str()));
                    break;
                    
                case SET_VGRID:
                    circuit->setVPitch(atof(words[2].c_str()));
                    break;
                    
                case SET_HGRID_OFFSET:
                    circuit->setHGridOffset(upcase(words[2].c_str())=="YES"?true:false);
                    break;
                    
                case SET_VGRID_OFFSET:
                    circuit->setVGridOffset(upcase(words[2].c_str())=="YES"?true:false);
                    break;
                    
                case SET_VDDNET:
                    circuit->setVddNet(upcase(words[2].c_str()));
                    break;
                    
                case SET_GNDNET:
                    circuit->setGndNet(upcase(words[2].c_str()));
                    break;
                    
                case SET_ROWHEIGHT:
                    circuit->setRowHeight(atoi(words[2].c_str()));
                    break;
                    
                case SET_SUPPLYSIZE:
                    circuit->setSupplyVSize(atof(words[2].c_str()));
                    break;
                    
                case SET_NWELLPOS:
                    circuit->setnWellPos(atof(words[2].c_str()));
                    break;
                    
                case SET_NWELLBORDER:
                    circuit->setnWellBorder(atof(words[2].c_str()));
                    break;
                    
                case SET_PNSELBORDER:
                    circuit->setpnSelBorder(atof(words[2].c_str()));
                    break;
                    
                case SET_CELLTEMPLATE:
                    circuit->setCellTemplate(words[2].c_str());
                    break;
                    
                    /****  FLOORPLAN - 3  ****/
                case SET_TOPCELL:
                    circuit->setTopCell(upcase(words[2]));
                    break;
                    
                case SET_AREA:
                    placer->setArea(atoi(words[2].c_str()), atof(words[3].c_str()));
                    break;
                    
                case SET_MARGINS:
                    circuit->setMargins(atof(words[2].c_str()), atof(words[3].c_str()), atof(words[4].c_str()), atof(words[5].c_str()));
                    break;
                    
                case CALCPINSPOS:
                    circuit->calculateCellsPins();
                    break;
                    
                    /****  CELLGEN - 9  ****/
                    
                case CELLGEN_SELECT:
                    autocell->selectCell(circuit.get(),upcase(words[2]));
                    break;
                    
                case CELLGEN_AUTOFLOW:
                    autocell->autoFlow(lpSolverFile);
                    break;

                case CELLGEN_FOLD:
                    autocell->calcArea(atoi(words[2].c_str()), atoi(words[3].c_str()));
                    autocell->foldTrans();
                    break;
                    
                case CELLGEN_PLACE:
                    autocell->placeTrans(true, atoi(words[2].c_str()), atoi(words[3].c_str()), atoi(words[4].c_str()), atoi(words[5].c_str()), atoi(words[6].c_str()), atoi(words[7].c_str()), atoi(words[8].c_str()));
                    break;
                    
                case CELLGEN_GETARCCOST:
                    cout << autocell->getRouting()->getArcCost(atoi(words[2].c_str()), atoi(words[3].c_str()));
                    break;
                    
                case CELLGEN_SETARCCOST:
                    cout << autocell->getRouting()->setArcCost(atoi(words[2].c_str()), atoi(words[3].c_str()), atoi(words[4].c_str()));
                    break;
                    
                case CELLGEN_ROUTE:
                    autocell->route(atoi(words[2].c_str()), atoi(words[3].c_str()), atoi(words[4].c_str()), atoi(words[5].c_str()));
                    break;
                    
                case CELLGEN_COMPACT:
                    if(!autocell->compact(lpSolverFile, atoi(words[2].c_str()), atoi(words[3].c_str()), atoi(words[4].c_str()), atoi(words[5].c_str()), atoi(words[6].c_str()), atoi(words[7].c_str()), atoi(words[8].c_str()), atoi(words[9].c_str()), atoi(words[10].c_str()), atoi(words[11].c_str())))
                        throw AstranError("Could not solve the ILP model. Try to adjust the constraints!");                        
                    break;
                    
                    /****  HELP - 2  ****/
                case HELP:
                    for (int i=0; i<NR_COMMANDS; ++i)
                        cout << commands_lst[i].name << endl;
                    cout << "-> Note: For detailed instructions, use command \"HELP <str_Command>\"\n\n";
                    break;
                    
                case HELP_PARAM:
                    for (int i=0; i<NR_COMMANDS; ++i)
                        if (commands_lst[i].name.find(upcase(words[1])) != string::npos)
                            cout << commands_lst[i].name << " - " << commands_lst[i].desc << endl;
                    break;
                    
                case COMMENT:
                    break;
                case EXIT:
                    exit(EXIT_SUCCESS);
                    break;
            }
        }
    } catch (AstranError& e){
        cout << "** ERROR: " << e.what() << endl;
        return false;
    }
    return true;
}