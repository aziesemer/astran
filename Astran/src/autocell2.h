/***************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Cristiano Lazzari     *
 *   (amziesemerj,clazz)@inf.ufrgs.br                                      *
 ***************************************************************************/
#pragma once

/**
 Draw a Cell automatically
 
 @author Adriel Mota Ziesemer Jr.
 */

#include <vector>
#include <sstream>
#include <iterator>
#include <time.h>
#include <memory>
#include "clayout.h"
#include "draw.h"
#include "rules.h"
#include "circuit.h"
#include "graphrouter.h"
#include "compaction.h"
#include "util.h"

class AutoCell{
public:
    AutoCell();
    ~AutoCell();
    void clear();
    void calcArea(int nrIntTracks, int reduceMetTracks);
    void autoFlow(string lpSolverFile);
    void foldTrans();
    void placeTrans(bool ep, int saquality, int nrAttempts, int wC, int gmC, int rC, int congC, int ngC);
    void route(bool hPoly, bool increaseIntTracks, int reduceVRt, bool optimize);
    bool compact(string lpSolverFile, int diffStretching, int griddedPolly, int rdCntsCost, int maxDiffConts, int alignDiffConts, int reduceLturns, bool enableDFM, bool experimental, bool debug, int timeLimit);
    void selectCell(Circuit* c, string cell);
    void setMetPriority(int x) {metal_priority=x;};
    int getMetPriority() {return metal_priority;};
    void setPolPriority(int x) {poly_priority=x;};
    int setPolPriority() {return poly_priority;};
    void setWidthPriority(int x) {width_priority=x;};
    int getWidthPriority() {return width_priority;};
    void setGateSpacingPriority(int x) {gatespacing_priority=x;};
    int getGateSpacingPriority() {return gatespacing_priority;};
    int getState(){return state;};
    float getPSize(){return static_cast<float>(pSize/currentCircuit->getRules()->getScale());};
    float getNSize(){return static_cast<float>(nSize/currentCircuit->getRules()->getScale());};
    CellNetlst* getTmpNetlist(){return &currentNetList;};
    CLayout getLayout(){return currentLayout;};
    GraphRouter* getRouting(){return rt.get();};
    void printGraph();
    
protected:
    int hGrid, vGrid, supplySize, height;
    int vdd,gnd,poly_priority,metal_priority,width_priority,gatespacing_priority;
    
    int supWidth,btDifWidth, posNWell;
    int nSize,pSize;
    int center;
    int pDif_iniY, pDif_endY, nDif_iniY, nDif_endY;
    int diffStretching, griddedPoly, rdCntsCost, alignDiffConts, reduceLturns, maxDiffCnts, enableDFM, experimental,reduceMetTracks;
    int index, reduceVRt;
    bool hPoly;
    
    vector<int> trackPos, diffPini, diffNini;
    int state;
    static const int relaxation = 20000;
    
    
    class Element{
    public:
        TransitorTerminal linkN, linkP;
        vector<int> met, pol;
        int diffN, diffP;
        int diffNIni, diffPIni;
        int diffNEnd, diffPEnd;
        int inoutCnt;
        bool gapP, gapN;
        
        virtual std::string toString() const {
            std::ostringstream os;
            os << "<" << (gapN?"G":" ") << diffN << "," << (gapP?"G":" ") << diffP << ">,";
            for(int x=0;x<met.size();x++)
                os << "[" << met[x] << "," << pol[x] << "] ";
            os << "-> " << inoutCnt << endl;
            return os.str();
        };
        
        friend std::ostream& operator<<(std::ostream& os, const Element& obj){
            return os << obj.toString();
        }
    };
    
    list<Element> elements;
    map<string,int> inoutPins;
    Element* createElement(int vcost, int nDiffIni, int pDiffIni, int nEnd, int pEnd, TransTerminalType type);
    CellNetlst* currentCell;
    Rules* currentRules;
    Circuit* currentCircuit;
    std::unique_ptr<GraphRouter> rt;
    CellNetlst currentNetList;
    CLayout currentLayout;
    
    void insertCntPol(vector<Box*> &geometries, Compaction &cpt, string cntPos, vector<string> polTracks, int pos);
    string insertCntDif(vector<Box*> &geometries, Compaction &cpt, string currentCnt, string &lastGate, string &lastCnt,  layer_name l);
    string newDif(vector<Box*> &geometries, Compaction &cpt, string &lastGate, string &lastDiff, string &currentDiff, string &diffEnc,  layer_name l, bool endDiff);
    string insertVia(vector<Box*> &geometries, Compaction &cpt, string metNode, bool minArea);
    string insertCnt(vector<Box*> &geometries, Compaction &cpt, list<Element>::iterator elements_it, vector<string>& metTracks, vector<string>& lastContacts, vector<string>& currentContacts, int pos, bool isDiff);
    string insertGate(vector<Box*> &geometries, Compaction &cpt, int transistor, list<Element>::iterator elements_it, vector<string> &currentPolNodes, string lastCnt, string lastDiffCnt, string &lastGate, int &lastGateLength, string lastDiff, string currentDiff, layer_name l);
    string createGeometry(vector<Box*> &geometries, Compaction &cpt, string netName, int priority, layer_name l);
    void createNode(vector<Box*> &geometries, Compaction &cpt, list<Element>::iterator elements_it, int pos, vector<string> &currentMetNode, string netName, layer_name l);
    void createTrack(vector<Box*> &geometries, Compaction &cpt, string lastMetNode, string currentMetNode, string netName, layer_name l, HorV dir);
    void insertDistanceRuleInteligent(vector<Box*> &geometries, Compaction &cpt, string lastX, string currentX, string lastY, string currentY, layer_name l);
    void insertDistanceRuleInteligent2(vector<Box*> &geometries, Compaction &cpt, string lastX, string currentX, string lastY, string currentY, int minDist, string booleanFlag);
    void insertDistanceRuleInteligent3(vector<Box*> &geometries, Compaction &cpt, string lastX, string currentX, string lastY, string currentY, int minDist, string booleanFlag);
    void insertDistanceRuleInteligent3x1(vector<Box*> &geometries, Compaction &cpt, string last1, string last2, string last3, string current, int minDist, HorV dir, layer_name l);
    void insertDistanceRuleDumb(vector<Box*> &geometries, Compaction &cpt, string last, string current, int minDist, HorV dir, layer_name l);
    void checkState(int nextState);
    bool testGap(vector<TransitorTerminal>::iterator last_it, vector<TransitorTerminal>::iterator eulerPath_it, vector<TransitorTerminal>& ordering, TransTerminalType type);
};