/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
 *   amziesemerj[at]inf.ufrgs.br                                           *
 ***************************************************************************/
#include "designmng.h"

DesignMng::DesignMng(){
	viewerProgram="";
	verboseMode=0;
	poscmdlog = 0;
	historyFile="";
	circuit=NULL;
	router=NULL;
	rules=NULL;
	placer=NULL;
	autocell=NULL;
	readCommand("new design new_design");
}

DesignMng::~DesignMng(){
	delete circuit;
	delete placer;
	delete rules;
	delete autocell;
	delete router;
}

// vector shell_cmd: command received from shell
// vector reg_cmd: command registered in commands_lst[]
int DesignMng::getCommandCode(vector<string> shell_cmd){
	string cmd_tmp;
	vector<string> reg_cmd;
	bool same_cmd;
	
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
						return (-2); // ERROR - parameters invalid
				return (i); // OK - command found and valid parameters, return its code
			}
		}
	}
	return (-1); // ERROR - invalid command or wrong number of parameters
}

bool DesignMng::run(string filename) {
	cout << "Reading file: " << filename << endl;
	ifstream file(filename.c_str()); // Read
	if (!file){
		cout << "File not found\n";
		return false;
	}
	int line=1;
	string str_tmp;
	while(getline(file, str_tmp)){
		if((str_tmp[0] != '*') && !readCommand(str_tmp))
			cout << "ERROR on line: " << line << endl;
		line++;
	}
	return true;
}

bool DesignMng::history(string cmd){
	if(historyFile!=""){
		ofstream file;
		file.open(historyFile.c_str(),ios_base::app); // Write
		if (!file) {
			cout << "Could not save history to file: " << historyFile << endl; 
			return false;
		}
		else
			if(cmd.size())
				file << cmd << endl;
		file.close();
	}
	
	return true;
}

bool DesignMng::saveHistory(string filename){
	ofstream file_out;
	file_out.open(filename.c_str());
	
	if (!file_out){
		cout << "Cannot save history to file\n";
		return false;
	}
	if (commandlog.size() > 1) {
		for (int i=0; i < commandlog.size()-1; ++i)
			file_out << commandlog[i] << endl;
	} else 
		cout << "Command log is empty\n";
	
	file_out.close();
	
	return true;
}

bool DesignMng::saveProjectConfig(string filename, string project_name){
	ofstream file_out;
	file_out.open(filename.c_str());
	
	if (!file_out){
		cout << "Cannot save project to file\n";
		return false;
	}
	
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
	
	file_out.close();
	return true;
}

int DesignMng::readCommand(string cmd){
	int ret=5;
	
	if(verboseMode){
		cout << "[" << name << "]$ " << cmd << endl;
		ret = history(cmd);
		if (cmd.size()){
			commandlog.push_back(cmd);
			poscmdlog = commandlog.size();
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
	// end, tokens are separated and inserted in vector 'words'
	
	if (words.size() != 0){
		switch (getCommandCode(words)){
				
				// ERROR
			case -1:
				ret=5;
				break;
				
				// ERROR
			case -2:
				ret=6;
				break;
				
				/****  NEW - 2  ****/
			case NEW_DESIGN:
			{
				delete circuit;
				delete placer;
				delete rules;
				delete autocell;
				delete router;
				
				circuit = new Circuit();
				placer = new Placer();
				rules = new Rules();
				autocell = new AutoCell();
				router = new Router();
				circuit->setRules(rules);
				placer->setCircuit(circuit);
				router->setCircuit(circuit);
				
				setName(words[2].c_str());
				ret=1;
			}
				break;
				
			case NEW_CELL:
			{
				CellNetlst tmp;
				tmp.setName(words[2]);
				ret=circuit->insertCell(tmp);
			}
				break;
				
				/****  LOAD - 6  ****/
			case LOAD_PROJECT:
				cout << "Loading project from file: " << words[2] << endl;
				verboseMode=0;
				ret=run(words[2]);
				verboseMode=1;
				break;
				
			case LOAD_TECHNOLOGY:
				cout << "Loading technology from file: " << words[2] << endl;
				ret=rules->readRules(words[2]);
				break;
				
			case LOAD_NETLIST:
			{
				string tipo=upcase(getExt(words[2]));
				cout << "Loading cells netlist from file: " << words[2] << endl;
				if(tipo=="V"){
					Verilog vlog;
					ret=vlog.readFile(words[2], *circuit);
				}else {//if(tipo=="SP" || tipo="LIB"){
					Spice spice;
					ret=spice.readFile(words[2], *circuit, false);
				}
			}
				break;
				
			case LOAD_LAYOUTS:
			{
				cout << "Loading LEF library: " << words[2] << endl;
				Lef lef;
				ret=lef.readFile(words[2], *circuit, false);
			}
				break;
				
			case LOAD_PLACEMENT:
			{
				string tipo=upcase(getExt(words[2]));
				cout << "Loading placement from file: " << words[2] << endl;
				if(tipo=="PL")
					ret=placer->readBookshelfPlacement(words[2]);
				else if(tipo=="MPP")
					ret=placer->readMangoParrotPlacement(words[2]);
				else{
					cout << "File extension not supportated" << endl;
					ret=0;
				}
			}
				break;
				
			case LOAD_ROUTING:
				cout << "Loading routing from file: " << words[2] << endl;
				ret=router->readRoutingRotdl(words[2]);
				break;
				
				/****  SAVE - 7  ****/
			case SAVE_PROJECT:
			{
				cout << "Saving project to file: " << words[2] << endl;
				
				string project_name = removeExt(words[2]);
				string file_to_save;
				
				ret = saveProjectConfig(words[2], project_name);
				
				//save technology
				file_to_save = project_name + ".rul";
				ret &= rules->saveRules(file_to_save);
				
				//save netlist
				file_to_save = project_name + ".sp";
				Spice spice;
				ret &= spice.saveFile(file_to_save, *circuit);
				
				//save cell library
				file_to_save = project_name + ".lay";
				Lef lef;
				ret &= lef.saveFile(file_to_save, *circuit);
				
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
			}
				break;
				
			case SAVE_TECHNOLOGY:
				ret=rules->saveRules(words[2]);
				break;
				
			case SAVE_NETLIST:
			{
				cout << "Saving spice netlist to file: " << words[2] << endl;
				Spice spice;
				ret=spice.saveFile(words[2], *circuit);
			}
				break;
				
			case SAVE_LAYOUTS:
			{
				cout << "Saving layouts to file: " << words[2] << endl;
				Lef lef;
				ret=lef.saveFile(words[2], *circuit);
			}
				break;
				
			case SAVE_PLACEMENT:
				cout << "Saving placement to file: " << words[2] << endl;
				ret = placer->writeBookshelfFiles(removeExt(getFileName(words[2])), true);
				break;
				
			case SAVE_ROUTING:
				cout << "Saving routing to file: " << words[2] << endl;
				ret = router->saveRoutingRotdl(words[2]);
				break;
				
			case SAVE_HISTORY:
				cout << "Saving script to file: " << words[2] << endl;
				ret=saveHistory(words[2]);
				break;
				
				/****  IMPORT - 2  ****/
			case IMPORT_NETLIST:
			{
				cout << "Importing netlist from file: " << words[2] << endl;
				Spice spice;
				ret=spice.readFile(words[2], *circuit, true);
			}
				break;
				
			case IMPORT_LEF:
			{
				cout << "Importing LEF library: " << words[2] << endl;
				Lef lef;
				ret=lef.readFile(words[2], *circuit, true);
			}
				break;
				
				/****  EXPORT - 3  ****/
			case EXPORT_LAYOUT:
			{
				string tipo=upcase(getExt(words[3]));
				string filename=words[3];
				if(circuit->getLayout(upcase(words[2]))){
					if(tipo=="GDS"){
						rules->saveGDSIILayerTable(getPath(filename)+"GDSIILTable.txt");
						cout << "Saving layout " << words[2] << " to file: " << filename << endl;
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
								int layer =layers_it->first;
								for ( layer_it = layers_it->second.begin(); layer_it != layers_it->second.end(); layer_it++ ){
									long int x1 = layer_it->getX1();
									long int y1 = layer_it->getY1();
									long int x2 = layer_it->getX2();
									long int y2 = layer_it->getY2();
									g.generateBox(layer, x1, y1, x2, y2);
									//									cout << layer << "-"<< x1 << "-" << y1 << "-" << x2 << "-" << y2 << endl; 
									g.generateEndelement();
								}
							}
						}
						g.generateEndStruct();
						g.generateEndLibrary();
						ret=1;
					}else if(tipo=="C2C"){
						Cif cifOut(removeExt(filename)+".cif", *rules);
						cifOut.cellCif(*(circuit->getLayouts()), words[2]);
						cifOut.cif2Cadence(name,  words[2]);
						ret=1;
					}else if(tipo=="CIF"){
						Cif cifOut(filename, *rules);
						cifOut.cellCif(*(circuit->getLayouts()), words[2]);
						ret=1;
					}else cout << "Unknown file type: " << tipo << endl;			
				}else cout << "Cell not found: " << words[2] << endl;
			}
				break;
				
			case EXPORT_CELLSIZES:
				cout << "Writing Cell Sizes to file: " << words[2] << endl;
				ret=circuit->writeCellSizesFile(words[2]);
				break;
				
			case EXPORT_PLACEMENT:
				cout << "Saving placement to file: " << words[2] << endl;
				ret = placer->writeCadende(words[2]);
				break;
				
			case READ:
				ret=run(words[1]);
				break;
				
				/****  PLACE - 7  ****/
			case PLACE_TERMINALS:
				ret=placer->placeInterfaceTerminals();
				break;
				
			case PLACE_FPLACE:
			{
				cout << "Writing bookshelf files for placement..." << endl;
				ret=placer->writeBookshelfFiles("test", false);
				if(ret){
					string cmd = "\"" + placerFile + "\"" + " test > temp.log";
					cout << "Calling placement tool: " << cmd << endl;
					FILE *x = _popen(cmd.c_str(), "r");
					if ( x == NULL ) ret=0;
					_pclose(x);
					if(ret){
						cout << "Placing Cells..." << endl;
						ret=placer->readBookshelfPlacement("test_mp.pl");
					}
				}
			}
				break;
				
			case PLACE_GETWL:
				ret=placer->checkWL();
				break;
				
			case PLACE_INSTANCE:
			{
				CLayout *tmp=circuit->getLayout(upcase(words[2]));
				ret=tmp&&tmp->placeCell(upcase(words[3]), atoi(words[4].c_str()), atoi(words[5].c_str()), atoi(words[6].c_str()), atoi(words[7].c_str()));
			}
				break;
				
			case PLACE_AUTOFLIP:
				ret=placer->autoFlip();
				break;
				
			case PLACE_INCREMENTAL:
				ret=placer->incrementalPlacement(router, lpSolverFile);
				break;
				
			case PLACE_CHECK:
				ret=placer->checkPlacement();
				break;
				
				/****  ROUTE - 6  ****/
			case ROUTE_ROTDL:
				ret=router->setup(placer->getHSize(), placer->getVSize(), 3)&&router->rotdl(rotdlFile);
				break;
				
			case ROUTE_PFINDER:
				ret=router->setup(placer->getHSize(), placer->getVSize(), 3)&&router->route(atoi(words[2].c_str()));
				break;
				
			case ROUTE_OPTIMIZE:
				ret=router->optimize();
				break;
				
			case ROUTE_COMPACT:
				ret=router->compactLayout(lpSolverFile);
				break;
				
			case ROUTE_TEST:
				router->test(rotdlFile);
				ret=1;
				break;
				
			case ROUTE_CLEAR:
				router->getPathfinderRt()->clear();
				ret=1;
				break;
				
				/****  PRINT - 4  ****/
			case PRINT_INSTANCE:
				ret=circuit->printInstance(circuit->getLayout(upcase(words[2])), upcase(words[3]));
				break;
				
			case PRINT_CELL:
				if(circuit->getCellNetlst(upcase(words[2]))){
					circuit->getCellNetlst(upcase(words[2]))->print();
					ret=1;
				}else ret=0;
				break;
				
			case PRINT_NET:
				ret=circuit->printNet(upcase(words[2]));
				break;
				
			case PRINT_INTERFACE:
				ret=circuit->printInterface(upcase(words[2]));
				break;
				
				/****  PREFERENCES - 6  ****/
			case SET_PLACER:
				if (fileExist(words[2])){
					placerFile=words[2];
					cout << "Setting placer executable to: " << placerFile << endl;
					ret=1;
				}else ret=7;
				break;
				
			case SET_ROTDL:
				if (fileExist(words[2])){
					rotdlFile=words[2];
					cout << "Setting rotdl executable path to: " << rotdlFile << endl;
					ret=1;
				}else ret=7;
				break;
				
			case SET_VIEWER:
				if (fileExist(words[2])){
					viewerProgram=words[2];
					cout << "Setting viewer executable path to: " << viewerProgram << endl;
					ret=1;
				}else ret=7;
				break;
				
			case SET_LPSOLVE:
				if (fileExist(words[2])){
					lpSolverFile=words[2];
					cout << "Setting lpsolve executable to: " << lpSolverFile << endl;
					ret=1;
				}else ret=7;
				break;
				
			case SET_LOG:
				historyFile = words[2];
				remove(historyFile.c_str());
				cout << "Saving history log to file: " << historyFile << endl;
				ret=1;
				break;
				
			case SET_VERBOSEMODE:
				cout << "Setting verbose mode = " << words[2] << endl;
				verboseMode=atoi(words[2].c_str());
				ret=1;
				break;
				
				/****  TECHNOLOGY - 8  ****/
			case SET_TECH_NAME:
				rules->setTechName(words[3].c_str());
				ret=1;
				break;
				
			case SET_TECH_MLAYERS:
				rules->setMLayers(words[3].c_str());
				ret=1;
				break;
				
			case SET_TECH_SOI:
				rules->setSOI(words[3].c_str());
				ret=1;
				break;
				
			case SET_TECH_RESOLUTION:
				rules->setResolution(atoi(words[3].c_str()));
				ret=1;
				break;
				
			case SET_TECH_RULE:
			{
				int rule=rules->findRule(words[3].c_str());
				if (rule!=-1){
					rules->setRule((rule_name)rule, atof(words[4].c_str()));
					ret=1;
				} else ret=0;
			}
				break;
				
			case SET_TECH_CIF:
			{
				int layer=rules->findLayerName(words[3].c_str());
				if(layer!=-1){
					rules->setCIFVal((layer_name)layer, words[4].c_str());
					ret=1;
				}else ret=0;
			}
				break;
				
			case SET_TECH_GDSII:
			{
				int layer=rules->findLayerName(words[3].c_str());
				if(layer!=-1){
					rules->setGDSIIVal((layer_name)layer, words[4].c_str());
					ret=1;
				}else ret=0;
			}
				break;
				
			case SET_TECH_VALTECH:
			{
				int layer=rules->findLayerName(words[3].c_str());
				if(layer!=-1){
					rules->setTechVal((layer_name)layer, words[4].c_str());
					ret=1;
				}else ret=0;
			}
				break;
				
				/****  CIRCUIT - 8  ****/
			case SET_DESIGNNAME:
				setName(words[2].c_str());
				ret=1;
				break;
				
			case SET_GRID:
				circuit->setHPitch(atof(words[2].c_str()));
				circuit->setVPitch(atof(words[3].c_str()));
				ret=1;
				break;
				
			case SET_HGRID:
				circuit->setHPitch(atof(words[2].c_str()));
				ret=1;
				break;
				
			case SET_VGRID:
				circuit->setVPitch(atof(words[2].c_str()));
				ret=1;
				break;
				
			case SET_HGRID_OFFSET:
				circuit->setHGridOffset(upcase(words[2].c_str())=="YES"?true:false);
				ret=1;
				break;
				
			case SET_VGRID_OFFSET:
				circuit->setVGridOffset(upcase(words[2].c_str())=="YES"?true:false);
				ret=1;
				break;
				
			case SET_VDDNET:
				circuit->setVddNet(upcase(words[2].c_str()));
				ret=1;
				break;
				
			case SET_GNDNET:
				circuit->setGndNet(upcase(words[2].c_str()));
				ret=1;
				break;
				
			case SET_ROWHEIGHT:
				circuit->setRowHeight(atoi(words[2].c_str()));
				ret=1;
				break;
				
			case SET_SUPPLYSIZE:
				circuit->setSupplyVSize(atof(words[2].c_str()));
				ret=1;
				break;

			case SET_NWELLPOS:
				circuit->setnWellPos(atof(words[2].c_str()));
				ret=1;
				break;
				
			case SET_NWELLBORDER:
				circuit->setnWellBorder(atof(words[2].c_str()));
				ret=1;
				break;
				
			case SET_PNSELBORDER:
				circuit->setpnSelBorder(atof(words[2].c_str()));
				ret=1;
				break;
				
			case SET_TAPLESS:
				circuit->setTapless(upcase(words[2].c_str())=="YES"?true:false);
				ret=1;
				break;
				
				/****  FLOORPLAN - 3  ****/
			case SET_TOPCELL:
				ret=1;
				circuit->setTopCell(upcase(words[2]));
				break;
				
			case SET_AREA:
				ret=placer->setArea(atoi(words[2].c_str()), atof(words[3].c_str()));
				break;
				
			case SET_MARGINS:
				ret=circuit->setMargins(atof(words[2].c_str()), atof(words[3].c_str()), atof(words[4].c_str()), atof(words[5].c_str()));
				break;
				
			case CALCPINSPOS:
				circuit->calculateCellsPins();
				ret=1;
				break;
				
				/****  CELLGEN - 9  ****/
				
			case CELLGEN_SELECT:
				ret=autocell->calcArea(circuit);
				cout << "Selecting cell: " << upcase(words[2]) << endl;
				ret=autocell->selectCell(upcase(words[2]));
				break;
				
			case CELLGEN_FOLD:
				ret=autocell->foldTrans();
				break;
				
			case CELLGEN_PLACE:
				ret=autocell->placeTrans(false, atoi(words[2].c_str()), atoi(words[3].c_str()), atoi(words[4].c_str()), atoi(words[5].c_str()), atoi(words[6].c_str()), atoi(words[7].c_str()), atoi(words[8].c_str()));
				break;
				
			case CELLGEN_GETARCCOST:
				cout << autocell->getRouting()->getArcCost(atoi(words[2].c_str()), atoi(words[3].c_str()));
				ret=1;
				break;
				
			case CELLGEN_SETARCCOST:
				cout << autocell->getRouting()->setArcCost(atoi(words[2].c_str()), atoi(words[3].c_str()), atoi(words[4].c_str()));
				ret=1;
				break;
				
			case CELLGEN_ROUTE:
				ret=autocell->route(atoi(words[2].c_str()), atoi(words[3].c_str()), atoi(words[4].c_str()), atoi(words[5].c_str()));
				break;
				
			case CELLGEN_COMPACT:
				ret=autocell->compact(lpSolverFile, atoi(words[2].c_str()), atoi(words[3].c_str()), atoi(words[4].c_str()), atoi(words[5].c_str()));

				break;
				
				/****  HELP - 2  ****/
			case HELP:
			{
				cout << "Note: For detailed instructions, use command \"HELP <str_Command>\"\n\n";
				for (int i=0; i<NR_COMMANDS; ++i)
					cout << commands_lst[i].name << endl;
			}
				break;
				
			case HELP_PARAM:
			{
				for (int i=0; i<NR_COMMANDS; ++i)
					if (commands_lst[i].name.find(upcase(words[1])) != string::npos)
						cout << commands_lst[i].name << "\n\t" << commands_lst[i].desc << endl;
			}
				break;
				
			case EXIT:
				exit(1);
				break;
		}
	}
	
	if(verboseMode || ret==0){
		switch(ret){
			case 0:	cout << "ERROR" << endl; break;
				//			case 1:	cout << "OK" << endl; break;
				//			case 2: break; //HIDEN
			case 5:	cout << "Command invalid or wrong number of parameters\n"; break;
			case 6:	cout << "Parameters invalid\n"; break;
			case 7: cout << "Warning: File not found\n"; break;
		}
	}
	return ret==1;
}