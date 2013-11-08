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

Element* AutoCell::createElement(int vcost, int nDiffIni, int pDiffIni, int nDiffEnd, int pDiffEnd, bool isDiff) {
    Element tmp;
    tmp.diffP = rt->createNode();
    tmp.diffN = rt->createNode();
    tmp.linkP.type = tmp.linkN.type = GAP;
    tmp.gapP = tmp.gapN = false;
    tmp.met.resize(trackPos.size());
    tmp.pol.resize(trackPos.size());
    tmp.diffNEnd = nDiffEnd;
    tmp.diffNIni = nDiffIni;
    tmp.diffPIni = pDiffIni;
    tmp.diffPEnd = pDiffEnd;
    
    for (int x = 0; x < trackPos.size(); x++) {
        tmp.met[x] = rt->createNode();
        if (x) rt->addArc(tmp.met[x], tmp.met[x - 1], vcost);
        if (elements.size())
            rt->addArc(tmp.met[x], elements.back().met[x], 4); //if it's not the first, connect to the last element
        
        tmp.pol[x] = rt->createNode();
        if (x && (!isDiff || (x<nDiffEnd) || (x > nDiffIni+1 && x < pDiffIni) || (x>pDiffEnd+1))) rt->addArc(tmp.pol[x], tmp.pol[x - 1], 6);
        if ((x<nDiffEnd) || (x > nDiffIni && x < pDiffIni) || (x>pDiffEnd)) { //CHECK IF THERE IS ENOUGHT SPACE IN THE EXTERNAL AREA 
            if(x>1 && x<trackPos.size()-1){
                if(isDiff) rt->addArc(tmp.pol[x], tmp.met[x], 200);
                else rt->addArc(tmp.pol[x], tmp.met[x], 20);
            }
            if (hPoly && elements.size() && elements.back().pol[x] != -1)
                rt->addArc(tmp.pol[x], elements.back().pol[x], 6); //if it's not the first, connect to the last element
        }
    }
    rt->addNodetoNet(gnd, tmp.met[0]);
    rt->addNodetoNet(vdd, tmp.met[trackPos.size() - 1]);
    
    tmp.inoutCnt = rt->createNode();
    
    //conecta aos pinos de entrada e saida
    for (int x = 0; x < trackPos.size(); x++)
        rt->addArc(tmp.inoutCnt, tmp.met[x], (x > nDiffIni && x < pDiffIni)?495:500);            
    
    elements.push_back(tmp);
    return &elements.back();
}

void AutoCell::calcArea(int nrIntTracks) {
    checkState(1);
    currentRules = currentCircuit->getRules();
    
    cout << "-> Calculating cell area..." << endl;
    
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
    nDif_iniY = min(nDif_iniY, (center+int(ceil(nrIntTracks/2.0)-1)) * vGrid - (currentRules->getRule(W2CT) / 2 + currentRules->getRule(E2P1CT) + currentRules->getRule(S1DFP1)));
    pDif_iniY = max(pDif_iniY, (center+int(floor(nrIntTracks/2.0))) * vGrid + (currentRules->getRule(W2CT) / 2 + currentRules->getRule(E2P1CT) + currentRules->getRule(S1DFP1)));
    
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
    state++;
}

void AutoCell::selectCell(Circuit* c, string cell) {
    state = 0;
    currentCircuit = c;
    currentCell = currentCircuit->getCellNetlst(cell);    
    cout << "-> Selecting cell netlist: " << currentCell->getName() << endl;
    currentNetList.clear();
    currentNetList = currentCircuit->getFlattenCell(cell);
    state++;
}

void AutoCell::foldTrans() {
    checkState(2);
    cout << "-> Applying folding..." << endl;
    cout << "-> Number of transistors before folding: " << currentCell->size() << " -> P(" << currentCell->pSize() << ") N(" << currentCell->nSize() << ")" << endl;
    currentNetList.folding(float(pSize) / currentRules->getScale(), float(nSize) / currentRules->getScale());
    cout << "-> Number of transistors after folding: " << currentNetList.size() << " -> P(" << currentNetList.pSize() << ") N(" << currentNetList.nSize() << ")" << endl;
    state++;
}

void AutoCell::placeTrans(bool ep, int saquality, int nrAttempts, int wC, int gmC, int rC, int congC, int ngC) {
    checkState(3);
    cout << "-> Placing transistors..." << endl;
    if (!currentNetList.transPlacement(ep, saquality, nrAttempts, wC, gmC, rC, congC, ngC)) 
        throw AstranError("Could not place the transistors");
    
    state++;
}

void AutoCell::route(bool hPoly, bool increaseIntTracks, bool optimize) {
    checkState(4);
    cout << "-> Routing cell..." << endl;
    
    this->hPoly=hPoly;
    // CALCULATE DIFFUSION POSITIONS
    diffPini.clear();
    diffNini.clear();
    for (int x = 0; x < currentNetList.getOrderingP().size(); x++) {
        int p;
        for (p = center + 1; trackPos[p] < pDif_iniY; ++p);
        for (; increaseIntTracks && p<trackPos.size()-1 && trackPos[p] <pDif_endY-currentRules->getIntValue(currentNetList.getTrans(currentNetList.getOrderingP()[x].link).width && trackPos[p] < height - (supWidth + currentRules->getRule(S2M1M1))); ++p);
        diffPini.push_back(p);
    }
    
    for (int x = 0; x < currentNetList.getOrderingN().size(); x++) {
        int p;
        for (p = center - 1; trackPos[p] > nDif_iniY; --p);
        for (; increaseIntTracks && p>1 && trackPos[p] > nDif_endY + currentRules->getIntValue(currentNetList.getTrans(currentNetList.getOrderingN()[x].link).width)
             && trackPos[p] > supWidth + currentRules->getRule(S2M1M1)+ currentRules->getRule(S2M1M1); --p);
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
    tmp = createElement(16, center, center,center,center,false);
    
    //conecta sinais de entrada e saida com o nó inoutCnt do elemento
    map<string, int>::iterator inoutPins_it;
    for (inoutPins_it = inoutPins.begin(); inoutPins_it != inoutPins.end(); inoutPins_it++)
        rt->addArc(tmp->inoutCnt, inoutPins_it->second, 0);
    
    vector<t_net2>::iterator eulerPathP_it = currentNetList.getOrderingP().begin(), eulerPathN_it = currentNetList.getOrderingN().begin(), lastP_it, lastN_it;
    int nDiffTrackIni, pDiffTrackIni, nDiffTrackEnd, pDiffTrackEnd;
    
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
        nDiffTrackIni=diffNini[eulerPathN_it - currentNetList.getOrderingN().begin()];
        pDiffTrackIni=diffPini[eulerPathP_it - currentNetList.getOrderingP().begin()];
        //serch for first track above the transistor
        nDiffTrackEnd=nDiffTrackIni;
        pDiffTrackEnd=pDiffTrackIni;
        while (nDiffTrackEnd && (trackPos[nDiffTrackEnd-1] >= trackPos[nDiffTrackIni] - currentRules->getIntValue(currentNetList.getTrans(eulerPathN_it->link).width))) nDiffTrackEnd--;
        while (pDiffTrackEnd<trackPos.size()-1 && (trackPos[pDiffTrackEnd+1] <= trackPos[pDiffTrackIni] + currentRules->getIntValue(currentNetList.getTrans(eulerPathP_it->link).width))) pDiffTrackEnd++;
        //        cout << nDiffTrackEnd << " " << nDiffTrackIni << " " << pDiffTrackIni << " " << pDiffTrackEnd << endl;
        
        if (gapP || gapN || eulerPathP_it == currentNetList.getOrderingP().begin() || eulerPathN_it == currentNetList.getOrderingN().begin()) {
            lastElement = tmp;
            tmp = createElement(4, nDiffTrackIni, pDiffTrackIni, nDiffTrackEnd, pDiffTrackEnd, true);
            
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
            for(int x = pDiffTrackIni; x<=pDiffTrackEnd; x++)
                rt->addArc(tmp->met[x], tmp->diffP, COST_CNT_INSIDE_DIFF);
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
            for(int x = nDiffTrackIni; x>=nDiffTrackEnd; x--)
                rt->addArc(tmp->met[x], tmp->diffN, COST_CNT_INSIDE_DIFF);
        }
        
        //GATE
        //desenha gate do transistor se nao for GAP
        lastElement = tmp;        
        tmp = createElement(16, nDiffTrackIni, pDiffTrackIni, nDiffTrackEnd, pDiffTrackEnd,false);
        
        if (eulerPathP_it->link != -1) { // nao é GAP na difusao P
            tmp->linkP = *eulerPathP_it;
            tmp->linkP.type = GATE;
            for (int pos = tmp->diffPIni; pos<=pDiffTrackEnd; pos++)
                rt->addNodetoNet(currentNetList.getTrans(eulerPathP_it->link).gate, tmp->pol[pos]);
        } else tmp->linkP.type = GAP;
        
        for (inoutPins_it = inoutPins.begin(); inoutPins_it != inoutPins.end(); inoutPins_it++)
            rt->addArc(tmp->inoutCnt, inoutPins_it->second, 0);
        
        if (eulerPathN_it->link != -1) { // nao é GAP na difusao N
            tmp->linkN = *eulerPathN_it;
            tmp->linkN.type = GATE;
            for (int pos = tmp->diffNIni; pos>=nDiffTrackEnd; pos--)
                rt->addNodetoNet(currentNetList.getTrans(eulerPathN_it->link).gate, tmp->pol[pos]);
            
        } else tmp->linkN.type = GAP;
        
        // DIFF
        lastElement = tmp;
        tmp = createElement(4, nDiffTrackIni, pDiffTrackIni, nDiffTrackEnd, pDiffTrackEnd,true);
        
        if (eulerPathP_it->link != -1) { // nao é GAP na difusao P
            tmp->linkP = *eulerPathP_it;
            if (eulerPathP_it->type == DRAIN) {
                tmp->linkP.type = SOURCE;
                rt->addNodetoNet(currentNetList.getTrans(eulerPathP_it->link).source, tmp->diffP);
            } else {
                tmp->linkP.type = DRAIN;
                rt->addNodetoNet(currentNetList.getTrans(eulerPathP_it->link).drain, tmp->diffP);
            }
            for(int x = pDiffTrackIni; x<=pDiffTrackEnd; x++)
                rt->addArc(tmp->met[x], tmp->diffP, COST_CNT_INSIDE_DIFF);
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
            for(int x = nDiffTrackIni; x>=nDiffTrackEnd; x--)
                rt->addArc(tmp->met[x], tmp->diffN, COST_CNT_INSIDE_DIFF);
        } else tmp->linkN.type = GAP;
        
        //conecta sinais de entrada e saida com o nó inoutCnt do elemento
        for (inoutPins_it = inoutPins.begin(); inoutPins_it != inoutPins.end(); inoutPins_it++)
            rt->addArc(tmp->inoutCnt, inoutPins_it->second, 0);
        
        lastP_it = eulerPathP_it++;
        lastN_it = eulerPathN_it++;
    }
    
    //cria elemento lateral para roteamento
    tmp = createElement(16, center, center,center,center,false);
    
    //conecta sinais de entrada e saida com o nó inoutCnt do elemento
    for (inoutPins_it = inoutPins.begin(); inoutPins_it != inoutPins.end(); inoutPins_it++)
        rt->addArc(tmp->inoutCnt, inoutPins_it->second, 0);
    
    
    if (!rt->routeNets(8000))
        throw AstranError("Unable to route this circuit");
    if(optimize) rt->optimize();
    state = 5;
}

void AutoCell::compact(string lpSolverFile, int diffStretching, int griddedPoly, int rdCntsCost, int maxDiffCnts, int alignDiffConts, int reduceLturns, bool enableDFM, bool experimental, bool debug, int timeLimit) {
    checkState(5);
    cout << "-> Compacting layout..." << endl;
    this->diffStretching=diffStretching;
    this->griddedPoly=griddedPoly;
    this->rdCntsCost=rdCntsCost;
    this->maxDiffCnts=maxDiffCnts;
    this->alignDiffConts=alignDiffConts;
    this->reduceLturns=reduceLturns;
    this->experimental=true;
    this->enableDFM=enableDFM;
    
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
    
    if(debug){
        cpt.insertConstraint("ZERO", "height", CP_MIN, height);
        cpt.insertLPMinVar("height", 1);
    }else{
        cpt.insertConstraint("ZERO", "height", CP_EQ, height);
        cpt.insertConstraint("ZERO", "posNWell", CP_EQ, currentRules->getIntValue(currentCircuit->getnWellPos()));
    }
    cpt.insertConstraint("ZERO", "yNDiffa", CP_EQ, nDif_endY);
    cpt.insertConstraint("yNDiffb", "posNWell", CP_EQ, currentRules->getRule(S1DNWN));
    cpt.insertConstraint("posNWell", "yPDiffa", CP_EQ, currentRules->getRule(E1WNDP));
    cpt.insertConstraint("ZERO", "yPDiffb", CP_EQ, pDif_endY);
    cpt.insertConstraint("ZERO", "yGNDb", CP_EQ, max(currentRules->getIntValue(currentCircuit->getSupplyVSize()), currentRules->getRule(W1M1)) / 2);
    cpt.insertConstraint("yVDDa", "height", CP_EQ, max(currentRules->getIntValue(currentCircuit->getSupplyVSize()), currentRules->getRule(W1M1)) / 2);
    
    list<Element>::iterator lastElements_it;
    vector<string> currentMetNodes(trackPos.size(), ""), currentPolNodes(trackPos.size(), ""), lastContacts(trackPos.size(), ""), currentContacts(trackPos.size(), "");
    vector<string> lastMetNodes(trackPos.size(), ""), lastPolNodes(trackPos.size(), "");
    vector<string> befLastMetNodes(trackPos.size(), ""), befLastPolNodes(trackPos.size(), "");
    int x;
    
    //  Transistor Variables
    bool gapP=false, gapN=false;
    string lastDiffP, lastDiffN, currentDiffP="", currentDiffN="", outPolP, outPolN, lastNGatePos, lastPGatePos, lastNGateDiff, lastPGateDiff;
    string lastNContact="", lastPContact="", lastNContactDiff="", lastPContactDiff="";
    int lastPGateLength=0, lastNGateLength=0;
    
    //Compact the routing tracks
    for (list<Element>::iterator elements_it = elements.begin(); elements_it != elements.end(); ++elements_it) {
        //        elements_it->print();
        string lastPolNodeV = "", lastMetNodeV = "";
        lastMetNodes[0]=""; lastMetNodes[trackPos.size() - 1]="";
        //create metals and polys for intra-cell routing
        for (x = 0; x < trackPos.size(); x++) {
            //conecta as trilhas de metal horizontalmente
            //creates each metal node, but the disconected supply ones
            if (rt->getNet(elements_it->met[x]) != -1 && 
                !((x==0 && !rt->areConnected(elements_it->met[x], elements_it->met[x+1])&& !rt->areConnected(elements_it->met[x], elements_it->diffN)) ||
                  (x==trackPos.size()-1 && !rt->areConnected(elements_it->met[x], elements_it->met[x-1]) && !rt->areConnected(elements_it->met[x], elements_it->diffP)))){
                    createNode(geometries, cpt, elements_it, x, currentMetNodes, currentNetList.getNetName(rt->getNet(elements_it->met[x])), MET1);                    
                    //insert space or a track between current and lasts H met node, if it exists
                    for (int c = 0; c < trackPos.size(); c++) {
                        if(lastMetNodes[c]!=""){
                            if(c<x && rt->getNet(lastElements_it->met[c])!=rt->getNet(elements_it->met[x])){
                                insertDistanceRuleInteligent(geometries, cpt, lastMetNodes[c], currentMetNodes[x], lastMetNodes[c], currentMetNodes[x], MET1);
                            }
                            else if(c>x && rt->getNet(lastElements_it->met[c])!=rt->getNet(elements_it->met[x]))
                                insertDistanceRuleInteligent(geometries, cpt, lastMetNodes[c], currentMetNodes[x], currentMetNodes[x], lastMetNodes[c], MET1);
                            else if(c==x){
                                if(!rt->areConnected(lastElements_it->met[c], elements_it->met[x]))
                                    insertDistanceRuleDumb(geometries, cpt, lastMetNodes[c], currentMetNodes[x], currentRules->getRule(S1M1M1), H, MET1);
                                else
                                    createTrack(geometries, cpt, lastMetNodes[c], currentMetNodes[x], currentNetList.getNetName(rt->getNet(elements_it->met[x])), MET1, H);
                            }
                        }
                        if(c && befLastMetNodes[c]!=""){
                            if(experimental && c<x)
                                insertDistanceRuleInteligent(geometries, cpt, befLastMetNodes[c], currentMetNodes[x], befLastMetNodes[c], currentMetNodes[x], MET1);
                            else if(experimental && c>x)
                                insertDistanceRuleInteligent(geometries, cpt, befLastMetNodes[c], currentMetNodes[x], currentMetNodes[x], befLastMetNodes[c], MET1);
                            else 
                                insertDistanceRuleDumb(geometries, cpt, befLastMetNodes[c], currentMetNodes[x], currentRules->getRule(S1M1M1), H, MET1);
                        }
                    }
                    //insert space or a track between current and last V met node, if it exists
                    if (lastMetNodeV!=""){
                        if(!rt->areConnected(elements_it->met[x], elements_it->met[x-1]))
                            insertDistanceRuleDumb(geometries, cpt, lastMetNodeV, currentMetNodes[x], currentRules->getRule(S1M1M1), V, MET1);
                        else{
                            createTrack(geometries, cpt, lastMetNodeV, currentMetNodes[x], currentNetList.getNetName(rt->getNet(elements_it->met[x])), MET1, V);                        
                        }
                    }
                    
                    lastMetNodeV = currentMetNodes[x];
                }
            //OBS: SEMPRE DEVE HAVER UMA INTERSECCAO DE UM QUADRADO COM LADO W2M1 ENTRE METAIS OU W2P1 ENTRE POLYS
            
            //conecta as trilhas de poly
            if (rt->getNet(elements_it->pol[x]) != -1) {
                //creates each poly node. creates only one for the gates
                if(x && rt->isSource(elements_it->pol[x]) && rt->isSource(elements_it->pol[x-1])){
                    currentPolNodes[x]=currentPolNodes[x-1];
                } else {
                    createNode(geometries, cpt, elements_it, x, currentPolNodes, "",  POLY);
                    
                    //insert space or a track between current and last H poly node, if it exists
                    for (int c = 0; c < trackPos.size(); c++) {
                        if(lastPolNodes[c]!="") {
                            if(c<x && rt->getNet(lastElements_it->pol[c])!=rt->getNet(elements_it->pol[x]))
                                insertDistanceRuleInteligent(geometries, cpt, lastPolNodes[c], currentPolNodes[x], lastPolNodes[c], currentPolNodes[x], POLY);
                            else if(c>x && rt->getNet(lastElements_it->pol[c])!=rt->getNet(elements_it->pol[x]))
                                insertDistanceRuleInteligent(geometries, cpt, lastPolNodes[c], currentPolNodes[x], currentPolNodes[x], lastPolNodes[c], POLY);
                            else if(c==x){
                                if(!rt->areConnected(elements_it->pol[x], lastElements_it->pol[x]))
                                    insertDistanceRuleDumb(geometries, cpt, lastPolNodes[c], currentPolNodes[x], currentRules->getRule(S1P1P1), H, POLY);
                                else
                                    createTrack(geometries, cpt, lastPolNodes[x], currentPolNodes[x], "", POLY, H);
                            }
                        }
                        if(c && befLastPolNodes[c]!=""){
                            if(experimental && c<x)
                                insertDistanceRuleInteligent(geometries, cpt, befLastPolNodes[c], currentPolNodes[x], befLastPolNodes[c], currentPolNodes[x], POLY);
                            else if(experimental && c>x)
                                insertDistanceRuleInteligent(geometries, cpt, befLastPolNodes[c], currentPolNodes[x], currentPolNodes[x], befLastPolNodes[c], POLY);
                            else insertDistanceRuleDumb(geometries, cpt, befLastPolNodes[c], currentPolNodes[x], currentRules->getRule(S1P1P1), H, POLY);
                        }
                    }
                    
                    //insert space or a track between current and last V poly node, if it exists
                    if (lastPolNodeV!=""){
                        if(!rt->areConnected(elements_it->pol[x], elements_it->pol[x-1]))
                            insertDistanceRuleDumb(geometries, cpt, lastPolNodeV, currentPolNodes[x], currentRules->getRule(S1P1P1), V, POLY);
                        else{
                            createTrack(geometries, cpt, lastPolNodeV, currentPolNodes[x], "", POLY, V);                        
                        }
                    }
                    //space poly and last diff
                    if(lastNGateDiff!="" && x<elements_it->diffNEnd)
                        insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[x], lastNGateDiff, currentPolNodes[x], lastNGateDiff, currentRules->getRule(S1DFP1),"");
                    if(lastNGateDiff!="" && x>elements_it->diffNIni)
                        insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[x], lastNGateDiff, lastNGateDiff, currentPolNodes[x], currentRules->getRule(S1DFP1),"");
                    if(lastPGateDiff!="" && x<elements_it->diffPIni)
                        insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[x], lastPGateDiff, currentPolNodes[x], lastPGateDiff, currentRules->getRule(S1DFP1),"");
                    if(lastPGateDiff!="" && x>elements_it->diffPEnd)
                        insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[x], lastPGateDiff, lastPGateDiff, currentPolNodes[x], currentRules->getRule(S1DFP1),"");
                    
                    lastPolNodeV = currentPolNodes[x];
                }
            }
            
            //conecta os polys com os metais das trilhas
            if (rt->areConnected(elements_it->met[x], elements_it->pol[x])) {
                currentContacts[x] = insertCnt(geometries, cpt, elements_it, currentMetNodes, lastContacts, x, false);
                insertCntPol(geometries, cpt, currentContacts[x], currentPolNodes, x);
            }
            
            //se for entrada/saida, alinha o metal1 com a grade
            if (rt->areConnected(elements_it->met[x], elements_it->inoutCnt)) {
                IOgeometries[currentNetList.getNetName(rt->getNet(elements_it->inoutCnt))] = strToInt(insertVia(geometries, cpt, currentMetNodes[x]));
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
                        string currentCnt = insertCnt(geometries, cpt, elements_it, currentMetNodes, lastContacts, x,true);
                        list<Element>::iterator next = elements_it; next++;
                        string currentDiff = insertCntDif(geometries, cpt, x, currentCnt, lastNGatePos, lastNContact, currentDiffN, currentPolNodes, lastPolNodes, NDIF, next->gapN==true || next->linkN.type==GAP);                        
                        if(gapN && lastNContactDiff!="") 
                            cpt.insertConstraint("x" + lastNContactDiff + "b", "x" + currentDiff + "a", CP_MIN, currentRules->getRule(S1DFDF));
                        lastNContactDiff = currentDiff;
                        break;
                    }
                }
                gapN = false;
                break;
                
            case GATE:
                lastNGatePos = insertGate(geometries, cpt,elements_it->linkN.link, elements_it, currentPolNodes, lastPolNodes, lastNContact, lastNContactDiff, lastNGatePos, lastNGateLength, lastDiffN, currentDiffN, NDIF);
                currentDiffN;
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
                        string currentCnt = insertCnt(geometries, cpt, elements_it, currentMetNodes, lastContacts, x,true);
                        list<Element>::iterator next = elements_it; next++;
                        string currentDiff = insertCntDif(geometries, cpt,  x, currentCnt, lastPGatePos, lastPContact, currentDiffP, currentPolNodes, lastPolNodes, PDIF, next->gapP==true || next->linkP.type==GAP);
                        if(gapP && lastPContactDiff!="") 
                            cpt.insertConstraint("x" + lastPContactDiff + "b", "x" + currentDiff + "a", CP_MIN, currentRules->getRule(S1DFDF));
                        lastPContactDiff = currentDiff;
                        break;
                    }
                }
                gapP = false;
                break;
                
            case GATE:
                lastPGatePos = insertGate(geometries, cpt, elements_it->linkP.link, elements_it, currentPolNodes, lastPolNodes, lastPContact, lastPContactDiff, lastPGatePos, lastPGateLength, lastDiffP, currentDiffP, PDIF);                
                break;
        }
        
        for (int c = 0; c < trackPos.size(); c++) {
            //insert contacts to diff space rule
            if(currentContacts[c]!=""){
                if(elements_it->linkN.type!=GAP){
                    if(c<elements_it->diffNEnd){
                        if(currentDiffN!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], currentDiffN, currentContacts[c], currentDiffN, currentRules->getRule(S1CTDF),"");
                        if(lastDiffN!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], lastDiffN, currentContacts[c], lastDiffN, currentRules->getRule(S1CTDF),"");
                    }else if(c>elements_it->diffNIni){
                        if(currentDiffN!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], currentDiffN, currentDiffN, currentContacts[c], currentRules->getRule(S1CTDF),"");
                        if(lastDiffN!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], lastDiffN, lastDiffN, currentContacts[c], currentRules->getRule(S1CTDF),"");
                    }else
                        if(lastNGateDiff!="") insertDistanceRuleInteligent3(geometries, cpt, lastNGateDiff, currentContacts[c], currentContacts[c], lastNGateDiff, currentRules->getRule(S1CTDF),"");
                    
                }
                if(elements_it->linkP.type!=GAP){
                    if(c<elements_it->diffPIni){
                        if(currentDiffP!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], currentDiffP, currentContacts[c], currentDiffP, currentRules->getRule(S1CTDF),"");
                        if(lastDiffP!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], lastDiffP, currentContacts[c], lastDiffP, currentRules->getRule(S1CTDF),"");
                    }else if(c>elements_it->diffPEnd){
                        if(currentDiffP!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], currentDiffP, currentDiffP, currentContacts[c], currentRules->getRule(S1CTDF),"");
                        if(lastDiffP!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], lastDiffP, lastDiffP, currentContacts[c], currentRules->getRule(S1CTDF),"");
                    }else
                        if(lastPGateDiff!="") insertDistanceRuleInteligent3(geometries, cpt, lastPGateDiff, currentContacts[c], currentContacts[c], lastPGateDiff, currentRules->getRule(S1CTDF),"");
                }
            }
            
            //insert diff to poly space rule
            
            if(currentDiffN!=""){
                if(elements_it->linkN.type!=GAP && c<elements_it->diffNEnd){
                    if(currentPolNodes[c]!="") insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[c], currentDiffN, currentPolNodes[c], currentDiffN, currentRules->getRule(S1DFP1),"");
                    if(lastPolNodes[c]!="") insertDistanceRuleInteligent2(geometries, cpt, lastPolNodes[c], currentDiffN, lastPolNodes[c], currentDiffN, currentRules->getRule(S1DFP1),"");
                }else if(c>elements_it->diffNIni){
                    if(currentPolNodes[c]!="") insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[c], currentDiffN, currentDiffN, currentPolNodes[c], currentRules->getRule(S1DFP1),"");
                    if(lastPolNodes[c]!="") insertDistanceRuleInteligent2(geometries, cpt, lastPolNodes[c], currentDiffN, currentDiffN, lastPolNodes[c], currentRules->getRule(S1DFP1),"");
                }
            }
            if(elements_it->linkP.type!=GAP && currentDiffP!=""){
                if(c<elements_it->diffPIni){
                    if(currentPolNodes[c]!="") insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[c], currentDiffP, currentPolNodes[c], currentDiffP, currentRules->getRule(S1DFP1),"");
                    if(lastPolNodes[c]!="") insertDistanceRuleInteligent2(geometries, cpt, lastPolNodes[c], currentDiffP, lastPolNodes[c], currentDiffP, currentRules->getRule(S1DFP1),"");
                }else if(c>elements_it->diffPEnd){
                    if(currentPolNodes[c]!="") insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[c], currentDiffP, currentDiffP, currentPolNodes[c], currentRules->getRule(S1DFP1),"");
                    if(lastPolNodes[c]!="") insertDistanceRuleInteligent2(geometries, cpt, lastPolNodes[c], currentDiffP, currentDiffP, lastPolNodes[c], currentRules->getRule(S1DFP1),"");
                }
            }
            
        }
        lastDiffN=currentDiffN;
        lastDiffP=currentDiffP;
        if(lastNGateDiff != currentDiffN) lastNGateDiff = currentDiffN;
        if(lastPGateDiff != currentDiffP) lastPGateDiff = currentDiffP;        
        lastElements_it = elements_it;
        for (x = 0; x < trackPos.size(); x++){
            if(currentMetNodes[x]!=lastMetNodes[x]) befLastMetNodes[x]=lastMetNodes[x];
            if(currentPolNodes[x]!=lastPolNodes[x]) befLastPolNodes[x]=lastPolNodes[x];
        }
        lastMetNodes = currentMetNodes;
        lastPolNodes = currentPolNodes;
        lastContacts = currentContacts;
    }
    
    
    
    cpt.insertConstraint("ZERO", "width", CP_MIN, 0);
    cpt.insertConstraint("ZERO", "width", CP_EQ_VAR_VAL, "width_gpos", hGrid);
    cpt.forceIntegerVar("width_gpos");
    cpt.insertLPMinVar("width", 5000);
    
    if (!cpt.solve(lpSolverFile, timeLimit))
        throw AstranError("Could not solve the ILP model. Try to adjust the constraints!");
    
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
    state++;
}


string AutoCell::insertGate(vector<Box*> &geometries, compaction &cpt, int transistor, list<Element>::iterator elements_it, vector<string> &currentPolNodes, vector<string> &lastPolTrack, string lastCnt, string lastDiffCnt, string &lastGate, int &lastGateLength, string lastDiff, string currentDiff, layer_name l){
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
    cpt.insertConstraint("x" + currentPolNodes[gateIni] + "a", "x" + currentPolNodes[gateIni] + "b", CP_EQ, transLength);
    cpt.insertConstraint("x" + currentDiff + "a", "x" + currentPolNodes[gateIni] + "a", CP_MIN, currentRules->getRule(E1DFP1));
    cpt.insertConstraint("x" + currentPolNodes[gateIni] + "b", "x" + currentDiff + "b", CP_MIN, currentRules->getRule(E1DFP1));
    cpt.insertConstraint("ZERO", "b" + currentDiff + "_smallTransWidth", CP_EQ, (transWidth<currentRules->getRule(S3DFP1)?1:0));    
    cpt.insertConstraint("y" + currentDiff + "a", "y" + currentDiff + "b", CP_EQ, transWidth);
    cpt.insertConstraint("ZERO", "y" + currentPolNodes[gateIni] + "a", CP_MIN, currentRules->getRule(S1P1P1) / 2);
    cpt.insertConstraint("y" + currentPolNodes[gateIni] + "b", "height", CP_MIN, currentRules->getRule(S1P1P1) / 2);        
    
    if (lastGate != ""){
        cpt.insertConstraint("x" + lastGate + "b", "x" + currentPolNodes[gateIni] + "a", CP_MIN, currentRules->getRule(S2P1P1));
        if(transLength > currentRules->getRule(S2P1P1_1) || lastGateLength > currentRules->getRule(S2P1P1_1))
            cpt.insertConstraint("x" + lastGate + "b", "x" + currentPolNodes[gateIni] + "a", CP_MIN, currentRules->getRule(S2P1P1_2));
    }
    
    //poly distance to active region
    if(l==NDIF){
        if(gateIni+1 < currentPolNodes.size() && currentPolNodes[gateIni+1]!="")
            insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[gateIni+1], currentPolNodes[gateIni], currentDiff, currentPolNodes[gateIni+1], currentRules->getRule(S2DFP1),"b" + currentDiff + "_smallTransWidth");
        if(gateEnd && currentPolNodes[gateEnd-1]!="")
            insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[gateEnd-1], currentPolNodes[gateEnd], currentPolNodes[gateEnd-1], currentDiff, currentRules->getRule(S2DFP1),"b" + currentDiff + "_smallTransWidth");
        cpt.insertConstraint("y" + currentDiff + "b", "yNDiffb", CP_MIN, 0);
        
    }else{
        if(gateIni && currentPolNodes[gateIni-1]!="")
            insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[gateIni-1], currentPolNodes[gateIni], currentPolNodes[gateIni-1], currentDiff, currentRules->getRule(S2DFP1),"b" + currentDiff + "_smallTransWidth");
        if(gateEnd+1 < currentPolNodes.size() && currentPolNodes[gateEnd+1]!="")
            insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[gateEnd+1], currentPolNodes[gateEnd], currentDiff, currentPolNodes[gateEnd+1], currentRules->getRule(S2DFP1),"b" + currentDiff + "_smallTransWidth");
        cpt.insertConstraint("yPDiffa", "y" + currentDiff + "a", CP_MIN, 0);
    }
    
    //gate extension rule for L shape transistor if diff dist to gate < E3P1DF
    cpt.forceBinaryVar("b" + lastDiffCnt + "_LshapeBeforeGate");
    cpt.forceBinaryVar("b" + currentPolNodes[gateIni] + "_applyExtraExtBeforeGate");
    cpt.insertConstraint("b" + lastDiffCnt + "_applyS2BeforeGate", "b" + lastDiffCnt + "_LshapeBeforeGate", CP_MAX, "b" + currentPolNodes[gateIni] + "_applyExtraExtBeforeGate");
    cpt.insertConstraint("y" + currentPolNodes[gateIni] + "a", "y" + currentDiff + "a", CP_MIN, currentRules->getRule(E1P1DF));
    cpt.insertConstraint("y" + currentPolNodes[gateIni] + "a", "y" + currentDiff + "a", CP_MIN, "b" + currentPolNodes[gateIni] + "_applyExtraExtBeforeGate", currentRules->getRule(E2P1DF));
    cpt.insertConstraint("y" + currentDiff + "b", "y" + currentPolNodes[gateIni] + "b", CP_MIN, currentRules->getRule(E1P1DF));
    cpt.insertConstraint("y" + currentDiff + "b", "y" + currentPolNodes[gateIni] + "b", CP_MIN, "b" + currentPolNodes[gateIni] + "_applyExtraExtBeforeGate", currentRules->getRule(E2P1DF));
    
    //minimize gate size
    cpt.insertConstraint("y" + currentPolNodes[gateIni] + "a", "y" + currentPolNodes[gateIni] + "b", CP_EQ, "y" + currentPolNodes[gateIni] + "min");
    cpt.insertLPMinVar("y" + currentPolNodes[gateIni] + "min");
    
    //insert conditional diff to gate rule in L shape transistors considering S3DFP1 (big transistor width)
    cpt.insertConstraint("x" + lastDiffCnt + "b", "x" + currentPolNodes[gateIni] + "a", CP_MIN, "b" + lastDiffCnt + "_LshapeBeforeGate", currentRules->getRule(S1DFP1));
    cpt.insertConstraint("ZERO", "y" + lastDiffCnt + "LdistBeforeGateOut", CP_MAX, "b" + lastDiffCnt + "_LshapeBeforeGate", 100000);             
    cpt.insertConstraint("ZERO", "y" + lastDiffCnt + "LdistBeforeGateIn", CP_MAX, "b" + lastDiffCnt + "_LshapeBeforeGate", 100000);             
    cpt.insertConstraint("UM", "b" + currentDiff + "_smallTransWidth" + " + " + "b" + lastDiffCnt + "_LshapeBeforeGate", CP_MAX, "b" + lastDiffCnt + "_applyS2BeforeGate");
    cpt.insertConstraint("x" + lastDiffCnt + "b", "x" + currentPolNodes[gateIni] + "a", CP_MIN, "b" + lastDiffCnt + "_applyS2BeforeGate", currentRules->getRule(S2DFP1));
    
    //contact to gate distance
    cpt.insertConstraint("x" + lastCnt + "b", "x" + currentPolNodes[gateIni] + "a", CP_MIN, currentRules->getRule(S1CTP1));
    
    lastGate = currentPolNodes[gateIni];
    lastGateLength = transLength;
    return currentPolNodes[gateIni];
}

string AutoCell::insertCntDif(vector<Box*> &geometries, compaction &cpt, int pos, string currentCnt, string &lastGate, string &lastCnt, string &lastDiff, vector<string> &currentPolNodes, vector<string> &lastPolTrack,  layer_name l, bool endDiff) {
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
    cpt.insertConstraint( "yNDiffa", "y" + diffEnc + "a", CP_MIN, 0);
    cpt.insertConstraint("y" + diffEnc + "b", "yPDiffb", CP_MIN, 0);
    cpt.insertConstraint("y" + diffEnc + "a", "y" + currentCnt + "a", CP_MIN, currentRules->getRule(E1DFCT));
    cpt.insertConstraint("y" + diffEnc + "a", "y" + currentCnt + "a", CP_MIN, "b" + diffEnc + "_b", currentRules->getRule(E2DFCT));
    cpt.insertConstraint("y" + currentCnt + "b", "y" + diffEnc + "b", CP_MIN, currentRules->getRule(E1DFCT));
    cpt.insertConstraint("y" + currentCnt + "b", "y" + diffEnc + "b", CP_MIN, "b" + diffEnc + "_t", currentRules->getRule(E2DFCT));
    
    //if there is a gate before
    if (lastGate != "") {
        //minimize contact to contact distance
        cpt.insertConstraint("x" + lastCnt + "a", "x" + currentCnt + "b", CP_EQ, "x" + currentCnt + "_boundbx_min");
        cpt.insertLPMinVar("x" + currentCnt + "_boundbx_min",15);
        cpt.insertConstraint("y" + currentCnt + "_boundbx_a", "y" + lastCnt + "a", CP_MIN, 0);
        cpt.insertConstraint("y" + currentCnt + "_boundbx_a", "y" + currentCnt + "a", CP_MIN, 0);
        cpt.insertConstraint("y" + lastCnt + "b", "y" + currentCnt + "_boundbx_b", CP_MIN, 0);
        cpt.insertConstraint("y" + currentCnt + "b", "y" + currentCnt + "_boundbx_b", CP_MIN, 0);
        cpt.insertConstraint("y" + currentCnt + "_boundbx_a", "y" + currentCnt + "_boundbx_b", CP_EQ, "y" + currentCnt + "_boundbx_min");
        cpt.insertLPMinVar("y" + currentCnt + "_boundbx_min",(alignDiffConts?2:0));
        
        cpt.insertConstraint("x" + lastGate + "b", "x" + currentCnt + "a", CP_MIN, currentRules->getRule(S1CTP1));
        cpt.insertConstraint("x" + diffEnc + "b", "width", CP_MIN, currentRules->getRule(S1DFDF) / 2);
        cpt.insertConstraint("x" + diffEnc + "b", "x" + lastDiff + "b", CP_EQ, 0);
        //        cpt.insertConstraint("x" + lastDiff + "b", "x" + diffEnc + "b", CP_EQ, "x" + lastDiff + "minR");
        //        cpt.insertLPMinVar("x" + lastDiff + "minR", 5);
        
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
        cpt.insertLPMinVar("y" + diffEnc + "LdistAfterGateOut",(diffStretching?8:40));
        cpt.insertLPMinVar("y" + diffEnc + "LdistAfterGateIn",(diffStretching?8:40));
    }
    
    //if there is not a gap after
    if(!endDiff){
        //create next diffusion
        geometries.push_back(&currentLayout.addPolygon(0, 0, 0, 0, l));
        string currentDiff = intToStr(geometries.size() - 1);
        
        cpt.insertConstraint("x" + currentDiff + "a", "x" + currentDiff + "b", CP_EQ, "x" + currentDiff + "min");
        cpt.insertLPMinVar("x" + currentDiff + "min",4);
        cpt.insertConstraint("ZERO", "x" + diffEnc + "a", CP_MIN, currentRules->getRule(S1DFDF) / 2);
        cpt.insertConstraint("x" + currentDiff + "a", "x" + diffEnc + "a", CP_EQ, 0);
        //        cpt.insertConstraint("x" + diffEnc + "a", "x" + currentDiff + "a", CP_EQ, "x" + currentDiff + "minL");
        //        cpt.insertLPMinVar("x" + currentDiff + "minL", 5);
        
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
        cpt.insertLPMinVar("y" + diffEnc + "LdistBeforeGateOut",(diffStretching?8:40));
        cpt.insertLPMinVar("y" + diffEnc + "LdistBeforeGateIn",(diffStretching?8:40));
        
        lastDiff = currentDiff;
    }
    
    lastGate = "";
    lastCnt = currentCnt;
    return diffEnc;
}

string AutoCell::insertCnt(vector<Box*> &geometries, compaction &cpt, list<Element>::iterator elements_it, vector<string>& metTracks, vector<string>& lastContacts, int pos, bool isDiff) {
    geometries.push_back(&currentLayout.addLayer(0, 0, 0, 0, CONT));
    string cnt = intToStr(geometries.size() - 1);
    
    cpt.insertConstraint("x" + cnt + "a", "x" + cnt + "b", CP_EQ, currentRules->getRule(W2CT));
    cpt.insertConstraint("y" + cnt + "a", "y" + cnt + "b", CP_EQ, currentRules->getRule(W2CT));
    
    string cntBndBox=cnt+"_BoundBx_";
    cpt.insertConstraint("x" + cntBndBox + "a", "x" + cnt + "a", CP_MIN, 0);
    cpt.insertConstraint("x" + cnt + "b", "x" + cntBndBox + "b", CP_MIN, 0);
    cpt.insertConstraint("y" + cntBndBox + "a", "y" + cnt + "a", CP_MIN, 0);
    cpt.insertConstraint("y" + cnt + "b", "y" + cntBndBox + "b", CP_MIN, 0);
    
    if(isDiff && rdCntsCost){
        string lastCnt=cnt;
        for(int c=1;c<maxDiffCnts; ++c){
            geometries.push_back(&currentLayout.addLayer(0, 0, 0, 0, CONT));
            string cnt2 = intToStr(geometries.size() - 1);    
            cpt.forceBinaryVar("b" + cnt2); // 2nd contact
            cpt.insertLPMinVar("b" + cnt2, -rdCntsCost*8);
            cpt.insertConstraint("y" + lastCnt + "b", "y" + cnt2 + "a", CP_EQ, "b" + cnt2, currentRules->getRule(S2CTCT));
            cpt.insertConstraint("x" + cnt2 + "a", "x" + cnt2 + "b", CP_EQ, "b" + cnt2, currentRules->getRule(W2CT));
            cpt.insertConstraint("y" + cnt2 + "a", "y" + cnt2 + "b", CP_EQ, "b" + cnt2, currentRules->getRule(W2CT));
            cpt.insertConstraint("x" + cntBndBox + "a", "x" + cnt2 + "a", CP_MIN, 0);
            cpt.insertConstraint("x" + cnt2 + "b", "x" + cntBndBox + "b", CP_MIN, 0);
            cpt.insertConstraint("y" + cntBndBox + "a", "y" + cnt2 + "a", CP_MIN, 0);
            cpt.insertConstraint("y" + cnt2 + "b", "y" + cntBndBox + "b", CP_MIN, 0);
            lastCnt=cnt2;
        }
    }
    
    //metal head over contact
    cpt.forceBinaryVar("b" + cntBndBox + "_1M"); // horizontal stripe
    cpt.forceBinaryVar("b" + cntBndBox + "_2M"); // vertical stripe
    cpt.forceBinaryVar("b" + cntBndBox + "_3M"); // classic all around
    cpt.insertConstraint("ZERO", "b" + cntBndBox + "_1M" + " + " + "b" + cntBndBox + "_2M"  + " + " + "b" + cntBndBox + "_3M", CP_EQ, 1);
    
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
    
    //space contacts
    for (int c = 0; c < trackPos.size(); c++) {
        if(lastContacts[c]!=""){
            if(c<pos)
                insertDistanceRuleInteligent(geometries, cpt, lastContacts[c], cntBndBox, lastContacts[c], cntBndBox, CONT);
            else if(c>pos)
                insertDistanceRuleInteligent(geometries, cpt, lastContacts[c], cntBndBox, cntBndBox, lastContacts[c], CONT);
            else if(c==pos)
                insertDistanceRuleDumb(geometries, cpt, lastContacts[c], cntBndBox, currentRules->getRule(S1CTCT), H, CONT);
        }
    }
    
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
    cpt.insertLPMinVar("x" + currentGeo + "min", priority);    
    cpt.insertConstraint("y" + currentGeo + "a", "y" + currentGeo + "b", CP_MIN, minWidth);
    cpt.insertConstraint("y" + currentGeo + "a", "y" + currentGeo + "b", CP_EQ, "y" + currentGeo + "min");
    cpt.insertLPMinVar("y" + currentGeo + "min", priority);
    return currentGeo;
}

void AutoCell::createNode(vector<Box*> &geometries, compaction &cpt, list<Element>::iterator elements_it, int pos, vector<string> &currentNode, string netName, layer_name l){
    string currentGeo = createGeometry(geometries, cpt, netName, l==MET1?3:6, l);
    
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
    if(enableDFM){
        cpt.insertConstraint("x" + currentGeo + "a2", "x" + currentGeo + "a", CP_MIN, "max" + currentGeo+ "L");
        cpt.insertConstraint("x" + currentGeo + "b", "x" + currentGeo + "b2", CP_MIN, "max" + currentGeo+ "R");
        cpt.insertConstraint("y" + currentGeo + "a2", "y" + currentGeo + "a", CP_MIN, "max" + currentGeo+ "B");
        cpt.insertConstraint("y" + currentGeo + "b", "y" + currentGeo + "b2", CP_MIN, "max" + currentGeo+ "T");    
        minDist = (l==MET1 ? currentRules->getRule(S1M1M1) : currentRules->getRule(S1P1P1));
        cpt.insertConstraint("ZERO", "max" + currentGeo+ "L", CP_MAX, ceil(minDist*1.10));
        cpt.insertConstraint("ZERO", "max" + currentGeo+ "R", CP_MAX, ceil(minDist*1.10));
        cpt.insertConstraint("ZERO", "max" + currentGeo+ "B", CP_MAX, ceil(minDist*1.10));
        cpt.insertConstraint("ZERO", "max" + currentGeo+ "T", CP_MAX, ceil(minDist*1.10));
        cpt.insertLPMinVar("max" + currentGeo+ "L", -(l==MET1?5:8));
        cpt.insertLPMinVar("max" + currentGeo+ "R", -(l==MET1?5:8));
        cpt.insertLPMinVar("max" + currentGeo+ "B", -(l==MET1?5:8));
        cpt.insertLPMinVar("max" + currentGeo+ "T", -(l==MET1?5:8));
    }
    
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
        if (l==POLY && griddedPoly)
            cpt.insertConstraint("y" + lastNode + "a", "y" + currentNode + "a", CP_EQ, 0);
    }
    else {
        cpt.insertConstraint("y" + lastNode + "a", "y" + currentNode + "a", CP_MIN, 0);
        cpt.insertConstraint("y" + lastNode + "b", "y" + currentNode + "b", CP_MIN, 0);
        cpt.insertConstraint("x" + track + "a", "x" + track + "b", CP_EQ, minIntersection);
        cpt.insertConstraint("y" + lastNode + "a", "y" + track + "a", CP_MIN, 0);        
        cpt.insertConstraint("y" + track + "b", "y" + currentNode + "b", CP_MIN, 0);        
        if (l==POLY && griddedPoly)
            cpt.insertConstraint("x" + lastNode + "a", "x" + currentNode + "a", CP_EQ, 0);
    }
    
    //minimizes L turns
    if(reduceLturns){
        if(dir==H)
            cpt.insertConstraint("x" + track + "a - " + intToStr(minIntersection) + " UM", "x" + track + "b", CP_MAX, "b"+track+"_reduceLturns", relaxation);
        else
            cpt.insertConstraint("y" + track + "a - " + intToStr(minIntersection) + " UM", "y" + track + "b", CP_MAX, "b"+track+"_reduceLturns", relaxation);
        cpt.forceBinaryVar("b"+track+"_reduceLturns");
        cpt.insertLPMinVar("b"+track+"_reduceLturns",l==MET1?4:7);
    }
}

void AutoCell::insertDistanceRuleInteligent(vector<Box*> &geometries, compaction &cpt, string lastX, string currentX, string lastY, string currentY, layer_name l){
    int minDist;
    if (l==MET1) minDist = currentRules->getRule(S1M1M1);
    else if(l==POLY) minDist = currentRules->getRule(S1P1P1);
    else if(l==CONT) minDist = currentRules->getRule(S1CTCT);
    
    //select between different space possibilities
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_1"); //left
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_2"); //up
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_3"); //diagonal
    
    cpt.insertConstraint("ZERO", "b" + lastX + "_" + currentX + "_1" + " + " + "b" + lastX + "_" + currentX + "_2" + " + " + "b" + lastX + "_" + currentX + "_3", CP_EQ, 1);
    cpt.insertConstraint("x" + lastX + "b + RELAXATION", "x" + currentX + "a2", CP_MIN, "b" + lastX + "_" + currentX + "_1", minDist + relaxation);
    cpt.insertConstraint("x" + lastX + "b2 + RELAXATION", "x" + currentX + "a", CP_MIN, "b" + lastX + "_" + currentX + "_1", minDist + relaxation);
    
    cpt.insertConstraint("y" + lastY + "b + RELAXATION", "y" + currentY + "a2", CP_MIN, "b" + lastX + "_" + currentX + "_2", minDist + relaxation);
    cpt.insertConstraint("y" + lastY + "b2 + RELAXATION", "y" + currentY + "a", CP_MIN, "b" + lastX + "_" + currentX + "_2", minDist + relaxation);
    
    if(l==CONT) minDist = currentRules->getRule(S3CTCT);
    minDist=int(ceil(2*(minDist/sqrt(2.0))));
    if(l==CONT) minDist = max(minDist, currentRules->getRule(S1CTCT));
    
    cpt.insertConstraint("ZERO", "x" + currentX +  "a2" + " - " + "x" + lastX +  "b" + " + "+ "y" + currentY +  "a2" + " - " + "y" + lastY +  "b + RELAXATION", CP_MIN, "b" + lastX + "_" + currentX+ "_3", minDist + relaxation);    
    cpt.insertConstraint("ZERO", "x" + currentX +  "a" + " - " + "x" + lastX +  "b2" + " + "+ "y" + currentY +  "a" + " - " + "y" + lastY +  "b2 + RELAXATION", CP_MIN, "b" + lastX + "_" + currentX+ "_3", minDist + relaxation);
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

void AutoCell::insertDistanceRuleInteligent3(vector<Box*> &geometries, compaction &cpt, string lastX, string currentX, string lastY, string currentY, int minDist, string booleanFlag){
    static int index=0;
    string id=intToStr(index++);
    //select between different space possibilities
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_1_"+id); //front
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_2_"+id); //one side
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_3_"+id); //other side
    cpt.insertConstraint("ZERO", "b" + lastX + "_" + currentX + "_1_" + id + " + " + "b" + lastX + "_" + currentX + "_2_" +id+ " + " + "b" + lastX + "_" + currentX + "_3_"+id, CP_EQ, (booleanFlag==""?"UM":booleanFlag));
    cpt.insertConstraint("x" + lastX + "b + RELAXATION", "x" + currentX + "a", CP_MIN, "b" + lastX + "_" + currentX + "_1_"+id, minDist + relaxation);
    cpt.insertConstraint("y" + lastY + "b + RELAXATION", "y" + currentY + "a", CP_MIN, "b" + lastX + "_" + currentX + "_2_"+id, minDist + relaxation);
    cpt.insertConstraint("y" + currentY + "b + RELAXATION", "y" + lastY + "a", CP_MIN, "b" + lastX + "_" + currentX + "_3_"+id, minDist + relaxation);
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

void AutoCell::printGraph() {
    for (list<Element>::iterator elements_it = elements.begin(); elements_it != elements.end(); elements_it++) {
        elements_it->print();
    }
}


void AutoCell::checkState(int nextState){
    if(state<nextState){
        switch(state){
            case 0: throw AstranError("Select the cell first");
            case 1: throw AstranError("Calculate cell area first");
            case 2: throw AstranError("Fold the transistors first");
            case 3: throw AstranError("Place the transistors first");
            case 4: throw AstranError("Route the cell first");
        }
    }
    state=nextState;
}