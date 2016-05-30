/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
 *   amziesemerj[at]inf.ufrgs.br                                           *
 **************************************************************************/
#include "router.h"

Router::Router():sizeX(0), sizeY(0), sizeZ(0){}

Router::~Router(){}

void Router::test(string r){
	//char* name;
	teste=true;
	sizeX=500;
	sizeY=500;
	sizeZ=2;
	for(int nrNets=50; nrNets<=450; nrNets+=50){
	//int nrNets=3;
		netIndex.clear();
		cout << endl << "-> Routing test circuit with #Nets=" << nrNets << endl; 
		rt.clear();
		rt.setSize(sizeX,sizeY,sizeZ);
		rt.setLayerCosts(0,	11, 4, 4);
		rt.setLayerCosts(1,	4, 11, 4);
		for(int x=1; x<=nrNets; x++){
			netIndex[to_string(x)]=x;
			int netSize=2+rand()%5;
			for(int y=0; y<netSize; y++){
				int node=rand()%(sizeX*sizeY);
				if(rt.getNet(node)){ --y; continue;}
				rt.addNodetoNet(netIndex[to_string(x)], node);
				Point p((rt.getPosX(node)+1)*pitchH, (rt.getPosY(node)+1)*pitchV);
			}
		}
		cout << "-> OURS: " << endl;
		rt.routeNets(200);
		//sprintf(name,"%d",nrNets) ;
		//saveRoutingRotdl("teste");
		rt.reset();
		cout << "-> ROTDL: " << endl;
		rotdl(r);
		//readRoutingRotdl("teste");
	}	
}

void Router::setup(int sX, int sY, int sZ){
	teste=false;
	sizeX=sX;
	sizeY=sY;
	sizeZ=sZ;
	rt.clear();
	rt.setSize(sizeX,sizeY,sizeZ);
	rt.setLayerCosts(0,	999 , 999, 4);
	rt.setLayerCosts(1,	11, 4, 4);
	rt.setLayerCosts(2,	4, 11, 4);
	
	int pitchH=currentCircuit->getHPitch()*currentCircuit->getRules()->getScale();
	int pitchV=currentCircuit->getVPitch()*currentCircuit->getRules()->getScale();
	
	netIndex.clear();
	int i=1;
	vector<Net>& nets=currentCircuit->getCellNetlst(currentCircuit->getTopCell())->getNets();
	for(vector<Net>::iterator nets_it=nets.begin(); nets_it!=nets.end(); nets_it++){
		if(nets_it->name!=currentCircuit->getVddNet() && nets_it->name!=currentCircuit->getGndNet()){
			if(netIndex.find(nets_it->name)==netIndex.end()) netIndex[nets_it->name]=i++;
			Circuit::Interface* tmp_int=currentCircuit->getInterface(nets_it->name);
			if(tmp_int){
				//					cerr << netIndex[nets_it->name] << " " << tmp_int->pos.getX()+currentCircuit->getLMargin() << " " << tmp_int->pos.getY()+currentCircuit->getBMargin() << endl;
				rt.addNodetoNet(netIndex[nets_it->name], rt.getPos(tmp_int->pos.getX()+currentCircuit->getLMargin() ,tmp_int->pos.getY()+currentCircuit->getBMargin(), 2));
			}
			for(list<InstancePin>::iterator nodes_it=nets_it->insts.begin(); nodes_it!=nets_it->insts.end(); nodes_it++){
				Instance* tmp_inst=currentCircuit->getLayout(currentCircuit->getTopCell() + "_PL")->getInstance(nodes_it->targetCellInst);
				if(tmp_inst){
					CLayout* tmp_layout=currentCircuit->getLayout(tmp_inst->getTargetCell());
					if(tmp_layout){
						multimap<string,Pin>::iterator pin_it = tmp_layout->getPins().find(currentCircuit->getCellNetlst(tmp_inst->getTargetCell())->getInout(nodes_it->targetPin));
						if(pin_it!=tmp_layout->getPins().end()){
							int posx=pin_it->second.getX(), posy=pin_it->second.getY();
							if(tmp_inst->getMX())
								posy=tmp_inst->getY()-posy;
							else
								posy+=tmp_inst->getY();
							if(tmp_inst->getMY())
								posx=tmp_inst->getX()-posx;
							else
								posx+=tmp_inst->getX();								
							rt.addNodetoNet(netIndex[nets_it->name], rt.getPos(round(static_cast<float>(posx)/pitchH)+currentCircuit->getLMargin() ,round(static_cast<float>(posy)/pitchV)+currentCircuit->getBMargin(),0));
						}
                        else
                            throw AstranError("Pin " + currentCircuit->getCellNetlst(tmp_inst->getTargetCell())->getInout(nodes_it->targetPin) + " not found in cell instance " + nodes_it->targetCellInst);
					}else
                        throw AstranError("Cell " + tmp_inst->getTargetCell() + " not found");
                }else
                    throw AstranError("Cell instance " + nodes_it->targetCellInst + " not found");
			}
		}
	}
//	if(netIndex.find("bl")!=netIndex.end()) rt.setBlockageNet(netIndex["bl"]);
}

void Router::route(int effort){
	rt.routeNets(effort);
}

void Router::optimize(){
	rt.optimize(); 
}

void Router::compactLayout(string lpSolverFile){
	vector<layer_name> rtLayers(5);
	rtLayers[0]=MET1;
	rtLayers[1]=VIA1;
	rtLayers[2]=MET2;
	rtLayers[3]=VIA2;
	rtLayers[4]=MET3;
	rtLayers[5]=VIA3;
	rtLayers[6]=MET4;
	
	int pitchH=currentCircuit->getHPitch()*currentCircuit->getRules()->getScale();
	int pitchV=currentCircuit->getVPitch()*currentCircuit->getRules()->getScale();
	
	vector<Box*> geometries;
	
	Compaction cpt(CP_LP, "ILProuting");
	Rules* currentRules=currentCircuit->getRules();
	cpt.insertConstraint( "ZERO", "MAXM1", CP_EQ, currentRules->getRule(W1M1)*1.5);
	cpt.insertConstraint( "ZERO", "MAXM12", CP_EQ, currentRules->getRule(S1M1M1)*3.5);
	
	vector<CLayout> rtLayout(netIndex.size());
	map<string, int>::iterator netIndex_it;
	for(netIndex_it=netIndex.begin(); netIndex_it!=netIndex.end(); netIndex_it++)
		rtLayout[netIndex_it->second-1].setName(netIndex_it->first);
	
	string lastx, lastSeg;
	bool adj;
	vector<string> labels(sizeX*sizeY*sizeZ, "");
	int currentNode;
	for(int z=0; z<sizeZ; z++){
		vector<string> lasty(sizeY,""), lastSegY(sizeY,"");
		for(int y=0; y<sizeY; y++){
			lastx="";
			lastSeg="";
			adj=false;
			for(int x=0; x<sizeX; x++){
				currentNode=rt.getPos(x,y,z);
				if(rt.isSource(currentNode)){
					Point p((x+1)*pitchH,(y+1)*pitchV);
					rtLayout[rt.getNet(currentNode)-1].addLabel("X",p);
				}
				if(/* DISABLES CODE */ (true)){
					if(rt.areConnected(currentNode, RT_EAST)){
						rtLayout[rt.getNet(currentNode)-1].addPath((x+1)*pitchH, (y+1)*pitchV, currentRules->getRule(W1M1),pitchH, true, E, rtLayers[z*2]);
					}
					if(rt.areConnected(currentNode, RT_NORTH)){
						rtLayout[rt.getNet(currentNode)-1].addPath((x+1)*pitchH, (y+1)*pitchV, currentRules->getRule(W1M1),pitchV, true, N, rtLayers[z*2]);
					}
					if(rt.areConnected(currentNode, RT_UP)){
						Box b=rtLayout[rt.getNet(currentNode)-1].addLayer((x+1)*pitchH, (y+1)*pitchV, currentRules->getRule(W2VI), currentRules->getRule(W2VI), rtLayers[(z*2)+1]);
						if(z) rtLayout[rt.getNet(currentNode)-1].addEnc(b, currentRules->getRule(E1M1VI), rtLayers[z*2]);
						rtLayout[rt.getNet(currentNode)-1].addEnc(b, currentRules->getRule(E1M2VI), rtLayers[(z+1)*2]);
					}
				}else{
/*					if(z%2==1){
						//se conecta com o da direira ou para baixo cria dogleg
						if(x<sizeX-1 && rt.areConnected(currentNode,RT_EAST)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(0,((y+1)*pitchV)-currentRules->getRule(W1M1)/2, 0, ((y+1)*pitchV)+currentRules->getRule(W1M1)/2, rtLayers[z*2]));
								labels[currentNode]=to_string(geometries.size()-1);
								cpt.insertConstraint( "x" + labels[currentNode] + "a", "x" + labels[currentNode] + "b", CP_EQ, "x" + labels[currentNode] + "min");
								cpt.insertLPMinVar("x" + labels[currentNode] + "min");
								if(lastx!="")
									cpt.insertConstraint( "x" + lastx + "b", "x" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lastx=labels[currentNode];
							}
							labels[currentNode+1]=labels[currentNode];
						}
						
						if(y<sizeY-1 && rt.areConnected(currentNode,RT_NORTH)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(0,((y+1)*pitchV)-currentRules->getRule(W1M1)/2, 0, ((y+1)*pitchV)+currentRules->getRule(W1M1)/2, rtLayers[z*2]));
								labels[currentNode]=to_string(geometries.size()-1);
								cpt.insertConstraint( "x" + labels[currentNode] + "a", "x" + labels[currentNode] + "b", CP_EQ, "x" + labels[currentNode] + "min");
								cpt.insertLPMinVar("x" + labels[currentNode] + "min");
								if(lastx!="")
									cpt.insertConstraint( "x" + lastx + "b", "x" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lastx=labels[currentNode]; //armazena o último segmento
							}
						}
						
						if(y>0 && rt.areConnected(currentNode, RT_DOWN)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(0,((y+1)*pitchV)-currentRules->getRule(W1M1)/2, 0, ((y+1)*pitchV)+currentRules->getRule(W1M1)/2, rtLayers[z*2]));
								labels[currentNode]=to_string(geometries.size()-1);
								cpt.insertConstraint( "x" + labels[currentNode] + "a", "x" + labels[currentNode] + "b", CP_EQ, "x" + labels[currentNode] + "min");
								cpt.insertLPMinVar("x" + labels[currentNode] + "min");
								if(lastx!="")
									cpt.insertConstraint( "x" + lastx + "b", "x" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lastx=labels[currentNode]; //armazena o último segmento
							}
							
							geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(0,y*pitchV, 0, (y+1)*pitchV, rtLayers[z*2]));
							string px=to_string(geometries.size()-1);
							cpt.insertConstraint( "x" + px + "a", "x" + px + "b", CP_MIN, currentRules->getRule(W1M1));
							cpt.insertConstraint( "x" + px + "a", "x" + px + "b", CP_EQ, "x" + px + "s");
							cpt.insertConstraint( "x" + px + "s", "MAXM1", CP_EQ, "x" + px + "min");
							cpt.insertLPMinVar("x" + px + "min",2);
							
							cpt.insertConstraint( "x" + labels[currentNode] + "a", "x" + px + "a", CP_MIN, "ZERO");
							cpt.insertConstraint( "x" + px + "b", "x" + labels[currentNode] + "b", CP_MIN, "ZERO");
							
							cpt.insertConstraint( "x" + labels[rt.getPos(x,y-1,z)] + "a", "x" + px + "a", CP_MIN, "ZERO");
							cpt.insertConstraint( "x" + px + "b", "x" + labels[rt.getPos(x,y-1,z)] + "b", CP_MIN, "ZERO");
							
							if(lastSeg!=""){
								cpt.insertConstraint( "x" + lastSeg + "b", "x" + px + "a", CP_MIN, currentRules->getRule(S1M1M1));
								cpt.insertConstraint( "x" + lastSeg + "b", "x" + px + "a", CP_EQ, "x" + px + "se");
								if(adj){
									cpt.insertConstraint( "x" + px + "se", "MAXM12", CP_EQ, "x" + px + "mine");
									cpt.insertLPMinVar("x" + px + "mine",3);
								}
							}
							
							lastSeg=px;
							adj=true;
						}
						else adj=false;
						
						if(z<sizeZ-1 && rt.areConnected(currentNode,RT_UP)){
							Box b=rtLayout[rt.getNet(currentNode)-1].addLayer((x+1)*pitchH, (y+1)*pitchV, currentRules->getRule(W2VI), currentRules->getRule(W2VI), rtLayers[(z*2)+1]);
							rtLayout[rt.getNet(currentNode)-1].addEnc(b, currentRules->getRule(E1M1VI), rtLayers[z*2]);
							rtLayout[rt.getNet(currentNode)-1].addEnc(b, currentRules->getRule(E1M2VI), rtLayers[(z+1)*2]);
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(0,((y+1)*pitchV)-currentRules->getRule(W1M1)/2, 0, ((y+1)*pitchV)+currentRules->getRule(W1M1)/2, rtLayers[z*2]));
								labels[currentNode]=to_string(geometries.size()-1);
								cpt.insertConstraint( "x" + labels[currentNode] + "a", "x" + labels[currentNode] + "b", CP_EQ, "x" + labels[currentNode] + "min");
								cpt.insertLPMinVar("x" + labels[currentNode] + "min");
								if(lastx!="")
									cpt.insertConstraint( "x" + lastx + "b", "x" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lastx=labels[currentNode]; //armazena o último segmento
							}
							cpt.insertConstraint("ZERO",  "x" + labels[currentNode] + "a", CP_MAX, ((x+1)*pitchH)-(currentRules->getRule(W2VI)/2)-currentRules->getRule(E1M1VI));
							cpt.insertConstraint("ZERO",  "x" + labels[currentNode] + "b", CP_MIN, ((x+1)*pitchH)+(currentRules->getRule(W2VI)/2)+currentRules->getRule(E1M1VI));
						}
						
						if(z>0 && rt.areConnected(currentNode, RT_DOWN)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(0,((y+1)*pitchV)-currentRules->getRule(W1M1)/2, 0, ((y+1)*pitchV)+currentRules->getRule(W1M1)/2, rtLayers[z*2]));
								labels[currentNode]=to_string(geometries.size()-1);
								cpt.insertConstraint( "x" + labels[currentNode] + "a", "x" + labels[currentNode] + "b", CP_EQ, "x" + labels[currentNode] + "min");
								cpt.insertLPMinVar("x" + labels[currentNode] + "min");
								if(lastx!="")
									cpt.insertConstraint( "x" + lastx + "b", "x" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lastx=labels[currentNode]; //armazena o último segmento
							}
							cpt.insertConstraint("ZERO",  "x" + labels[currentNode] + "a", CP_MAX, ((x+1)*pitchH)-(currentRules->getRule(W2VI)/2)-currentRules->getRule(E1M1VI));
							cpt.insertConstraint("ZERO",  "x" + labels[currentNode] + "b", CP_MIN, ((x+1)*pitchH)+(currentRules->getRule(W2VI)/2)+currentRules->getRule(E1M1VI));
						}
						if(rt.isSource(currentNode)){
							cpt.insertConstraint("ZERO",  "x" + labels[currentNode] + "a", CP_MAX, ((x+1)*pitchH));
							cpt.insertConstraint("ZERO",  "x" + labels[currentNode] + "b", CP_MIN, ((x+1)*pitchH));
						}
						
					}
					else{
						if(y<sizeY-1 && rt.areConnected(currentNode,RT_NORTH)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(((x+1)*pitchH)-currentRules->getRule(W1M1)/2, 0, ((x+1)*pitchH)+currentRules->getRule(W1M1)/2, 0, rtLayers[z*2]));
								labels[currentNode]=to_string(geometries.size()-1);
								cpt.insertConstraint( "y" + labels[currentNode] + "a", "y" + labels[currentNode] + "b", CP_EQ, "y" + labels[currentNode] + "min");
								cpt.insertLPMinVar("y" + labels[currentNode] + "min");
								if(lasty[x]!="")
									cpt.insertConstraint( "y" + lasty[x] + "b", "y" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lasty[x]=labels[currentNode]; //armazena o último segmento
							}
							labels[rt.getPos(x,y+1,z)]=labels[currentNode];
						}
						if(x<sizeX-1 && rt.areConnected(currentNode,RT_EAST)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(((x+1)*pitchH)-currentRules->getRule(W1M1)/2, 0, ((x+1)*pitchH)+currentRules->getRule(W1M1)/2, 0, rtLayers[z*2]));
								labels[currentNode]=to_string(geometries.size()-1);
								cpt.insertConstraint( "y" + labels[currentNode] + "a", "y" + labels[currentNode] + "b", CP_EQ, "y" + labels[currentNode] + "min");
								cpt.insertLPMinVar("y" + labels[currentNode] + "min");
								if(lasty[x]!="")
									cpt.insertConstraint( "y" + lasty[x] + "b", "y" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lasty[x]=labels[currentNode];
							}
						}
						
						if(x>0 && rt.areConnected(currentNode, RT_WEST)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(((x+1)*pitchH)-currentRules->getRule(W1M1)/2, 0, ((x+1)*pitchH)+currentRules->getRule(W1M1)/2, 0, rtLayers[z*2]));
								labels[currentNode]=to_string(geometries.size()-1);
								cpt.insertConstraint( "y" + labels[currentNode] + "a", "y" + labels[currentNode] + "b", CP_EQ, "y" + labels[currentNode] + "min");
								cpt.insertLPMinVar("y" + labels[currentNode] + "min");
								if(lasty[x]!="")
									cpt.insertConstraint( "y" + lasty[x] + "b", "y" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lasty[x]=labels[currentNode]; //armazena o último segmento
							}
							
							geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(x*pitchH, 0, (x+1)*pitchH, 0, rtLayers[z*2]));
							string px=to_string(geometries.size()-1);
							cpt.insertConstraint( "y" + px + "a", "y" + px + "b", CP_MIN, currentRules->getRule(W1M1));
							cpt.insertConstraint( "y" + px + "a", "y" + px + "b", CP_EQ, "y" + px + "s");
							cpt.insertConstraint( "y" + px + "s", "MAXM1", CP_EQ, "y" + px + "min");
							cpt.insertLPMinVar("y" + px + "min",2);
							
							cpt.insertConstraint( "y" + labels[currentNode] + "a", "y" + px + "a", CP_MIN, "ZERO");
							cpt.insertConstraint( "y" + px + "b", "y" + labels[currentNode] + "b", CP_MIN, "ZERO");
							
							cpt.insertConstraint( "y" + labels[rt.getPos(x-1,y,z)] + "a", "y" + px + "a", CP_MIN, "ZERO");
							cpt.insertConstraint( "y" + px + "b", "y" + labels[rt.getPos(x-1,y,z)] + "b", CP_MIN, "ZERO");
							
							if(lastSegY[x]!=""){
								cpt.insertConstraint( "y" + lastSegY[x] + "b", "y" + px + "a", CP_MIN, currentRules->getRule(S1M1M1));
								cpt.insertConstraint( "y" + lastSegY[x] + "b", "y" + px + "a", CP_EQ, "y" + px + "se");
								if(rt.areConnected(rt.getPos(x,y-1,z), RT_WEST)){
									cpt.insertConstraint( "y" + px + "se", "MAXM12", CP_EQ, "y" + px + "mine");
									cpt.insertLPMinVar("y" + px + "mine",3);
								}
							}						
							lastSegY[x]=px;
						}
						
						if(z<sizeZ-1 && rt.areConnected(currentNode,RT_UP)){
							Box b=rtLayout[rt.getNet(currentNode)-1].addLayer((x+1)*pitchH, (y+1)*pitchV, currentRules->getRule(W2VI), currentRules->getRule(W2VI), rtLayers[(z*2)+1]);
							rtLayout[rt.getNet(currentNode)-1].addEnc(b, currentRules->getRule(E1M1VI), rtLayers[z*2]);
							rtLayout[rt.getNet(currentNode)-1].addEnc(b, currentRules->getRule(E1M2VI), rtLayers[(z+1)*2]);
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(((x+1)*pitchH)-currentRules->getRule(W1M1)/2, 0, ((x+1)*pitchH)+currentRules->getRule(W1M1)/2, 0, rtLayers[z*2]));
								labels[currentNode]=to_string(geometries.size()-1);
								cpt.insertConstraint( "y" + labels[currentNode] + "a", "y" + labels[currentNode] + "b", CP_EQ, "y" + labels[currentNode] + "min");
								cpt.insertLPMinVar("y" + labels[currentNode] + "min");
								if(lasty[x]!="")
									cpt.insertConstraint( "y" + lasty[x] + "b", "y" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lasty[x]=labels[currentNode]; //armazena o último segmento
							}
							cpt.insertConstraint("ZERO",  "y" + labels[currentNode] + "a", CP_MAX, ((y+1)*pitchV)-(currentRules->getRule(W2VI)/2)-currentRules->getRule(E1M1VI));
							cpt.insertConstraint("ZERO",  "y" + labels[currentNode] + "b", CP_MIN, ((y+1)*pitchV)+(currentRules->getRule(W2VI)/2)+currentRules->getRule(E1M1VI));
						}
						
						if(z>0 && rt.areConnected(currentNode, RT_DOWN)){
							if(labels[currentNode]==""){ //cria um novo segmento
								geometries.push_back(&rtLayout[rt.getNet(currentNode)-1].addPolygon(((x+1)*pitchV)-currentRules->getRule(W1M1)/2, 0, ((x+1)*pitchV)+currentRules->getRule(W1M1)/2, 0, rtLayers[z*2]));
								labels[currentNode]=to_string(geometries.size()-1);
								cpt.insertConstraint( "y" + labels[currentNode] + "a", "y" + labels[currentNode] + "b", CP_EQ, "y" + labels[currentNode] + "min");
								cpt.insertLPMinVar("y" + labels[currentNode] + "min");
								if(lasty[x]!="")
									cpt.insertConstraint( "y" + lasty[x] + "b", "y" + labels[currentNode] + "a", CP_MIN, currentRules->getRule(S1M1M1));
								lasty[x]=labels[currentNode]; //armazena o último segmento
							}
							cpt.insertConstraint("ZERO",  "y" + labels[currentNode] + "a", CP_MAX, ((y+1)*pitchV)-(currentRules->getRule(W2VI)/2)-currentRules->getRule(E1M1VI));
							cpt.insertConstraint("ZERO",  "y" + labels[currentNode] + "b", CP_MIN, ((y+1)*pitchV)+(currentRules->getRule(W2VI)/2)+currentRules->getRule(E1M1VI));
						}
						if(rt.isSource(currentNode)){
							cpt.insertConstraint("ZERO",  "y" + labels[currentNode] + "a", CP_MAX, ((y+1)*pitchV));
							cpt.insertConstraint("ZERO",  "y" + labels[currentNode] + "b", CP_MIN, ((y+1)*pitchV));
						}
					}
*/				}
			}
		}
	}
	
	cpt.solve(lpSolverFile, 3600);
	
	for (unsigned int i =0; i < geometries.size(); i++ ) {		
		int xa = cpt.getVariableVal( "x" + to_string( i ) + "a" );
		int xb = cpt.getVariableVal( "x" + to_string( i ) + "b" );
		int ya = cpt.getVariableVal( "y" + to_string( i ) + "a" );
		int yb = cpt.getVariableVal( "y" + to_string( i ) + "b" );
		
		if(xa!=-1 && xb!=-1){
			geometries[i]->setWidth(xb-xa);
			geometries[i]->setX((xb+xa)/2);
		}
		if(ya!=-1 && yb!=-1){
			geometries[i]->setHeight(yb-ya);
			geometries[i]->setY((yb+ya)/2);
		}		
	}
	CLayout rt_insts(currentCircuit->getTopCell() + "_RT");
	currentCircuit->insertLayout(rt_insts);
	vector<CLayout>::iterator layout_it;
	for(layout_it=rtLayout.begin(); layout_it!=rtLayout.end(); layout_it++){
		layout_it->merge();
		currentCircuit->insertLayout(*layout_it);
		currentCircuit->getLayout(currentCircuit->getTopCell() + "_RT")->addInstance(layout_it->getName(), layout_it->getName());		
	}
	currentCircuit->getLayout(currentCircuit->getTopCell())->addInstance("ROUTING", currentCircuit->getTopCell() + "_RT");
	currentCircuit->getLayout(currentCircuit->getTopCell())->placeCell("ROUTING", -(pitchH+currentCircuit->getHPitch()), -(pitchV+currentCircuit->getVPitch()), false, false);
}

//Save routing result in Rotdl format
void Router::saveRoutingRotdl(string fileName){
	int net;
	ofstream froute(fileName.c_str());

	if (!froute.is_open())
        throw AstranError("Could not save routing to file: " + fileName);

	for(net=1;net<=rt.netlistSize();net++){
		rt.pathTree(net,froute);
	}
	froute.close();
}

//Read routing result file in Rotdl format
void Router::readRoutingRotdl(string fileName){
    rt.clear();

	ifstream file(fileName.c_str());
	if (!file.is_open())
        throw AstranError("Could not open routing to file: " + fileName);
    
	int fileline=0, pxini, pyini, pzini, nrMov, net;
	string str_tmp, str, typeMove, netName;
	while(getline(file,str_tmp)) {
		fileline++;
		if(!str_tmp.size()) continue;
		istrstream line(str_tmp.c_str());
		line >> str;
		if(upcase(str)=="NET"){
			line >> netName;
			while(line >> str){
				net=netIndex[netName];
				if(str=="("){
					line >> pxini;
					line >> pyini;				
					line >> pzini;
					line >> str;
					pzini+=!teste;
				}
				else if(str=="["){
					line >> nrMov;
					line >> typeMove;
					for(int x=0; x<nrMov; x++){
						if(typeMove=="UP"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini, pyini, pzini+1)); ++pzini;
						}else if(typeMove=="DOWN"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini, pyini, pzini-1)); --pzini;
						}else if(typeMove=="EAST"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini+1, pyini, pzini)); ++pxini;
						}else if(typeMove=="WEST"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini-1, pyini, pzini)); --pxini;
						}else if(typeMove=="NORTH"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini, pyini+1, pzini)); ++pyini;
						}else if(typeMove=="SOUTH"){
							rt.connect(net, rt.getPos(pxini, pyini, pzini), rt.getPos(pxini, pyini-1, pzini)); --pyini;
						}
					}
					line >> str; // ]
				}else
                    throw AstranError("Expected ( or [ in line: " + to_string(fileline));
			}
			
		}else if(upcase(str)=="ERROR:"){
			line >> str;
			line >> netName;
            throw AstranError("Unable to route net: " + netName + " in line: " + to_string(fileline));
		}else 
            throw AstranError("Parser error in line: " + to_string(fileline));
	}
	rt.showResult();
}

void Router::rotdl(string path){
	int pitchH=currentCircuit->getHPitch()*currentCircuit->getRules()->getScale();
	int pitchV=currentCircuit->getVPitch()*currentCircuit->getRules()->getScale();
	
	ofstream frot("temp.rot");
	if (!frot.is_open())
        throw AstranError("Could not save routing to file: " + path);
    
	frot << sizeX << " " << sizeY << " " << sizeZ-1+teste << endl;
	frot << "0 0" << endl;
	frot <<  pitchH << " " << pitchV << endl;
	frot << currentCircuit->getRules()->getCIFVal(MET2) << " "<< currentCircuit->getRules()->getCIFVal(VIA2) << " " << currentCircuit->getRules()->getCIFVal(MET3) << endl;
	frot << currentCircuit->getRules()->getRule(W1M2) << " "<< currentCircuit->getRules()->getRule(W2V2) << " " << currentCircuit->getRules()->getRule(W1M3) << endl;
	
	vector<int> *nodesList;
	vector<int>::iterator nodesList_it;
	map<string, int>::iterator netIndex_it;
	for(netIndex_it=netIndex.begin(); netIndex_it!=netIndex.end(); netIndex_it++){
		nodesList=rt.getNetNodes(netIndex_it->second);
		frot << netIndex_it->second << " " << netIndex_it->first << " " << nodesList->size() << " " << nodesList->size();
		for(nodesList_it=nodesList->begin(); nodesList_it!=nodesList->end(); nodesList_it++){
			if(!teste && rt.getPosZ(*nodesList_it)==0){
				frot << " (" << 0 << " " << rt.getPosX(*nodesList_it) << " " << rt.getPosY(*nodesList_it) << " 1h)";
				rt.connect(netIndex[netIndex_it->first], *nodesList_it, *nodesList_it+sizeX*sizeY);
			}else
				frot << " (" << rt.getPosZ(*nodesList_it)-1+teste << " " << rt.getPosX(*nodesList_it) << " " << rt.getPosY(*nodesList_it) << " 1h)";
		}
		frot << ";" << endl;
	}
	frot.close();
	
	string cmd = "\"" + path + "\" -outputConnections temp.rot > temp.log";
	cout << "-> Executing rotdl: " << cmd << endl;

	time_t start,end;
	time(&start); 
	
	FILE *x = _popen(cmd.c_str(), "r");
	
	if(x==NULL)
        throw AstranError("Failed to execute rotdl!");
	
	_pclose(x);

	time(&end); 
	cout << "-> Runtime = " << difftime(end, start) << "s" << endl;
	
    readRoutingRotdl("temp.rot.connections");
}

int Router::getNrNets(int x, int y, int z){
	return sizeX ? rt.getNrNets(rt.getPos(x,y,z)) : 0;
}
