/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
 *   amziesemerj[at]inf.ufrgs.br                                           *
 ***************************************************************************/
#include "autocell2.h"

AutoCell::AutoCell(): currentCell(NULL), state(0), index(0) {}

AutoCell::~AutoCell() {}

AutoCell::Element* AutoCell::createElement(int vcost, int nDiffIni, int pDiffIni, int nDiffEnd, int pDiffEnd, TransTerminalType type) {
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
    //    cout << nDiffEnd << " " << nDiffIni << " " << pDiffIni << " " << pDiffEnd << endl;
    for (int x = 0; x < trackPos.size(); x++) {
        tmp.met[x] = rt->createNode();
        if (x){
            if(x<=nDiffEnd || x> pDiffEnd)
                rt->addArc(tmp.met[x], tmp.met[x - 1], vcost-1);
            else if(x>nDiffIni && x<=pDiffIni && type!=GATE && type!=GAP)
                rt->addArc(tmp.met[x], tmp.met[x - 1], (reduceVRt?vcost*2:vcost+1));
            else
                rt->addArc(tmp.met[x], tmp.met[x - 1], vcost);
        }
        if (elements.size() && (x==0 || x==trackPos.size()-1 || (x>floor(reduceMetTracks/2.0) && x<trackPos.size()-1-ceil(reduceMetTracks/2.0)))){
            if(reduceVRt && x>=nDiffIni && x<=pDiffIni)
                rt->addArc(tmp.met[x], elements.back().met[x], 1); //if it's not the first, connect to the last element
            else
                rt->addArc(tmp.met[x], elements.back().met[x], 4); //if it's not the first, connect to the last element
        }
        tmp.pol[x] = rt->createNode();
        if (x && ((type==GATE) || (type==GAP) || (x<nDiffEnd) || (x > nDiffIni && x < pDiffIni) || (x>pDiffEnd)))
            rt->addArc(tmp.pol[x], tmp.pol[x - 1], 6);
        
        //check if outside diff
        if ((x<nDiffEnd) || ((x > nDiffIni) && (x < pDiffIni)) || (x>pDiffEnd)) { //CHECK IF THERE IS ENOUGHT SPACE IN THE EXTERNAL AREA
            if(x>1 && x<trackPos.size()-1){
                if((type==SOURCE || type==DRAIN)) rt->addArc(tmp.pol[x], tmp.met[x], 200);
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

void AutoCell::calcArea(int nrIntTracks, int reduceMetTracks) {
    this->reduceMetTracks=reduceMetTracks;
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
    center = round((static_cast<float>(posNWell) / vGrid) - 0.5);
    trackPos.push_back(center * vGrid);
    
    //tracks position under the central track
    int next;
    do {
        next = trackPos.front() - currentRules->getRule(S1M1M1) - currentRules->getRule(W1M1);
        trackPos.insert(trackPos.begin(), next);
    } while (next >= supWidth); //stop when it touchs gnd
    center = static_cast<int>(trackPos.size()-1);
    
    //tracks position above the central track
    do {
        next = trackPos.back() + currentRules->getRule(S1M1M1) + currentRules->getRule(W1M1);
        trackPos.push_back(next);
    } while (next + currentRules->getRule(S2M1M1) <= height - supWidth); //stop when it touchs vdd
    
    //    for (int x = 0; x < trackPos.size(); x++) cout << static_cast<float>(trackPos[x]) / currentRules->getScale() << " ";
    
    //IMPROVE
    if(reduceMetTracks){
        nDif_iniY = min(nDif_iniY, (center+static_cast<int>(ceil(nrIntTracks/2.0)-1)) * vGrid - (currentRules->getRule(W2CT) / 2 + currentRules->getRule(E2P1CT) + currentRules->getRule(S1DFP1)));
        pDif_iniY = max(pDif_iniY, (center+static_cast<int>(floor(nrIntTracks/2.0))) * vGrid + (currentRules->getRule(W2CT) / 2 + currentRules->getRule(E2P1CT) + currentRules->getRule(S1DFP1)));
    }
    
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
    
    if(nSize<currentRules->getRule(W2DF) || pSize<currentRules->getRule(W2DF))
        throw AstranError("P/N diffusion space is too small to fit a transistor. Try to adjust the cell template: height, nwell position, routing grid pitch or the design rules");
    //       cout << "Resume: tracks(" << trackPos.size() << ") " << nSize << "N and " << pSize << "P" <<endl;
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

void AutoCell::autoFlow(string lpSolverFile){
    int nrTracks=2, bestNrTracks, conservative=0;
    bestNrTracks=nrTracks;
    time_t start,end;
    time (&start);
    
    while(1){
        while(1){
            cout << "-> Trying with " << nrTracks << " tracks and conservative = " << conservative << " ..." << endl;
            calcArea(nrTracks, conservative);
            foldTrans();
            placeTrans(false, 150, 3, 4, 4, 1, 4, 2); //try with , 8)
            if(currentNetList.getMaxCongestioning()<=6 && nrTracks==2) break;
            if(currentNetList.getMaxCongestioning()<=8 && nrTracks==3) break;
            if(nrTracks==4) break;
            nrTracks++;
        }
        placeTrans(true, 150, 3, 4, 4, 1, 4, 2);
        route(true, false, (currentNetList.getMaxCongestioning()<=5?true:false), true);
        if(compact(lpSolverFile, true, false, 50, 2, true, true, true, false, false, 3600)) break;
        conservative++;
        if(conservative>4)
            throw AstranError("Could not generate cell layout automatically");
    }
    
    time (&end);
    double dif = difftime (end,start);
    cout << "-> Total generation time for cell " << currentCell->getName() << ": " << dif << "s" << endl;
}

void AutoCell::foldTrans() {
    checkState(2);
    cout << "-> Applying folding..." << endl;
    cout << "-> Number of transistors before folding: " << currentNetList.size() << " -> P(" << currentNetList.pSize() << ") N(" << currentNetList.nSize() << ")" << endl;
    currentNetList.folding(static_cast<float>(pSize) / currentRules->getScale(), static_cast<float>(nSize) / currentRules->getScale(), currentCircuit->getVddNet(), currentCircuit->getGndNet());
    cout << "-> Number of transistors after folding: " << currentNetList.size() << " -> P(" << currentNetList.pSize() << ") N(" << currentNetList.nSize() << ")" << endl;
    currentNetList.getOrderingP().clear();
    currentNetList.getOrderingN().clear();
    state++;
}

void AutoCell::placeTrans(bool speculate, int saquality, int nrAttempts, int wC, int gmC, int rC, int congC, int ngC) {
    checkState(3);
    cout << "-> Placing transistors..." << endl;
    int bestCost, currentCost;
    
    if (speculate){
        vector<TransitorTerminal> bestNOrdering;
        vector<TransitorTerminal> bestPOrdering;
        //get previosly cost
        if(currentNetList.getOrderingP().size()!=0 && currentNetList.getOrderingN().size()!=0){
            state = 4;
            route(true, false,1, false);
            state = 3;
            bestCost=rt->getCost();
            bestNOrdering=currentNetList.getOrderingN();
            bestPOrdering=currentNetList.getOrderingP();
        }
        else bestCost=-1;
        
        for(int c=0; c<nrAttempts; c++){
            if (!currentNetList.transPlacement(true, saquality, 1, wC, gmC, rC, congC, ngC, currentCircuit->getVddNet(), currentCircuit->getGndNet()))
                throw AstranError("Could not place the transistors");
            state = 4;
            route(true, false,1, false);
            state =  3;
            currentCost=rt->getCost();
            if(bestCost==-1 || currentCost<bestCost){
                bestCost=currentCost;
                bestNOrdering=currentNetList.getOrderingN();
                bestPOrdering=currentNetList.getOrderingP();
                cout << "-> New best transistor ordering found with cost: " << bestCost << endl;
            }
        }
        currentNetList.setOrderingN(bestNOrdering);
        currentNetList.setOrderingP(bestPOrdering);
    }else{
        if (!currentNetList.transPlacement(true, saquality, nrAttempts, wC, gmC, rC, congC, ngC, currentCircuit->getVddNet(), currentCircuit->getGndNet()))
            throw AstranError("Could not place the transistors");
    }
    
    currentNetList.printPlacement();
    state++;
}

bool AutoCell::testGap(vector<TransitorTerminal>::iterator last_it, vector<TransitorTerminal>::iterator eulerPath_it, vector<TransitorTerminal>& ordering, TransTerminalType type){
    bool gap = false;
    //tests if there was a gap between last it and current
    if (eulerPath_it != ordering.begin() && eulerPath_it->link != -1 && type != GAP) {
        if ((last_it->type == SOURCE && (
                                         (eulerPath_it->type == SOURCE && currentNetList.getTrans(last_it->link).drain != currentNetList.getTrans(eulerPath_it->link).source) ||
                                         (eulerPath_it->type == DRAIN && currentNetList.getTrans(last_it->link).drain != currentNetList.getTrans(eulerPath_it->link).drain))) ||
            (last_it->type == DRAIN && (
                                        (eulerPath_it->type == SOURCE && currentNetList.getTrans(last_it->link).source != currentNetList.getTrans(eulerPath_it->link).source) ||
                                        (eulerPath_it->type == DRAIN && currentNetList.getTrans(last_it->link).source != currentNetList.getTrans(eulerPath_it->link).drain))) ||
            eulerPath_it->link == -1)
            gap = true;
    }
    
    return gap;
}

void AutoCell::route(bool hPoly, bool increaseIntTracks, int reduceVRt, bool optimize) {
    checkState(4);
    cout << "-> Routing cell...";
    
    this->hPoly=hPoly;
    this->reduceVRt=reduceVRt;
    
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
        for (p = center - 1; p && (trackPos[p] > nDif_iniY); --p);
        for (; p && increaseIntTracks && p>1 && trackPos[p] > nDif_endY + currentRules->getIntValue(currentNetList.getTrans(currentNetList.getOrderingN()[x].link).width)
             && trackPos[p] > supWidth + currentRules->getRule(S2M1M1)+ currentRules->getRule(S2M1M1); --p);
        diffNini.push_back(p);
    }
    
    // CONSTRUCT THE ROUTING GRAPH
    int COST_CNT_INSIDE_DIFF = 20;
    
    Element *tmp, *lastElement;
    bool gapP, gapN;
    
    elements.clear();
    inoutPins.clear();
    rt = make_unique<GraphRouter>();
    
    //        currentCell->print();
    vector<int>::iterator inouts_it = currentNetList.getInouts().begin();
    vdd=gnd=-1;
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
    tmp = createElement(16, center, center,center,center, GAP);
    
    //conecta sinais de entrada e saida com o nó inoutCnt do elemento
    map<string, int>::iterator inoutPins_it;
    for (inoutPins_it = inoutPins.begin(); inoutPins_it != inoutPins.end(); inoutPins_it++)
        rt->addArc(tmp->inoutCnt, inoutPins_it->second, 0);
    
    vector<TransitorTerminal>::iterator eulerPathP_it = currentNetList.getOrderingP().begin(), eulerPathN_it = currentNetList.getOrderingN().begin(), lastP_it, lastN_it;
    int nDiffTrackIni, pDiffTrackIni, nDiffTrackEnd, pDiffTrackEnd;
    
    while (eulerPathP_it != currentNetList.getOrderingP().end() && eulerPathN_it != currentNetList.getOrderingN().end()) {
        gapP = testGap(lastP_it, eulerPathP_it, currentNetList.getOrderingP(), tmp->linkP.type);
        gapN = testGap(lastN_it, eulerPathN_it, currentNetList.getOrderingN(), tmp->linkN.type);
        
        // DIFF
        nDiffTrackIni=diffNini[eulerPathN_it - currentNetList.getOrderingN().begin()];
        pDiffTrackIni=diffPini[eulerPathP_it - currentNetList.getOrderingP().begin()];
        //serch for first track above the transistor
        nDiffTrackEnd=nDiffTrackIni;
        pDiffTrackEnd=pDiffTrackIni;
        while (nDiffTrackEnd && (trackPos[nDiffTrackEnd-1] >= trackPos[nDiffTrackIni] - currentRules->getIntValue(currentNetList.getTrans(eulerPathN_it->link).width))) nDiffTrackEnd--;
        while (pDiffTrackEnd<trackPos.size()-1 && (trackPos[pDiffTrackEnd+1] <= trackPos[pDiffTrackIni] + currentRules->getIntValue(currentNetList.getTrans(eulerPathP_it->link).width))) pDiffTrackEnd++;
        
        if (gapP || gapN || eulerPathP_it == currentNetList.getOrderingP().begin() || eulerPathN_it == currentNetList.getOrderingN().begin()) {
            lastElement = tmp;
            tmp = createElement(4, nDiffTrackIni, pDiffTrackIni, nDiffTrackEnd, pDiffTrackEnd, SOURCE);
            
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
        tmp = createElement(16, nDiffTrackIni, pDiffTrackIni, nDiffTrackEnd, pDiffTrackEnd,GATE);
        
        if (eulerPathP_it->link != -1) { // nao é GAP na difusao P
            tmp->linkP = *eulerPathP_it;
            tmp->linkP.type = GATE;
            for (int pos = tmp->diffPIni; pos<=pDiffTrackEnd; pos++)
                rt->addNodetoNet(currentNetList.getTrans(eulerPathP_it->link).gate, tmp->pol[pos]);
        }
        
        for (inoutPins_it = inoutPins.begin(); inoutPins_it != inoutPins.end(); inoutPins_it++)
            rt->addArc(tmp->inoutCnt, inoutPins_it->second, 0);
        
        if (eulerPathN_it->link != -1) { // nao é GAP na difusao N
            tmp->linkN = *eulerPathN_it;
            tmp->linkN.type = GATE;
            for (int pos = tmp->diffNIni; pos>=nDiffTrackEnd; pos--)
                rt->addNodetoNet(currentNetList.getTrans(eulerPathN_it->link).gate, tmp->pol[pos]);
        }
        
        // DIFF
        lastElement = tmp;
        
        lastP_it = eulerPathP_it++;
        lastN_it = eulerPathN_it++;
        
        if(eulerPathP_it!= currentNetList.getOrderingP().end() && (eulerPathP_it->link == -1 || lastP_it->link == -1 || !testGap(lastP_it, eulerPathP_it, currentNetList.getOrderingP(), SOURCE))){
            if(lastElement->linkP.type == GAP){
                pDiffTrackEnd=0; pDiffTrackIni= static_cast<int>(trackPos.size()-1);
            }
            int tmp, proximo = static_cast<int>(eulerPathP_it - currentNetList.getOrderingP().begin());
            pDiffTrackIni=min(pDiffTrackIni, diffPini[proximo]);
            tmp=pDiffTrackIni;
            while ((tmp<trackPos.size()-1) && (trackPos[tmp+1] <= trackPos[diffPini[proximo]] + currentRules->getIntValue(currentNetList.getTrans(eulerPathP_it->link).width))){
                ++tmp;
            }
            pDiffTrackEnd=max(pDiffTrackEnd, tmp);
        }
        
        if(eulerPathN_it != currentNetList.getOrderingN().end() && (eulerPathN_it->link == -1 || lastN_it->link == -1 || !testGap(lastN_it, eulerPathN_it, currentNetList.getOrderingN(), SOURCE))){
            if(lastElement->linkN.type == GAP){
                nDiffTrackEnd=static_cast<int>(trackPos.size()-1); nDiffTrackIni=0;
            }
            int tmp, proximo = static_cast<int>(eulerPathN_it - currentNetList.getOrderingN().begin());;
            nDiffTrackIni=max(nDiffTrackIni, diffNini[proximo]);
            tmp=nDiffTrackIni;
            while (tmp && (trackPos[tmp-1] >= trackPos[diffNini[proximo]] - currentRules->getIntValue(currentNetList.getTrans(eulerPathN_it->link).width)))
                --tmp;
            nDiffTrackEnd=min(nDiffTrackEnd, tmp);
        }
        
        
        tmp = createElement(4, nDiffTrackIni, pDiffTrackIni, nDiffTrackEnd, pDiffTrackEnd,SOURCE);
        
        if (lastP_it->link != -1) { // nao é GAP na difusao P
            tmp->linkP = *lastP_it;
            if (lastP_it->type == DRAIN) {
                tmp->linkP.type = SOURCE;
                rt->addNodetoNet(currentNetList.getTrans(lastP_it->link).source, tmp->diffP);
            } else {
                tmp->linkP.type = DRAIN;
                rt->addNodetoNet(currentNetList.getTrans(lastP_it->link).drain, tmp->diffP);
            }
            for(int x = pDiffTrackIni; x<=pDiffTrackEnd; x++)
                rt->addArc(tmp->met[x], tmp->diffP, COST_CNT_INSIDE_DIFF);
        }
        
        
        if (lastN_it->link != -1) { // nao é GAP na difusao N
            tmp->linkN = *lastN_it;
            if (lastN_it->type == SOURCE) {
                tmp->linkN.type = DRAIN;
                rt->addNodetoNet(currentNetList.getTrans(lastN_it->link).drain, tmp->diffN);
            } else {
                tmp->linkN.type = SOURCE;
                rt->addNodetoNet(currentNetList.getTrans(lastN_it->link).source, tmp->diffN);
            }
            for(int x = nDiffTrackIni; x>=nDiffTrackEnd; x--)
                rt->addArc(tmp->met[x], tmp->diffN, COST_CNT_INSIDE_DIFF);
        }
        
        //conecta sinais de entrada e saida com o nó inoutCnt do elemento
        for (inoutPins_it = inoutPins.begin(); inoutPins_it != inoutPins.end(); inoutPins_it++)
            rt->addArc(tmp->inoutCnt, inoutPins_it->second, 0);
    }
    
    //cria elemento lateral para roteamento
    tmp = createElement(16, center, center,center,center,GAP);
    
    //conecta sinais de entrada e saida com o nó inoutCnt do elemento
    for (inoutPins_it = inoutPins.begin(); inoutPins_it != inoutPins.end(); inoutPins_it++)
        rt->addArc(tmp->inoutCnt, inoutPins_it->second, 0);
    
    
    if (!rt->routeNets(8000))
        throw AstranError("Unable to route this circuit");
    if(optimize) rt->optimize() && rt->optimize();
    state = 5;
}

bool AutoCell::compact(string lpSolverFile, int diffStretching, int griddedPoly, int rdCntsCost, int maxDiffCnts, int alignDiffConts, int reduceLturns, bool enableDFM, bool experimental, bool debug, int timeLimit) {
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
    
    Compaction cpt(CP_LP, "ILPmodel");
    vector<Box*> geometries;
    
    cpt.insertConstraint("ZERO", "UM", CP_EQ, 1);
    cpt.insertConstraint("ZERO", "RELAXATION", CP_EQ, relaxation);
    cpt.insertConstraint("ZERO", "HGRID_OFFSET", CP_EQ, (currentCircuit->getHGridOffset()?hGrid / 2:0));
    cpt.insertConstraint("ZERO", "VGRID_OFFSET", CP_EQ, (currentCircuit->getVGridOffset()?vGrid / 2:0));
    
    if(debug){
        cpt.insertConstraint("ZERO", "height", CP_MIN, height);
        cpt.insertLPMinVar("height", 100);
        cpt.insertConstraint("ZERO", "posNWell", CP_MIN, currentRules->getIntValue(currentCircuit->getnWellPos()));
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
    
    list<Element>::iterator lastElement_it;
    vector<string> lastMetNodes(trackPos.size(), ""), lastPolNodes(trackPos.size(), ""), lastContacts(trackPos.size(), "");
    vector<string> befLastMetNodes(trackPos.size(), ""), befLastPolNodes(trackPos.size(), "");
    int x;
    
    //  Transistor Variables
    bool gapP=false, gapN=false;
    string lastDiffP, lastDiffN, currentDiffP="", currentDiffN="", outPolP, outPolN, lastNGatePos, lastPGatePos;
    string lastNContact="", lastPContact="", lastNContactDiff="", lastPContactDiff="";
    int lastPGateLength=0, lastNGateLength=0;
    
    //Compact the routing tracks
    for (list<Element>::iterator elements_it = elements.begin(); elements_it != elements.end(); ++elements_it) {
        //                elements_it->print();
        vector<string> currentMetNodes(trackPos.size(), ""), currentPolNodes(trackPos.size(), ""), currentContacts(trackPos.size(), "");
        list<Element>::iterator next = elements_it; next++;
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
                        if(lastMetNodes[c]!="" && !rt->areConnected2(lastElement_it->met[c], elements_it->met[x])){
                            if(c<x)
                                insertDistanceRuleInteligent(geometries, cpt, lastMetNodes[c], currentMetNodes[x], lastMetNodes[c], currentMetNodes[x], MET1);
                            else if(c>x)
                                insertDistanceRuleInteligent(geometries, cpt, lastMetNodes[c], currentMetNodes[x], currentMetNodes[x], lastMetNodes[c], MET1);
                            else
                                insertDistanceRuleDumb(geometries, cpt, lastMetNodes[c], currentMetNodes[x], currentRules->getRule(S1M1M1), H, MET1);
                        }
                        if(befLastMetNodes[c]!=""){
                            if(experimental && c<x)
                                insertDistanceRuleInteligent(geometries, cpt, befLastMetNodes[c], currentMetNodes[x], befLastMetNodes[c], currentMetNodes[x], MET1);
                            else if(experimental && c>x)
                                insertDistanceRuleInteligent(geometries, cpt, befLastMetNodes[c], currentMetNodes[x], currentMetNodes[x], befLastMetNodes[c], MET1);
                            else
                                insertDistanceRuleDumb(geometries, cpt, befLastMetNodes[c], currentMetNodes[x], currentRules->getRule(S1M1M1), H, MET1);
                        }
                    }
                    for(int c=x-1; c>=0; --c){
                        if(currentMetNodes[c]!="" && !rt->areConnected2(elements_it->met[c], elements_it->met[x])){
                            insertDistanceRuleDumb(geometries, cpt, currentMetNodes[c], currentMetNodes[x], currentRules->getRule(S1M1M1), V, MET1);
                        }
                    }
                    
                    //insert space or a track between current and last V met node, if it exists
                    if(lastMetNodes[x]!="" && rt->areConnected2(lastElement_it->met[x], elements_it->met[x]) && x && x<trackPos.size()-1)
                        createTrack(geometries, cpt, lastMetNodes[x], currentMetNodes[x], currentNetList.getNetName(rt->getNet(elements_it->met[x])), MET1, H);
                    
                    if(x && rt->areConnected(elements_it->met[x-1], elements_it->met[x]))
                        createTrack(geometries, cpt, currentMetNodes[x-1], currentMetNodes[x], currentNetList.getNetName(rt->getNet(elements_it->met[x])), MET1, V);
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
                        if(lastPolNodes[c]!="" && !rt->areConnected2(lastElement_it->pol[c],elements_it->pol[x])) {
                            if(c<x)
                                insertDistanceRuleInteligent(geometries, cpt, lastPolNodes[c], currentPolNodes[x], lastPolNodes[c], currentPolNodes[x], POLY);
                            else if(c>x)
                                insertDistanceRuleInteligent(geometries, cpt, lastPolNodes[c], currentPolNodes[x], currentPolNodes[x], lastPolNodes[c], POLY);
                            else
                                insertDistanceRuleDumb(geometries, cpt, lastPolNodes[c], currentPolNodes[x], currentRules->getRule(S1P1P1), H, POLY);
                        }
                        if(befLastPolNodes[c]!=""){
                            if(experimental && c<x)
                                insertDistanceRuleInteligent(geometries, cpt, befLastPolNodes[c], currentPolNodes[x], befLastPolNodes[c], currentPolNodes[x], POLY);
                            else if(experimental && c>x)
                                insertDistanceRuleInteligent(geometries, cpt, befLastPolNodes[c], currentPolNodes[x], currentPolNodes[x], befLastPolNodes[c], POLY);
                            else insertDistanceRuleDumb(geometries, cpt, befLastPolNodes[c], currentPolNodes[x], currentRules->getRule(S1P1P1), H, POLY);
                        }
                    }
                    for(int c=x-1; c>=0; --c){
                        if(currentPolNodes[c]!="" && !rt->areConnected2(elements_it->pol[c],elements_it->pol[x])){
                            insertDistanceRuleDumb(geometries, cpt, currentPolNodes[c], currentPolNodes[x], currentRules->getRule(S1P1P1), V, POLY);
                        }
                    }
                    
                    //insert space or a track between current and last poly node, if it exists
                    if(lastPolNodes[x]!="" && rt->areConnected2(lastElement_it->pol[x], elements_it->pol[x]))
                        createTrack(geometries, cpt, lastPolNodes[x], currentPolNodes[x], "", POLY, H);
                    
                    if(x && rt->areConnected(elements_it->pol[x-1], elements_it->pol[x]))
                        createTrack(geometries, cpt, currentPolNodes[x-1], currentPolNodes[x], "", POLY, V);
                    
                    //space poly and last diff
                    if(!rt->isSource(elements_it->pol[x])){
                        if(currentDiffN!="")
                            insertDistanceRuleInteligent3(geometries, cpt, currentDiffN, currentPolNodes[x], currentPolNodes[x], currentDiffN, currentRules->getRule(S1DFP1),"");
                        if(lastDiffN!="")
                            insertDistanceRuleInteligent3(geometries, cpt, lastDiffN, currentPolNodes[x], currentPolNodes[x], lastDiffN, currentRules->getRule(S1DFP1),"");
                        if(currentDiffP!="")
                            insertDistanceRuleInteligent3(geometries, cpt, currentDiffP, currentPolNodes[x], currentPolNodes[x], currentDiffP, currentRules->getRule(S1DFP1),"");
                        if(lastDiffP!="")
                            insertDistanceRuleInteligent3(geometries, cpt, lastDiffP, currentPolNodes[x], currentPolNodes[x], lastDiffP, currentRules->getRule(S1DFP1),"");
                    }
                }
            }
            
            //conecta os polys com os metais das trilhas
            if (rt->areConnected(elements_it->met[x], elements_it->pol[x])) {
                insertCnt(geometries, cpt, elements_it, currentMetNodes, lastContacts, currentContacts, x, false);
                insertCntPol(geometries, cpt, currentContacts[x], currentPolNodes, x);
            }
            
            //se for entrada/saida, alinha o metal1 com a grade
            if (rt->areConnected(elements_it->met[x], elements_it->inoutCnt)) {
                IOgeometries[currentNetList.getNetName(rt->getNet(elements_it->inoutCnt))] = strToInt(insertVia(geometries, cpt, currentMetNodes[x],rt->getNrFinalArcs(elements_it->met[x])<=2));
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
            case DRAIN:{
                string cntBndBox;
                for (x = center; x >= 0; x--) { // desenha contatos na difusao
                    if (rt->areConnected(elements_it->met[x], elements_it->diffN)){
                        string currentCnt = insertCnt(geometries, cpt, elements_it, currentMetNodes, lastContacts, currentContacts, x,true);
                        if(cntBndBox=="") cntBndBox= "_CntsBoundBx"+to_string(index++)+ "_";
                        
                        cpt.insertConstraint("x" + cntBndBox + "a", "x" + currentCnt + "a", CP_MIN, 0);
                        cpt.insertConstraint("x" + currentCnt + "b", "x" + cntBndBox + "b", CP_MIN, 0);
                        cpt.insertConstraint("y" + cntBndBox + "a", "y" + currentCnt + "a", CP_MIN, 0);
                        cpt.insertConstraint("y" + currentCnt + "b", "y" + cntBndBox + "b", CP_MIN, 0);
                    }
                }
                string diffEnc;
                if(cntBndBox!="")
                    diffEnc = insertCntDif(geometries, cpt, cntBndBox, lastNGatePos, lastNContact,  NDIF);
                
                if(!rt->areConnected(next->diffN, elements_it->diffN)){
                    if(diffEnc=="" && rt->areConnected(lastElement_it->diffN, elements_it->diffN))
                        diffEnc=lastNContactDiff;
                    
                    newDif(geometries, cpt, lastNGatePos, lastDiffN, currentDiffN, diffEnc, NDIF, next->gapN==true || next->linkN.type==GAP);
                    
                    if(cntBndBox!="" && (next->gapN==true || next->linkN.type==GAP)) lastNGatePos = "";
                }
                if(gapN && lastNContactDiff!=""  && diffEnc!="")
                    cpt.insertConstraint("x" + lastNContactDiff + "b", "x" + diffEnc + "a", CP_MIN, currentRules->getRule(S1DFDF));
                if(diffEnc!="")
                    lastNContactDiff = diffEnc;
                gapN = false;
                break;
            }
            case GATE:
                lastNGatePos = insertGate(geometries, cpt,elements_it->linkN.link, elements_it, currentPolNodes, lastNContact, lastNContactDiff, lastNGatePos, lastNGateLength, lastDiffN, currentDiffN, NDIF);
                lastNContactDiff="";
                
                break;
        }
        switch (elements_it->linkP.type) {
            case GAP:
                gapP = true;
                break;
            case SOURCE:
            case DRAIN:{
                string cntBndBox;
                for (x = center; x < elements_it->met.size(); ++x) { // desenha contatos na difusao
                    if (rt->areConnected(elements_it->met[x], elements_it->diffP)) {
                        string currentCnt = insertCnt(geometries, cpt, elements_it, currentMetNodes, lastContacts, currentContacts, x,true);
                        if(cntBndBox=="") cntBndBox= "_CntsBoundBx"+to_string(index++)+ "_";
                        
                        cpt.insertConstraint("x" + cntBndBox + "a", "x" + currentCnt + "a", CP_MIN, 0);
                        cpt.insertConstraint("x" + currentCnt + "b", "x" + cntBndBox + "b", CP_MIN, 0);
                        cpt.insertConstraint("y" + cntBndBox + "a", "y" + currentCnt + "a", CP_MIN, 0);
                        cpt.insertConstraint("y" + currentCnt + "b", "y" + cntBndBox + "b", CP_MIN, 0);
                    }
                }
                string diffEnc;
                if(cntBndBox!="")
                    diffEnc = insertCntDif(geometries, cpt, cntBndBox, lastPGatePos, lastPContact, PDIF);
                
                if(!rt->areConnected(next->diffP, elements_it->diffP)) {
                    if(diffEnc=="" && rt->areConnected(lastElement_it->diffP, elements_it->diffP))
                        diffEnc=lastPContactDiff;
                    
                    newDif(geometries, cpt, lastPGatePos, lastDiffP, currentDiffP, diffEnc, PDIF, next->gapP==true || next->linkP.type==GAP);
                    if(cntBndBox!="" && (next->gapP==true || next->linkP.type==GAP)) lastPGatePos = "";
                }
                if(gapP && lastPContactDiff!="" && diffEnc!="")
                    cpt.insertConstraint("x" + lastPContactDiff + "b", "x" + diffEnc + "a", CP_MIN, currentRules->getRule(S1DFDF));
                if(diffEnc!="")
                    lastPContactDiff = diffEnc;
                
                gapP = false;
                break;
            }
            case GATE:
                lastPGatePos = insertGate(geometries, cpt, elements_it->linkP.link, elements_it, currentPolNodes, lastPContact, lastPContactDiff, lastPGatePos, lastPGateLength, lastDiffP, currentDiffP, PDIF);
                lastPContactDiff="";
                break;
        }
        //               cout << elements_it->diffNEnd << "-" << elements_it->diffNIni << "/" << elements_it->diffPIni << "-" << elements_it->diffPEnd << endl;
        
        for (int c = 0; c < trackPos.size(); c++) {
            //insert contacts to diff space rule
            if(currentContacts[c]!=""){
                if(elements_it->linkN.type!=GAP && !rt->areConnected(elements_it->met[c], elements_it->diffN && !rt->areConnected(next->diffN, elements_it->diffN))){
                    if(c<elements_it->diffNEnd){
                        if(currentDiffN!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], currentDiffN, currentContacts[c], currentDiffN, currentRules->getRule(S1CTDF),"");
                        if(lastDiffN!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], lastDiffN, currentContacts[c], lastDiffN, currentRules->getRule(S1CTDF),"");
                    }else if(c>elements_it->diffNIni){
                        if(currentDiffN!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], currentDiffN, currentDiffN, currentContacts[c], currentRules->getRule(S1CTDF),"");
                        if(lastDiffN!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], lastDiffN, lastDiffN, currentContacts[c], currentRules->getRule(S1CTDF),"");
                    }
                    
                }
                
                if(elements_it->linkP.type!=GAP && !rt->areConnected(elements_it->met[c], elements_it->diffP && !rt->areConnected(next->diffP, elements_it->diffP))){
                    if(c<elements_it->diffPIni){
                        if(currentDiffP!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], currentDiffP, currentContacts[c], currentDiffP, currentRules->getRule(S1CTDF),"");
                        if(lastDiffP!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], lastDiffP, currentContacts[c], lastDiffP, currentRules->getRule(S1CTDF),"");
                    }else if(c>elements_it->diffPEnd){
                        if(currentDiffP!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], currentDiffP, currentDiffP, currentContacts[c], currentRules->getRule(S1CTDF),"");
                        if(lastDiffP!="") insertDistanceRuleInteligent2(geometries, cpt, currentContacts[c], lastDiffP, lastDiffP, currentContacts[c], currentRules->getRule(S1CTDF),"");
                    }
                }
            }
            
            //insert diff to poly space rule
            if(elements_it->linkN.type!=GAP && currentDiffN!="" && !rt->areConnected(next->diffN, elements_it->diffN)){
                if(c<elements_it->diffNEnd && currentPolNodes[c]!="")
                    insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[c], currentDiffN, currentPolNodes[c], currentDiffN, currentRules->getRule(S1DFP1),"");
                if(c<lastElement_it->diffNEnd && lastPolNodes[c]!="")
                    insertDistanceRuleInteligent2(geometries, cpt, lastPolNodes[c], currentDiffN, lastPolNodes[c], currentDiffN, currentRules->getRule(S1DFP1),"");
                if(c>elements_it->diffNIni && currentPolNodes[c]!="")
                    insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[c], currentDiffN, currentDiffN, currentPolNodes[c], currentRules->getRule(S1DFP1),"");
                if(c>lastElement_it->diffNIni && lastPolNodes[c]!="")
                    insertDistanceRuleInteligent2(geometries, cpt, lastPolNodes[c], currentDiffN, currentDiffN, lastPolNodes[c], currentRules->getRule(S1DFP1),"");
                if((c>=elements_it->diffNEnd && c<=elements_it->diffNIni) &&
                   lastPolNodes[c]!="" && !rt->isSource(lastElement_it->pol[c]) && elements_it->linkN.type!=GATE)
                    insertDistanceRuleDumb(geometries, cpt, lastPolNodes[c], currentDiffN, currentRules->getRule(S1DFP1), H, NDIF);
            }
            if(elements_it->linkP.type!=GAP && currentDiffP!="" && !rt->areConnected(next->diffP, elements_it->diffP)){
                if(c<elements_it->diffPIni && currentPolNodes[c]!="")
                    insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[c], currentDiffP, currentPolNodes[c], currentDiffP, currentRules->getRule(S1DFP1),"");
                if(c<lastElement_it->diffPIni && lastPolNodes[c]!="")
                    insertDistanceRuleInteligent2(geometries, cpt, lastPolNodes[c], currentDiffP, lastPolNodes[c], currentDiffP, currentRules->getRule(S1DFP1),"");
                if(c>elements_it->diffPEnd && currentPolNodes[c]!="")
                    insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[c], currentDiffP, currentDiffP, currentPolNodes[c], currentRules->getRule(S1DFP1),"");
                if(c>lastElement_it->diffPEnd && lastPolNodes[c]!="")
                    insertDistanceRuleInteligent2(geometries, cpt, lastPolNodes[c], currentDiffP, currentDiffP, lastPolNodes[c], currentRules->getRule(S1DFP1),"");
                if((c>=elements_it->diffPIni && c<=elements_it->diffPEnd) &&
                   lastPolNodes[c]!="" && !rt->isSource(lastElement_it->pol[c]) && elements_it->linkP.type!=GATE)
                    insertDistanceRuleDumb(geometries, cpt, lastPolNodes[c], currentDiffP, currentRules->getRule(S1DFP1), H, PDIF);
            }
        }
        
        //        cout << lastDiffN << "-" << currentDiffN << "/" << lastDiffP << "-" << currentDiffP << endl;
        lastElement_it = elements_it;
        for (x = 0; x < trackPos.size(); x++){
            if(currentMetNodes[x]!=""){
                befLastMetNodes[x]=lastMetNodes[x];
                lastMetNodes[x]=currentMetNodes[x];
            }
            if(currentPolNodes[x]!=""){
                befLastPolNodes[x]=lastPolNodes[x];
                lastPolNodes[x]=currentPolNodes[x];
            }
            if(currentContacts[x]!="")
                lastContacts[x]=currentContacts[x];
        }
    }
    
    
    
    cpt.insertConstraint("ZERO", "width", CP_MIN, 0);
    cpt.insertConstraint("ZERO", "width", CP_EQ_VAR_VAL, "width_gpos", hGrid);
    cpt.forceIntegerVar("width_gpos");
    cpt.insertLPMinVar("width", 5000);
    
    if (!cpt.solve(lpSolverFile, timeLimit))
        return false;
    
    for (int i = 0; i < geometries.size(); i++) {
        
        int xa = cpt.getVariableVal("x" + to_string(i) + "a");
        int xb = cpt.getVariableVal("x" + to_string(i) + "b");
        int ya = cpt.getVariableVal("y" + to_string(i) + "a");
        int yb = cpt.getVariableVal("y" + to_string(i) + "b");
        
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
    int height = cpt.getVariableVal("height");
    
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
        //search for blocking regions in the PTAP area add its interval to a list
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
        //search for empty regions in the list and insert the PTAP geometries where it fits
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
        
        //search for blocking regions in the NTAP area add its interval to a list
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
        //search for empty regions in the list and insert the NTAP geometries where it fits
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
        
        if(!btP) cout << "-> WARNING: Could not insert PTAPs" << endl;
        if(!btN) cout << "-> WARNING: Could not insert NTAPs" << endl;
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
    currentLayout.addPolygon(-nWellBorder, -nWellBorder, width + nWellBorder, cpt.getVariableVal("posNWell"), PWEL);
    
    
    //	currentLayout.merge();
    currentCircuit->insertLayout(currentLayout);
    cout << "-> Cell Size (W x H): " << static_cast<float>(currentLayout.getWidth()) / currentRules->getScale() << " x " << static_cast<float>(currentLayout.getHeight()) / currentRules->getScale() << endl;
    state++;
    return true;
}


string AutoCell::insertGate(vector<Box*> &geometries, Compaction &cpt, int transistor, list<Element>::iterator elements_it, vector<string> &currentPolNodes, string lastCnt, string diffEnc, string &lastGate, int &lastGateLength, string lastDiff, string currentDiff, layer_name l){
    int gateIni, gateEnd;
    int transWidth = round(currentNetList.getTrans(transistor).width * currentRules->getScale());
    int transLength = round(currentNetList.getTrans(transistor).length * currentRules->getScale());
    if (transLength < currentRules->getRule(W2P1))
        throw AstranError("-> Gate length of transistor " + currentNetList.getTrans(transistor).name + " is smaller than the minimum of the technology");
    
    if(transWidth < currentRules->getRule(W2DF))
        throw AstranError("-> Gate Width of transistor " + currentNetList.getTrans(transistor).name + " is smaller than the minimum of the technology");
    
    //get gate
    if (l==NDIF){
        gateIni = elements_it->diffNIni;
        gateEnd = elements_it->diffNEnd;
    }else{
        gateIni = elements_it->diffPIni;
        gateEnd = elements_it->diffPEnd;
    }
    //minimum Area
    if(transWidth>=currentRules->getRule(W2CT)+2*currentRules->getRule(E1DFCT))
        cpt.insertConstraint("x" + currentDiff + "a", "x" + currentDiff + "b", CP_MIN, currentRules->getIntValue(currentRules->getRulef(A1DF)/currentNetList.getTrans(transistor).width));
    
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
    }else{
        if(gateIni && currentPolNodes[gateIni-1]!="")
            insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[gateIni-1], currentPolNodes[gateIni], currentPolNodes[gateIni-1], currentDiff, currentRules->getRule(S2DFP1),"b" + currentDiff + "_smallTransWidth");
        if(gateEnd+1 < currentPolNodes.size() && currentPolNodes[gateEnd+1]!="")
            insertDistanceRuleInteligent2(geometries, cpt, currentPolNodes[gateEnd+1], currentPolNodes[gateEnd], currentDiff, currentPolNodes[gateEnd+1], currentRules->getRule(S2DFP1),"b" + currentDiff + "_smallTransWidth");
    }
    
    //gate extension rule for L shape transistor if diff dist to gate < E3P1DF
    cpt.forceBinaryVar("b" + currentDiff + "_LshapeBeforeGate");
    cpt.forceBinaryVar("b" + currentDiff + "_LshapeBeforeGateIn");
    cpt.forceBinaryVar("b" + currentDiff + "_LshapeBeforeGateOut");
    cpt.forceBinaryVar("b" + currentPolNodes[gateIni] + "_applyExtraExtBeforeGate");
    cpt.insertConstraint("b" + currentDiff + "_applyS2BeforeGate", "b" + currentDiff + "_LshapeBeforeGate", CP_MAX, "b" + currentPolNodes[gateIni] + "_applyExtraExtBeforeGate");
    cpt.insertConstraint("y" + currentPolNodes[gateIni] + "a", "y" + currentDiff + "a", CP_MIN, currentRules->getRule(E1P1DF));
    cpt.insertConstraint("y" + currentPolNodes[gateIni] + "a", "y" + currentDiff + "a", CP_MIN, "b" + currentPolNodes[gateIni] + "_applyExtraExtBeforeGate", currentRules->getRule(E2P1DF));
    cpt.insertConstraint("y" + currentDiff + "b", "y" + currentPolNodes[gateIni] + "b", CP_MIN, currentRules->getRule(E1P1DF));
    cpt.insertConstraint("y" + currentDiff + "b", "y" + currentPolNodes[gateIni] + "b", CP_MIN, "b" + currentPolNodes[gateIni] + "_applyExtraExtBeforeGate", currentRules->getRule(E2P1DF));
    
    //minimize gate size
    cpt.insertConstraint("y" + currentPolNodes[gateIni] + "a", "y" + currentPolNodes[gateIni] + "b", CP_EQ, "y" + currentPolNodes[gateIni] + "min");
    cpt.insertLPMinVar("y" + currentPolNodes[gateIni] + "min");
    
    //insert conditional diff to gate rule in L shape transistors considering S3DFP1 (big transistor width)
    if (diffEnc != "")
        cpt.insertConstraint("x" + diffEnc + "b", "x" + currentPolNodes[gateIni] + "a", CP_MIN, "b" + currentDiff + "_LshapeBeforeGate", currentRules->getRule(S1DFP1));
    if (lastDiff != "")
        cpt.insertConstraint("x" + lastDiff + "b", "x" + currentPolNodes[gateIni] + "a", CP_MIN, "b" + currentDiff + "_LshapeBeforeGate", currentRules->getRule(S1DFP1));
    cpt.insertConstraint("ZERO", "y" + currentDiff + "LdistBeforeGateOut", CP_MAX, "b" + currentDiff + "_LshapeBeforeGateOut", 100000);
    cpt.insertConstraint("ZERO", "y" + currentDiff + "LdistBeforeGateIn", CP_MAX, "b" + currentDiff + "_LshapeBeforeGateIn", 100000);
    cpt.insertConstraint("ZERO", "b" + currentDiff + "_LshapeBeforeGateOut", CP_MAX, "b" + currentDiff + "_LshapeBeforeGate");
    cpt.insertConstraint("ZERO", "b" + currentDiff + "_LshapeBeforeGateIn", CP_MAX, "b" + currentDiff + "_LshapeBeforeGate");
    
    
    cpt.insertConstraint("UM", "b" + currentDiff + "_smallTransWidth" + " + " + "b" + currentDiff + "_LshapeBeforeGate", CP_MAX, "b" + currentDiff + "_applyS2BeforeGate");
    
    //U shape diffusion extra spacing
    cpt.forceBinaryVar("b" + currentDiff + "_Ushape");
    cpt.insertConstraint("UM", "b" + currentDiff + "_LshapeBeforeGateIn + " + "b" + currentDiff + "_LshapeAfterGateIn", CP_MAX, "b" + currentDiff + "_Ushape");
    cpt.insertConstraint("UM", "b" + currentDiff + "_LshapeBeforeGateOut + " + "b" + currentDiff + "_LshapeAfterGateOut", CP_MAX, "b" + currentDiff + "_Ushape");

    if (diffEnc != ""){
        cpt.insertConstraint("x" + diffEnc + "b", "x" + currentPolNodes[gateIni] + "a", CP_MIN, "b" + currentDiff + "_applyS2BeforeGate", currentRules->getRule(S2DFP1));
        cpt.insertConstraint("ZERO", "x" + diffEnc + "b", CP_MAX, "x" + currentDiff + "_lastDiffEnc_b");
    }
    if (lastDiff != ""){
        cpt.insertConstraint("x" + lastDiff + "b", "x" + currentPolNodes[gateIni] + "a", CP_MIN, "b" + currentDiff + "_applyS2BeforeGate", currentRules->getRule(S2DFP1));
        cpt.insertConstraint("ZERO", "x" + lastDiff + "b", CP_MAX, "x" + currentDiff + "_lastDiffEnc_b");
    }
    
    
    //contact to gate distance
    cpt.insertConstraint("x" + lastCnt + "b", "x" + currentPolNodes[gateIni] + "a", CP_MIN, currentRules->getRule(S1CTP1));
    
    lastGate = currentPolNodes[gateIni];
    lastGateLength = transLength;
    return currentPolNodes[gateIni];
}

string AutoCell::insertCntDif(vector<Box*> &geometries, Compaction &cpt, string currentCnt, string &lastGate, string &lastCnt, layer_name l) {
    
    geometries.push_back(&currentLayout.addEnc(*geometries.back(), 0, l));
    string diffEnc = to_string(geometries.size() - 1);
    
    //diffusion enclosure of contact - 3 sides E2DFCT, one side E1DFCT  (comentar que ele coloca o lado menor em cima ou embaixo tbm)
    cpt.forceBinaryVar("b" + diffEnc + "_v");
    cpt.forceBinaryVar("b" + diffEnc + "_h");
    cpt.insertConstraint("ZERO", "b" + diffEnc + "_v + b" + diffEnc + "_h", CP_EQ, 1);
    cpt.insertConstraint("x" + diffEnc + "a", "x" + currentCnt + "a", CP_MIN, currentRules->getRule(E1DFCT));
    cpt.insertConstraint("x" + diffEnc + "a", "x" + currentCnt + "a", CP_MIN, "b" + diffEnc + "_h", currentRules->getRule(E2DFCT));
    cpt.insertConstraint("x" + currentCnt + "b", "x" + diffEnc + "b", CP_MIN, currentRules->getRule(E1DFCT));
    cpt.insertConstraint("x" + currentCnt + "b", "x" + diffEnc + "b", CP_MIN, "b" + diffEnc + "_h", currentRules->getRule(E2DFCT));
    cpt.insertConstraint("y" + diffEnc + "a", "y" + currentCnt + "a", CP_MIN, currentRules->getRule(E1DFCT));
    cpt.insertConstraint("y" + diffEnc + "a", "y" + currentCnt + "a", CP_MIN, "b" + diffEnc + "_v", currentRules->getRule(E2DFCT));
    cpt.insertConstraint("y" + currentCnt + "b", "y" + diffEnc + "b", CP_MIN, currentRules->getRule(E1DFCT));
    cpt.insertConstraint("y" + currentCnt + "b", "y" + diffEnc + "b", CP_MIN, "b" + diffEnc + "_v", currentRules->getRule(E2DFCT));
    
    if (l==NDIF) {
        cpt.insertConstraint("yNDiffa", "y" + diffEnc + "a", CP_MIN, 0);
        cpt.insertConstraint("y" + diffEnc + "b", "yNDiffb", CP_MIN, 0);
    }else{
        cpt.insertConstraint("yPDiffa", "y" + diffEnc + "a", CP_MIN, 0);
        cpt.insertConstraint("y" + diffEnc + "b", "yPDiffb", CP_MIN, 0);
    }
    
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
    }
    lastCnt = currentCnt;
    return diffEnc;
}



string AutoCell::newDif(vector<Box*> &geometries, Compaction &cpt, string &lastGate, string &lastDiff, string &currentDiff, string &diffEnc,  layer_name l, bool endDiff) {
    
    lastDiff = currentDiff;
    
    if (diffEnc != "" && lastDiff != ""){
        cpt.insertConstraint("y" + diffEnc + "a", "y" + lastDiff + "b", CP_MIN, 0);
        cpt.insertConstraint("y" + lastDiff + "a", "y" + diffEnc + "b", CP_MIN, 0);
    }
    //if there is a gate after
    if(!endDiff){
        //create next gate diffusion
        geometries.push_back(&currentLayout.addPolygon(0, 0, 0, 0, l));
        string newDiff = to_string(geometries.size() - 1);
        
        cpt.insertConstraint("x" + newDiff + "a", "x" + newDiff + "b", CP_EQ, "x" + newDiff + "min");
        cpt.insertLPMinVar("x" + newDiff + "min",4);
        
        cpt.insertConstraint("ZERO", "x" + newDiff + "a", CP_MIN, static_cast<int>(ceil(currentRules->getRule(S1DFDF) / 2.0)));
        cpt.insertConstraint("x" + newDiff + "b", "width", CP_MIN, static_cast<int>(ceil(currentRules->getRule(S1DFDF) / 2.0)));
        if (l==NDIF) {
            cpt.insertConstraint("yNDiffa", "y" + newDiff + "a", CP_MIN, 0);
            cpt.insertConstraint("y" + newDiff + "b", "yNDiffb", CP_MIN, 0);
        }else{
            cpt.insertConstraint("yPDiffa", "y" + newDiff + "a", CP_MIN, 0);
            cpt.insertConstraint("y" + newDiff + "b", "yPDiffb", CP_MIN, 0);
        }
        
        if (diffEnc != "") {
            cpt.insertConstraint("y" + diffEnc + "a", "y" + newDiff + "b", CP_MIN, 0);
            cpt.insertConstraint("y" + newDiff + "a", "y" + diffEnc + "b", CP_MIN, 0);
            cpt.insertConstraint("x" + newDiff + "a", "x" + diffEnc + "a", CP_EQ, 0);
            //        cpt.insertConstraint("x" + diffEnc + "a", "x" + newDiff + "a", CP_EQ, "x" + newDiff + "minL");
            //        cpt.insertLPMinVar("x" + newDiff + "minL", 5);
        }
        //merge new diffusion to the last one and maximize intersection
        if (lastGate != "") {
            if (lastDiff=="")  throw AstranError("Float source/drain is not supported by ASTRAN (net degree = 1). Check your spice netlist!");
            cpt.insertConstraint("x" + newDiff + "a", "x" + lastDiff + "b", CP_MIN, 0);
            cpt.insertConstraint("y" + newDiff + "b_int", "y" + lastDiff + "b", CP_MIN, 0);
            cpt.insertConstraint("y" + newDiff + "b_int", "y" + newDiff + "b", CP_MIN, 0);
            cpt.insertConstraint("y" + lastDiff + "a", "y" + newDiff + "a_int", CP_MIN, 0);
            cpt.insertConstraint("y" + newDiff + "a", "y" + newDiff + "a_int", CP_MIN, 0);
            cpt.insertConstraint( "y" + newDiff + "a_int", "y" + newDiff + "b_int", CP_EQ,  "y" + newDiff + "a_int_min");
            cpt.insertConstraint( "y" + newDiff + "a_int", "y" + newDiff + "b_int", CP_MIN, currentRules->getRule(W2DF));
        }else{
            //insert GAP between diffusions if it's not the first diff
            if(lastDiff!="") {
                cpt.insertConstraint("x" + lastDiff + "b", "x" + newDiff + "a", CP_MIN, currentRules->getRule(S1DFDF));
                lastDiff="";
            }
        }
        
        //diffusion extension rules for next gate diffusion
        if (l==NDIF) {
            if (diffEnc != ""){
                cpt.insertConstraint("y" + diffEnc + "b", "y" + newDiff + "b", CP_MIN, 0);
                cpt.insertConstraint("y" + newDiff + "LdistBeforeGateOut", "y" + newDiff + "a", CP_MAX, "y" + diffEnc + "a");
                cpt.insertConstraint("y" + newDiff + "LdistBeforeGateIn", "y" + diffEnc + "b", CP_MAX, "y" + newDiff + "b");
            }
            if(lastDiff!=""){
                cpt.insertConstraint("y" + newDiff + "LdistBeforeGateOut", "y" + newDiff + "a", CP_MAX, "y" + lastDiff + "a");
                cpt.insertConstraint("y" + newDiff + "LdistBeforeGateIn", "y" + lastDiff + "b", CP_MAX, "y" + newDiff + "b");
                cpt.insertConstraint("y" + lastDiff + "LdistAfterGateOut", "y" + lastDiff + "a", CP_MAX, "y" + newDiff + "a");
                cpt.insertConstraint("y" + lastDiff + "LdistAfterGateIn", "y" + newDiff + "b", CP_MAX, "y" + lastDiff + "b");
            }
        }else{
            if (diffEnc != "") {
                cpt.insertConstraint("y" + newDiff + "a", "y" + diffEnc + "a", CP_MIN, 0);
                cpt.insertConstraint("y" + newDiff + "LdistBeforeGateOut", "y" + diffEnc + "b", CP_MAX, "y" + newDiff + "b");
                cpt.insertConstraint("y" + newDiff + "LdistBeforeGateIn", "y" + newDiff + "a", CP_MAX, "y" + diffEnc + "a");
            }
            if(lastDiff!=""){
                cpt.insertConstraint("y" + newDiff + "LdistBeforeGateOut", "y" + lastDiff + "b", CP_MAX, "y" + newDiff + "b");
                cpt.insertConstraint("y" + newDiff + "LdistBeforeGateIn", "y" + newDiff + "a", CP_MAX, "y" + lastDiff + "a");
                cpt.insertConstraint("y" + lastDiff + "LdistAfterGateOut", "y" + newDiff + "b", CP_MAX, "y" + lastDiff + "b");
                cpt.insertConstraint("y" + lastDiff + "LdistAfterGateIn", "y" + lastDiff + "a", CP_MAX, "y" + newDiff + "a");
            }
        }
        cpt.insertLPMinVar("y" + newDiff + "LdistBeforeGateOut",(diffStretching?9:40));
        cpt.insertLPMinVar("y" + newDiff + "LdistBeforeGateIn",(diffStretching?9:40));
        currentDiff = newDiff;
    }else
        currentDiff = "";
    
    
    //if there is a gate before
    if (lastGate != "") {
        if (diffEnc != "") cpt.insertConstraint("x" + diffEnc + "b", "x" + lastDiff + "b", CP_EQ, 0);
        if (currentDiff != "") cpt.insertConstraint("x" + currentDiff + "a", "x" + lastDiff + "b", CP_MIN, 0);
        //        cpt.insertConstraint("x" + currentDiff + "b", "x" + diffEnc + "b", CP_EQ, "x" + currentDiff + "minR");
        //        cpt.insertLPMinVar("x" + currentDiff + "minR", 5);
        
        //gate extension rule for L shape transistor if diff dist to gate < E3P1DF
        cpt.forceBinaryVar("b" + lastDiff + "_LshapeAfterGate");
        cpt.forceBinaryVar("b" + lastDiff + "_LshapeAfterGateIn");
        cpt.forceBinaryVar("b" + lastDiff + "_LshapeAfterGateOut");
        
        cpt.insertConstraint("b" + lastDiff + "_applyS2AfterGate", "b" + lastDiff + "_LshapeAfterGate", CP_MAX, "b" + lastGate + "_applyExtraExtAfterGate");
        cpt.insertConstraint("y" + lastGate + "a", "y" + lastDiff + "a", CP_MIN, currentRules->getRule(E1P1DF));
        cpt.insertConstraint("y" + lastGate + "a", "y" + lastDiff + "a", CP_MIN, "b" + lastGate + "_applyExtraExtAfterGate", currentRules->getRule(E2P1DF));
        cpt.insertConstraint("y" + lastDiff + "b", "y" + lastGate + "b", CP_MIN, currentRules->getRule(E1P1DF));
        cpt.insertConstraint("y" + lastDiff + "b", "y" + lastGate + "b", CP_MIN, "b" + lastGate + "_applyExtraExtAfterGate", currentRules->getRule(E2P1DF));
        
        //insert conditional diff to gate rule in L shape transistors considering S3DFP1 (big transistor width)
        if (diffEnc != "")
            cpt.insertConstraint("x" + lastGate + "b", "x" + diffEnc + "a", CP_MIN, "b" + lastDiff + "_LshapeAfterGate", currentRules->getRule(S1DFP1));
        if (currentDiff != "") cpt.insertConstraint("x" + lastGate + "b", "x" + currentDiff + "a", CP_MIN, "b" + lastDiff + "_LshapeAfterGate", currentRules->getRule(S1DFP1));
        
        cpt.insertConstraint("ZERO", "y" + lastDiff + "LdistAfterGateOut", CP_MAX, "b" + lastDiff + "_LshapeAfterGateOut", 100000);
        cpt.insertConstraint("ZERO", "y" + lastDiff + "LdistAfterGateIn", CP_MAX, "b" + lastDiff + "_LshapeAfterGateIn", 100000);
        cpt.insertConstraint("ZERO", "b" + lastDiff + "_LshapeAfterGateOut", CP_MAX, "b" + lastDiff + "_LshapeAfterGate");
        cpt.insertConstraint("ZERO", "b" + lastDiff + "_LshapeAfterGateIn", CP_MAX, "b" + lastDiff + "_LshapeAfterGate");
        
        cpt.insertConstraint("UM", "b" + lastDiff + "_smallTransWidth" + " + " + "b" + lastDiff + "_LshapeAfterGate", CP_MAX, "b" + lastDiff + "_applyS2AfterGate");
        if (diffEnc != "")
            cpt.insertConstraint("x" + lastGate + "b", "x" + diffEnc + "a", CP_MIN, "b" + lastDiff + "_applyS2AfterGate", currentRules->getRule(S2DFP1));
        if (currentDiff != "")
            cpt.insertConstraint("x" + lastGate + "b", "x" + currentDiff + "a", CP_MIN, "b" + lastDiff + "_applyS2AfterGate", currentRules->getRule(S2DFP1));
        
        //diffusion extension rules for last gate diffusion
        if(lastDiff!="" && diffEnc != ""){
            if (l==NDIF) {
                cpt.insertConstraint("y" + diffEnc + "b", "y" + lastDiff + "b", CP_MIN, 0);
                cpt.insertConstraint("y" + lastDiff + "LdistAfterGateOut", "y" + lastDiff + "a", CP_MAX, "y" + diffEnc + "a");
                cpt.insertConstraint("y" + lastDiff + "LdistAfterGateIn", "y" + diffEnc + "b", CP_MAX, "y" + lastDiff + "b");
            }else{
                cpt.insertConstraint("y" + lastDiff + "a", "y" + diffEnc + "a", CP_MIN, 0);
                cpt.insertConstraint("y" + lastDiff + "LdistAfterGateOut", "y" + diffEnc + "b", CP_MAX, "y" + lastDiff + "b");
                cpt.insertConstraint("y" + lastDiff + "LdistAfterGateIn", "y" + lastDiff + "a", CP_MAX, "y" + diffEnc + "a");
            }
        }
        cpt.insertLPMinVar("y" + lastDiff + "LdistAfterGateOut",(diffStretching?9:40));
        cpt.insertLPMinVar("y" + lastDiff + "LdistAfterGateIn",(diffStretching?9:40));
        
        //U shape spacing
        if (diffEnc != "")
            cpt.insertConstraint("x" + lastDiff + "_lastDiffEnc_b", "x" + diffEnc + "a", CP_MIN, "b" + lastDiff + "_Ushape", currentRules->getRule(S2DFDF));
        
    }
    
    return diffEnc;
}

string AutoCell::insertCnt(vector<Box*> &geometries, Compaction &cpt, list<Element>::iterator elements_it, vector<string>& metTracks, vector<string>& lastContacts, vector<string>& currentContacts, int pos, bool isDiff) {
    geometries.push_back(&currentLayout.addLayer(0, 0, 0, 0, CONT));
    string cnt = to_string(geometries.size() - 1);
    
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
            string cnt2 = to_string(geometries.size() - 1);
            cpt.forceBinaryVar("b" + cnt2); // 2nd contact
            cpt.insertLPMinVar("b" + cnt2, -rdCntsCost*9);
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
    
    //   cpt.insertConstraint("ZERO", "b" + cntBndBox + "_3M", CP_EQ, 1);
    
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
        if(currentContacts[c]!="" && c<pos)
            insertDistanceRuleInteligent(geometries, cpt, currentContacts[c], cntBndBox, currentContacts[c], cntBndBox, CONT);
        
        if(lastContacts[c]!=""){
            if(c<pos)
                insertDistanceRuleInteligent(geometries, cpt, lastContacts[c], cntBndBox, lastContacts[c], cntBndBox, CONT);
            else if(c>pos)
                insertDistanceRuleInteligent(geometries, cpt, lastContacts[c], cntBndBox, cntBndBox, lastContacts[c], CONT);
            else if(c==pos)
                insertDistanceRuleDumb(geometries, cpt, lastContacts[c], cntBndBox, currentRules->getRule(S1CTCT), H, CONT);
        }
    }
    currentContacts[pos]=cntBndBox;
    return cntBndBox;
}

string AutoCell::insertVia(vector<Box*> &geometries, Compaction &cpt, string metNode, bool minArea) {
    geometries.push_back(&currentLayout.addLayer(0, 0, 0, 0, MET1P));
    string via = to_string(geometries.size() - 1);
    
    cpt.insertConstraint("x" + via + "a", "x" + via + "b", CP_EQ, currentRules->getRule(W2VI));
    cpt.insertConstraint("y" + via + "a", "y" + via + "b", CP_EQ, currentRules->getRule(W2VI));
    
    //choose a kind of metal enclosure of via
    cpt.forceBinaryVar("b" + via + "_1V");
    cpt.forceBinaryVar("b" + via + "_2V");
    cpt.insertConstraint("ZERO", "b" + via + "_1V" + " + " + "b" + via + "_2V", CP_EQ, 1);
    
    cpt.insertConstraint("x" + metNode + "a", "x" + via + "a", CP_MIN, "b" + via + "_1V", currentRules->getRule(E2M1VI));
    cpt.insertConstraint("x" + via + "b", "x" + metNode + "b", CP_MIN, "b" + via + "_1V", currentRules->getRule(E2M1VI));
    cpt.insertConstraint("y" + metNode + "a", "y" + via + "a", CP_MIN, "b" + via + "_1V", currentRules->getRule(E1M1VI));
    cpt.insertConstraint("y" + via + "b", "y" + metNode + "b", CP_MIN, "b" + via + "_1V", currentRules->getRule(E1M1VI));
    
    cpt.insertConstraint("x" + metNode + "a", "x" + via + "a", CP_MIN, "b" + via + "_2V", currentRules->getRule(E1M1VI));
    cpt.insertConstraint("x" + via + "b", "x" + metNode + "b", CP_MIN, "b" + via + "_2V", currentRules->getRule(E1M1VI));
    cpt.insertConstraint("y" + metNode + "a", "y" + via + "a", CP_MIN, "b" + via + "_2V", currentRules->getRule(E2M1VI));
    cpt.insertConstraint("y" + via + "b", "y" + metNode + "b", CP_MIN, "b" + via + "_2V", currentRules->getRule(E2M1VI));
    
    //choose a kind of metal area format
    if(minArea){
        cpt.insertConstraint("x" + metNode + "a", "x" + metNode + "b", CP_EQ,  "x" + metNode +  "_width");
        cpt.insertConstraint("y" + metNode + "a", "y" + metNode + "b", CP_EQ, "y" + metNode +  "_width");

        double A=currentRules->getRulef(A1M1);
        double W=currentRules->getRulef(W2VI);
        double tmp1=(sqrt(A)-(A/W))/(sqrt(A)-W);
        cpt.insertConstraint("ZERO", "y" + metNode +  "_width" + " + " + to_string(-tmp1) + " x" + metNode +  "_width", CP_MIN, currentRules->getIntValue(A/W-W*tmp1));
        double tmp2=(sqrt(A)-W)/(sqrt(A)-(A/W));
        cpt.insertConstraint("ZERO", "y" + metNode +  "_width" + " + " + to_string(-tmp2) + " x" + metNode +  "_width", CP_MIN, currentRules->getIntValue(W-(A/W)*tmp2));
    }
    
    //allign via to the routing grid
    cpt.insertConstraint( "HGRID_OFFSET", "x" + via + "g", CP_EQ_VAR_VAL,  "x" + via + "gpos", hGrid);
    cpt.forceIntegerVar("x" + via + "gpos");
    cpt.insertConstraint( "x" + via + "a", "x" + via + "g", CP_EQ, currentRules->getRule(W2VI)/2);
    cpt.insertConstraint( "x" + via + "g", "x" + via + "b", CP_EQ, currentRules->getRule(W2VI)/2);
    
    cpt.insertConstraint( "VGRID_OFFSET", "y" + via + "g", CP_EQ_VAR_VAL,  "y" + via + "gpos", vGrid);
    cpt.forceIntegerVar("y" + via + "gpos");
    cpt.insertConstraint( "y" + via + "a", "y" + via + "g", CP_EQ, currentRules->getRule(W2VI)/2);
    cpt.insertConstraint( "y" + via + "g", "y" + via + "b", CP_EQ, currentRules->getRule(W2VI)/2);
    
    return via;
}


void AutoCell::insertCntPol(vector<Box*> &geometries, Compaction &cpt, string cnt, vector<string> currentPolNodes, int pos) {
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

string AutoCell::createGeometry(vector<Box*> &geometries, Compaction &cpt, string netName, int priority, layer_name l){
    geometries.push_back(&currentLayout.addLayer(0, 0, 0, 0, l));
    geometries.back()->setNet(netName);
    string currentGeo = to_string(geometries.size() - 1);
    
    int minWidth = (l==MET1 ? currentRules->getRule(W1M1) : currentRules->getRule(W2P1));
    cpt.insertConstraint("x" + currentGeo + "a", "x" + currentGeo + "b", CP_MIN, minWidth);
    cpt.insertConstraint("x" + currentGeo + "a", "x" + currentGeo + "b", CP_EQ, "x" + currentGeo + "min");
    cpt.insertLPMinVar("x" + currentGeo + "min", priority);
    cpt.insertConstraint("y" + currentGeo + "a", "y" + currentGeo + "b", CP_MIN, minWidth);
    cpt.insertConstraint("y" + currentGeo + "a", "y" + currentGeo + "b", CP_EQ, "y" + currentGeo + "min");
    cpt.insertLPMinVar("y" + currentGeo + "min", priority);
    return currentGeo;
}

void AutoCell::createNode(vector<Box*> &geometries, Compaction &cpt, list<Element>::iterator elements_it, int pos, vector<string> &currentNode, string netName, layer_name l){
    string currentGeo = createGeometry(geometries, cpt, netName, l==MET1?3:6, l);
    
    //apply minimum distance rules to cell borders
    int minDist = (l==MET1 ? currentRules->getRule(S1M1M1) : currentRules->getRule(S1P1P1));
    cpt.insertConstraint("ZERO", "x" + currentGeo + "a", CP_MIN, minDist / 2);
    cpt.insertConstraint("x" + currentGeo + "b", "width", CP_MIN, minDist / 2);
    
    //special rules for wide metals and p/nplus enclusure of poly
    minDist = (l==MET1 ? currentRules->getRule(S2M1M1) : currentRules->getRule(S1P1P1));
    if(l==MET1){
        if (currentNetList.getNetName(rt->getNet(elements_it->met[pos])) != currentCircuit->getGndNet())
            cpt.insertConstraint("yGNDb", "y" + currentGeo + "a", CP_MIN, minDist);
        
        if (currentNetList.getNetName(rt->getNet(elements_it->met[pos])) != currentCircuit->getVddNet())
            cpt.insertConstraint("y" + currentGeo + "b", "yVDDa", CP_MIN, minDist);
        
        if (pos == 0)
            createTrack(geometries, cpt, "GND", currentGeo, netName, l, V);
        
        if (pos == trackPos.size() - 1)
            createTrack(geometries, cpt, currentGeo, "VDD", netName, l, V);
    }else if(l==POLY){
        cpt.insertConstraint("ZERO", "y" + currentGeo + "a", CP_MIN, minDist / 2);
        cpt.insertConstraint("y" + currentGeo + "b", "height", CP_MIN, minDist / 2);
    }
    
    //line ends rule
    int minExt = (l==MET1 ? currentRules->getRule(S3M1M1)-currentRules->getRule(S1M1M1) : currentRules->getRule(S3P1P1)-currentRules->getRule(S1P1P1));
    cpt.forceBinaryVar("b" + currentGeo+ "_endline_v");
    cpt.forceBinaryVar("b" + currentGeo+ "_endline_h");
    cpt.insertConstraint("ZERO", "b" + currentGeo+ "_endline_v" + " + " "b" + currentGeo+ "_endline_h", CP_EQ, 1);
    cpt.insertConstraint("x" + currentGeo + "a2", "x" + currentGeo + "a", CP_MIN, "b" + currentGeo+ "_endline_h", minExt);
    cpt.insertConstraint("x" + currentGeo + "b", "x" + currentGeo + "b2", CP_MIN, "b" + currentGeo+ "_endline_h", minExt);
    cpt.insertConstraint("y" + currentGeo + "a2", "y" + currentGeo + "a", CP_MIN, "b" + currentGeo+ "_endline_v", minExt);
    cpt.insertConstraint("y" + currentGeo + "b", "y" + currentGeo + "b2", CP_MIN, "b" + currentGeo+ "_endline_v", minExt);
    
    if(enableDFM){
        cpt.insertConstraint("x" + currentGeo + "a2", "x" + currentGeo + "a", CP_MIN, "max" + currentGeo+ "H");
        cpt.insertConstraint("x" + currentGeo + "b", "x" + currentGeo + "b2", CP_MIN, "max" + currentGeo+ "H");
        cpt.insertConstraint("y" + currentGeo + "a2", "y" + currentGeo + "a", CP_MIN, "max" + currentGeo+ "V");
        cpt.insertConstraint("y" + currentGeo + "b", "y" + currentGeo + "b2", CP_MIN, "max" + currentGeo+ "V");
        minDist = (l==MET1 ? currentRules->getRule(S1M1M1) : currentRules->getRule(S1P1P1));
        cpt.insertConstraint("ZERO", "max" + currentGeo+ "H", CP_MAX, ceil(minDist*0.10));
        cpt.insertConstraint("ZERO", "max" + currentGeo+ "V", CP_MAX, ceil(minDist*0.10));
        cpt.insertLPMinVar("max" + currentGeo+ "H", -(l==MET1?10:16));
        cpt.insertLPMinVar("max" + currentGeo+ "V", -(l==MET1?10:16));
    }
    
    currentNode[pos]=currentGeo;
}

void AutoCell::createTrack(vector<Box*> &geometries, Compaction &cpt, string lastNode, string currentNode, string netName, layer_name l, HorV dir){
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
            cpt.insertConstraint("x" + track + "a - " + to_string(minIntersection) + " UM", "x" + track + "b", CP_MAX, "b"+track+"_reduceLturns", relaxation);
        else
            cpt.insertConstraint("y" + track + "a - " + to_string(minIntersection) + " UM", "y" + track + "b", CP_MAX, "b"+track+"_reduceLturns", relaxation);
        cpt.forceBinaryVar("b"+track+"_reduceLturns");
        cpt.insertLPMinVar("b"+track+"_reduceLturns",l==MET1?4:7);
    }
}

void AutoCell::insertDistanceRuleInteligent(vector<Box*> &geometries, Compaction &cpt, string lastX, string currentX, string lastY, string currentY, layer_name l){
    int minDist;
    if (l==MET1) minDist = currentRules->getRule(S1M1M1);
    else if(l==POLY) minDist = currentRules->getRule(S1P1P1);
    else if(l==CONT) minDist = currentRules->getRule(S1CTCT);
    
    //select between different space possibilities
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_1"); //left
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_2"); //up
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_3"); //diagonal
    cpt.insertConstraint("ZERO", "b" + lastX + "_" + currentX + "_1" + " + " + "b" + lastX + "_" + currentX + "_2" + " + " + "b" + lastX + "_" + currentX + "_3", CP_EQ, 1);
    
    //    cpt.insertConstraint("ZERO", "b" + lastX + "_" + currentX+ "_3", CP_EQ, 0);
    
    if(l==MET1 || l==POLY){
        cpt.insertConstraint("x" + lastX + "b + RELAXATION", "x" + currentX + "a2", CP_MIN, "b" + lastX + "_" + currentX + "_1", minDist + relaxation);
        cpt.insertConstraint("x" + lastX + "b2 + RELAXATION", "x" + currentX + "a", CP_MIN, "b" + lastX + "_" + currentX + "_1", minDist + relaxation);
        
        cpt.insertConstraint("y" + lastY + "b + RELAXATION", "y" + currentY + "a2", CP_MIN, "b" + lastX + "_" + currentX + "_2", minDist + relaxation);
        cpt.insertConstraint("y" + lastY + "b2 + RELAXATION", "y" + currentY + "a", CP_MIN, "b" + lastX + "_" + currentX + "_2", minDist + relaxation);
        
        minDist=static_cast<int>(ceil(2*(minDist/sqrt(2.0))));
        
        cpt.insertConstraint("ZERO", "x" + currentX +  "a2" + " - " + "x" + lastX +  "b" + " + "+ "y" + currentY +  "a2" + " - " + "y" + lastY +  "b + RELAXATION", CP_MIN, "b" + lastX + "_" + currentX+ "_3", minDist + relaxation);
        cpt.insertConstraint("ZERO", "x" + currentX +  "a" + " - " + "x" + lastX +  "b2" + " + "+ "y" + currentY +  "a" + " - " + "y" + lastY +  "b2 + RELAXATION", CP_MIN, "b" + lastX + "_" + currentX+ "_3", minDist + relaxation);
        
        //        cpt.insertConstraint("y" + lastY + "b + RELAXATION", "y" + currentY + "a2", CP_MIN, "b" + lastX + "_" + currentX + "_3", relaxation);
        //        cpt.insertConstraint("y" + lastY + "b2 + RELAXATION", "y" + currentY + "a", CP_MIN, "b" + lastX + "_" + currentX + "_3", relaxation);
        //      cpt.insertConstraint("x" + lastX + "b + RELAXATION", "x" + currentX + "a2", CP_MIN, "b" + lastX + "_" + currentX + "_3", relaxation);
        //    cpt.insertConstraint("x" + lastX + "b2 + RELAXATION", "x" + currentX + "a", CP_MIN, "b" + lastX + "_" + currentX + "_3", relaxation);
    }else{
        cpt.insertConstraint("x" + lastX + "b + RELAXATION", "x" + currentX + "a", CP_MIN, "b" + lastX + "_" + currentX + "_1", minDist + relaxation);
        cpt.insertConstraint("y" + lastY + "b + RELAXATION", "y" + currentY + "a", CP_MIN, "b" + lastX + "_" + currentX + "_2", minDist + relaxation);
        if(l==CONT) minDist = currentRules->getRule(S3CTCT);
        minDist=static_cast<int>(ceil(2*(minDist/sqrt(2.0))));
        
        cpt.insertConstraint("ZERO", "x" + currentX +  "a" + " - " + "x" + lastX +  "b" + " + "+ "y" + currentY +  "a" + " - " + "y" + lastY +  "b + RELAXATION", CP_MIN, "b" + lastX + "_" + currentX+ "_3", minDist + relaxation);
    }
    
}

void AutoCell::insertDistanceRuleInteligent2(vector<Box*> &geometries, Compaction &cpt, string lastX, string currentX, string lastY, string currentY, int minDist, string booleanFlag){
    string id=to_string(index++);
    //select between different space possibilities
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_1_"+id); //front
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_2_"+id); //one side
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_3_"+id); //other side
    cpt.insertConstraint("ZERO", "b" + lastX + "_" + currentX + "_1_" + id + " + " + "b" + lastX + "_" + currentX + "_2_" +id+ " + " + "b" + lastX + "_" + currentX + "_3_"+id, CP_EQ, (booleanFlag==""?"UM":booleanFlag));
    cpt.insertConstraint("y" + lastY + "b + RELAXATION", "y" + currentY + "a", CP_MIN, "b" + lastX + "_" + currentX + "_1_"+id, minDist + relaxation);
    cpt.insertConstraint("x" + lastX + "b + RELAXATION", "x" + currentX + "a", CP_MIN, "b" + lastX + "_" + currentX + "_2_"+id, minDist + relaxation);
    cpt.insertConstraint("x" + currentX + "b + RELAXATION", "x" + lastX + "a", CP_MIN, "b" + lastX + "_" + currentX + "_3_"+id, minDist + relaxation);
}

void AutoCell::insertDistanceRuleInteligent3x1(vector<Box*> &geometries, Compaction &cpt, string last1, string last2, string last3, string current, int minDist, HorV dir, layer_name l){
    string id=to_string(index++);
    //select between different space possibilities
    cpt.forceBinaryVar("b" + last1 + "_" + current+ "_1_"+id); //bellow
    cpt.forceBinaryVar("b" + last2 + "_" + current+ "_2_"+id); //right
    cpt.forceBinaryVar("b" + last3 + "_" + current+ "_3_"+id); //above
    cpt.insertConstraint("ZERO", "b" + last1 + "_" + current + "_1_" + id + " + " + "b" + last2 + "_" + current + "_2_" +id+ " + " + "b" + last3 + "_" + current + "_3_"+id, CP_EQ, 1);
    cpt.insertConstraint("x" + last1 + "b + RELAXATION", "x" + current + "a", CP_MIN, "b" + last1 + "_" + current + "_2_"+id, minDist + relaxation);
    cpt.insertConstraint("x" + last2 + "b + RELAXATION", "x" + current + "a", CP_MIN, "b" + last2 + "_" + current + "_2_"+id, minDist + relaxation);
    cpt.insertConstraint("x" + last3 + "b + RELAXATION", "x" + current + "a", CP_MIN, "b" + last3 + "_" + current + "_2_"+id, minDist + relaxation);
}

void AutoCell::insertDistanceRuleInteligent3(vector<Box*> &geometries, Compaction &cpt, string lastX, string currentX, string lastY, string currentY, int minDist, string booleanFlag){
    string id=to_string(index++);
    //select between different space possibilities
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_1_"+id); //bellow
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_2_"+id); //right
    cpt.forceBinaryVar("b" + lastX + "_" + currentX+ "_3_"+id); //above
    cpt.insertConstraint("ZERO", "b" + lastX + "_" + currentX + "_1_" + id + " + " + "b" + lastX + "_" + currentX + "_2_" +id+ " + " + "b" + lastX + "_" + currentX + "_3_"+id, CP_EQ, (booleanFlag==""?"UM":booleanFlag));
    cpt.insertConstraint("y" + lastY + "b + RELAXATION", "y" + currentY + "a", CP_MIN, "b" + lastX + "_" + currentX + "_1_"+id, minDist + relaxation);
    cpt.insertConstraint("x" + lastX + "b + RELAXATION", "x" + currentX + "a", CP_MIN, "b" + lastX + "_" + currentX + "_2_"+id, minDist + relaxation);
    cpt.insertConstraint("y" + currentY + "b + RELAXATION", "y" + lastY + "a", CP_MIN, "b" + lastX + "_" + currentX + "_3_"+id, minDist + relaxation);
}

void AutoCell::insertDistanceRuleDumb(vector<Box*> &geometries, Compaction &cpt, string last, string next, int minDist, HorV dir, layer_name l){
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
        cout << *elements_it;
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