#include "cellnetlst.h"
#include <algorithm>

bool CellNetlst::verifyCnt(int pos){
	if(nets[pos].trans.size()>2) 	// if is connected to more than 2 transistors
		return true;
	else
		for(vector<int>::iterator tmp=inouts.begin(); tmp!=inouts.end(); tmp++) // or is connected to a IO pin
			if(*tmp==pos) return true;	
	return false;
}

bool CellNetlst::visit( int nDifs, t_net2& transP, t_net2& transN){
	bool ok=false;
	
	trans[transP.link].visited=true;
	trans[transN.link].visited=true;
	orderingP.push_back(transP);
	orderingN.push_back(transN);
	
	//	cout << "(" << nets[trans[transP.link].gate].name << "-"<< nets[trans[transN.link].gate].name << ")";
	
	if(orderingP.size() + orderingN.size()==trans.size()) return true;
	
	if(!(ok=visit(nDifs))){
		//		cout << ".";
		trans[transP.link].visited=false;
		trans[transN.link].visited=false;
		orderingP.pop_back();
		orderingN.pop_back();
	}
	
	return ok;
}

bool CellNetlst::visit( int nDifs){
	bool ok=false;
	int netP, netN, netP_2, netN_2;
	list<t_net2>::iterator transP_it, transN_it;
	
	if(orderingP.size()){
		//netP pega a redes conectada ao ultimo transistor P
		if(orderingP.back().type==SOURCE) netP=trans[orderingP.back().link].drain;
		else netP=trans[orderingP.back().link].source;
	}
	
	if(orderingN.size()){
		//netN pega a rede conectada ao ultimo transistor N
		if(orderingN.back().type==SOURCE) netN=trans[orderingN.back().link].drain;
		else netN=trans[orderingN.back().link].source;
	}
	
	if(orderingP.size() && orderingN.size()){
		//tenta visita os transistores das 2 redes P e N para ver se acha um com o mesmo gate
		transP_it=nets[netP].trans.begin();
		while(transP_it!=nets[netP].trans.end() && !ok){
			//			cout << trans[transP_it->link].name << " " << endl;
			if(transP_it->type!=GATE && !trans[transP_it->link].visited && trans[transP_it->link].type==PMOS){
				transN_it=nets[netN].trans.begin();
				while(transN_it!=nets[netN].trans.end() && !ok){
					//					cout << trans[transN_it->link].name << " " << endl;
					if(transN_it->type!=GATE && !trans[transN_it->link].visited && trans[transN_it->link].type==NMOS){
						if(trans[transP_it->link].gate==trans[transN_it->link].gate)
							ok=visit(nDifs, *transP_it, *transN_it);
					}
					transN_it++;
				}
			}
			transP_it++;
		}
	}
	
	if(nDifs){
		nDifs--;
		if(!ok && orderingN.size()){		
			//continua a difusao no N e insere um GAP no P
			netP_2=0;
			while(netP_2<nets.size() && !ok){
				if(netP_2!=netP){
					transP_it=nets[netP_2].trans.begin();
					while(transP_it!=nets[netP_2].trans.end() && !ok){
						if(transP_it->type!=GATE && !trans[transP_it->link].visited && trans[transP_it->link].type==PMOS){
							transN_it=nets[netN].trans.begin();
							while(transN_it!=nets[netN].trans.end() && !ok){
								if(transN_it->type!=GATE && !trans[transN_it->link].visited && trans[transN_it->link].type==NMOS){
									if(trans[transP_it->link].gate==trans[transN_it->link].gate)
										ok=visit(nDifs, *transP_it, *transN_it);
								}
								transN_it++;
							}
						}
						transP_it++;
					}
				}
				netP_2++;
			}
		}
		
		if(!ok && orderingP.size()){
			//continua a difusao no P e insere um GAP no N
			netN_2=0;
			transP_it=nets[netP].trans.begin();
			while(transP_it!=nets[netP].trans.end() && !ok){
				if(transP_it->type!=GATE && !trans[transP_it->link].visited && trans[transP_it->link].type==PMOS){
					while(netN_2<nets.size() && !ok){
						if(netN_2!=netN){
							transN_it=nets[netN_2].trans.begin();
							while(transN_it!=nets[netN_2].trans.end() && !ok){
								if(transN_it->type!=GATE && !trans[transN_it->link].visited && trans[transN_it->link].type==NMOS){
									if(trans[transP_it->link].gate==trans[transN_it->link].gate)
										ok=visit(nDifs, *transP_it, *transN_it);
								}
								transN_it++;
							}
						}
						netN_2++;
					}
				}
				transP_it++;
			}
		}
		
		if(!ok){
			//procura um novo comeco com gates iguais nas dif P e N
			netP=0;
			while(netP<nets.size() && !ok){
				transP_it=nets[netP].trans.begin();
				while(transP_it!=nets[netP].trans.end() && !ok){
					if(transP_it->type!=GATE && !trans[transP_it->link].visited && trans[transP_it->link].type==PMOS){
						netN=0;
						while(netN<nets.size() && !ok){
							transN_it=nets[netN].trans.begin();
							while(transN_it!=nets[netN].trans.end() && !ok){
								if(transN_it->type!=GATE && !trans[transN_it->link].visited && trans[transN_it->link].type==NMOS){
									if(trans[transP_it->link].gate==trans[transN_it->link].gate)
										ok=visit(nDifs, *transP_it, *transN_it);
								}
								transN_it++;
							}
							netN++;
						}
					}
					transP_it++;
				}
				netP++;
			}
		}
		
		if(!ok){
			//procura um novo comeco com gates diferentes nas dif P e N
			netP=0;
			while(netP<nets.size() && !ok){
				transP_it=nets[netP].trans.begin();
				while(transP_it!=nets[netP].trans.end() && !ok){
					if(transP_it->type!=GATE && !trans[transP_it->link].visited && trans[transP_it->link].type==PMOS){
						netN=0;
						while(netN<nets.size() && !ok){
							transN_it=nets[netN].trans.begin();
							while(transN_it!=nets[netN].trans.end() && !ok){
								if(transN_it->type!=GATE && !trans[transN_it->link].visited && trans[transN_it->link].type==NMOS){
									if(trans[transP_it->link].gate!=trans[transN_it->link].gate)
										ok=visit(nDifs, *transP_it, *transN_it);
								}
								transN_it++;
							}
							netN++;
						}
					}
					transP_it++;
				}
				netP++;
			}
		} 
	}
	return ok;
}

void CellNetlst::appendNetNameSuffix(string n){
	for(int c=0;c<nets.size();++c) nets[c].name+=n;	
}

bool CellNetlst::eulerpath(){
	int cntDifs=0;
	bool ok=false;
	do{
		cout << "-> Trying to find a transistors distribution with "<< cntDifs << " gap(s)... ";
		cntDifs++;
		ok=visit(cntDifs);
		if(!ok) cout << "-> Euler Path not found" << endl;
		else cout << "-> DONE" << endl;
	}while(!ok && cntDifs<=trans.size()/2);
	return ok;
}

bool CellNetlst::insertInOut(string name){
	int c=0;
	if(!isIO(name)){
	while(c<nets.size() && nets[c].name!=name) c++;
	inouts.push_back(c);
	inouts_type.push_back(IOTYPE_INOUT);
	if(c==nets.size()){		// if doesnt exist, create a new net
		t_net tmp;
		tmp.name=name;
		nets.push_back(tmp);
		return true;
	}else
		return false;
	}
	return true;
}

bool CellNetlst::setIOType(string name, IOType t){
	int c=0;
	while(c<inouts.size() && nets[inouts[c]].name!=name) c++;
	if(c<inouts.size()){
		inouts_type[c]=t;
		return true;
	}
	return false;
}
int CellNetlst::insertNet(string& name, t_DGorS type, int t){
	int c=0;
	t_net2 tmp2;
	tmp2.link=t;
	tmp2.type=type;
	
	while(c<nets.size() && nets[c].name!=name) c++;
	if(c==nets.size()){		// if doesnt exist, create a new net and add the new transistor
		t_net tmp;
		tmp.name=name;
		tmp.trans.push_back(tmp2);
		nets.push_back(tmp);
	}else				// otherwise, add the new transistor to the net
		nets[c].trans.push_back(tmp2);
	return(c);			// return the label of the net
}

bool CellNetlst::check(){
	int c=0;
	while(c<nets.size()){
		if((nets[c].name=="VDD" || nets[c].name=="VCC" || nets[c].name=="GND" || nets[c].name=="0") && find(inouts.begin(), inouts.end(), c)==inouts.end()){
			cout << "-> Net " << nets[c].name << " seems to be a supply signal but is not declared as input/output." << endl;
			inouts.push_back(c);
		}
		if(nets[c].trans.size()<2 && find(inouts.begin(), inouts.end(), c)==inouts.end()){
			cout << "-> WARNING: Net " << nets[c].name << " is connected just to a single node." << endl;
			return false;
		}
		c++;
	}
	return true;
}

void CellNetlst::folding(float pSize, float nSize){
	orderingP.clear();
	orderingN.clear();
	for(int c=0; c<trans.size(); c++){
		switch(trans[c].type){
			case PMOS:
				if(trans[c].width>pSize+0.00001f){
					int n=ceil(trans[c].width/(pSize+0.00001f));
					trans[c].width = trans[c].width/n;
					for(;n>1;n--){
					    string tmp=trans[c].name+"_"+intToStr(n);
					    insertTrans(tmp, getNetName(trans[c].drain), getNetName(trans[c].gate), getNetName(trans[c].source), trans[c].type, trans[c].length, trans[c].width);
					}
				}
				break;
			case NMOS:
				if(trans[c].width>nSize+0.00001f){
					int n=ceil(trans[c].width/(nSize+0.00001f));
					trans[c].width = trans[c].width/n;
					for(;n>1;n--){
					    string tmp=trans[c].name+"_"+intToStr(n);
					    insertTrans(tmp, getNetName(trans[c].drain), getNetName(trans[c].gate), getNetName(trans[c].source), trans[c].type, trans[c].length, trans[c].width);
					}
				}
				break;
		}
	}
}

void CellNetlst::insertTrans(string name, string d, string g, string s, transType t, float l, float w){
	Trans tmp;
	tmp.name=name;
	tmp.type=t;
	tmp.length=l;
	tmp.width=w;
	tmp.visited=false;
	tmp.drain=insertNet(d,DRAIN,trans.size());	// Create a bi-directional link between both objects
	tmp.gate=insertNet(g,GATE,trans.size());
	tmp.source=insertNet(s,SOURCE,trans.size());
	trans.push_back(tmp);
}

void CellNetlst::insertInstance(string instName, string subCircuit, vector<string>& ports){
	Inst tmp;
	tmp.subCircuit=subCircuit;
	vector<string>::iterator ports_it;
	for(int port=0; port<ports.size(); ++port){
		int c=0;
		while(c<nets.size() && nets[c].name!=ports[port]) c++;
		if(c==nets.size()){		// if doesnt exist
			t_net tmp2;
			tmp2.name=ports[port];
			nets.push_back(tmp2);			
			tmp.ports.push_back(nets.size()-1);
		}
		else tmp.ports.push_back(c);
		t_inst tmp_inst;
		tmp_inst.targetCellInst=instName;
		tmp_inst.targetPin=port;
		nets[c].insts.push_back(tmp_inst);
	}
	instances[instName]=tmp;	
}

void CellNetlst::print(){
	cout << "-> Ports: ";
	for(int c=0; c<inouts.size(); c++){
		cout << "(" << nets[inouts[c]].name << ",";
		switch(inouts_type[c]){
			case IOTYPE_INPUT: cout << "IN"; break;
			case IOTYPE_OUTPUT: cout << "OUT"; break;
			case IOTYPE_INOUT: cout << "INOUT"; break; 
		}
		cout << ") ";
	}
	cout << endl;
/*	cout << "Nets: " << endl;
	for(vector<t_net>::iterator tmp=nets.begin(); tmp!=nets.end(); tmp++){
		cout << tmp->name << ":";
		for(list<t_net2>::iterator tmp2=tmp->trans.begin(); tmp2!=tmp->trans.end(); tmp2++)
			cout << trans[tmp2->link].name << " ";
		cout << endl;
	}
*/	cout << "-> Transistors: " << endl;
	for(int tmp=0; tmp<trans.size(); tmp++)
		cout << trans[tmp].name << "\t\t" << nets[trans[tmp].drain].name << "\t\t" << nets[trans[tmp].gate].name << "\t\t" << nets[trans[tmp].source].name << "\t\t" <<
			trans[tmp].type << "\t\t" << trans[tmp].length << "\t\t" << trans[tmp].width << endl;
	cout << "-> Instances: " << endl;
	for(map<string,Inst>::iterator tmp=instances.begin(); tmp!=instances.end(); ++tmp){
		cout << tmp->first << " ";
		for(vector<int>::iterator tmp2=tmp->second.ports.begin(); tmp2!=tmp->second.ports.end(); ++tmp2)
			cout << getNetName(*tmp2) << " ";
		cout << tmp->second.subCircuit << endl;
	}
};

void CellNetlst::clear(){
	trans.clear();
	inouts.clear();
	nets.clear();
	orderingP.clear();
	orderingN.clear();
	instances.clear();	
}

int CellNetlst::GetCost(){
	static int lastRightP, lastRightN, rightP, rightN, leftP, leftN, pos, x;
	static string nName;
	
	vector<int> boundBoxNets_end(nets.size(),-1);
	vector<int> boundBoxNets_ini(nets.size(),-1);
	
	wGaps=0; mismatchesGate=0; wRouting=0; maxCong=0;posPN=0;

	for(pos=0; pos<orderingP.size(); pos++){
		//analisa verticalmente o custo
		if(orderingP[pos].link!=-1){
			switch(orderingP[pos].type){
			case DRAIN: 
				leftP=trans[orderingP[pos].link].drain; 
				rightP=trans[orderingP[pos].link].source; 
				break;
			case SOURCE:
				leftP=trans[orderingP[pos].link].source; 
				rightP=trans[orderingP[pos].link].drain; 
				break;
			}
		}
		if(orderingN[pos].link!=-1){
			switch(orderingN[pos].type){
			case DRAIN: 
				leftN=trans[orderingN[pos].link].drain; 
				rightN=trans[orderingN[pos].link].source; 
				break;
			case SOURCE:
				leftN=trans[orderingN[pos].link].source; 
				rightN=trans[orderingN[pos].link].drain; 
				break;
			}
		}
		if(orderingP[pos].link!=-1 && orderingN[pos].link!=-1 && trans[orderingP[pos].link].gate!=trans[orderingN[pos].link].gate) ++mismatchesGate;
		//		if(leftP!=leftN && !((nets[leftP].name=="VDD" || nets[leftP].name=="VCC") && (nets[leftN].name=="GND" || nets[leftN].name=="0"))) missMatchSourceDrain++;
		//		if(rightP!=rightN && !((nets[rightP].name=="VDD" || nets[rightP].name=="VCC") && (nets[rightN].name=="GND" || nets[rightN].name=="0"))) missMatchSourceDrain++;
		
		//analisa horizontalmente o custo
		if(pos){
			if(orderingP[pos-1].link!=-1 && orderingP[pos].link!=-1 && leftP!=lastRightP) ++wGaps;
			if(orderingN[pos-1].link!=-1 && orderingN[pos].link!=-1 && leftN!=lastRightN) ++wGaps;
			if((orderingP[pos-1].link!=-1 && orderingP[pos].link!=-1 && leftP!=lastRightP) || (orderingN[pos-1].link!=-1 && orderingN[pos].link!=-1 && leftN!=lastRightN)) posPN+=2;
		}
		
		//pega as boundbox das redes
		if(orderingN[pos].link!=-1){
			if(boundBoxNets_ini[leftN]==-1) boundBoxNets_ini[leftN]=posPN;
			boundBoxNets_end[leftN]=posPN;
		}
		if(orderingP[pos].link!=-1){
			if(boundBoxNets_ini[leftP]==-1) boundBoxNets_ini[leftP]=posPN;
			boundBoxNets_end[leftP]=posPN;
		}
		++posPN;
		if(orderingP[pos].link!=-1){
			if(boundBoxNets_ini[trans[orderingP[pos].link].gate]==-1) boundBoxNets_ini[trans[orderingP[pos].link].gate]=posPN;
			boundBoxNets_end[trans[orderingP[pos].link].gate]=posPN;
		}
		if(orderingN[pos].link!=-1){
			if(boundBoxNets_ini[trans[orderingN[pos].link].gate]==-1) boundBoxNets_ini[trans[orderingN[pos].link].gate]=posPN;
			boundBoxNets_end[trans[orderingN[pos].link].gate]=posPN;
		}
		++posPN;
		if(orderingN[pos].link!=-1){
			if(boundBoxNets_ini[rightN]==-1) boundBoxNets_ini[rightN]=posPN;
			boundBoxNets_end[rightN]=posPN;
		}
		if(orderingP[pos].link!=-1){
			if(boundBoxNets_ini[rightP]==-1) boundBoxNets_ini[rightP]=posPN;
			boundBoxNets_end[rightP]=posPN;
		}
		lastRightP=rightP;
		lastRightN=rightN;
	}
	++posPN;
	vector<int> congestioning(posPN+1,0);
	for(pos=0; pos<boundBoxNets_ini.size(); pos++){
		nName=getNetName(pos);
		if(boundBoxNets_ini[pos]!=-1 && nName!="VDD!" && nName!="VDD" && nName!="VCC" && nName!="GND" && nName!="0"){ //TO IMPROVE!!!
			wRouting+=boundBoxNets_end[pos]-boundBoxNets_ini[pos]; 			
			//cout << nName << "-" << boundBoxNets_ini[pos] <<"-"<< boundBoxNets_end[pos] << endl;
			for(x=boundBoxNets_ini[pos]; x<=boundBoxNets_end[pos]; x++)
				++congestioning[x];
		}
	}
    int localCong=0;
    for(pos=0; pos<congestioning.size(); pos++){
        maxCong=max(maxCong,congestioning[pos]);
        localCong+=congestioning[pos]*congestioning[pos];
    }
	//  cout << mismatchesGate<< "-" << wGaps << "-" << wRouting << endl;
	return(localCong + 100*(congCost*maxCong + gmCost*mismatchesGate + ngCost*wGaps + rCost*wRouting + wCost*posPN));
}

int CellNetlst::Perturbation(){
	custo_anterior = custo_atual;
	PorN=rand()%4;
	switch(PorN){
		case 0: cpP=orderingP; move(orderingP); break;
		case 1: cpN=orderingN; move(orderingN); break;
		default: cpP=orderingP; cpN=orderingN; move(orderingP); move(orderingN); break;
	}
	custo_atual = GetCost();
	return custo_atual-custo_anterior;
}

void CellNetlst::move(vector<t_net2>& ref){
	static vector<t_net2> aux;
	static t_net2 tmp;
	static int size, origin, dest;
	size=1+(rand()%ref.size());
	origin=rand()%(ref.size()-size+1);
	if(rand()%2==0){
		//shift
		aux.resize(size);
		for(int c=0; c<size; c++)
			aux[c]=ref[origin+c];
		ref.erase(ref.begin()+origin,ref.begin()+origin+size);	
		dest=rand()%(ref.size()+1);
		ref.insert(ref.begin()+dest, aux.begin(), aux.end());			
	}else{
		//mirror
		for(int c=origin; c<origin+size; c++){
			switch(ref[c].type){
				case DRAIN:
					ref[c].type=SOURCE;
					break;
				case SOURCE:			
					ref[c].type=DRAIN;
					break;
			}
		}
		for(int c=0; c<size/2; c++){
			tmp=ref[origin+c];
			ref[origin+c]=ref[origin+size-1-c];
			ref[origin+size-1-c]=tmp;
		}
	}
}

int CellNetlst::GetProblemSize(){
	return orderingP.size()*orderingN.size();
}

void CellNetlst::UndoPerturbation(){
	switch(PorN){
		case 0: orderingP=cpP; break;
		case 1: orderingN=cpN; break;
		default: orderingP=cpP; orderingN=cpN; break;
	}
	
	custo_atual = custo_anterior;
}

bool CellNetlst::transPlacement(bool ep, int saquality, int nrattempts, int wC, int gmC, int rC, int congC, int ngC){
	wCost=wC;
	gmCost=gmC;
	rCost=rC;
	congCost=congC;
	ngCost=ngC;

	int i;
	if(!orderingP.size() && !orderingN.size()){
		if(!ep || !eulerpath()){ 
//			cout << "- Creating a random initial solution... " << endl;
			
			t_net2 tmp;	
			tmp.type=DRAIN;
			
			for(i=0;i<trans.size();++i){
				tmp.link=i;
				if(trans[i].type==PMOS) 
					orderingP.push_back(tmp);
				else 
					orderingN.push_back(tmp);
			}
			
			tmp.type=DRAIN;
			tmp.link=-1;
			for(i=orderingP.size();i<max(orderingP.size(),orderingN.size());i++)
				orderingP.push_back(tmp);
			for(i=orderingN.size();i<max(orderingP.size(),orderingN.size());i++)
				orderingN.push_back(tmp);
		}
	}
	int best=0;
	int best_cost=GetCost();
//	cout << "- Initial Cost (0): " << best_cost << endl;
	
	tmpP=orderingP;
	tmpN=orderingN;
	
	for(int at=1; at<=nrattempts ; at++){
		custo_atual=custo_anterior=GetCost();
//		cout << "- Running Threshold Accept algorithm (" << at << "): " << endl;
//		*this = SimulatedAnnealing<CellNetlst>(*this,saquality,false,true,!ep || at!=1);
		*this = ThresholdAccept<CellNetlst>(*this,saquality,false,true,!ep || at!=1);
		cout << endl;
		if(best_cost>=GetCost()){
			best=at;
			best_cost=GetCost();
			tmpP=orderingP;
			tmpN=orderingN;
		}
	}
	
	orderingP=tmpP;
	orderingN=tmpN;
	GetCost();
	cout << "-> Final cost: Width=" << posPN << "; Gate Mismatches="<<  mismatchesGate << "; WL="<< wRouting << "; Rt. Density=" << maxCong << "; Nr. Gaps=" << wGaps << endl;
//	cout << "-> Transistor Ordering (" << best << "): " << endl;
	cout << "-> PMOS: ";
	for(i=0;i<orderingP.size();i++){ 
		if(orderingP[i].link==-1) cout << "GAP";
		else cout << trans[orderingP[i].link].name;
		cout << "-";
	}
	cout << endl;
	cout << "-> NMOS: ";
	for(i=0;i<orderingN.size();i++){
		if(orderingN[i].link==-1) cout << "GAP";
		else cout << trans[orderingN[i].link].name;
		cout << "-";
	}	
	cout << endl;
	return true;
}

int CellNetlst::pSize(){
	int c=0;
	for(int i=0;i<trans.size();++i)
		if(trans[i].type==PMOS) c++;
	return c;
}

int CellNetlst::nSize(){
	int c=0;
	for(int i=0;i<trans.size();++i)
		if(trans[i].type==NMOS) c++;
	return c;
}

bool CellNetlst::isIO(string n){
	vector<int>::iterator inouts_it;
	for( inouts_it = getInouts().begin(); inouts_it!=getInouts().end(); ++inouts_it)
		if(n == nets[*inouts_it].name) return true;
	return false;
}

string CellNetlst::getInout(int n){
	if(n<inouts.size()){
		return nets[inouts[n]].name;
	}
	return "";
}
