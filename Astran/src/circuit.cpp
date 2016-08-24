/***************************************************************************
 *   Copyright (C) 2013 by Adriel Mota Ziesemer Jr.                         *
 *   amziesemerj[at]inf.ufrgs.br                                            *
 ****************************************************************************/
#include "circuit.h"

Circuit::Circuit(){
	hGrid=0.20f;
	vGrid=0.20f;
    hGridOffset=false;
    vGridOffset=false;
	supplyVSize=0.72f;
	nWellBorder=0.235f;
	pnSelBorder=0.09f;
	nWellPos=1.14f;
	cellTemplate="Tapless";
	vddNet="VCC";
	gndNet="GND";
	cellsHeight=13;
	c2lMargin=c2rMargin=c2tMargin=c2bMargin=0;
	topCell="";
}

vector<string> Circuit::getCellTemplatesList(){
    vector<string> list;
    list.push_back("Tapless");
    list.push_back("Taps close to the boundary");
    list.push_back("Taps w/ continuous diff");
    return list;
}

bool Circuit::setMargins(float l, float r, float t, float b){
	c2lMargin=round(l/getHPitch());
	c2rMargin=round(r/getHPitch());
	c2tMargin=round(t/getVPitch());
	c2bMargin=round(b/getVPitch());
	cout << "-> Margins: L=" << c2lMargin*getHPitch() << "um  R=" << c2rMargin*getHPitch() << "um  T=" << c2tMargin*getVPitch() << "um  B=" << c2bMargin*getVPitch() << "um"  << endl;
	return true;
}

CellNetlst* Circuit::getCellNetlst(string n){
    CellNetlst* tmp=findCellNetlst(n);
	if(!tmp)
        throw AstranError("Could not find cell netlist: " + n);
    return tmp;
}

CellNetlst* Circuit::findCellNetlst(string n){
	map<string,CellNetlst>::iterator tmp=cellNetlsts.find(n);
	if(tmp==cellNetlsts.end())
        return NULL;
    return &tmp->second;
}

string* Circuit::getEquation(string n){
	map<string,string>::iterator tmp=equations.find(n);
	if(tmp!=equations.end())
		return &tmp->second;
	else
		return NULL;
}

Circuit::Interface* Circuit::getInterface(string n){
	map<string,Interface>::iterator tmp=interfaces.find(n);
	if(tmp!=interfaces.end())
		return &tmp->second;
	else
		return NULL;
}

bool Circuit::insertLayout(CLayout& tmp){
	// map<string, CLayout>::iterator cells_it=layouts.find(tmp.getName());
	//	if(cells_it!=layouts.end())
	//		cout << "-> Cell layout " << tmp.getName() << " already exists. Overwriting..." << endl;
	layouts[tmp.getName()]=tmp;
	return true;
}

CLayout* Circuit::getLayout(string n){
	map<string,CLayout>::iterator tmp=layouts.find(n);
	if(tmp!=layouts.end())
		return &tmp->second;
	else
		return NULL;
}

void Circuit::insertCell(CellNetlst& tmp){
	map<string,CellNetlst>::iterator cells_it=cellNetlsts.find(tmp.getName());
    if(cells_it!=cellNetlsts.end())
		cout << "-> Overwriting cell netlist: " << tmp.getName() << endl;
	cellNetlsts[tmp.getName()]=tmp;
}

bool Circuit::insertEquation(string cellName, string& tmp){
	//map<string, string>::iterator cells_it=equations.find(cellName);
	//	if(cells_it!=equations.end())
	//		cout << "-> Cell equation " << cellName << " already exists. Overwriting..." << endl;
	equations[cellName]=tmp;
	return true;
}

bool Circuit::insertInterface(string name, direction orient, IOType type, int posx, int posy){
	Interface tmp;
	tmp.orient=orient;
	tmp.ioType=type;
	tmp.pos.setX(posx);
	tmp.pos.setY(posy);
	interfaces[name]=tmp;
	return true;
}

/*
 bool Circuit::promoteCell2Top(string cellName){
 CLayout instances(currentCircuit->getTopCell() + "_PL");
 insertLayout(instances);
 
 CellNetlst* cell=getCellNetlst(cellName);
 for(int c=0; c<cell->getInouts().size(); ++c)
 insertInterface(cell->getNetName(cell->getInouts()[c]), N, cell->getIOType(c), 0, 0);
 for(map<string,CellInstance>::iterator inst_it=cell->getInstances().begin(); inst_it!=cell->getInstances().end(); ++inst_it){
 list<string> inouts;
 for(int c=0; c<inst_it->second.ports.size(); ++c)
 inouts.push_back(cell->getNetName(inst_it->second.ports[c]));
 insertInstance(currentCircuit->getTopCell() + "_PL", inst_it->first, inst_it->second.subCircuit, inouts);
 }
 return true;
 */

/*
 void Circuit::printCells(){
 map<string,Cell>::iterator cells_it;
 for(cells_it=cells.begin(); cells_it!=cells.end(); cells_it++)
 cells_it->second.print();
 }
 */

CellNetlst Circuit::getFlattenCell(string name){
	CellNetlst tmp;
	int n;
	if(cellNetlsts.find(name)==cellNetlsts.end()){
		cout << "-> ERROR: Cell not found: " << name << endl;
	}else{
		tmp=cellNetlsts[name];
		map<string,CellInstance>& instances=tmp.getInstances();
		for(map<string,CellInstance>::iterator instance_it=instances.begin(); instance_it!=instances.end(); ++instance_it){
			if(instance_it==instances.begin()) cout << "-> Removing cell hierarchy in " << name << endl;
			CellNetlst currentCell=getFlattenCell(instance_it->second.subCircuit);
			currentCell.appendNetNameSuffix(instance_it->first);
			if(instance_it->second.ports.size()!=currentCell.getInouts().size()){
				cout << "-> ERROR: Call for cell " << instance_it->second.subCircuit << " have different number of IOs in subcircuit " << name << endl;
				return tmp;
			}
			for(n=0;n<instance_it->second.ports.size();++n)
				currentCell.getNet(currentCell.getInouts().at(n)).name=tmp.getNetName(instance_it->second.ports[n]);
			for(n=0;n<currentCell.size();++n){
				Transistor& currentTrans=currentCell.getTrans(n);
				currentTrans.name=currentTrans.name+"_"+instance_it->first;
				tmp.insertTrans(currentTrans.name, currentCell.getNetName(currentTrans.drain), currentCell.getNetName(currentTrans.gate), currentCell.getNetName(currentTrans.source), currentTrans.type, currentTrans.length, currentTrans.width);
			}
		}
	}	
	return tmp;
}

void Circuit::printInterface(string net){
	map<string, Interface>::iterator interface_it=interfaces.find(net);
	if(interface_it==interfaces.end())
        throw AstranError("Could not find interface: " + net);
    
    cout << "-> Interface: " <<  net << " => Orientation: ";
    switch(interface_it->second.orient){
        case N: cout << "N"; break;
        case S: cout << "S"; break;
        case W: cout << "W"; break;
        case E: cout << "E"; break;
    }
    cout << "; Type: ";
    switch(interface_it->second.ioType){
        case IOTYPE_INPUT: cout << "INPUT"; break;
        case IOTYPE_OUTPUT: cout << "OUTPUT"; break;
        case IOTYPE_INOUT: cout << "INOUT"; break;
    }
    cout << "; Position: (" << interface_it->second.pos.getX() << "," <<  interface_it->second.pos.getY() << ")";
    cout << endl;
}

void Circuit::printInstance(CLayout* l, string instance){
	Instance *currentInstance = l->getInstance(instance);
	if(!currentInstance)
        throw AstranError("Instance " + instance + " not found");
    
    cout << "-> Instance: " <<  instance << " => " << currentInstance->getTargetCell() << endl;
    currentInstance->print();
    cout << "-> Pins Assignment (Global Net->Pin): ";
    /*		map<string, list<Net> >::iterator netList_it;
     for(netList_it=netList.begin(); netList_it!=netList.end(); netList_it++)
     for(tmp_it=netList_it->second.begin(); tmp_it!=netList_it->second.end(); tmp_it++)
     if(tmp_it->targetCellInst==instance)
     cout << " (" << netList_it->first << "->" << tmp_it->targetPin << ") ";
     cout << endl;
     */
}

void Circuit::printNet(string net){
    /*	map<string, list<Net> >::iterator netList_it=netList.find(net);
     list<Net>::iterator tmp_it;
     if(netList_it!=netList.end()){
     cout << "Net: " <<  netList_it->first << " -> ";
     for(tmp_it=netList_it->second.begin(); tmp_it!=netList_it->second.end(); tmp_it++)
     cout << "(" << tmp_it->targetCellInst << "," << tmp_it->targetPin << ") ";
     cout << endl;
     return true;
     }
     else
     */
}

void Circuit::writeCellSizesFile(string filename){
	ofstream file;
	file.open(filename.c_str()); // Write
	
	if ((!file))
        throw AstranError("Could not save file: " + filename);
    
    map<string,CellNetlst>::iterator cells_it;
    for(cells_it=cellNetlsts.begin(); cells_it!=cellNetlsts.end(); cells_it++){
        CLayout* l=getLayout(cells_it->first);
        if(!l)
            throw AstranError("Cell Layout " + cells_it->first + " not found");
        
        file << cells_it->first << endl;
        file << l->getWidth()/currentRules->getIntValue(getHPitch()) << endl;
        file << l->getHeight()/currentRules->getIntValue(getVPitch()) << endl;
    }
}


/*
 bool Circuit::checkCells(){
 bool ok=true;
 map<string,Cell>::iterator cells_it=cells.begin();
 
 while(ok && cells_it!=cells.end()){
 ok && cells_it->second.check();
 cells_it++;
 }
 return ok;
 }
 */

void Circuit::calculateCellsPins(){
	int halfPinSize=currentRules->getRule(E1M1VI)+currentRules->getRule(W2VI)/2;
	int hGrid=currentRules->getIntValue(getHPitch()), vGrid=currentRules->getIntValue(getVPitch());
	for(map<string, CellNetlst>::iterator cellNetlsts_it=cellNetlsts.begin(); cellNetlsts_it!=cellNetlsts.end(); ++cellNetlsts_it){
		map<string, CLayout>::iterator layouts_it=layouts.find(cellNetlsts_it->first);
		if(layouts_it!=layouts.end()){
			layouts_it->second.getPins().clear();
			for(int x=hGrid/2; x<=layouts_it->second.getWidth(); x+=hGrid){
				for(int y=vGrid; y<=layouts_it->second.getHeight()-vGrid; y+=vGrid){
					string connectedNet="none";
					string aroundNet="none";
					bool fail=false;
					int finalX, finalY;
					for(map <layer_name , list<Box> >::iterator layers_it = layouts_it->second.layers.begin(); layers_it != layouts_it->second.layers.end(); layers_it++){
						if(layers_it->first==MET1){
							for(list <Box>::iterator layer_it = layers_it->second.begin(); layer_it != layers_it->second.end(); layer_it++){
								int dx=max(layer_it->getX1(),x-halfPinSize) - min(layer_it->getX2(),x+halfPinSize);
								int dy=max(layer_it->getY1(),y-halfPinSize) - min(layer_it->getY2(),y+halfPinSize);
								if(dx<=-(2*halfPinSize-1) && dy<=-(2*halfPinSize-1)){
									if(connectedNet=="none"){
										connectedNet=layer_it->getNet();
										finalX=x;
										finalY=y;
									}
									if(connectedNet!=layer_it->getNet() || (aroundNet!="none" && aroundNet!=connectedNet)) fail=true;									
								}else if((dx<0 && dy<0) || 
										 (dx<=0 && dy>=0 && dy<currentRules->getRule(S1M1M1)-1) ||
										 (dx>=0 && dy<=0 && dx<currentRules->getRule(S1M1M1)-1) ||
										 (dx>=0 && dy>=0 && sqrt(static_cast<float>(dx*dx+dy*dy))<currentRules->getRule(S1M1M1)-1)){
									if(aroundNet=="none") aroundNet=layer_it->getNet();
									if(aroundNet!=layer_it->getNet() || (connectedNet!="none" && aroundNet!=connectedNet)) fail=true;
								}								
							}
						}
					}
					Pin p;
					p.setPos(x,y);
					p.setLayer(MET1);
					if(!fail && connectedNet!="none" && connectedNet!=""){
						layouts_it->second.getPins().insert(make_pair(connectedNet,p));
						layouts_it->second.addLabel(connectedNet, Point(x, y));
					} else if(!(connectedNet=="none" && aroundNet=="none")){
						layouts_it->second.getPins().insert(make_pair("bl",p));
                        //						layouts_it->second.addLabel("bl", Point(x, y));
					}
				} 
			}
		}
	}		
}
