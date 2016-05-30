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

bool CellNetlst::visit( int nDifs, TransitorTerminal& transP, TransitorTerminal& transN){
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
    list<TransitorTerminal>::iterator transP_it, transN_it;
    
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
            Net tmp;
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
int CellNetlst::insertNet(string& name, TransTerminalType type, int t){
    int c=0;
    TransitorTerminal tmp2;
    tmp2.link=t;
    tmp2.type=type;
    
    while(c<nets.size() && nets[c].name!=name) c++;
    if(c==nets.size()){		// if doesnt exist, create a new net and add the new transistor
        Net tmp;
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

void CellNetlst::folding(float pSize, float nSize, string VddNet_Name, string GndNet_Name){
    orderingP.clear();
    orderingN.clear();
    
    //Aplica o folding nas series da rede de transistores
    this->defineIOToGraph(VddNet_Name, GndNet_Name);
    this->foldingSeries(pSize, nSize);
    
    for(int c=0; c<trans.size(); c++){
        switch(trans[c].type){
            case PMOS:
                if(trans[c].width>pSize+0.00001f){
                    int n=ceil(trans[c].width/(pSize+0.00001f));
                    trans[c].width = trans[c].width/n;
                    for(;n>1;n--){
                        string tmp=trans[c].name+"_"+to_string(n);
                        insertTrans(tmp, getNetName(trans[c].drain), getNetName(trans[c].gate), getNetName(trans[c].source), trans[c].type, trans[c].length, trans[c].width);
                    }
                }
                break;
            case NMOS:
                if(trans[c].width>nSize+0.00001f){
                    int n=ceil(trans[c].width/(nSize+0.00001f));
                    trans[c].width = trans[c].width/n;
                    for(;n>1;n--){
                        string tmp=trans[c].name+"_"+to_string(n);
                        insertTrans(tmp, getNetName(trans[c].drain), getNetName(trans[c].gate), getNetName(trans[c].source), trans[c].type, trans[c].length, trans[c].width);
                    }
                }
                break;
        }
    }
}

void CellNetlst::insertTrans(string name, string d, string g, string s, TransType t, float l, float w){
    Transistor tmp;
    tmp.name=name;
    tmp.type=t;
    tmp.length=l;
    tmp.width=w;
    tmp.visited=false;
    tmp.drain=insertNet(d,DRAIN,static_cast<int>(trans.size()));	// Create a bi-directional link between both objects
    tmp.gate=insertNet(g,GATE,static_cast<int>(trans.size()));
    tmp.source=insertNet(s,SOURCE,static_cast<int>(trans.size()));
    trans.push_back(tmp);
}

void CellNetlst::insertInstance(string instName, string subCircuit, vector<string>& ports){
    CellInstance tmp;
    tmp.subCircuit=subCircuit;
    vector<string>::iterator ports_it;
    for(int port=0; port<ports.size(); ++port){
        int c=0;
        while(c<nets.size() && nets[c].name!=ports[port]) c++;
        if(c==nets.size()){		// if doesnt exist
            Net tmp2;
            tmp2.name=ports[port];
            nets.push_back(tmp2);
            tmp.ports.push_back(static_cast<int>(nets.size()-1));
        }
        else tmp.ports.push_back(c);
        InstancePin tmp_inst;
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
     for(vector<Net>::iterator tmp=nets.begin(); tmp!=nets.end(); tmp++){
     cout << tmp->name << ":";
     for(list<TransitorTerminal>::iterator tmp2=tmp->trans.begin(); tmp2!=tmp->trans.end(); tmp2++)
     cout << trans[tmp2->link].name << " ";
     cout << endl;
     }
     */	cout << "-> Transistors: " << endl;
    for(int tmp=0; tmp<trans.size(); tmp++)
        cout << trans[tmp].name << "     " << nets[trans[tmp].drain].name << "     " << nets[trans[tmp].gate].name << "     " << nets[trans[tmp].source].name << "     " <<
        (trans[tmp].type?"NMOS":"PMOS") << "     L=" << trans[tmp].length << "     W=" << trans[tmp].width << endl;
    cout << "-> Instances: " << endl;
    for(map<string,CellInstance>::iterator tmp=instances.begin(); tmp!=instances.end(); ++tmp){
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

int CellNetlst::getCost(){
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
                default: assert(false);
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
                default: assert(false);
            }
        }
        if(orderingP[pos].link!=-1 && orderingN[pos].link!=-1 && trans[orderingP[pos].link].gate!=trans[orderingN[pos].link].gate) ++mismatchesGate;
        //		if(leftP!=leftN && !((nets[leftP].name=="VDD" || nets[leftP].name=="VCC") && (nets[leftN].name=="GND" || nets[leftN].name=="0"))) missMatchSourceDrain++;
        //		if(rightP!=rightN && !((nets[rightP].name=="VDD" || nets[rightP].name=="VCC") && (nets[rightN].name=="GND" || nets[rightN].name=="0"))) missMatchSourceDrain++;
        
        //analisa horizontalmente o custo
        if(pos){
            if(orderingP[pos-1].link!=-1 && orderingP[pos].link!=-1 && leftP!=lastRightP) ++wGaps;
            if(orderingN[pos-1].link!=-1 && orderingN[pos].link!=-1 && leftN!=lastRightN) ++wGaps;
            if((orderingP[pos-1].link!=-1 && orderingP[pos].link!=-1 && leftP!=lastRightP) || (orderingN[pos-1].link!=-1 && orderingN[pos].link!=-1 && leftN!=lastRightN)) posPN+=1;
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
        if(boundBoxNets_ini[pos]!=-1 && nName!=vddNet && nName!=gndNet){
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

int CellNetlst::perturbation(){
    custo_anterior = custo_atual;
    PorN=rand()%4;
    switch(PorN){
        case 0: cpP=orderingP; move(orderingP); break;
        case 1: cpN=orderingN; move(orderingN); break;
        default: cpP=orderingP; cpN=orderingN; move(orderingP); move(orderingN); break;
    }
    custo_atual = getCost();
    return custo_atual-custo_anterior;
}

void CellNetlst::move(vector<TransitorTerminal>& ref){
    static vector<TransitorTerminal> aux;
    static TransitorTerminal tmp;
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
                default: assert(false);
            }
        }
        for(int c=0; c<size/2; c++){
            tmp=ref[origin+c];
            ref[origin+c]=ref[origin+size-1-c];
            ref[origin+size-1-c]=tmp;
        }
    }
}

int CellNetlst::getProblemSize(){
    return static_cast<int>(orderingP.size()*orderingN.size());
}

void CellNetlst::undoPerturbation(){
    switch(PorN){
        case 0: orderingP=cpP; break;
        case 1: orderingN=cpN; break;
        default: orderingP=cpP; orderingN=cpN; break;
    }
    
    custo_atual = custo_anterior;
}

bool CellNetlst::transPlacement(bool newPl, int saquality, int nrattempts, int wC, int gmC, int rC, int congC, int ngC, string vddNet, string gndNet){
    wCost=wC;
    gmCost=gmC;
    rCost=rC;
    congCost=congC;
    ngCost=ngC;
    this->vddNet=vddNet;
    this->gndNet=gndNet;
    
    unsigned long i;
    if(newPl){
        orderingP.clear();
        orderingN.clear();
        TransitorTerminal tmp;
        tmp.type=DRAIN;
        
        for(i=0;i<trans.size();++i){
            tmp.link= static_cast<int>(i);
            if(trans[i].type==PMOS)
                orderingP.push_back(tmp);
            else
                orderingN.push_back(tmp);
        }
        
        tmp.type=DRAIN;
        tmp.link=-1;
        unsigned long width=max(orderingP.size(),orderingN.size());
        for(i=orderingP.size();i<width;i++)
            orderingP.push_back(tmp);
        for(i=orderingN.size();i<width;i++)
            orderingN.push_back(tmp);
    }
    int best=0;
    int best_cost=getCost();
    //	cout << "- Initial Cost (0): " << best_cost << endl;
    
    tmpP=orderingP;
    tmpN=orderingN;
    
    for(int at=1; at<=nrattempts ; at++){
        custo_atual=custo_anterior=getCost();
        //		cout << "- Running Threshold Accept algorithm (" << at << "): " << endl;
        //		*this = SimulatedAnnealing<CellNetlst>(*this,saquality,false,true,!ep || at!=1);
        *this = ThresholdAccept<CellNetlst>(*this,saquality,false,true,true);
        cout << endl;
        if(best_cost>=getCost()){
            best=at;
            best_cost=getCost();
            tmpP=orderingP;
            tmpN=orderingN;
        }
    }
    
    orderingP=tmpP;
    orderingN=tmpN;
    return true;
}
void CellNetlst::printPlacement(){
    getCost();
    cout << "-> Final cost: Width=" << posPN << "; Gate Mismatches="<<  mismatchesGate << "; WL="<< wRouting << "; Rt. Density=" << maxCong << "; Nr. Gaps=" << wGaps << endl;
    //	cout << "-> Transistor Ordering (" << best << "): " << endl;
    cout << "   PMOS: ";
    for(int i=0;i<orderingP.size();i++){
        if(orderingP[i].link==-1) cout << "GAP";
        else cout << trans[orderingP[i].link].name;
        cout << "-";
    }
    cout << endl;
    cout << "   NMOS: ";
    for(int i=0;i<orderingN.size();i++){
        if(orderingN[i].link==-1) cout << "GAP";
        else cout << trans[orderingN[i].link].name;
        cout << "-";
    }
    cout << endl;
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

bool CellNetlst::defineIOToGraph(string Vdd_Name, string Gnd_Name){
    //Define quais sao as entradas e saidas, tanto "externas" quanto "internas" de cada circuito
    list<TransitorTerminal>::iterator net2_it;
    list<TransitorTerminal>::iterator net2Aux_it;
    vector<int>::iterator gates_it;
    vector<int>::iterator outs_it;
    
    try{
        //Definir quais nets sao gates, para posteriormente ter as saidas "internas"
        for (int i = 0; i < this->trans.size(); i++){
            if (gates.empty())
                gates.push_back(trans[i].gate);
            if (!isGate(trans[i].gate))
                gates.push_back(trans[i].gate);
        }
        //Determinar qual o indice da net que é vcc e gnd
        for (int i = 0; i < nets.size(); i++) {
            if(nets[i].name==Vdd_Name)
                this->vcc = i;
            if(nets[i].name== Gnd_Name)
                this->gnd = i;
        }
        
        //Definir as saidas "interna", ou seja, que nao estao descritas no arquivo spice, mas sao internamente utilizadas como saida
        //Se a net for gate e também for drain/source, quer dizer que ela é uma saída interna, visto que este drain/source sera gate em outro transistor
        for (gates_it = gates.begin(); gates_it != gates.end(); gates_it++){
            for (net2_it = nets[*gates_it].trans.begin(); net2_it != nets[*gates_it].trans.end(); net2_it++){
                if ((net2_it->type == DRAIN || net2_it->type == SOURCE) && (!isOut(*gates_it))){
                    outs.push_back(*gates_it);
                    break;
                }
            }
        }
        
        //Definir as saidas 'externas' do circuito
        for (int i = 0; i < this->inouts.size(); i++){
            for (net2_it = nets[inouts[i]].trans.begin(); net2_it != nets[inouts[i]].trans.end(); net2_it++){
                for (net2Aux_it = nets[inouts[i]].trans.begin(); net2Aux_it != nets[inouts[i]].trans.end(); net2Aux_it++ ){
                    if ((trans[net2Aux_it->link].type != trans[net2_it->link].type) && (!isGate(inouts[i]))){
                        if (!isOut(inouts[i])){
                            outs.push_back(inouts[i]);
                        }
                    }
                }
            }
        }
        
        return true;
        
    } catch(exception){
        cout << "\n-> Error: CellNetlst_defineIOToGraph method";
    }
    return false;
}

bool CellNetlst::isOut(int out_id){ // Testa se ID eh uma saida armazenada no vetor de saidas;
    for (int j = 0; j < outs.size(); j++){
        if (out_id == outs[j]) return true;
    }
    return false;
}

bool CellNetlst::isGate(int gate_id){ //Testa se ID eh um gate armazenado no vetor dos gates
    for (int j = 0; j < gates.size(); j++){
        if (gate_id == gates[j]) return true;
    }
    return false;
}

bool CellNetlst::wasVisit(int indexNet){
    for (int i = 0; i < visitToFolding.size(); i++){
        if (visitToFolding[i] == indexNet) return true;
    }
    return false;
}

int CellNetlst::getSeriesToFolding(int currentNet, int currentTrans, int nextNet){
    //Descobrir quais transistores estao em serie, para aplicar o folding
    try{
        this->columnToFolding = -1;
        do {
            // Preencher matriz com os transistores que estao em serie.
            this->columnToFolding++;
            this->transToFolding[this->lineToFolding][this->columnToFolding] = currentTrans;
            
            if (nets[nextNet].trans.front().link == currentTrans){
                currentTrans = nets[nextNet].trans.back().link;
                if (trans[currentTrans].drain == nextNet){
                    nextNet = trans[currentTrans].source;
                } else nextNet = trans[currentTrans].drain;
                
            } else {
                currentTrans = nets[nextNet].trans.front().link;
                
                if (trans[currentTrans].drain == nextNet){
                    nextNet = trans[currentTrans].source;
                } else nextNet = trans[currentTrans].drain;
            }
            
        }while ((nets[nextNet].trans.size() == 2)
                && (!isOut(nextNet))
                && (nextNet != this->gnd)
                && (nextNet != this->vcc));
        this->columnToFolding++;
        this->transToFolding[this->lineToFolding][this->columnToFolding] = currentTrans;
        this->lineToFolding++;
        return nextNet;
        
    } catch (exception) {
        cout << "\n-> Error: CellNetlst_getSeriesToFolding method";
    }
    return -1;
}

void CellNetlst::findSeriesToFolding(int startNet){
    //Metodo que apartir de um determinado vertice, procura os transistores que estao em serie
    list<TransitorTerminal>::iterator transNet_it;
    int nextNet;
    try {
        
        if(isOut(startNet) || wasVisit(startNet)) return;
        
        this->visitToFolding.push_back(startNet);
        
        for (transNet_it = nets[startNet].trans.begin(); transNet_it != nets[startNet].trans.end(); transNet_it++){
            if (transNet_it->type == SOURCE){
                nextNet = trans[transNet_it->link].drain;
            } else {
                nextNet = trans[transNet_it->link].source;
            }
            
            if ((nets[nextNet].trans.size() == 2) && (!isOut(nextNet)) && (nextNet != startNet)){
                //Método que procura os vertices em serie;
                nextNet = this->getSeriesToFolding(startNet, transNet_it->link , nextNet);
            }
            if((!wasVisit(nextNet)) && (nextNet != this->gnd) && (nextNet != this->vcc)){
                this->findSeriesToFolding(nextNet);
            }
        }
        
    } catch (exception) {
        cout << "\n-> Error: CellNetlst_findSeriesToFolding method";
    }
}

void CellNetlst::foldingSeries(float pSize, float nSize){
    int numTransSeries, legs, biggerTrans, numSeries;
    float widthBiggerTrans = 0;
    bool folding = false;
    numTransSeries = biggerTrans = legs = 0;
    
    try{
        this->totalTrans = static_cast<int>(trans.size());
        numSeries = (static_cast<int>(totalTrans-(totalTrans*0.1)));
        this->transToFolding = new int*[numSeries];
        for (int i = 0; i < numSeries; i++){
            this->transToFolding[i] = new int[totalTrans];
        }
        
        this->lineToFolding = this->columnToFolding = 0;
        this->visitToFolding.clear();
        
        for(int i = 0; i < numSeries; i++){
            for (int j = 0; j < this->totalTrans; j++)
                this->transToFolding[i][j] = -1;
        }
        
        //Preenche a matriz com os transistores que estao em serie
        this->findSeriesToFolding(this->gnd);
        this->findSeriesToFolding(this->vcc);
        
        if (this->transToFolding[0][0] == -1) return;
        
        for(int i = 0; i < numSeries; i++){
            if (this->transToFolding[i][0] == -1) break;
            folding = false;
            numTransSeries = biggerTrans = legs = 0;
            widthBiggerTrans = 0;
            int auxIndex = 0;
            int transSeries = 0;
            for (int j = 0; j < this->totalTrans; j++){
                auxIndex = this->transToFolding[i][j];
                if (auxIndex == -1) break;
                numTransSeries = j;
                //Procura o maior transistor da serie e determina o numero de quebras
                switch(trans[auxIndex].type){
                    case PMOS:
                        if (this->trans[auxIndex].width > pSize+0.00001f) {
                            transSeries++;
                            if (widthBiggerTrans < this->trans[auxIndex].width){
                                biggerTrans = auxIndex;
                                widthBiggerTrans = this->trans[auxIndex].width;
                                legs = ceil(trans[auxIndex].width/(pSize+0.00001f));
                            }
                        }
                        break;
                    case NMOS:
                        if (this->trans[auxIndex].width > nSize+0.00001f) {
                            transSeries++;
                            if (widthBiggerTrans < this->trans[auxIndex].width){
                                biggerTrans = auxIndex;
                                widthBiggerTrans = this->trans[auxIndex].width;
                                legs = ceil(trans[auxIndex].width/(nSize+0.00001f));
                            }
                        }
                        break;
                }
            }
            
            if ((transSeries-1) == numTransSeries) folding = true;
            
            if (folding == true){
                //Aplica o folding na serie
                seriesFolding(i, numTransSeries, legs, biggerTrans);
                folding = false;
            }
        }
        
        for (int i = 0; i < numSeries; i++){
            delete []this->transToFolding[i];
        }
        delete this->transToFolding;
        this->transToFolding = NULL;
        
    } catch(exception){
        cout << "\n-> Error: CellNetlst_foldingSeries method";
    }
    
}

bool CellNetlst::seriesFolding(int numSequence, int numTrans, int numLegs, int biggerTrans){
    float widthTrans;
    int indexTrans;
    indexTrans = -1;
    int indexProxTrans = -1;
    int indexPrevTrans = -1;
    int legs = numLegs;
    
    for (;numLegs > 1;numLegs--){
        for (int i = 0; i <= numTrans ; i++) {
            indexTrans = this->transToFolding[numSequence][i];
            indexProxTrans = this->transToFolding[numSequence][i+1];
            indexPrevTrans = this->transToFolding[numSequence][i-1];
            widthTrans = trans[indexTrans].width/legs;
            /*Testa o primeiro transistor da sequencia, pois ou o dreno ou a fonte dele
             deve possuir a mesma conexão que a serie original*/
            if (i == 0){
                //Teste para o primeiro transistor da serie
                if (this->trans[indexTrans].drain == this->trans[indexProxTrans].source){
                    //O transistor da ponto estah conectado ao outro transistor da serie pelo dreno
                    string auxDrain = this->getNetName(this->trans[indexTrans].drain)+"_"+to_string(numLegs);
                    string auxName = this->trans[indexTrans].name+"_"+to_string(numLegs);
                    string auxSource = this->getNetName(this->trans[indexTrans].source);
                    this->insertTrans(auxName, auxDrain, getNetName(this->trans[indexTrans].gate), auxSource, this->trans[indexTrans].type, this->trans[indexTrans].length, widthTrans);
                } else if (this->trans[indexProxTrans].drain == this->trans[indexTrans].source){
                    //O transistor da ponto estah conectado ao outro transistor da série pela fonte
                    string auxSource = this->getNetName(this->trans[indexTrans].source)+"_"+to_string(numLegs);
                    string auxName = this->trans[indexTrans].name+"_"+to_string(numLegs);
                    string auxDrain = this->getNetName(this->trans[indexTrans].drain);
                    this->insertTrans(auxName, auxDrain, getNetName(this->trans[indexTrans].gate), auxSource, this->trans[indexTrans].type, this->trans[indexTrans].length, widthTrans);
                } else if (this->trans[indexTrans].drain == this->trans[indexProxTrans].drain){
                    //Quando o drain dos dois transistores sao iguais;
                    string auxDrain = this->getNetName(this->trans[indexTrans].drain)+"_"+to_string(numLegs);
                    string auxName = this->trans[indexTrans].name+"_"+to_string(numLegs);
                    string auxSource = this->getNetName(this->trans[indexTrans].source);
                    this->insertTrans(auxName, auxDrain, getNetName(this->trans[indexTrans].gate), auxSource, this->trans[indexTrans].type, this->trans[indexTrans].length, widthTrans);
                } else if (this->trans[indexTrans].source == this->trans[indexProxTrans].source){
                    //Quando o source dos dois transistores sao iguais
                    string auxSource = this->getNetName(this->trans[indexTrans].source)+"_"+to_string(numLegs);
                    string auxName = this->trans[indexTrans].name+"_"+to_string(numLegs);
                    string auxDrain = this->getNetName(this->trans[indexTrans].drain);
                    this->insertTrans(auxName, auxDrain, getNetName(this->trans[indexTrans].gate), auxSource, this->trans[indexTrans].type, this->trans[indexTrans].length, widthTrans);
                }
            } else if (i == (numTrans)) {
                //Teste para o ultimo transistor da serie
                if (this->trans[indexTrans].drain == this->trans[indexPrevTrans].source){
                    string auxDrain = this->getNetName(this->trans[indexTrans].drain)+"_"+to_string(numLegs);
                    string auxName = this->trans[indexTrans].name+"_"+to_string(numLegs);
                    string auxSource = this->getNetName(this->trans[indexTrans].source);
                    this->insertTrans(auxName, auxDrain, getNetName(this->trans[indexTrans].gate), auxSource, this->trans[indexTrans].type, this->trans[indexTrans].length, widthTrans);
                } else if (this->trans[indexPrevTrans].drain == this->trans[indexTrans].source){
                    string auxSource = this->getNetName(this->trans[indexTrans].source)+"_"+to_string(numLegs);
                    string auxName = this->trans[indexTrans].name+"_"+to_string(numLegs);
                    string auxDrain = this->getNetName(this->trans[indexTrans].drain);
                    this->insertTrans(auxName, auxDrain, getNetName(this->trans[indexTrans].gate), auxSource, this->trans[indexTrans].type, this->trans[indexTrans].length, widthTrans);
                } else if (this->trans[indexTrans].drain == this->trans[indexPrevTrans].drain){
                    //Quando o drain dos dois transistores sao iguais;
                    string auxDrain = this->getNetName(this->trans[indexTrans].drain)+"_"+to_string(numLegs);
                    string auxName = this->trans[indexTrans].name+"_"+to_string(numLegs);
                    string auxSource = this->getNetName(this->trans[indexTrans].source);
                    this->insertTrans(auxName, auxDrain, getNetName(this->trans[indexTrans].gate), auxSource, this->trans[indexTrans].type, this->trans[indexTrans].length, widthTrans);
                } else if (this->trans[indexTrans].source == this->trans[indexPrevTrans].source){
                    //Quando o source dos dois transistores sao iguais
                    string auxSource = this->getNetName(this->trans[indexTrans].source)+"_"+to_string(numLegs);
                    string auxName = this->trans[indexTrans].name+"_"+to_string(numLegs);
                    string auxDrain = this->getNetName(this->trans[indexTrans].drain);
                    this->insertTrans(auxName, auxDrain, getNetName(this->trans[indexTrans].gate), auxSource, this->trans[indexTrans].type, this->trans[indexTrans].length, widthTrans);
                }
                
            } else { //Transistores que estão no meio da serie
                string auxDrain = this->getNetName(this->trans[indexTrans].drain)+"_"+to_string(numLegs);
                string auxName = this->trans[indexTrans].name+"_"+to_string(numLegs);
                string auxSource = this->getNetName(this->trans[indexTrans].source)+"_"+to_string(numLegs);
                this->insertTrans(auxName, auxDrain, getNetName(this->trans[indexTrans].gate), auxSource, this->trans[indexTrans].type, this->trans[indexTrans].length, widthTrans);
            }
        }
    }
    //Atualiza o tamanho dos transistores originais
    for (int i = 0; i <= numTrans; i++) {
        indexTrans = this->transToFolding[numSequence][i];
        this->trans[indexTrans].width = trans[indexTrans].width/legs;
    }
    return true;
}


