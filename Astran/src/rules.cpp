/**************************************************************************
*   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Cristiano Lazzari     *
*   (amziesemerj,clazz)@inf.ufrgs.br                                      *
***************************************************************************/
#include "rules.h"

Rules::Rules() {
	resolution=40;

	// initialize rules' values
	for (int r=0; r<N_RULES; ++r)
		rules_lst[r].val = 0;

	// initialize layers' values
	for (int l=0; l<N_LAYER_NAMES; ++l){
		layer_labels_lst[l].valCIF = "";
		layer_labels_lst[l].valGDSII = "";
		layer_labels_lst[l].valTech = "";
	}
	layer_labels_lst[TECHNAME].valCIF = "TECHNAME";
	layer_labels_lst[MLAYERS].valCIF = "3";
	layer_labels_lst[SOI].valCIF = "YES";
}

Rules::~Rules() {}

// General methods
void Rules::setTechName(string newTechName) {
	layer_labels_lst[TECHNAME].valCIF = newTechName;
}

void Rules::setMLayers(string newMLayers){
	layer_labels_lst[MLAYERS].valCIF = newMLayers;
}

void Rules::setSOI(string newSOI){
	layer_labels_lst[SOI].valCIF = newSOI;
}

void Rules::readRules(string filename) {
	ifstream file(filename.c_str()); // Read
	if ((!file))
		throw AstranError("Rules file " + filename + " could not be opened");
	
	//Search for the rules
	int i=0, fileline=0;
	resolution=0;
	string str_tmp, parm_tmp;
	while(getline(file,str_tmp)) {
		fileline++;
		istrstream line(str_tmp.c_str());    
		if (line >> parm_tmp  && parm_tmp[0] != '*') {
			if (parm_tmp == "MINSTEP"){
				line >> parm_tmp;
				if(resolution==0) resolution= static_cast<int>(round(1.0/atof(parm_tmp.c_str())));
				else cout << "-> " << fileline << ": MINSTEP or CIFMET was already set. Ignoring this line" << endl;
			}
			else if (parm_tmp == "CIFMET"){
				line >> parm_tmp;
				if(resolution==0)
					resolution=static_cast<int>(round(atof(parm_tmp.c_str())));
				else cout << "-> " << fileline << ": MINSTEP or CIFMET was already set. Ignoring this line" << endl;
			}
			else if ( (i = findRule(parm_tmp)) >=0){
				if (line >> parm_tmp)
					rules_lst[i].val=atof(parm_tmp.c_str());
				else cout << "-> " << fileline << ": Argument invalid \"" << parm_tmp << "\" - Ignoring it" << endl;
			}
			else if ( (i = findLayerName(parm_tmp)) >=0){
				if (line >> parm_tmp){
					layer_labels_lst[i].valCIF=parm_tmp;
					if (line >> parm_tmp){
						layer_labels_lst[i].valGDSII=parm_tmp;
						if (line >> parm_tmp)
							layer_labels_lst[i].valTech=parm_tmp;
					}
				}
				else cout << "-> " << fileline << ": Error reading cell arguments" << endl;
			}
			else
				if (parm_tmp != "VDD" && parm_tmp != "FIMP")
					cout << "-> " << fileline << ": Unknown command \"" << parm_tmp << "\" - Ignoring this line" << endl;
		}
	}
	
	if(resolution==0)
		throw AstranError("Mandatory rule MINSTEP (Defines the technology resolution) was not set");
}

void Rules::saveRules(string filename) {
	ofstream file_out;
	file_out.open(filename.c_str());

	if (file_out.fail())
        throw AstranError("Could not save rules to file: " + filename);

	printHeader(file_out, "* ", "");

	for (int header = TECHNAME; header < N_LAYER_NAMES; ++header) {
		file_out << layer_labels_lst[header].name << "\t\t";
		file_out << layer_labels_lst[header].valCIF << endl;
	}

	file_out << "MINSTEP\t\t" << 1.0/resolution << endl << endl;

	for (int i=0; i < N_RULES; ++i) {
		if (rules_lst[i].val){
			file_out << rules_lst[i].name << "\t\t";
			file_out << rules_lst[i].val << "\t* ";
			file_out << rules_lst[i].desc << endl;
		}
	}

	file_out << endl << endl;
	file_out << "* Layers' names for files\n";
	file_out << "* layer\t\tCIF\tGDSII\tTech\n";

	for (int i=0; i < TECHNAME; ++i) {
		if (!layer_labels_lst[i].valCIF.empty() && !layer_labels_lst[i].valGDSII.empty() && !layer_labels_lst[i].valTech.empty()){
			file_out << layer_labels_lst[i].name << "\t\t";
			file_out << layer_labels_lst[i].valCIF << "\t";
			file_out << layer_labels_lst[i].valGDSII << "\t";
			file_out << layer_labels_lst[i].valTech << endl;
		}
	}

	file_out.close();
}

bool Rules::saveGDSIILayerTable(string filename) {
	ofstream lTabfile;
	cout << "-> Saving GDSII Technology Mapping File: " << filename << endl;
	
	lTabfile.open(filename.c_str()); // Write
	if (!lTabfile){
        cout << filename << ": Layer Table file could not be created" << endl;
		return false;
	}
	
	lTabfile << "#Cadence layer  Cadence layer   Stream layer    Stream data" << endl;
	lTabfile << "#name           purpose         number          type" << endl;
	lTabfile << "# " << endl;
	
	for(int i=0; i<TECHNAME; i++)
		if(layer_labels_lst[i].valGDSII!="" && layer_labels_lst[i].valTech!="")
			lTabfile << layer_labels_lst[i].valTech << "\t\t" << getLayerPurposeText(layer_labels_lst[i].purpose) << "\t\t" << layer_labels_lst[i].valGDSII << "\t\t0" << endl;
	lTabfile.close();
	return true;
}

string Rules::getLayerPurposeText(layerPurpose l){
	switch(l){
		case L_DRAWING:	return("drawing");
		case L_PIN:	return("pin");
		case L_NET:	return("net");
        case L_OTHER: return "";
    }
}

bool Rules::saveCIFLayerTable(string filename) {
	ofstream lTabfile;
	cout << "-> Saving CIF Technology Mapping File: " << filename << endl;

	lTabfile.open(filename.c_str()); // Write
	if (!lTabfile){
		cout << filename << ": Layer Table file could not be created" << endl;
		return false;
	}
	
	lTabfile << "#Cadence layer  Cadence layer   CIF layer" << endl;
	lTabfile << "#name           purpose         name" << endl;
	lTabfile << "# " << endl;
	
	for(int i=0; i<TECHNAME; i++)
		if(layer_labels_lst[i].valCIF!="" && layer_labels_lst[i].valTech!="")
			lTabfile << layer_labels_lst[i].valTech << "\t\t" << getLayerPurposeText(layer_labels_lst[i].purpose) << "\t\t" << layer_labels_lst[i].valCIF << endl;
	lTabfile.close();
	return true;
}

// Rules' methods
int Rules::getRule(rule_name name) {	
	return(round(rules_lst[name].val*resolution));
}

float Rules::getRulef(rule_name name) {
		return rules_lst[name].val;
}

string Rules::getRuleDesc(rule_name name) {
	return(rules_lst[name].desc);
}

int Rules::findRule(string name) {
	for(int i=0; i<N_RULES; i++)
		if(name == rules_lst[i].name)
			return(i);
	return(-1);
}

void Rules::setRule(rule_name name, float val) {
	rules_lst[name].val=val;
}

void Rules::listRules() {
	cout << "-> RULES: \n";
	for(int i=0; i<N_RULES; i++)
		cout << '\t' << rules_lst[i].name << " = " << rules_lst[i].val
        << '\t' << rules_lst[i].desc << endl;
}

// Layers' methods
string Rules::getCIFVal(layer_name name) {
	return(layer_labels_lst[name].valCIF);
}

string Rules::getGDSIIVal(layer_name name) {
	return(layer_labels_lst[name].valGDSII);
}

string Rules::getTechVal(layer_name name) {
	return(layer_labels_lst[name].valTech);
}

string Rules::getLayerDesc(layer_name name) {
	return(layer_labels_lst[name].desc);
}

int Rules::findLayerName(string name) {
	for(int i=0; i<N_LAYER_NAMES; i++)
		if(name == layer_labels_lst[i].name)
			return(i);
	return(-1);
}

int Rules::findCIFVal(string val) {
	for(int i=0; i<N_LAYER_NAMES; i++)
		if(val == layer_labels_lst[i].valCIF)
			return(i);
	return(-1);
}

int Rules::findTechVal(string val) {
	for(int i=0; i<N_LAYER_NAMES; i++)
		if(val == layer_labels_lst[i].valTech)
			return(i);
	return(-1);
}

void Rules::setCIFVal(layer_name name, string val) {
	layer_labels_lst[name].valCIF=val;
}

void Rules::setGDSIIVal(layer_name name, string val) {
	layer_labels_lst[name].valGDSII=val;
}

void Rules::setTechVal(layer_name name, string val) {
	layer_labels_lst[name].valTech=val;
}

void Rules::listLayersLabels() {
	cout << "-> Layer Names : \n";
	for(int i=0; i<N_LAYER_NAMES; i++)
		cout << '\t' << layer_labels_lst[i].name << " = " << layer_labels_lst[i].valCIF << "\t" << layer_labels_lst[i].valGDSII << "\t" << layer_labels_lst[i].valTech << endl;
}

int Rules::getIntValue(float f){
	return ceil(f*resolution);
}