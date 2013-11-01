/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
 *   amziesemerj[at]inf.ufrgs.br                                           *
 ***************************************************************************/
#include "autocell2.h"

AutoCell::AutoCell() {
    currentCell = NULL;
    rt = NULL;
    state = 0;
}

AutoCell::~AutoCell() {
    clear();
}

void AutoCell::clear() {
    elements.clear();
    inoutPins.clear();
    if (rt != NULL) delete(rt);
}

Element* AutoCell::createElement(int vcost, int nDiffIni, int pDiffIni, int nEnd, int pEnd) {
    Element tmp;
    tmp.diffP = rt->createNode();
    tmp.diffN = rt->createNode();
    tmp.linkP.type = tmp.linkN.type = GAP;
    tmp.gapP = tmp.gapN = false;
    tmp.met.resize(trackPos.size());
    tmp.pol.resize(trackPos.size());
    tmp.diffNEnd = nEnd+1;
    tmp.diffNIni = nDiffIni;
    tmp.diffPIni = pDiffIni;
    tmp.diffPEnd = pEnd-1;
    
    for (int x = 0; x < trackPos.size(); x++) {
        tmp.met[x] = rt->createNode();
        if (x) rt->addArc(tmp.met[x], tmp.met[x - 1], 4);
        if (elements.size())
            rt->addArc(tmp.met[x], elements.back().met[x], 4); //if it's not the first, connect to the last element
        
        tmp.pol[x] = rt->createNode();
        if (x) rt->addArc(tmp.pol[x], tmp.pol[x - 1], 6);
        if ((x<=nEnd) || (x > nDiffIni && x < pDiffIni) || (x>=pEnd)) { //CHECK IF THERE IS ENOUGHT SPACE IN THE EXTERNAL AREA 
            rt->addArc(tmp.pol[x], tmp.met[x], 20);
            if (elements.size() && elements.back().pol[x] != -1)
                rt->addArc(tmp.pol[x], elements.back().pol[x], 6); //if it's not the first, connect to the last element
        }
    }
    rt->addNodetoNet(gnd, tmp.met[0]);
    rt->addNodetoNet(vdd, tmp.met[trackPos.size() - 1]);
    
    tmp.inoutCnt = rt->createNode();
    
    //conecta aos pinos de entrada e saida
    for (int x = 0; x < trackPos.size(); x++)
        rt->addArc(tmp.inoutCnt, tmp.met[x], 500);            
    
    elements.push_back(tmp);
    return &elements.back();
}

bool AutoCell::calcArea(Circuit* c) {
    currentCircuit = c;
    currentRules = currentCircuit->getRules();
    
    cout << "-> Calculating cell area..." << endl;
    state = 0;
    
    vGrid = currentRules->getIntValue(currentCircuit->getVPitch());
    hGrid = currentRules->getIntValue(currentCircuit->getHPitch());
    height = currentCircuit->getRowHeight() * vGrid;
    
    supWidth = max(currentRules->getIntValue(currentCircuit->getSupplyVSize()), currentRules->getRule(W1M1)) / 2;
    posNWell = currentRules->getIntValue(currentCircuit->getnWellPos());
    nDif_iniY = posNWell - currentRules->getRule(S1DNWN);
    pDif_iniY = posNWell + currentRules->getRule(E1WNDP);
    trackPos.clear();
    //central track position
    center = round((float(posNWell) / vGrid) - 0.5);
    trackPos.push_back(center * vGrid);
    
    //tracks position under the central track
    int next;
    do {
        next = trackPos.front() - currentRules->getRule(S1M1M1) - currentRules->getRule(W1M1);
        trackPos.insert(trackPos.begin(), next);
    } while (next >= supWidth); //stop when it touchs gnd
    
    //tracks position above the central track
    do {
        next = trackPos.back() + currentRules->getRule(S1M1M1) + currentRules->getRule(W1M1);
        trackPos.push_back(next);
    } while (next + currentRules->getRule(S1M1M1) <= height - supWidth); //stop when it touchs vdd
    
    //    for (int x = 0; x < trackPos.size(); x++) cout << float(trackPos[x]) / currentRules->getScale() << " ";
    
    //IMPROVE
    nDif_iniY = min(nDif_iniY, center * vGrid - (currentRules->getRule(W2CT) / 2 + currentRules->getRule(E2P1CT) + currentRules->getRule(S1DFP1)));
    pDif_iniY = max(pDif_iniY, center * vGrid + (currentRules->getRule(W2CT) / 2 + currentRules->getRule(E2P1CT) + currentRules->getRule(S1DFP1)));
    
    if(currentCircuit->getCellTemplate()=="Taps close to the boundary"){
        nDif_endY = max(currentRules->getRule(E1INDF), currentRules->getRule(S1P1P1) / 2 + currentRules->getRule(E1P1DF));
        pDif_endY = height - nDif_endY;
    }else if(currentCircuit->getCellTemplate()=="Taps w/ continuous diff"){
        nDif_endY = currentRules->getRule(W2DF)/2 + currentRules->getRule(E1IPDF) + currentRules->getRule(E1P1DF);
        pDif_endY = height - nDif_endY;
    }else{
        nDif_endY = currentRules->getRule(E1P1DF) + currentRules->getRule(S1P1P1) / 2 ;
        pDif_endY = height - nDif_endY;
    }
    
    nSize = nDif_iniY - nDif_endY;
    pSize = pDif_endY - pDif_iniY;
    
    //   cout << "Resume: tracks(" << trackPos.size() << ") " << nSize << "N and " << pSize << "P" <<endl;
    return state++;
}

bool AutoCell::selectCell(string c) {
    if (state < 1) return 0;
    state = 1;
    
    if ((currentCell = currentCircuit->getCellNetlst(c))) {
        cout << "-> Selecting cell netlist: " << currentCell->getName() << endl;
        currentNetList.clear();
        currentNetList = currentCircuit->getFlattenCell(c);
        state = 2;
    }
    return state == 2;
}

bool AutoCell::foldTrans() {
    cout << "-> Applying folding..." << endl;
    if (state < 2) return 0;
    state = 2;
    cout << "-> Number of transistors before folding: " << currentCell->size() << " -> P(" << currentCell->pSize() << ") N(" << currentCell->nSize() << ")" << endl;
    if (currentNetList.folding(float(pSize) / currentRules->getScale(), float(nSize) / currentRules->getScale())) {
        cout << "-> Number of transistors after folding: " << currentNetList.size() << " -> P(" << currentNetList.pSize() << ") N(" << currentNetList.nSize() << ")" << endl;
        state = 3;
    }
    
    
    return state == 3;
}

bool AutoCell::placeTrans(bool ep, int saquality, int nrAttempts, int wC, int gmC, int rC, int congC, int ngC) {
    cout << "-> Placing transistors..." << endl;
    if (state < 3) return 0;
    state = 3;
    if (currentNetList.transPlacement(ep, saquality, nrAttempts, wC, gmC, rC, congC, ngC)) {
        // CALCULATE THE NR OF INTERNAL TRACKS
        diffPini.clear();
        diffNini.clear();
        for (int x = 0; x < currentNetList.getOrderingP().size(); x++) {
            int p;
            for (p = center + 1; trackPos[p] < pDif_iniY; ++p) {
            }
            //            cout << p;
            diffPini.push_back(p);
        }
        for (int x = 0; x < currentNetList.getOrderingN().size(); x++) {
            int p;
            for (p = center - 1; trackPos[p] > nDif_iniY; --p) {
            }
            //            cout << p;
            diffNini.push_back(p);
        }
        
        // CONSTRUCT THE ROUTING GRAPH
        int COST_CNT_INSIDE_DIFF = 10;
        
        Element *tmp, *lastElement;
        bool gapP, gapN;
        
        clear();
        rt = new Pathfinder2();
        
        //        currentCell->print();
        vector<int>::iterator inouts_it = currentNetList.getInouts().begin();
        while (inouts_it != currentNetList.getInouts().end()) {
            //		cerr << currentCircuit->getGndNet()  << " - " <<  currentCircuit->getVddNet() << " - " << currentNetList.getNetName(*inouts_it) << endl;
            if (currentNetList.getNetName(*inouts_it) == currentCircuit->getVddNet()) {
                vdd = *inouts_it;
            } else if (currentNetList.getNetName(*inouts_it) == currentCircuit->getGndNet()) {
                gnd = *inouts_it;
            } else {
                inoutPins[currentNetList.getNetName(*inouts_it)] = rt->createNode();
                rt->addNodetoNet(*inouts_it, inoutPins[currentNetList.getNetName(*inouts_it)]);
            }
            inouts_it++;
        }
        //cout << currentNetList.getNetName(0);
        
        //cria elemento para roteamento lateral
        tmp = createElement(20, center, center,center,center);
        
        //conecta sinais de entrada e saida com o nó inoutCnt do elemento
        map<string, int>::iterator inoutPins_it;
        for (inoutPins_it = inoutPins.begin(); inoutPins_it != inoutPins.end(); inoutPins_it++)
            rt->addArc(tmp->inoutCnt, inoutPins_it->second, 0);
        
        vector<t_net2>::iterator eulerPathP_it = currentNetList.getOrderingP().begin(), eulerPathN_it = currentNetList.getOrderingN().begin(), lastP_it, lastN_it;
        int nDiffTop,pDiffTop;
        
        while (eulerPathP_it != currentNetList.getOrderingP().end() && eulerPathN_it != currentNetList.getOrderingN().end()) {
            gapP = false;
            gapN = false;
            
            if (eulerPathP_it != currentNetList.getOrderingP().begin() && eulerPathP_it->link != -1 && tmp->linkP.type != GAP) {
                if ((lastP_it->type == SOURCE && (
                                                  (eulerPathP_it->type == SOURCE && currentNetList.getTrans(lastP_it->link).drain != currentNetList.getTrans(eulerPathP_it->link).source) ||
                                                  (eulerPathP_it->type == DRAIN && currentNetList.getTrans(lastP_it->link).drain != currentNetList.getTrans(eulerPathP_it->link).drain))) ||
                    (lastP_it->type == DRAIN && (
                                                 (eulerPathP_it->type == SOURCE && currentNetList.getTrans(lastP_it->link).source != currentNetList.getTrans(eulerPathP_it->link).source) ||
                                                 (eulerPathP_it->type == DRAIN && currentNetList.getTrans(lastP_it->link).source != currentNetList.getTrans(eulerPathP_it->link).drain))) ||
                    eulerPathP_it->link == -1)
                    gapP = true;
            }
            if (eulerPathN_it != currentNetList.getOrderingN().begin() && eulerPathN_it->link != -1 && tmp->linkN.type != GAP) {
                if ((lastN_it->type == SOURCE && (
                                                  (eulerPathN_it->type == SOURCE && currentNetList.getTrans(lastN_it->link).drain != currentNetList.getTrans(eulerPathN_it->link).source) ||
                                                  (eulerPathN_it->type == DRAIN && currentNetList.getTrans(lastN_it->link).drain != currentNetList.getTrans(eulerPathN_it->link).drain))) ||
                    (lastN_it->type == DRAIN && (
                                                 (eulerPathN_it->type == SOURCE && currentNetList.getTrans(lastN_it->link).source != currentNetList.getTrans(eulerPathN_it->link).source) ||
                                                 (eulerPathN_it->type == DRAIN && currentNetList.getTrans(lastN_it->link).source != currentNetList.getTrans(eulerPathN_it->link).drain))) ||
                    eulerPathN_it->link == -1)
                    gapN = true;
            }
            
            // DIFF
            if (gapP || gapN || eulerPathP_it == currentNetList.getOrderingP().begin() || eulerPathN_it == currentNetList.getOrderingN().begin()) {
                lastElement = tmp;
                //serch for first track above the transistor
                nDiffTop=center;
                pDiffTop=center;
                while (trackPos[--nDiffTop] >= nDif_iniY - currentRules->getIntValue(currentNetList.getTrans(eulerPathN_it->link).width));
                while (trackPos[++pDiffTop] <= pDif_iniY + currentRules->getIntValue(currentNetList.getTrans(eulerPathP_it->link).width));
                tmp = createElement(5, diffNini[eulerPathN_it - currentNetList.getOrderingN().begin()], diffPini[eulerPathP_it - currentNetList.getOrderingP().begin()], nDiffTop, pDiffTop);
                
                //conecta sinais de entrada e saida com o nó inoutCnt do elemento
                for (inoutPins_it = inoutPins.begin(); inoutPins_it != inoutPins.end(); inoutPins_it++)
                    rt->addArc(tmp->inoutCnt, inoutPins_it->second, 0);
            }
            
            if (tmp->linkP.type == GAP && eulerPathP_it->link != -1) { // nao é GAP na difusao P
                tmp->linkP = *eulerPathP_it;
                if (gapP) tmp->gapP = true;
                if (eulerPathP_it->type == SOURCE) {
                    rt->addNodetoNet(currentNetList.getTrans(eulerPathP_it->link).source, tmp->diffP);
                    if (lastElement->linkP.type != GAP && rt->getNet(lastElement->diffP) == rt->getNet(tmp->diffP) && currentNetList.getNet(currentNetList.getTrans(eulerPathP_it->link).source).trans.size() >= 2) //VERIFICAR POSTERIORMENTE ==2?
                        rt->addArc(tmp->diffP, lastElement->diffP, 0); //tira a ponte entre a difusao atual e ela mesma 
                } else {
                    rt->addNodetoNet(currentNetList.getTrans(eulerPathP_it->link).drain, tmp->diffP);
                    if (lastElement->linkP.type != GAP && rt->getNet(lastElement->diffP) == rt->getNet(tmp->diffP) && currentNetList.getNet(currentNetList.getTrans(eulerPathP_it->link).drain).trans.size() >= 2) //VERIFICAR POSTERIORMENTE ==2?
                        rt->addArc(tmp->diffP, lastElement->diffP, 0); //tira a ponte entre a difusao atual e ela mesma 
                }
                int x = diffPini[eulerPathP_it - currentNetList.getOrderingP().begin()];
                do {
                    if (trackPos[x] >= pDif_iniY) rt->addArc(tmp->met[x], tmp->diffP, COST_CNT_INSIDE_DIFF);
                } while (trackPos[++x] <= pDif_iniY + currentRules->getIntValue(currentNetList.getTrans(eulerPathP_it->link).width));
            }
            
            if (tmp->linkN.type == GAP && eulerPathN_it->link != -1) { // nao é GAP na difusao N
                tmp->linkN = *eulerPathN_it;
                if (gapN) tmp->gapN = true;
                if (eulerPathN_it->type == SOURCE) {
                    rt->addNodetoNet(currentNetList.getTrans(eulerPathN_it->link).source, tmp->diffN);
                    if (lastElement->linkN.type != GAP && rt->getNet(lastElement->diffN) == rt->getNet(tmp->diffN) && currentNetList.getNet(currentNetList.getTrans(eulerPathN_it->link).source).trans.size() >= 2) //VERIFICAR POSTERIORMENTE ==2?
                        rt->addArc(tmp->diffN, lastElement->diffN, 0); //tira a ponte entre a difusao atual e ela mesma
                } else {
                    rt->addNodetoNet(currentNetList.getTrans(eulerPathN_it->link).drain, tmp->diffN);
                    if (lastElement->linkN.type != GAP && rt->getNet(lastElement->diffN) == rt->getNet(tmp->diffN) && currentNetList.getNet(currentNetList.getTrans(eulerPathN_it->link).drain).trans.size() >= 2) //VERIFICAR POSTERIORMENTE ==2?
                        rt->addArc(tmp->diffN, lastElement->diffN, 0); //tira a ponte entre a difusao atual e ela mesma 
                }
                int x = diffNini[eulerPathN_it - currentNetList.getOrderingN().begin()];
                do {
                    if (trackPos[x] <= nDif_iniY) rt->addArc(tmp->met[x], tmp->diffN, COST_CNT_INSIDE_DIFF);
                } while (trackPos[--x] >= nDif_iniY - currentRules->getIntValue(currentNetList.getTrans(eulerPathN_it->link).width));
            }
            
            //GATE
            //desenha gate do transistor se nao for GAP
            lastElement = tmp;
            nDiffTop=center;
            pDiffTop=center;    //CONSIDERAR W DO PROXIMO TRANSISTOR
            while (trackPos[--nDiffTop] >= nDif_iniY - currentRules->getIntValue(currentNetList.getTrans(eulerPathN_it->link).width));
            while (trackPos[++pDiffTop] <= pDif_iniY + currentRules->getIntValue(currentNetList.getTrans(eulerPathP_it->link).width));
            
            tmp = createElement(20, diffNini[eulerPathN_it - currentNetList.getOrderingN().begin()], diffPini[eulerPathP_it - currentNetList.getOrderingP().begin()], nDiffTop, pDiffTop);
            
            if (eulerPathP_it->link != -1) { // nao é GAP na difusao P
                tmp->linkP = *eulerPathP_it;
                tmp->linkP.type = GATE;
                for (int pos = tmp->diffPIni; pos<pDiffTop; pos++)
                    rt->addNodetoNet(currentNetList.getTrans(eulerPathP_it->link).gate, tmp->pol[pos]);
            } else tmp->linkP.type = GAP;
            
            for (inoutPins_it = inoutPins.begin(); inoutPins_it != inoutPins.end(); inoutPins_it++)
                rt->addArc(tmp->inoutCnt, inoutPins_it->second, 0);
            
            if (eulerPathN_it->link != -1) { // nao é GAP na difusao N
                tmp->linkN = *eulerPathN_it;
                tmp->linkN.type = GATE;
                for (int pos = tmp->diffNIni; pos>nDiffTop; pos--)
                    rt->addNodetoNet(currentNetList.getTrans(eulerPathN_it->link).gate, tmp->pol[pos]);
                
            } else tmp->linkN.type = GAP;
            
            // DIFF
            lastElement = tmp;
            nDiffTop=center;
            pDiffTop=center;   //CONSIDERAR W DO  TRANSISTOR ANTERIOR
            while (trackPos[--nDiffTop] >= nDif_iniY - currentRules->getIntValue(currentNetList.getTrans(eulerPathN_it->link).width));
            while (trackPos[++pDiffTop] <= pDif_iniY + currentRules->getIntValue(currentNetList.getTrans(eulerPathP_it->link).width));
            tmp = createElement(5, diffNini[eulerPathN_it - currentNetList.getOrderingN().begin()], diffPini[eulerPathP_it - currentNetList.getOrderingP().begin()], nDiffTop, pDiffTop);
            
            if (eulerPathP_it->link != -1) { // nao é GAP na difusao P
                tmp->linkP = *eulerPathP_it;
                if (eulerPathP_it->type == DRAIN) {
                    tmp->linkP.type = SOURCE;
                    rt->addNodetoNet(currentNetList.getTrans(eulerPathP_it->link).source, tmp->diffP);
                } else {
                    tmp->linkP.type = DRAIN;
                    rt->addNodetoNet(currentNetList.getTrans(eulerPathP_it->link).drain, tmp->diffP);
                }
                int x = diffPini[eulerPathP_it - currentNetList.getOrderingP().begin()];
                do {
                    if (trackPos[x] >= pDif_iniY) rt->addArc(tmp->met[x], tmp->diffP, COST_CNT_INSIDE_DIFF);
                    //cout << "aquip " << x << endl;
                } while (trackPos[++x] <= pDif_iniY + currentRules->getIntValue(currentNetList.getTrans(eulerPathP_it->link).width));
            } else
                tmp->linkP.type = GAP;
            
            
            if (eulerPathN_it->link != -1) { // nao é GAP na difusao N
                tmp->linkN = *eulerPathN_it;
                if (eulerPathN_it->type == SOURCE) {
                    tmp->linkN.type = DRAIN;
                    rt->addNodetoNet(currentNetList.getTrans(eulerPathN_it->link).drain, tmp->diffN);
                } else {
                    tmp->linkN.type = SOURCE;
                    rt->addNodetoNet(currentNetList.getTrans(eulerPathN_it->link).source, tmp->diffN);
                }
                int x = diffNini[eulerPathN_it - currentNetList.getOrderingN().begin()];
                do {
                    if (trackPos[x] <= nDif_iniY) rt->addArc(tmp->met[x], tmp->diffN, COST_CNT_INSIDE_DIFF);
                    //cout << "aquin " << x << endl;
                } while (trackPos[--x] >= nDif_iniY - currentRules->getIntValue(currentNetList.getTrans(eulerPathN_it->link).width));
            } else tmp->linkN.type = GAP;
            
            //conecta sinais de entrada e saida com o nó inoutCnt do elemento
            for (inoutPins_it = inoutPins.begin(); inoutPins_it != inoutPins.end(); inoutPins_it++)
                rt->addArc(tmp->inoutCnt, inoutPins_it->second, 0);
            
            lastP_it = eulerPathP_it++;
            lastN_it = eulerPathN_it++;
        }
        
        //cria elemento lateral para roteamento
        tmp = createElement(20, diffNini[eulerPathN_it - currentNetList.getOrderingN().begin()], diffPini[eulerPathP_it - currentNetList.getOrderingP().begin()], center, center); //MELHORAR!!!!!! max entre trans atual e próximo
        
        //conecta sinais de entrada e saida com o nó inoutCnt do elemento
        for (inoutPins_it = inoutPins.begin(); inoutPins_it != inoutPins.end(); inoutPins_it++)
            rt->addArc(tmp->inoutCnt, inoutPins_it->second, 0);
        
        state++;
        return true;
    } else
        return false;
}

bool AutoCell::route(int mCost, int pCost, int cCost, int ioCost) {
    cout << "-> Routing cell..." << endl;
    showIOCost();
    if (state < 4) return 0;
    state = 4;
    
    if (rt->routeNets(8000) && rt->optimize() && rt->optimize()) {
        rt->showResult();
        state = 5;
    } else {
        cout << "** Unable to route this circuit" << endl;
    }
    return state == 5;
}

bool AutoCell::compact(string lpSolverFile, int diffStretching, int griddedPolly, int ddCntsCost, int alignDiffConts) {
    cout << "-> Compacting layout..." << endl;
    if (state < 5) return 0;
    state = 5;
    
    this->diffStretching=diffStretching;
    this->griddedPolly=griddedPolly;
    this->ddCntsCost=ddCntsCost;
    this->alignDiffConts=alignDiffConts;
    
    currentRules = currentCircuit->getRules();
    
    vGrid = currentRules->getIntValue(currentCircuit->getVPitch());
    hGrid = currentRules->getIntValue(currentCircuit->getHPitch());
    height = currentCircuit->getRowHeight() * vGrid;
    
    currentLayout.clear();
    currentLayout.setName(currentCell->getName());
    
    map<string, int> IOgeometries;
    
    compaction cpt(CP_LP, "ILPmodel");
    vector<Box*> geometries;
    
    cpt.insertConstraint("ZERO", "UM", CP_EQ, 1);
    cpt.insertConstraint("ZERO", "RELAXATION", CP_EQ, relaxation);
    cpt.insertConstraint("ZERO", "HGRID_OFFSET", CP_EQ, (currentCircuit->getHGridOffset()?hGrid / 2:0));
    cpt.insertConstraint("ZERO", "VGRID_OFFSET", CP_EQ, (currentCircuit->getVGridOffset()?vGrid / 2:0));
    
    cpt.insertConstraint("ZERO", "height", CP_EQ, height);
    cpt.insertConstraint("ZERO", "yGNDb", CP_EQ, max(currentRules->getIntValue(currentCircuit->getSupplyVSize()), currentRules->getRule(W1M1)) / 2);
    cpt.insertConstraint("yVDDa", "height", CP_EQ, max(currentRules->getIntValue(currentCircuit->getSupplyVSize()), currentRules->getRule(W1M1)) / 2);
    cpt.insertConstraint("ZERO", "posNWell", CP_EQ, currentRules->getIntValue(currentCircuit->getnWellPos()));
    cpt.insertConstraint("yNDiffb", "posNWell", CP_MIN, currentRules->getRule(S1DNWN));
    cpt.insertConstraint("posNWell", "yPDiffa", CP_MIN, currentRules->getRule(E1WNDP));
    
    list<Element>::iterator lastElements_it;
    vector<string> currentMetNode(trackPos.size(), ""), currentPolNode(trackPos.size(), ""), currentContacts(trackPos.size(), "");
    vector<string> lastMetNode(trackPos.size(), ""), lastPolNode(trackPos.size(), "");
    vector<string> beforeLastMetNode(trackPos.size(), ""), beforeLastPolNode(trackPos.size(), "");
    int x;
    
    //  Transistor Variables
    bool gapP=false, gapN=false;
    string lastDiffP, lastDiffN, currentDiffP="", currentDiffN="", outPolP, outPolN, lastNGatePos, lastPGatePos;
    string lastNContact="", lastPContact="", lastNContactDiff="", lastPContactDiff="";
    
    //Compact the routing tracks
    for (list<Element>::iterator elements_it = elements.begin(); elements_it != elements.end(); ++elements_it) {
        //        elements_it->print();
        string lastPolNodeV = "", lastMetNodeV = "";
        lastMetNode[0]=""; lastMetNode[trackPos.size() - 1]="";
        //create metals and polys for intra-cell routing
        for (x = 0; x < trackPos.size(); x++) {
            //conecta as trilhas de metal horizontalmente
            //creates each metal node, but the disconected supply ones
            if (rt->getNet(elements_it->met[x]) != -1 && 
                !((x==0 && !rt->areConnected(elements_it->met[x], elements_it->met[x+1])&& !rt->areConnected(elements_it->met[x], elements_it->diffN)) ||
                  (x==trackPos.size()-1 && !rt->areConnected(elements_it->met[x], elements_it->met[x-1]) && !rt->areConnected(elements_it->met[x], elements_it->diffP)))){
                    createNode(geometries, cpt, elements_it, x, currentMetNode, currentNetList.getNetName(rt->getNet(elements_it->met[x])), MET1);                    
                    //insert space or a track between current and lasts H met node, if it exists
                    for (int c = 0; c < trackPos.size(); c++) {
                        if(lastMetNode[c]!=""){
                            if(c<x){
                                insertDistanceRuleInteligent(geometries, cpt, lastMetNode[c], currentMetNode[x], lastMetNode[c], currentMetNode[x], MET1);
                            }
                            else if(c>x)
                                insertDistanceRuleInteligent(geometries, cpt, lastMetNode[c], currentMetNode[x], currentMetNode[x], lastMetNode[c], MET1);
                            else if(c==x){
                                if(!rt->areConnected(elements_it->met[x], lastElements_it->met[x]))
                                    insertDistanceRuleDumb(geometries, cpt, lastMetNode[c], currentMetNode[x], currentRules->getRule(S1M1M1), H, MET1);
                                else
                                    createTrack(geometries, cpt, lastMetNode[x], currentMetNode[x], currentNetList.getNetName(rt->getNet(elements_it->met[x])), MET1, H);
                            }
                        }
                        if(c && beforeLastMetNode[c]!="")
                            insertDistanceRuleDumb(geometries, cpt, beforeLastMetNode[c], currentMetNode[x], currentRules->getRule(S3M1M1), H, MET1);
                    }
                    //insert space or a track between current and last V met node, if it exists
                    if (lastMetNodeV!=""){
                        if(!rt->areConnected(elements_it->met[x], elements_it->met[x-1]))
                            insertDistanceRuleDumb(geometries, cpt, lastMetNodeV, currentMetNode[x], currentRules->getRule(S1M1M1), V, MET1);
                        else{
                            createTrack(geometries, cpt, lastMetNodeV, currentMetNode[x], currentNetList.getNetName(rt->getNet(elements_it->met[x])), MET1, V);                        
                        }
                    }
                    
                    lastMetNodeV = currentMetNode[x];
                }
            //OBS: SEMPRE DEVE HAVER UMA INTERSECCAO DE UM QUADRADO COM LADO W2M1 ENTRE METAIS OU W2P1 ENTRE POLYS
            
            //conecta as trilhas de poly
            if (rt->getNet(elements_it->pol[x]) != -1) {
                //creates each poly node. creates only one for the gates
                if(x && rt->isSource(elements_it->pol[x]) && rt->isSource(elements_it->pol[x-1])){
                    currentPolNode[x]=currentPolNode[x-1];
                } else {
                    createNode(geometries, cpt, elements_it, x, currentPolNode, "",  POLY);
                    
                    //insert space or a track between current and last H poly node, if it exists
                    for (int c = 0; c < trackPos.size(); c++) {
                        if(lastPolNode[c]!="") {
                            if(c<x && rt->getNet(lastElements_it->pol[c])!=rt->getNet(elements_it->pol[x]))
                                insertDistanceRuleInteligent(geometries, cpt, lastPolNode[c], currentPolNode[x], lastPolNode[c], currentPolNode[x], POLY);
                            else if(c>x && rt->getNet(lastElements_it->pol[c])!=rt->getNet(elements_it->pol[x]))
                                insertDistanceRuleInteligent(geometries, cpt, lastPolNode[c], currentPolNode[x], currentPolNode[x], lastPolNode[c], POLY);
                            else if(c==x){
                                if(!rt->areConnected(elements_it->pol[x], lastElements_it->pol[x]))
                                    insertDistanceRuleDumb(geometries, cpt, lastPolNode[c], currentPolNode[x], currentRules->getRule(S1P1P1), H, POLY);
                                else
                                    createTrack(geometries, cpt, lastPolNode[x], currentPolNode[x], "", POLY, H);
                            }
                        }
                        if(c && beforeLastPolNode[c]!="")
                            insertDistanceRuleDumb(geometries, cpt, beforeLastPolNode[c], currentPolNode[x], currentRules->getRule(S3P1P1), H, POLY);
                    }
                    
                    //insert space or a track between current and last V poly node, if it exists
                    if (lastPolNodeV!=""){
                        if(!rt->areConnected(elements_it->pol[x], elements_it->pol[x-1]))
                            insertDistanceRuleDumb(geometries, cpt, lastPolNodeV, currentPolNode[x], currentRules->getRule(S1P1P1), V, POLY);
                        else{
                            createTrack(geometries, cpt, lastPolNodeV, currentPolNode[x], "", POLY, V);                        
                        }
                    }
                    
                    lastPolNodeV = currentPolNode[x];
                }
            }
            
            //conecta os polys com os metais das trilhas
            if (rt->areConnected(elements_it->met[x], elements_it->pol[x])) {
                currentContacts[x] = insertCnt(geometries, cpt, elements_it, currentMetNode, x, false);
                insertCntPol(geometries, cpt, currentContacts[x], currentPolNode, x);
            }
            
            //se for entrada/saida, alinha o metal1 com a grade
            if (rt->areConnected(elements_it->met[x], elements_it->inoutCnt)) {
                IOgeometries[currentNetList.getNetName(rt->getNet(elements_it->inoutCnt))] = strToInt(insertVia(geometries, cpt, currentMetNode[x]));
                ;
                //				if(lastIO!="") cpt.insertConstraint("x" + lastIO + "b", "x" + tmp + "a", CP_MIN, 0);
                //				lastIO=tmp;
            }
            
        }
        if (elements_it->gapP) gapP = true;
        if (elements_it->gapN) gapN = true;
        
        switch (elements_it->linkN.type) {
            case GAP:
                gapN = true;
                break;
            case SOURCE:
            case DRAIN:
                for (x = center; x >= 0; x--) { // desenha contatos na difusao
                    if (rt->areConnected(elements_it->met[x], elements_it->diffN)) {
                        string currentCnt = insertCnt(geometries, cpt, elements_it, currentMetNode, x,true);
                        list<Element>::iterator next = elements_it; next++;
                        string currentDiff = insertCntDif(geometries, cpt, x, currentCnt, lastNGatePos, lastNContact, currentDiffN, currentPolNode, lastPolNode, NDIF, next->gapN==true || next->linkN.type==GAP);                        
                        if(gapN && lastNContactDiff!="") 
                            cpt.insertConstraint("x" + lastNContactDiff + "b", "x" + currentDiff + "a", CP_MIN, currentRules->getRule(S1DFDF));
                        lastNContactDiff = currentDiff;
                        break;
                    }
                }
                gapN = false;
                break;
                
            case GATE:
                lastNGatePos = insertGate(geometries, cpt,elements_it->linkN.link, elements_it, currentPolNode, lastPolNode, lastNContact, lastNContactDiff, lastNGatePos, lastDiffN, currentDiffN, NDIF);
                break;
        }
        switch (elements_it->linkP.type) {
            case GAP:
                gapP = true;
                break;
            case SOURCE:
            case DRAIN:
                for (x = center; x < elements_it->met.size(); ++x) { // desenha contatos na difusao
                    if (rt->areConnected(elements_it->met[x], elements_it->diffP)) {
                        string currentCnt = insertCnt(geometries, cpt, elements_it, currentMetNode, x,true);
                        list<Element>::iterator next = elements_it; next++;
                        string currentDiff = insertCntDif(geometries, cpt,  x, currentCnt, lastPGatePos, lastPContact, currentDiffP, currentPolNode, lastPolNode, PDIF, next->gapP==true || next->linkP.type==GAP);
                        if(gapP && lastPContactDiff!="") 
                            cpt.insertConstraint("x" + lastPContactDiff + "b", "x" + currentDiff + "a", CP_MIN, currentRules->getRule(S1DFDF));
                        lastPContactDiff = currentDiff;
                        break;
                    }
                }
                gapP = false;
                break;
                
            case GATE:
                lastPGatePos = insertGate(geometries, cpt, elements_it->linkP.link, elements_it, currentPolNode, lastPolNode, lastPContact, lastPContactDiff, lastPGatePos, lastDiffP, currentDiffP, PDIF);                
                break;
        }
        
        //insert contacts to diff space rule
        for (int c = 0; c < trackPos.size(); c++) {
            if(currentContacts[c]!=""){
                if(elements_it->linkN.type!=GAP)
                    if(c<elements_it->diffNEnd)
                        insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], currentDiffN, currentContacts[c], currentDiffN, currentRules->getRule(S1CTDF),"");
                    else if(c>elements_it->diffNIni)
                        insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], currentDiffN, currentDiffN, currentContacts[c], currentRules->getRule(S1CTDF),"");
                if(elements_it->linkP.type!=GAP)
                    if(c<elements_it->diffPIni)
                        insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], currentDiffP, currentContacts[c], currentDiffP, currentRules->getRule(S1CTDF),"");
                    else if(c>elements_it->diffPEnd)
                        insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], currentDiffP, currentDiffP, currentContacts[c], currentRules->getRule(S1CTDF),"");
            }
        }        
        
        
        lastDiffN=currentDiffN;
        lastDiffP=currentDiffP;
        lastElements_it = elements_it;
        for (x = 0; x < trackPos.size(); x++){
            if(currentMetNode[x]!=lastMetNode[x]) beforeLastMetNode[x]=lastMetNode[x];
            if(currentPolNode[x]!=lastPolNode[x]) beforeLastPolNode[x]=lastPolNode[x];
        }
        lastMetNode = currentMetNode;
        lastPolNode = currentPolNode;
    }
    
    
    
    cpt.insertConstraint("ZERO", "width", CP_MIN, 0);
    cpt.insertConstraint("ZERO", "width", CP_EQ_VAR_VAL, "width_gpos", hGrid);
    cpt.forceIntegerVar("width_gpos");
    cpt.insertLPMinVar("width", 5000);
    
    if (!cpt.solve(lpSolverFile)) {
        cout << "** Unable to compact" << endl;
        return false;
    }
    
    for (int i = 0; i < geometries.size(); i++) {
        
        int xa = cpt.getVariableVal("x" + intToStr(i) + "a");
        int xb = cpt.getVariableVal("x" + intToStr(i) + "b");
        int ya = cpt.getVariableVal("y" + intToStr(i) + "a");
        int yb = cpt.getVariableVal("y" + intToStr(i) + "b");
        
        if (xa != -1 && xb != -1) {
            geometries[i]->setWidth(xb - xa);
            geometries[i]->setX((xb + xa) / 2);
        }
        if (ya != -1 && yb != -1) {
            geometries[i]->setHeight(yb - ya);
            geometries[i]->setY((yb + ya) / 2);
        }
        
    }
    int width = cpt.getVariableVal("width");
    
    //Create M1P Layers
    list <Box>::iterator net_it;
    for ( net_it = currentLayout.layers[MET1].begin(); net_it != currentLayout.layers[MET1].end(); net_it++ )
        if(currentNetList.isIO(net_it->getNet())) currentLayout.addEnc(*net_it,  0 , MET1P); 
    
    for (map<string,int>::iterator IOgeometries_it=IOgeometries.begin(); IOgeometries_it != IOgeometries.end(); IOgeometries_it++ ) {
        Pin p;
        p.setX(geometries[IOgeometries_it->second]->getX());
        p.setY(geometries[IOgeometries_it->second]->getY());
        p.setLayer(MET1P);
        currentLayout.setPin(IOgeometries_it->first,p);
        currentLayout.addLabel(IOgeometries_it->first,p);
    }
    
    list <Box>::iterator layer_it;
    //Insert TAPs
    if(currentCircuit->getCellTemplate()!="Tapless"){
        bool btP=false, btN=false;
        int minDiffDist=0, cntPos=0;
        list<int> btIntervalsP,btIntervalsN;
        if(currentCircuit->getCellTemplate()=="Taps close to the boundary"){
            btIntervalsP.push_back(0); //IMPROVE
            btIntervalsP.push_back(width);
            cntPos= currentRules->getRule(S1CTCT)/2;
            minDiffDist= cntPos+currentRules->getRule(W2CT)+currentRules->getRule(E2DFCT)+currentRules->getRule(S1DFDF);
        }else if(currentCircuit->getCellTemplate()=="Taps w/ continuous diff"){ 
            cntPos= currentRules->getRule(W2DF)/2+currentRules->getRule(E2DFCT);
            minDiffDist= cntPos+currentRules->getRule(W2CT)+currentRules->getRule(E2DFCT)+currentRules->getRule(S1DFDF);
            currentLayout.addPolygon(0, -currentRules->getRule(W2DF)/2, width, currentRules->getRule(W2DF)/2, PDIF);            
            currentLayout.addPolygon(0, -currentRules->getRule(W2DF)/2-currentRules->getRule(E1IPDF), width, currentRules->getRule(W2DF)/2+currentRules->getRule(E1IPDF), PSEL);            
            currentLayout.addPolygon(0, height+currentRules->getRule(W2DF)/2, width, height-currentRules->getRule(W2DF)/2, NDIF);            
            currentLayout.addPolygon(0, height+currentRules->getRule(W2DF)/2+currentRules->getRule(E1INDF), width, height-currentRules->getRule(W2DF)/2-currentRules->getRule(E1IPDF), NSEL);            
        }
        btIntervalsN=btIntervalsP;
        for ( layer_it = currentLayout.layers[NDIF].begin(); layer_it != currentLayout.layers[NDIF].end(); layer_it++ ){
            if(layer_it->getY1() < minDiffDist){
                for(list<int>::iterator btIntervals_it=btIntervalsN.begin(); btIntervals_it!=btIntervalsN.end(); ++btIntervals_it){
                    list<int>::iterator ini, end;
                    ini=btIntervals_it;
                    end=++btIntervals_it;
                    if(layer_it->getX1()>*ini && layer_it->getX2()<*end){
                        btIntervalsN.insert(end, layer_it->getX1());
                        btIntervalsN.insert(end, layer_it->getX2());
                    }
                    else if(layer_it->getX1()<*ini && layer_it->getX2()>*end){
                        btIntervalsN.erase(ini);
                        btIntervalsN.erase(end);
                    }				
                    else if(layer_it->getX1()<*ini && layer_it->getX2()>*ini) 
                        *ini=layer_it->getX2();
                    else if(layer_it->getX1()<*end && layer_it->getX2()>*end) 
                        *end=layer_it->getX1();
                }
            }
        }
        for(list<int>::iterator btIntervals_it=btIntervalsN.begin(); btIntervals_it!=btIntervalsN.end(); ++btIntervals_it){
            list<int>::iterator ini, end;
            ini=btIntervals_it;
            end=++btIntervals_it;
            if(*ini+currentRules->getRule(W2CT)+2*currentRules->getRule(E2DFCT)+2*currentRules->getRule(S1DFDF)<=*end){
                btP=true;
                currentLayout.addPolygon(*ini+currentRules->getRule(S1DFDF)-currentRules->getRule(E1IPDF), 0, *end-currentRules->getRule(S1DFDF)+currentRules->getRule(E1IPDF), currentRules->getRule(S1CTCT)/2+currentRules->getRule(W2CT)+currentRules->getRule(E2DFCT)+currentRules->getRule(E1IPDF), PSEL);
                currentLayout.addPolygon(*ini+currentRules->getRule(S1DFDF), 0, *end-currentRules->getRule(S1DFDF), currentRules->getRule(S1CTCT)/2+currentRules->getRule(W2CT)+currentRules->getRule(E2DFCT), PDIF);
                for(int cnt=*ini+currentRules->getRule(S1DFDF)+currentRules->getRule(E2DFCT); cnt<=*end-currentRules->getRule(S1DFDF)-currentRules->getRule(E2DFCT)-currentRules->getRule(W2CT); cnt+=currentRules->getRule(W2CT)+currentRules->getRule(S1CTCT))
                    currentLayout.addPolygon(cnt, cntPos, cnt+currentRules->getRule(W2CT), cntPos+currentRules->getRule(W2CT), CONT);				
            }
        }
        
        for ( layer_it = currentLayout.layers[PDIF].begin(); layer_it != currentLayout.layers[PDIF].end(); layer_it++ ){
            if(layer_it->getY2() > height-minDiffDist){
                for(list<int>::iterator btIntervals_it=btIntervalsP.begin(); btIntervals_it!=btIntervalsP.end(); ++btIntervals_it){
                    list<int>::iterator ini, end;
                    ini=btIntervals_it;
                    end=++btIntervals_it;
                    if(layer_it->getX1()>*ini && layer_it->getX2()<*end){
                        btIntervalsP.insert(end, layer_it->getX1());
                        btIntervalsP.insert(end, layer_it->getX2());
                    }
                    else if(layer_it->getX1()<*ini && layer_it->getX2()>*end){
                        btIntervalsP.erase(ini);
                        btIntervalsP.erase(end);
                    }				
                    else if(layer_it->getX1()<*ini && layer_it->getX2()>*ini) 
                        *ini=layer_it->getX2();
                    else if(layer_it->getX1()<*end && layer_it->getX2()>*end) 
                        *end=layer_it->getX1();
                }
            }
        }
        for(list<int>::iterator btIntervals_it=btIntervalsP.begin(); btIntervals_it!=btIntervalsP.end(); ++btIntervals_it){
            list<int>::iterator ini, end;
            ini=btIntervals_it;
            end=++btIntervals_it;
            if(*ini+currentRules->getRule(W2CT)+2*currentRules->getRule(E2DFCT)+2*currentRules->getRule(S1DFDF)<=*end){
                btN=true;
                currentLayout.addPolygon(*ini+currentRules->getRule(S1DFDF)-currentRules->getRule(E1INDF), height-(currentRules->getRule(S1CTCT)/2+currentRules->getRule(W2CT)+currentRules->getRule(E2DFCT)+currentRules->getRule(E1INDF)), *end-currentRules->getRule(S1DFDF)+currentRules->getRule(E1INDF), height, NSEL);
                currentLayout.addPolygon(*ini+currentRules->getRule(S1DFDF), height-(currentRules->getRule(S1CTCT)/2+currentRules->getRule(W2CT)+currentRules->getRule(E2DFCT)), *end-currentRules->getRule(S1DFDF), height, NDIF);
                for(int cnt=*ini+currentRules->getRule(S1DFDF)+currentRules->getRule(E2DFCT); cnt<=*end-currentRules->getRule(S1DFDF)-currentRules->getRule(E2DFCT)-currentRules->getRule(W2CT); cnt+=currentRules->getRule(W2CT)+currentRules->getRule(S1CTCT))
                    currentLayout.addPolygon(cnt, height-(cntPos+currentRules->getRule(W2CT)), cnt+currentRules->getRule(W2CT), height-cntPos, CONT);				
            }
        }
        
        if(!btP) cout << "** WARNING: Could not insert bodye ties to the P transistors" << endl;
        if(!btN) cout << "** WARNING: Could not insert bodye ties to the N transistors" << endl;
    }
    currentLayout.setWidth(width);
    currentLayout.setHeight(height);
    
    //Draw sel Borders
    int selBorder=currentRules->getIntValue(currentCircuit->getpnSelBorder());
    currentLayout.addPolygon(-selBorder, -selBorder, width+selBorder, cpt.getVariableVal("posNWell"), NSEL);
    currentLayout.addPolygon(-selBorder, height+selBorder, width+selBorder, cpt.getVariableVal("posNWell"), PSEL);
    
    currentLayout.addPolygon(0, 0, width, supWidth, MET1).setNet(currentCircuit->getGndNet());
    currentLayout.addPolygon(0, 0, width, supWidth, MET1P);
    currentLayout.addPolygon(0, height - supWidth, width, height, MET1).setNet(currentCircuit->getVddNet());
    currentLayout.addPolygon(0, height - supWidth, width, height, MET1P);
    currentLayout.addPolygon(0, 0, width, height, CELLBOX);
    int nWellBorder=currentRules->getIntValue(currentCircuit->getnWellBorder());
    currentLayout.addPolygon(-nWellBorder, height + nWellBorder, width + nWellBorder, cpt.getVariableVal("posNWell"), NWEL);
    
    
    //	currentLayout.merge();
    currentCircuit->insertLayout(currentLayout);
    cout << "** Cell Size (W x H): " << float(currentLayout.getWidth()) / currentRules->getScale() << " x " << float(currentLayout.getHeight()) / currentRules->getScale() << endl;
    state = 6;
    return state == 6;
}


string AutoCell::insertGate(vector<Box*> &geometries, compaction &cpt, int transistor, list<Element>::iterator elements_it, vector<string> &currentPolTrack, vector<string> &lastPolTrack, string lastCnt, string lastDiffCnt, string &lastGate, string lastDiff, string currentDiff, layer_name l){
    int gateIni, gateEnd;
    int transWidth = round(currentNetList.getTrans(transistor).width * currentRules->getScale());
    int transLength = round(currentNetList.getTrans(transistor).length * currentRules->getScale());
    if (transLength < currentRules->getRule(W2P1)) cout << "** Gate length of transistor " << currentNetList.getTrans(transistor).name << " is smaller than the minimum of the technology" << endl;
    
    //get gate
    if (l==NDIF){
        gateIni = elements_it->diffNIni;
        gateEnd = elements_it->diffNEnd;
    }else{
        gateIni = elements_it->diffPIni;
        gateEnd = elements_it->diffPEnd;
    }
    cpt.insertConstraint("x" + currentPolTrack[gateIni] + "a", "x" + currentPolTrack[gateIni] + "b", CP_EQ, transLength);
    cpt.insertConstraint("x" + currentDiff + "a", "x" + currentPolTrack[gateIni] + "a", CP_MIN, currentRules->getRule(E1DFP1));
    cpt.insertConstraint("x" + currentPolTrack[gateIni] + "b", "x" + currentDiff + "b", CP_MIN, currentRules->getRule(E1DFP1));
    cpt.insertConstraint("ZERO", "b" + currentDiff + "_smallTransWidth", CP_EQ, (transWidth<currentRules->getRule(S3DFP1)?1:0));    
    cpt.insertConstraint("y" + currentDiff + "a", "y" + currentDiff + "b", CP_EQ, transWidth);
    cpt.insertConstraint("ZERO", "y" + currentPolTrack[gateIni] + "a", CP_MIN, currentRules->getRule(S1P1P1) / 2);
    cpt.insertConstraint("y" + currentPolTrack[gateIni] + "b", "height", CP_MIN, currentRules->getRule(S1P1P1) / 2);        
    
    if (lastGate != ""){
        cpt.insertConstraint("x" + lastGate + "b", "x" + currentPolTrack[gateIni] + "a", CP_MIN, currentRules->getRule(S2P1P1));
    }
    
    //poly distance to active region
    if(l==NDIF){
        if(gateIni+1 < currentPolTrack.size() && currentPolTrack[gateIni+1]!="")
            insertDistanceRuleInteligent2(geometries, cpt, currentPolTrack[gateIni+1], currentPolTrack[gateIni], currentDiff, currentPolTrack[gateIni+1], currentRules->getRule(S2DFP1),"b" + currentDiff + "_smallTransWidth");
        if(gateEnd && currentPolTrack[gateEnd-1]!="")
            insertDistanceRuleInteligent2(geometries, cpt, currentPolTrack[gateEnd-1], currentPolTrack[gateEnd], currentPolTrack[gateEnd-1], currentDiff, currentRules->getRule(S2DFP1),"b" + currentDiff + "_smallTransWidth");
        cpt.insertConstraint("y" + currentDiff + "b", "yNDiffb", CP_MIN, 0);
        
    }else{
        if(gateIni && currentPolTrack[gateIni-1]!="")
            insertDistanceRuleInteligent2(geometries, cpt, currentPolTrack[gateIni-1], currentPolTrack[gateIni], currentPolTrack[gateIni-1], currentDiff, currentRules->getRule(S2DFP1),"b" + currentDiff + "_smallTransWidth");
        if(gateEnd+1 < currentPolTrack.size() && currentPolTrack[gateEnd+1]!="")
            insertDistanceRuleInteligent2(geometries, cpt, currentPolTrack[gateEnd+1], currentPolTrack[gateEnd], currentDiff, currentPolTrack[gateEnd+1], currentRules->getRule(S2DFP1),"b" + currentDiff + "_smallTransWidth");
        cpt.insertConstraint("yPDiffa", "y" + currentDiff + "a", CP_MIN, 0);
    }
    
    int c;
    
    // space diff from the tracks below
    for (c=gateIni; c >= 0; --c) 
        if(currentPolTrack[gateIni]!=currentPolTrack[c]){
            if(currentPolTrack[c]!=""){
                insertDistanceRuleInteligent2(geometries, cpt, currentPolTrack[c], currentDiff, currentPolTrack[c], currentDiff, currentRules->getRule(S1DFP1),"");
            }
            if(lastPolTrack[c]!=""){
                insertDistanceRuleInteligent2(geometries, cpt, lastPolTrack[c], currentDiff, lastPolTrack[c], currentDiff, currentRules->getRule(S1DFP1),"");
            }
        }
    // space diff from the tracks above
    for (c=gateIni; c < currentPolTrack.size(); ++c) 
        if(currentPolTrack[gateIni]!=currentPolTrack[c]){
            if(currentPolTrack[c]!=""){
                insertDistanceRuleInteligent2(geometries, cpt, currentPolTrack[c], currentDiff, currentDiff, currentPolTrack[c], currentRules->getRule(S1DFP1),"");
            }
            if(lastPolTrack[c]!=""){
                insertDistanceRuleInteligent2(geometries, cpt, lastPolTrack[c], currentDiff, currentDiff, lastPolTrack[c], currentRules->getRule(S1DFP1),"");
            }
        }
    
    //gate extension rule for L shape transistor if diff dist to gate < E3P1DF
    cpt.forceBinaryVar("b" + lastDiffCnt + "_LshapeBeforeGate");
    cpt.forceBinaryVar("b" + currentPolTrack[gateIni] + "_applyExtraExtBeforeGate");
    cpt.insertConstraint("b" + lastDiffCnt + "_applyS2BeforeGate", "b" + lastDiffCnt + "_LshapeBeforeGate", CP_MAX, "b" + currentPolTrack[gateIni] + "_applyExtraExtBeforeGate");
    cpt.insertConstraint("y" + currentPolTrack[gateIni] + "a", "y" + currentDiff + "a", CP_MIN, currentRules->getRule(E1P1DF));
    cpt.insertConstraint("y" + currentPolTrack[gateIni] + "a", "y" + currentDiff + "a", CP_MIN, "b" + currentPolTrack[gateIni] + "_applyExtraExtBeforeGate", currentRules->getRule(E2P1DF));
    cpt.insertConstraint("y" + currentDiff + "b", "y" + currentPolTrack[gateIni] + "b", CP_MIN, currentRules->getRule(E1P1DF));
    cpt.insertConstraint("y" + currentDiff + "b", "y" + currentPolTrack[gateIni] + "b", CP_MIN, "b" + currentPolTrack[gateIni] + "_applyExtraExtBeforeGate", currentRules->getRule(E2P1DF));
    
    //minimize gate size
    cpt.insertConstraint("y" + currentPolTrack[gateIni] + "a", "y" + currentPolTrack[gateIni] + "b", CP_EQ, "y" + currentPolTrack[gateIni] + "min");
    cpt.insertLPMinVar("y" + currentPolTrack[gateIni] + "min");
    
    //insert conditional diff to gate rule in L shape transistors considering S3DFP1 (big transistor width)
    cpt.insertConstraint("x" + lastDiffCnt + "b", "x" + currentPolTrack[gateIni] + "a", CP_MIN, "b" + lastDiffCnt + "_LshapeBeforeGate", currentRules->getRule(S1DFP1));
    cpt.insertConstraint("ZERO", "y" + lastDiffCnt + "LdistBeforeGateOut", CP_MAX, "b" + lastDiffCnt + "_LshapeBeforeGate", 100000);             
    cpt.insertConstraint("ZERO", "y" + lastDiffCnt + "LdistBeforeGateIn", CP_MAX, "b" + lastDiffCnt + "_LshapeBeforeGate", 100000);             
    cpt.insertConstraint("UM", "b" + currentDiff + "_smallTransWidth" + " + " + "b" + lastDiffCnt + "_LshapeBeforeGate", CP_MAX, "b" + lastDiffCnt + "_applyS2BeforeGate");
    cpt.insertConstraint("x" + lastDiffCnt + "b", "x" + currentPolTrack[gateIni] + "a", CP_MIN, "b" + lastDiffCnt + "_applyS2BeforeGate", currentRules->getRule(S2DFP1));
    
    //contact to gate distance
    cpt.insertConstraint("x" + lastCnt + "b", "x" + currentPolTrack[gateIni] + "a", CP_MIN, currentRules->getRule(S1CTP1));
    
    /*                if (rt->areConnected(elements_it->diffN, elements_it->pol[trackPos.size() - 1])) {
     if (rt->getNrFinalArcs(elements_it->pol[trackPos.size() - 1]) > 1) {
     geometries.push_back(&currentLayout.addPolygon(0, nDif_iniY, 0, trackPos[trackPos.size() - 1], POLY));
     string ponte = intToStr(geometries.size() - 1);
     
     cpt.insertConstraint("x" + ponte + "a", "x" + ponte + "b", CP_EQ, currentRules->getRule(W2P1));
     cpt.insertConstraint("x" + currentPolTrack[gateIni] + "a", "x" + ponte + "a", CP_MIN, 0);
     cpt.insertConstraint("x" + ponte + "b", "x" + currentPolTrack[gateIni] + "b", CP_MIN, 0);
     cpt.insertConstraint("x" + currentPolTrack[trackPos.size() - 1] + "a", "x" + ponte + "a", CP_MIN, 0);
     cpt.insertConstraint("x" + ponte + "b", "x" + currentPolTrack[trackPos.size() - 1] + "b", CP_MIN, 0);
     }
     }
     */
    lastGate = currentPolTrack[gateIni];
    return currentPolTrack[gateIni];
}

string AutoCell::insertCntDif(vector<Box*> &geometries, compaction &cpt, int pos, string currentCnt, string &lastGate, string &lastCnt, string &lastDiff, vector<string> &currentPolTrack, vector<string> &lastPolTrack,  layer_name l, bool endDiff) {
    geometries.push_back(&currentLayout.addEnc(*geometries.back(), 0, l));
    string diffEnc = intToStr(geometries.size() - 1);
    
    //diffusion enclosure of contact - 3 sides E2DFCT, one side E1DFCT  (comentar que ele coloca o lado menor em cima ou embaixo tbm)
    cpt.forceBinaryVar("b" + diffEnc + "_l");
    cpt.forceBinaryVar("b" + diffEnc + "_r");
    cpt.forceBinaryVar("b" + diffEnc + "_b");
    cpt.forceBinaryVar("b" + diffEnc + "_t");
    cpt.insertConstraint("ZERO", "b" + diffEnc + "_l + b" + diffEnc + "_r + b" + diffEnc + "_b + b" + diffEnc + "_t", CP_EQ, 3);
    cpt.insertConstraint("x" + diffEnc + "a", "x" + currentCnt + "a", CP_MIN, currentRules->getRule(E1DFCT));
    cpt.insertConstraint("x" + diffEnc + "a", "x" + currentCnt + "a", CP_MIN, "b" + diffEnc + "_l", currentRules->getRule(E2DFCT));
    cpt.insertConstraint("x" + currentCnt + "b", "x" + diffEnc + "b", CP_MIN, currentRules->getRule(E1DFCT));
    cpt.insertConstraint("x" + currentCnt + "b", "x" + diffEnc + "b", CP_MIN, "b" + diffEnc + "_r", currentRules->getRule(E2DFCT));
    cpt.insertConstraint("y" + diffEnc + "a", "y" + currentCnt + "a", CP_MIN, currentRules->getRule(E1DFCT));
    cpt.insertConstraint("y" + diffEnc + "a", "y" + currentCnt + "a", CP_MIN, "b" + diffEnc + "_b", currentRules->getRule(E2DFCT));
    cpt.insertConstraint("y" + currentCnt + "b", "y" + diffEnc + "b", CP_MIN, currentRules->getRule(E1DFCT));
    cpt.insertConstraint("y" + currentCnt + "b", "y" + diffEnc + "b", CP_MIN, "b" + diffEnc + "_t", currentRules->getRule(E2DFCT));
    
    //if there is a gate before
    if (lastGate != "") {
        //minimize contact to contact distance
        cpt.insertConstraint("x" + lastCnt + "b", "x" + currentCnt + "a", CP_EQ, "x" + currentCnt + "min");
        cpt.insertLPMinVar("x" + currentCnt + "min",6);
        cpt.insertConstraint("y" + currentCnt + "_boundbx_a", "y" + lastCnt + "a", CP_MIN, 0);
        cpt.insertConstraint("y" + currentCnt + "_boundbx_a", "y" + currentCnt + "a", CP_MIN, 0);
        cpt.insertConstraint("y" + lastCnt + "b", "y" + currentCnt + "_boundbx_b", CP_MIN, 0);
        cpt.insertConstraint("y" + currentCnt + "b", "y" + currentCnt + "_boundbx_b", CP_MIN, 0);
        cpt.insertConstraint("y" + currentCnt + "_boundbx_a", "y" + currentCnt + "_boundbx_b", CP_EQ, "y" + currentCnt + "_boundbx_min");
        cpt.insertLPMinVar("y" + currentCnt + "_boundbx_min",(alignDiffConts?2:0));
        
        cpt.insertConstraint("x" + lastGate + "b", "x" + currentCnt + "a", CP_MIN, currentRules->getRule(S1CTP1));
        cpt.insertConstraint("x" + diffEnc + "b", "width", CP_MIN, currentRules->getRule(S1DFDF) / 2);
        cpt.insertConstraint("x" + diffEnc + "a", "x" + lastDiff + "b", CP_MIN, 0);
        cpt.insertConstraint("x" + lastDiff + "b", "x" + diffEnc + "b", CP_EQ, "x" + lastDiff + "minR");
        cpt.insertLPMinVar("x" + lastDiff + "minR", 5);
        
        //gate extension rule for L shape transistor if diff dist to gate < E3P1DF
        cpt.forceBinaryVar("b" + diffEnc + "_LshapeAfterGate");
        cpt.insertConstraint("b" + diffEnc + "_applyS2AfterGate", "b" + diffEnc + "_LshapeAfterGate", CP_MAX, "b" + lastGate + "_applyExtraExtAfterGate");
        cpt.insertConstraint("y" + lastGate + "a", "y" + lastDiff + "a", CP_MIN, currentRules->getRule(E1P1DF));
        cpt.insertConstraint("y" + lastGate + "a", "y" + lastDiff + "a", CP_MIN, "b" + lastGate + "_applyExtraExtAfterGate", currentRules->getRule(E2P1DF));
        cpt.insertConstraint("y" + lastDiff + "b", "y" + lastGate + "b", CP_MIN, currentRules->getRule(E1P1DF));
        cpt.insertConstraint("y" + lastDiff + "b", "y" + lastGate + "b", CP_MIN, "b" + lastGate + "_applyExtraExtAfterGate", currentRules->getRule(E2P1DF));
        
        //insert conditional diff to gate rule in L shape transistors considering S3DFP1 (big transistor width)
        cpt.insertConstraint("x" + lastGate + "b", "x" + diffEnc + "a", CP_MIN, "b" + diffEnc + "_LshapeAfterGate", currentRules->getRule(S1DFP1));
        cpt.insertConstraint("ZERO", "y" + diffEnc + "LdistAfterGateOut", CP_MAX, "b" + diffEnc + "_LshapeAfterGate", 100000);             
        cpt.insertConstraint("ZERO", "y" + diffEnc + "LdistAfterGateIn", CP_MAX, "b" + diffEnc + "_LshapeAfterGate", 100000);             
        cpt.insertConstraint("UM", "b" + lastDiff + "_smallTransWidth" + " + " + "b" + diffEnc + "_LshapeAfterGate", CP_MAX, "b" + diffEnc + "_applyS2AfterGate");
        cpt.insertConstraint("x" + lastGate + "b", "x" + diffEnc + "a", CP_MIN, "b" + diffEnc + "_applyS2AfterGate", currentRules->getRule(S2DFP1));
        
        //diffusion extension rules for last gate diffusion
        if (l==NDIF) {
            cpt.insertConstraint("y" + diffEnc + "b", "y" + lastDiff + "b", CP_MIN, 0);
            cpt.insertConstraint("y" + diffEnc + "LdistAfterGateOut", "y" + lastDiff + "a", CP_MAX, "y" + diffEnc + "a");            
            cpt.insertConstraint("y" + diffEnc + "LdistAfterGateIn", "y" + diffEnc + "b", CP_MAX, "y" + lastDiff + "b");            
        }else{
            cpt.insertConstraint("y" + lastDiff + "a", "y" + diffEnc + "a", CP_MIN, 0);
            cpt.insertConstraint("y" + diffEnc + "LdistAfterGateOut", "y" + diffEnc + "b", CP_MAX, "y" + lastDiff + "b");            
            cpt.insertConstraint("y" + diffEnc + "LdistAfterGateIn", "y" + lastDiff + "a", CP_MAX, "y" + diffEnc + "a");            
        }
        cpt.insertLPMinVar("y" + diffEnc + "LdistAfterGateOut",(diffStretching?5:40));
        cpt.insertLPMinVar("y" + diffEnc + "LdistAfterGateIn",(diffStretching?5:40));
    }
    
    //if there is not a gap after
    if(!endDiff){
        //create next diffusion
        geometries.push_back(&currentLayout.addPolygon(0, 0, 0, 0, l));
        string currentDiff = intToStr(geometries.size() - 1);
        
        cpt.insertConstraint("x" + currentDiff + "a", "x" + currentDiff + "b", CP_EQ, "x" + currentDiff + "min");
        cpt.insertLPMinVar("x" + currentDiff + "min",4);
        cpt.insertConstraint("ZERO", "x" + diffEnc + "a", CP_MIN, currentRules->getRule(S1DFDF) / 2);
        cpt.insertConstraint("x" + currentDiff + "a", "x" + diffEnc + "b", CP_MIN, 0);
        cpt.insertConstraint("x" + diffEnc + "a", "x" + currentDiff + "a", CP_EQ, "x" + currentDiff + "minL");
        cpt.insertLPMinVar("x" + currentDiff + "minL", 5);
        
        //merge new diffusion to the last one and maximize intersection
        if (lastGate != "") {
            cpt.insertConstraint("x" + currentDiff + "a", "x" + lastDiff + "b", CP_MIN, 0);
            cpt.insertConstraint("y" + currentDiff + "b_int", "y" + lastDiff + "b", CP_MIN, 0);
            cpt.insertConstraint("y" + currentDiff + "b_int", "y" + currentDiff + "b", CP_MIN, 0);
            cpt.insertConstraint("y" + lastDiff + "a", "y" + currentDiff + "a_int", CP_MIN, 0);
            cpt.insertConstraint("y" + currentDiff + "a", "y" + currentDiff + "a_int", CP_MIN, 0);
            cpt.insertConstraint( "y" + currentDiff + "a_int", "y" + currentDiff + "b_int", CP_EQ,  "y" + currentDiff + "a_int_min");
            //            cpt.insertLPMinVar("y" + currentDiff + "a_int_min",-4);
        }else{
            //insert GAP between diffusions if it's not the first diff
            if(lastDiff!="") {
                cpt.insertConstraint("x" + lastDiff + "b", "x" + currentDiff + "a", CP_MIN, currentRules->getRule(S1DFDF));
                lastDiff="";
            }
        }
        
        //diffusion extension rules for next gate diffusion
        if (l==NDIF) {
            cpt.insertConstraint("y" + diffEnc + "b", "y" + currentDiff + "b", CP_MIN, 0);
            cpt.insertConstraint("y" + diffEnc + "LdistBeforeGateOut", "y" + currentDiff + "a", CP_MAX, "y" + diffEnc + "a");            
            cpt.insertConstraint("y" + diffEnc + "LdistBeforeGateIn", "y" + diffEnc + "b", CP_MAX, "y" + currentDiff + "b");            
            if(lastDiff!=""){
                cpt.insertConstraint("y" + diffEnc + "LdistBeforeGateOut", "y" + currentDiff + "a", CP_MAX, "y" + lastDiff + "a");            
                cpt.insertConstraint("y" + diffEnc + "LdistBeforeGateIn", "y" + lastDiff + "b", CP_MAX, "y" + currentDiff + "b");            
                cpt.insertConstraint("y" + diffEnc + "LdistAfterGateOut", "y" + lastDiff + "a", CP_MAX, "y" + currentDiff + "a");
                cpt.insertConstraint("y" + diffEnc + "LdistAfterGateIn", "y" + currentDiff + "b", CP_MAX, "y" + lastDiff + "b");            
            }
        }else{
            cpt.insertConstraint("y" + currentDiff + "a", "y" + diffEnc + "a", CP_MIN, 0);
            cpt.insertConstraint("y" + diffEnc + "LdistBeforeGateOut", "y" + diffEnc + "b", CP_MAX, "y" + currentDiff + "b");            
            cpt.insertConstraint("y" + diffEnc + "LdistBeforeGateIn", "y" + currentDiff + "a", CP_MAX, "y" + diffEnc + "a");            
            if(lastDiff!=""){
                cpt.insertConstraint("y" + diffEnc + "LdistBeforeGateOut", "y" + lastDiff + "b", CP_MAX, "y" + currentDiff + "b");            
                cpt.insertConstraint("y" + diffEnc + "LdistBeforeGateIn", "y" + currentDiff + "a", CP_MAX, "y" + lastDiff + "a");            
                cpt.insertConstraint("y" + diffEnc + "LdistAfterGateOut", "y" + currentDiff + "b", CP_MAX, "y" + lastDiff + "b");            
                cpt.insertConstraint("y" + diffEnc + "LdistAfterGateIn", "y" + lastDiff + "a", CP_MAX, "y" + currentDiff + "a");            
            }
        }
        cpt.insertLPMinVar("y" + diffEnc + "LdistBeforeGateOut",(diffStretching?5:40));
        cpt.insertLPMinVar("y" + diffEnc + "LdistBeforeGateIn",(diffStretching?5:40));
        
        lastDiff = currentDiff;
    }
    
    lastGate = "";
    lastCnt = currentCnt;
    return diffEnc;
}

string AutoCell::insertCnt(vector<Box*> &geometries, compaction &cpt, list<Element>::iterator elements_it, vector<string>& metTracks, int pos, bool isDiff) {
    geometries.push_back(&currentLayout.addLayer(0, 0, 0, 0, CONT));
    string cnt = intToStr(geometries.size() - 1);
    
    cpt.insertConstraint("x" + cnt + "a", "x" + cnt + "b", CP_EQ, currentRules->getRule(W2CT));
    cpt.insertConstraint("y" + cnt + "a", "y" + cnt + "b", CP_EQ, currentRules->getRule(W2CT));
    
    string cntBndBox=cnt+"_BoundBx_";
    cpt.insertConstraint("x" + cntBndBox + "a", "x" + cnt + "a", CP_MIN, 0);
    cpt.insertConstraint("x" + cnt + "b", "x" + cntBndBox + "b", CP_MIN, 0);
    cpt.insertConstraint("y" + cntBndBox + "a", "y" + cnt + "a", CP_MIN, 0);
    cpt.insertConstraint("y" + cnt + "b", "y" + cntBndBox + "b", CP_MIN, 0);
    
    if(isDiff && ddCntsCost){
        geometries.push_back(&currentLayout.addLayer(0, 0, 0, 0, CONT));
        string cnt2 = intToStr(geometries.size() - 1);    
        cpt.forceBinaryVar("b" + cnt2); // 2nd contact
        cpt.insertLPMinVar("b" + cnt2, -ddCntsCost*8);
        //        cpt.insertConstraint("ZERO", "b" + cnt2, CP_EQ, 1);
        cpt.insertConstraint("y" + cnt + "b", "y" + cnt2 + "a", CP_EQ, "b" + cnt2, currentRules->getRule(S2CTCT));
        cpt.insertConstraint("x" + cnt2 + "a", "x" + cnt2 + "b", CP_EQ, "b" + cnt2, currentRules->getRule(W2CT));
        cpt.insertConstraint("y" + cnt2 + "a", "y" + cnt2 + "b", CP_EQ, "b" + cnt2, currentRules->getRule(W2CT));
        cpt.insertConstraint("x" + cntBndBox + "a", "x" + cnt2 + "a", CP_MIN, 0);
        cpt.insertConstraint("x" + cnt2 + "b", "x" + cntBndBox + "b", CP_MIN, 0);
        cpt.insertConstraint("y" + cnt2 + "b", "y" + cntBndBox + "b", CP_MIN, 0);
    }
    
    //metal head over contact
    cpt.forceBinaryVar("b" + cntBndBox + "_1M"); // horizontal stripe
    cpt.forceBinaryVar("b" + cntBndBox + "_2M"); // vertical stripe
    cpt.forceBinaryVar("b" + cntBndBox + "_3M"); // classic all around
    cpt.insertConstraint("ZERO", "b" + cntBndBox + "_1M" + " + " + "b" + cntBndBox + "_2M"  + " + " + "b" + cntBndBox + "_3M", CP_EQ, 1);
    cpt.insertConstraint("ZERO", "b" + cntBndBox + "_1M", CP_EQ, 0);
    
    cpt.insertConstraint("ZERO", "x" + cntBndBox + "hM", CP_MIN, "b" + cntBndBox + "_1M", currentRules->getRule(E2M1CT));
    cpt.insertConstraint("ZERO", "x" + cntBndBox + "hM", CP_MIN, "b" + cntBndBox + "_2M", currentRules->getRule(E1M1CT));
    cpt.insertConstraint("ZERO", "x" + cntBndBox + "hM", CP_MIN, "b" + cntBndBox + "_3M", currentRules->getRule(E3M1CT));
    cpt.insertConstraint("x" + metTracks[pos] + "a", "x" + cntBndBox + "a", CP_MIN, "x" + cntBndBox + "hM");
    cpt.insertConstraint("x" + cntBndBox + "b", "x" + metTracks[pos] + "b", CP_MIN, "x" + cntBndBox + "hM");
    cpt.insertConstraint("x" + metTracks[pos] + "a", "x" + metTracks[pos] + "b", CP_EQ, "x" + cntBndBox + "hminM");
    cpt.insertLPMinVar("x" + cntBndBox + "hminM",3);
    
    cpt.insertConstraint("ZERO", "y" + cntBndBox + "vM", CP_MIN, "b" + cntBndBox + "_1M", currentRules->getRule(E1M1CT));
    cpt.insertConstraint("ZERO", "y" + cntBndBox + "vM", CP_MIN, "b" + cntBndBox + "_2M", currentRules->getRule(E2M1CT));
    cpt.insertConstraint("ZERO", "y" + cntBndBox + "vM", CP_MIN, "b" + cntBndBox + "_3M", currentRules->getRule(E3M1CT));
    cpt.insertConstraint("y" + metTracks[pos] + "a", "y" + cntBndBox + "a", CP_MIN, "y" + cntBndBox + "vM");
    cpt.insertConstraint("y" + cntBndBox + "b", "y" + metTracks[pos] + "b", CP_MIN, "y" + cntBndBox + "vM");
    cpt.insertConstraint("y" + metTracks[pos] + "a", "y" + metTracks[pos] + "b", CP_EQ, "y" + cntBndBox + "vminM");
    cpt.insertLPMinVar("y" + cntBndBox + "vminM",3);
    
    return cntBndBox;
}

string AutoCell::insertVia(vector<Box*> &geometries, compaction &cpt, string metNode) {
    geometries.push_back(&currentLayout.addLayer(0, 0, 0, 0, MET1P));
    string via = intToStr(geometries.size() - 1);
    
    cpt.insertConstraint("x" + via + "a", "x" + via + "b", CP_EQ, currentRules->getRule(W2VI));
    cpt.insertConstraint("y" + via + "a", "y" + via + "b", CP_EQ, currentRules->getRule(W2VI));
    
    //choose a kind of metal enclosure of via
    cpt.forceBinaryVar("b" + via + "_1V");
    cpt.forceBinaryVar("b" + via + "_2V");
    cpt.insertConstraint("ZERO", "b" + via + "_1V" + " + " + "b" + via + "_2V", CP_EQ, 1);
    
    cpt.insertConstraint("x" + metNode + "a", "x" + via + "a", CP_MIN, "b" + via + "_1V", currentRules->getRule(E2M1VI));
    cpt.insertConstraint("x" + via + "b", "x" + metNode + "b", CP_MIN, "b" + via + "_1V", currentRules->getRule(E2M1VI));
    
    cpt.insertConstraint("y" + metNode + "a", "y" + via + "a", CP_MIN, "b" + via + "_2V", currentRules->getRule(E2M1VI));
    cpt.insertConstraint("y" + via + "b", "y" + metNode + "b", CP_MIN, "b" + via + "_2V", currentRules->getRule(E2M1VI));
    
    //choose a kind of metal area format
    cpt.forceBinaryVar("b" + via + "_1A"); // linear
    cpt.forceBinaryVar("b" + via + "_2A"); //square
    
    cpt.insertConstraint("ZERO", "b" + via + "_1A" + " + " + "b" + via + "_2A", CP_EQ, 1);
    
    cpt.insertConstraint("ZERO", "x" + metNode +  "_width" + " + " + "y" + metNode +  "_width", CP_MIN, "b" + via + "_1A", currentRules->getIntValue(currentRules->getRulef(A1M1)/currentRules->getRulef(W2VI)+currentRules->getRulef(W2VI)));    
    cpt.insertConstraint("x" + metNode + "a", "x" + metNode + "b", CP_MIN,  "x" + metNode +  "_width");
    cpt.insertConstraint("y" + metNode + "a", "y" + metNode + "b", CP_MIN, "y" + metNode +  "_width");
    
    cpt.insertConstraint("x" + metNode + "a", "x" + metNode + "b", CP_MIN, "b" + via + "_2A", currentRules->getIntValue(sqrt(currentRules->getRulef(A1M1))));
    cpt.insertConstraint("y" + metNode + "a", "y" + metNode + "b", CP_MIN, "b" + via + "_2A", currentRules->getIntValue(sqrt(currentRules->getRulef(A1M1))));
    
    
    //allign via to the routing grid
    cpt.insertConstraint( "HGRID_OFFSET", "x" + via + "g", CP_EQ_VAR_VAL,  "x" + via + "gpos", hGrid);
    cpt.forceIntegerVar("x" + via + "gpos");
    cpt.insertConstraint( "x" + via + "a", "x" + via + "g", CP_EQ, currentRules->getRule(E1M1VI)+currentRules->getRule(W2VI)/2);
    cpt.insertConstraint( "x" + via + "g", "x" + via + "b", CP_EQ, currentRules->getRule(E1M1VI)+currentRules->getRule(W2VI)/2);
    
    cpt.insertConstraint( "VGRID_OFFSET", "y" + via + "g", CP_EQ_VAR_VAL,  "y" + via + "gpos", hGrid);
    cpt.forceIntegerVar("y" + via + "gpos");
    cpt.insertConstraint( "y" + via + "a", "y" + via + "g", CP_EQ, currentRules->getRule(E1M1VI)+currentRules->getRule(W2VI)/2);
    cpt.insertConstraint( "y" + via + "g", "y" + via + "b", CP_EQ, currentRules->getRule(E1M1VI)+currentRules->getRule(W2VI)/2);
    
    return via;
}


void AutoCell::insertCntPol(vector<Box*> &geometries, compaction &cpt, string cnt, vector<string> currentPolNodes, int pos) {
    //poly enclosure of contact
    cpt.forceBinaryVar("b" + cnt + "_1P");
    cpt.forceBinaryVar("b" + cnt + "_2P");
    cpt.forceBinaryVar("b" + cnt + "_3P");
    
    cpt.insertConstraint("ZERO", "b" + cnt + "_1P" + " + " + "b" + cnt + "_2P"  + " + " + "b" + cnt + "_3P", CP_EQ, 1);
    //    cpt.insertConstraint("ZERO", "b" + cnt + "_3P", CP_EQ, 1);
    
    cpt.insertConstraint("ZERO", "x" + cnt + "hP", CP_MIN, "b" + cnt + "_1P", currentRules->getRule(E2P1CT));
    cpt.insertConstraint("ZERO", "x" + cnt + "hP", CP_MIN, "b" + cnt + "_2P", currentRules->getRule(E1P1CT));
    cpt.insertConstraint("ZERO", "x" + cnt + "hP", CP_MIN, "b" + cnt + "_3P", currentRules->getRule(E3P1CT));
    cpt.insertConstraint("x" + currentPolNodes[pos] + "a", "x" + cnt + "a", CP_MIN, "x" + cnt + "hP");
    cpt.insertConstraint("x" + cnt + "b", "x" + currentPolNodes[pos] + "b", CP_MIN, "x" + cnt + "hP");
    cpt.insertConstraint("x" + currentPolNodes[pos] + "a", "x" + currentPolNodes[pos] + "b", CP_EQ, "x" + cnt + "hminP");
    cpt.insertLPMinVar("x" + cnt + "hminP",2);
    
    cpt.insertConstraint("ZERO", "y" + cnt + "vP", CP_MIN, "b" + cnt + "_1P", currentRules->getRule(E1P1CT));
    cpt.insertConstraint("ZERO", "y" + cnt + "vP", CP_MIN, "b" + cnt + "_2P", currentRules->getRule(E2P1CT));
    cpt.insertConstraint("ZERO", "y" + cnt + "vP", CP_MIN, "b" + cnt + "_3P", currentRules->getRule(E3P1CT));
    cpt.insertConstraint("y" + currentPolNodes[pos] + "a", "y" + cnt + "a", CP_MIN, "y" + cnt + "vP");
    cpt.insertConstraint("y" + cnt + "b", "y" + currentPolNodes[pos] + "b", CP_MIN, "y" + cnt + "vP");
    cpt.insertConstraint("y" + currentPolNodes[pos] + "a", "y" + currentPolNodes[pos] + "b", CP_EQ, "y" + cnt + "vminP");
    cpt.insertLPMinVar("y" + cnt + "vminP",2);
}

string AutoCell::createGeometry(vector<Box*> &geometries, compaction &cpt, string netName, int priority, layer_name l){
    geometries.push_back(&currentLayout.addLayer(0, 0, 0, 0, l));
    geometries.back()->setNet(netName);
    string currentGeo = intToStr(geometries.size() - 1);
    
    int minWidth = (l==MET1 ? currentRules->getRule(W1M1) : currentRules->getRule(W2P1));
    cpt.insertConstraint("x" + currentGeo + "a", "x" + currentGeo + "b", CP_MIN, minWidth);
    cpt.insertConstraint("x" + currentGeo + "a", "x" + currentGeo + "b", CP_EQ, "x" + currentGeo + "min");
    cpt.insertLPMinVar("x" + currentGeo + "min", (l==POLY && griddedPolly?100:priority));     //MUCH TO IMPROVE!!!!
    cpt.insertConstraint("y" + currentGeo + "a", "y" + currentGeo + "b", CP_MIN, minWidth);
    cpt.insertConstraint("y" + currentGeo + "a", "y" + currentGeo + "b", CP_EQ, "y" + currentGeo + "min");
    cpt.insertLPMinVar("y" + currentGeo + "min", priority);
    return currentGeo;
}

void AutoCell::createNode(vector<Box*> &geometries, compaction &cpt, list<Element>::iterator elements_it, int pos, vector<string> &currentNode, string netName, layer_name l){
    string currentGeo = createGeometry(geometries, cpt, netName, 5, l);
    
    //apply minimum distance rules to cell borders
    int minDist = (l==MET1 ? currentRules->getRule(S1M1M1) : currentRules->getRule(S1P1P1));
    cpt.insertConstraint("ZERO", "x" + currentGeo + "a", CP_MIN, minDist / 2);
    cpt.insertConstraint("x" + currentGeo + "b", "width", CP_MIN, minDist / 2);
    
    //special rules for wide metals and p/nplus enclusure of poly
    minDist = (l==MET1 ? currentRules->getRule(S2M1M1) : currentRules->getRule(S2P1P1));
    if(l==MET1){
        if (currentNetList.getNetName(rt->getNet(elements_it->met[pos])) != currentCircuit->getGndNet())
            cpt.insertConstraint("yGNDb", "y" + currentGeo + "a", CP_MIN, minDist);
        
        if (currentNetList.getNetName(rt->getNet(elements_it->met[pos])) != currentCircuit->getVddNet())
            cpt.insertConstraint("y" + currentGeo + "b", "yVDDa", CP_MIN, minDist);
        
        if (pos == 0)
            cpt.insertConstraint("y" + currentGeo + "a", "yGNDb", CP_MIN, 0);
        
        if (pos == trackPos.size() - 1)
            cpt.insertConstraint("yVDDa", "y" + currentGeo + "b", CP_MIN, 0);
    }else if(l==POLY){
        cpt.insertConstraint("ZERO", "y" + currentGeo + "a", CP_MIN, minDist / 2);
        cpt.insertConstraint("y" + currentGeo + "b", "height", CP_MIN, minDist / 2);        
    }
    
    //line ends rule
    int minExt = (l==MET1 ? currentRules->getRule(S3M1M1)-currentRules->getRule(S1M1M1) : currentRules->getRule(S3P1P1)-currentRules->getRule(S1P1P1));
    cpt.forceBinaryVar("b" + currentGeo+ "_endline_v"); 
    cpt.forceBinaryVar("b" + currentGeo+ "_endline_h");
    cpt.insertConstraint("ZERO", "b" + currentGeo+ "_endline_v" + " + " "b" + currentGeo+ "_endline_h", CP_EQ, 1);
    cpt.insertConstraint("x" + currentGeo + "a2", "x" + currentGeo + "a", CP_EQ, "b" + currentGeo+ "_endline_h", minExt);
    cpt.insertConstraint("x" + currentGeo + "b", "x" + currentGeo + "b2", CP_EQ, "b" + currentGeo+ "_endline_h", minExt);
    cpt.insertConstraint("y" + currentGeo + "a2", "y" + currentGeo + "a", CP_EQ, "b" + currentGeo+ "_endline_v", minExt);
    cpt.insertConstraint("y" + currentGeo + "b", "y" + currentGeo + "b2", CP_EQ, "b" + currentGeo+ "_endline_v", minExt);    
    
    currentNode[pos]=currentGeo;
}

void AutoCell::createTrack(vector<Box*> &geometries, compaction &cpt, string lastNode, string currentNode, string netName, layer_name l, HorV dir){
    int min = l==MET1 ? 1 : 4;
    string track = createGeometry(geometries, cpt, netName, min, l);
    
    int minIntersection = (l==MET1 ? currentRules->getRule(W1M1) : currentRules->getRule(W2P1));
    //intersects with the last node
    cpt.insertConstraint("x" + track + "a", "x" + lastNode + "b", CP_MIN, minIntersection);
    cpt.insertConstraint("x" + lastNode + "a", "x" + track + "b", CP_MIN, minIntersection);
    cpt.insertConstraint("y" + track + "a", "y" + lastNode + "b", CP_MIN, minIntersection);
    cpt.insertConstraint("y" + lastNode + "a", "y" + track + "b", CP_MIN, minIntersection);
    //intersects with the current node
    cpt.insertConstraint("x" + currentNode + "a", "x" + track + "b", CP_MIN, minIntersection);
    cpt.insertConstraint("x" + track + "a", "x" + currentNode + "b", CP_MIN, minIntersection);
    cpt.insertConstraint("y" + currentNode + "a", "y" + track + "b", CP_MIN, minIntersection);
    cpt.insertConstraint("y" + track + "a", "y" + currentNode + "b", CP_MIN, minIntersection);
    
    //set track width
    if(dir==H) {
        cpt.insertConstraint("x" + lastNode + "a", "x" + currentNode + "a", CP_MIN, 0);
        cpt.insertConstraint("x" + lastNode + "b", "x" + currentNode + "b", CP_MIN, 0);
        cpt.insertConstraint("y" + track + "a", "y" + track + "b", CP_EQ, minIntersection);
        cpt.insertConstraint("x" + lastNode + "a", "x" + track + "a", CP_MIN, 0);        
        cpt.insertConstraint("x" + track + "b", "x" + currentNode + "b", CP_MIN, 0);        
    }
    else {
        cpt.insertConstraint("y" + lastNode + "a", "y" + currentNode + "a", CP_MIN, 0);
        cpt.insertConstraint("y" + lastNode + "b", "y" + currentNode + "b", CP_MIN, 0);
        cpt.insertConstraint("x" + track + "a", "x" + track + "b", CP_EQ, minIntersection);
        cpt.insertConstraint("y" + lastNode + "a", "y" + track + "a", CP_MIN, 0);        
        cpt.insertConstraint("y" + track + "b", "y" + currentNode + "b", CP_MIN, 0);        
    }
}

void AutoCell::insertDistanceRuleInteligent(vector<Box*> &geometries, compaction &cpt, string lastX, string currentX, string lastY, string currentY, layer_name l){
    int minDist = (l==MET1 ? currentRules->getRule(S1M1M1) : currentRules->getRule(S1P1P1));
    //select between different space possibilities
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_1"); //left
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_2"); //up
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_3"); //diagonal
    cpt.insertConstraint("ZERO", "b" + lastX + "_" + currentX + "_1" + " + " + "b" + lastX + "_" + currentX + "_2" + " + " + "b" + lastX + "_" + currentX + "_3", CP_EQ, 1);
    cpt.insertConstraint("x" + lastX + "b + RELAXATION", "x" + currentX + "a2", CP_MIN, "b" + lastX + "_" + currentX + "_1", minDist + relaxation);
    cpt.insertConstraint("y" + lastY + "b + RELAXATION", "y" + currentY + "a2", CP_MIN, "b" + lastX + "_" + currentX + "_2", minDist + relaxation);
    cpt.insertConstraint("x" + lastX + "b + RELAXATION", "x" + currentX + "a", CP_MIN, "b" + lastX + "_" + currentX + "_3", ceil(minDist/sqrt(2.0)) + relaxation);
    cpt.insertConstraint("y" + lastY + "b + RELAXATION", "y" + currentY + "a", CP_MIN, "b" + lastX + "_" + currentX + "_3", ceil(minDist/sqrt(2.0)) + relaxation);
    cpt.insertConstraint("x" + lastX + "b2 + RELAXATION", "x" + currentX + "a", CP_MIN, "b" + lastX + "_" + currentX + "_1", minDist + relaxation);
    cpt.insertConstraint("y" + lastY + "b2 + RELAXATION", "y" + currentY + "a", CP_MIN, "b" + lastX + "_" + currentX + "_2", minDist + relaxation);
    cpt.insertConstraint("x" + lastX + "b2 + RELAXATION", "x" + currentX + "a", CP_MIN, "b" + lastX + "_" + currentX + "_3", ceil(minDist/sqrt(2.0)) + relaxation);
    cpt.insertConstraint("y" + lastY + "b2 + RELAXATION", "y" + currentY + "a", CP_MIN, "b" + lastX + "_" + currentX + "_3", ceil(minDist/sqrt(2.0)) + relaxation);
}

void AutoCell::insertDistanceRuleInteligent2(vector<Box*> &geometries, compaction &cpt, string lastX, string currentX, string lastY, string currentY, int minDist, string booleanFlag){
    static int index=0;
    string id=intToStr(index++);
    //select between different space possibilities
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_1_"+id); //front
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_2_"+id); //one side
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_3_"+id); //other side
    cpt.insertConstraint("ZERO", "b" + lastX + "_" + currentX + "_1_" + id + " + " + "b" + lastX + "_" + currentX + "_2_" +id+ " + " + "b" + lastX + "_" + currentX + "_3_"+id, CP_EQ, (booleanFlag==""?"UM":booleanFlag));
    cpt.insertConstraint("y" + lastY + "b + RELAXATION", "y" + currentY + "a", CP_MIN, "b" + lastX + "_" + currentX + "_1_"+id, minDist + relaxation);
    cpt.insertConstraint("x" + lastX + "b + RELAXATION", "x" + currentX + "a", CP_MIN, "b" + lastX + "_" + currentX + "_2_"+id, minDist + relaxation);
    cpt.insertConstraint("x" + currentX + "b + RELAXATION", "x" + lastX + "a", CP_MIN, "b" + lastX + "_" + currentX + "_3_"+id, minDist + relaxation);
}

void AutoCell::insertDistanceRuleDumb(vector<Box*> &geometries, compaction &cpt, string last, string next, int minDist, HorV dir, layer_name l){
    if(l==MET1 || l==POLY){
        if(dir==H){
            cpt.insertConstraint("x" + last + "b", "x" + next + "a2", CP_MIN, minDist);
            cpt.insertConstraint("x" + last + "b2", "x" + next + "a", CP_MIN, minDist);
        }else{
            cpt.insertConstraint("y" + last + "b", "y" + next + "a2", CP_MIN, minDist);
            cpt.insertConstraint("y" + last + "b2", "y" + next + "a", CP_MIN, minDist);
        }
    }
    else{
        if(dir==H)
            cpt.insertConstraint("x" + last + "b", "x" + next + "a", CP_MIN, minDist);
        else
            cpt.insertConstraint("y" + last + "b", "y" + next + "a", CP_MIN, minDist);
    }
}

void AutoCell::showIOCost() {
    for (list<Element>::iterator elements_it = elements.begin(); elements_it != elements.end(); elements_it++) {
        elements_it->print();
    }
}

