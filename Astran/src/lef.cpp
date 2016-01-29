/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Renato Hentschke      *
 *   (amziesemerj,renato)[at]@inf.ufrgs.br                                 *
 ***************************************************************************/

#include "lef.h"

Lef::Lef(): fileline(0), cellsHeight(0){}

Lef::~Lef(){}

void Lef::readObs(Draw& tmp, string net){
	layer_name currentLayer;
	while(readLine(words) && words[0]!="END"){
		if(words[0]=="LAYER"){
			int tmp=readTech?currentRules->findTechVal(words[1]):currentRules->findCIFVal(words[1]);
			if(tmp==-1){
				cout << "-> WARNING: Layer " << words[1] << " not found"<< endl;
				tmp=0;
			}
			currentLayer=(layer_name)tmp;
		} else if(words[0]=="RECT")
			tmp.addPolygon(static_cast<int>(atof(words[1].c_str())*manufGrid),static_cast<int>(atof(words[2].c_str())*manufGrid),static_cast<int>(atof(words[3].c_str())*manufGrid),static_cast<int>(atof(words[4].c_str())*manufGrid),currentLayer).setNet(net);
	}
}

void Lef::readPin(CellNetlst& c, string name, CLayout& layout){
	string net=words[1];
	c.insertInOut(name);
	while(readLine(words) && words[0]!="END"){
		if(words[0]=="DIRECTION"){
			if(words[1]=="INPUT") c.setIOType(name,IOTYPE_INPUT);
			else if(words[1]=="OUTPUT") c.setIOType(name,IOTYPE_OUTPUT);
			else if(words[1]=="INOUT") c.setIOType(name,IOTYPE_INOUT);
		} else if(words[0]=="PORT"){
			CLayout tmp;
			readObs(tmp, net);
			for(map <layer_name , list<Box> >::iterator layers_it = tmp.layers.begin(); layers_it != tmp.layers.end(); layers_it++){
				for(list <Box>::iterator layer_it = layers_it->second.begin(); layer_it != layers_it->second.end(); layer_it++ ){
					layer_it->setNet(name);
					layout.layers[layers_it->first].push_back(*layer_it);
					switch(layers_it->first){
						case MET1: layout.layers[MET1P].push_back(*layer_it); break;
						case MET2: layout.layers[MET2P].push_back(*layer_it); break;
						case MET3: layout.layers[MET3P].push_back(*layer_it); break;
                        default:break;
					}
				}
			}
		}
	}
}

void Lef::readMacro(CellNetlst& c, CLayout& tmp){
	while(readLine(words) && words[0]!="END"){
		if(words[0]=="SIZE"){
			tmp.setWidth(static_cast<int>(atof(words[1].c_str())*manufGrid));
			tmp.setHeight(static_cast<int>(atof(words[3].c_str())*manufGrid));
			tmp.addPolygon(0, 0, tmp.getWidth(), tmp.getHeight(), CELLBOX);
		}
		else if(words[0]=="PIN"){
			readPin(c, words[1], tmp);
		}
		else if(words[0]=="OBS") readObs(tmp, "");
	}
}

void Lef::readUnits(){ // corrigir
	//	while(readLine(words) && words[0]!="END"){
	//		if(words[0]=="DATABASE" && words[1]=="MICRONS")
	//			convertFactor=atoi(words[2].c_str());
	//	}
}

void Lef::saveFile(string nome, Circuit& c){
	ofstream outfile;
	outfile.open(nome.c_str());
	if(!outfile.is_open())
        throw AstranError("Could not save LEF file: " + nome);
	
	outfile << setiosflags(ios::fixed) << setprecision(3);
	outfile << "VERSION 5.4 ;\n";
	outfile << "NAMESCASESENSITIVE ON;\n";
	outfile << "BUSBITCHARS \"[]\" ;\n";
	outfile << "DIVIDERCHAR \"/\" ;\n";
	outfile << "UNITS\n";
	outfile << " DATABASE MICRONS 1000 ;\n";
	outfile << "END UNITS\n\n";
	outfile << "MANUFACTURINGGRID " << 1.0/c.getRules()->getScale() << "\n\n";
	
	for(map<string,CLayout>::iterator cells_it=c.getLayouts()->begin(); cells_it!=c.getLayouts()->end(); cells_it++){
		if(c.findCellNetlst(cells_it->first)){
			outfile << "MACRO " << cells_it->first << "\n";
			outfile << " CLASS CORE\n";
			outfile << " ORIGIN 0.000 0.000\n";
			outfile << " SIZE " << static_cast<float>((cells_it->second).getWidth())/c.getRules()->getScale() << " BY " << static_cast<float>((cells_it->second).getHeight())/c.getRules()->getScale() << "\n";
			outfile << " SYMMETRY X Y  ;\n";
			outfile << " SITE standard ;\n";
			for(int pin_nr=0; pin_nr<c.getCellNetlst(cells_it->first)->getInouts().size(); ++pin_nr){
				string pinName=c.getCellNetlst(cells_it->first)->getNetName(pin_nr);
				outfile << " PIN " << pinName << "\n";
				outfile << "  DIRECTION ";
				switch(c.getCellNetlst(cells_it->first)->getIOType(pin_nr)){
					case IOTYPE_INPUT: outfile << "IN"; break;
					case IOTYPE_OUTPUT: outfile << "OUT"; break;
					case IOTYPE_INOUT: outfile << "INOUT"; break; 
				}
				outfile << ";\n";
				outfile << "  PORT \n";
				for(map<layer_name , list<Box> >::iterator layers_it=(cells_it->second).layers.begin(); layers_it!=(cells_it->second).layers.end(); layers_it++){
					bool newLayer=true;
					if ( !layers_it->second.empty() && layers_it->first!=MET1P && layers_it->first!=MET2P && layers_it->first!=MET3P) {
						for (list <Box>::iterator layer_it = layers_it->second.begin(); layer_it != layers_it->second.end(); layer_it++ ){
							if(layer_it->getNet()==pinName){
								if(newLayer) {
									outfile << "  LAYER " << c.getRules()->getCIFVal(layers_it->first) << " ;\n";
									newLayer=false;
								}
								outfile << "   RECT " <<  static_cast<float>(layer_it->getX()-static_cast<float>(layer_it->getWidth()/2))/c.getRules()->getScale() << " " <<
								static_cast<float>(layer_it->getY()-static_cast<float>(layer_it->getHeight()/2))/c.getRules()->getScale() << " " <<
								static_cast<float>(layer_it->getX()+static_cast<float>(layer_it->getWidth()/2))/c.getRules()->getScale() << " " <<
								static_cast<float>(layer_it->getY()+static_cast<float>(layer_it->getHeight()/2))/c.getRules()->getScale() << ";\n";
							}
						}
					}
				}
				outfile << "  END \n";
				outfile << " END " << pinName << "\n";
			}
			outfile << " OBS\n";
			list <Box>::iterator layer_it;
			for(map<layer_name , list<Box> >::iterator layers_it=(cells_it->second).layers.begin(); layers_it!=(cells_it->second).layers.end(); layers_it++){
				if ( !layers_it->second.empty() && !(layers_it->first==MET1P || layers_it->first==MET2P || layers_it->first==MET3P)) {
					outfile << "  LAYER " << c.getRules()->getCIFVal(layers_it->first) << " ;\n";
					for ( layer_it = layers_it->second.begin(); layer_it != layers_it->second.end(); layer_it++ ){
						if(layer_it->getNet()=="" || !c.getCellNetlst(cells_it->first)->isIO(layer_it->getNet())){
							outfile << "   RECT " <<  static_cast<float>(layer_it->getX()-static_cast<float>(layer_it->getWidth()/2))/c.getRules()->getScale() << " " <<
							static_cast<float>(layer_it->getY()-static_cast<float>(layer_it->getHeight()/2))/c.getRules()->getScale() << " " <<
							static_cast<float>(layer_it->getX()+static_cast<float>(layer_it->getWidth()/2))/c.getRules()->getScale() << " " <<
							static_cast<float>(layer_it->getY()+static_cast<float>(layer_it->getHeight()/2))/c.getRules()->getScale() << ";\n";
						}
					}
				}
			}
			outfile << " END \n";
			outfile << "END " << cells_it->first << "\n";
		}
	}
}

void Lef::readFile(string nome, Circuit& c, bool rTech){
	readTech=rTech;
	currentRules=c.getRules() ;
	arq.open(nome.c_str());
	if(!arq.is_open())
        throw AstranError("Could not open LEF file: " + nome);
	
	string str_tmp;
	vector<string> words;
	
	while(readLine(words)){
		if(words[0]=="MACRO"){
			if(!c.findCellNetlst(words[1])){
				CellNetlst tmp;
				tmp.setName(words[1]);
				c.insertCell(tmp);
			}
			
			CLayout tmp_layout;
			tmp_layout.setName(words[1]);
			readMacro(*(c.getCellNetlst(words[1])),tmp_layout);
			if(tmp_layout.getHeight()!=c.getRowHeight()*round(c.getVPitch()*c.getRules()->getScale()))
				cout << "-> WARNING: Cell height of " << tmp_layout.getName() << " differs from the row height of the circuit" << endl;
			c.insertLayout(tmp_layout);
		}
		else if(words[0]=="UNITS") readUnits();
		else if(words[0]=="MANUFACTURINGGRID") manufGrid=static_cast<int>(1/atof(words[1].c_str()));
	}
}

int Lef::readLine(vector<string>& words){
	string str_tmp;
	words.clear();
	while(words.size()==0 && getline(arq, str_tmp)) {
		fileline++;
		if (!str_tmp.empty()) {
			istrstream line(str_tmp.c_str());
			string str_tmp2;
			while (line >> str_tmp2)
				words.push_back(upcase(str_tmp2));			
			if (words[0][0]=='#') words.clear();
		}
	}
	return static_cast<int>(words.size());
}