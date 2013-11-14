/***************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Cristiano Lazzari     *
 *   (amziesemerj,clazz)@inf.ufrgs.br                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef AUTOCELL_H
#define AUTOCELL_H

/**
 Draw a Cell automatically
 
 @author Adriel Mota Ziesemer Jr.
 */

#include <vector>
#include <sstream>
#include <iterator>     // std::next
#include "clayout.h"
#include "draw.h"
#include "rules.h"
#include "circuit.h"
#include "pathfinder2.h"
#include "compaction.h"
#include "util.h"

using namespace std;

class Element{
public:
	t_net2 linkN, linkP;
	vector<int> met, pol;
	int diffN, diffP;
	int diffNIni, diffPIni;
	int diffNEnd, diffPEnd;
	int inoutCnt;
	bool gapP, gapN;
	
	void print(){
		cout << "<" << (gapN?"G":" ") << diffN << "," << (gapP?"G":" ") << diffP << ">,";
		for(int x=0;x<met.size();x++)
			cout << "[" << met[x] << "," << pol[x] << "] ";
		cout << "-> " << inoutCnt << endl;
	}
};

class AutoCell{
protected:
	
	int hGrid, vGrid, supplySize, height;
	int vdd,gnd,poly_priority,metal_priority,width_priority,gatespacing_priority; 
	
	int supWidth,btDifWidth, posNWell;
	int nSize,pSize;
	int center;
	int pDif_iniY, pDif_endY, nDif_iniY, nDif_endY;
    int diffStretching, griddedPoly, rdCntsCost, alignDiffConts,reduceLturns, maxDiffCnts, enableDFM, experimental;
    int index;
    bool hPoly;
    
	vector<int> trackPos, diffPini, diffNini;
	int state;
    static const int relaxation = 20000;

	
	list<Element> elements;
	map<string,int> inoutPins;
	
	Element* createElement(int vcost, int nDiffIni, int pDiffIni, int nEnd, int pEnd, bool isDiff);
	
	CellNetlst* currentCell;
	Rules* currentRules;
	Circuit* currentCircuit;
	
	Pathfinder2* rt;
	CellNetlst currentNetList;
	CLayout currentLayout;

    void insertCntPol(vector<Box*> &geometries, compaction &cpt, string cntPos, vector<string> polTracks, int pos);
    string insertCntDif(vector<Box*> &geometries, compaction &cpt, string currentCnt, string &lastGate, string &lastCnt,  layer_name l);
    string newDif(vector<Box*> &geometries, compaction &cpt, string &lastGate, string &lastDiff, string &currentDiff, string &diffEnc,  layer_name l, bool endDiff);
    string insertVia(vector<Box*> &geometries, compaction &cpt, string metNode);
    string insertCnt(vector<Box*> &geometries, compaction &cpt, list<Element>::iterator elements_it, vector<string>& metTracks, vector<string>& lastContacts, vector<string>& currentContacts, int pos, bool isDiff);
    string insertGate(vector<Box*> &geometries, compaction &cpt, int transistor, list<Element>::iterator elements_it, vector<string> &currentPolNodes, string lastCnt, string lastDiffCnt, string &lastGate, int &lastGateLength, string lastDiff, string currentDiff, layer_name l);
    string createGeometry(vector<Box*> &geometries, compaction &cpt, string netName, int priority, layer_name l);
    void createNode(vector<Box*> &geometries, compaction &cpt, list<Element>::iterator elements_it, int pos, vector<string> &currentMetNode, string netName, layer_name l);
    void createTrack(vector<Box*> &geometries, compaction &cpt, string lastMetNode, string currentMetNode, string netName, layer_name l, HorV dir);
    void insertDistanceRuleInteligent(vector<Box*> &geometries, compaction &cpt, string lastX, string currentX, string lastY, string currentY, layer_name l);
    void insertDistanceRuleInteligent2(vector<Box*> &geometries, compaction &cpt, string lastX, string currentX, string lastY, string currentY, int minDist, string booleanFlag);
    void insertDistanceRuleInteligent3(vector<Box*> &geometries, compaction &cpt, string lastX, string currentX, string lastY, string currentY, int minDist, string booleanFlag);
    void insertDistanceRuleDumb(vector<Box*> &geometries, compaction &cpt, string last, string current, int minDist, HorV dir, layer_name l);
    void checkState(int nextState);

    public:
	AutoCell();
	~AutoCell();
	void clear();
	void calcArea(int nrIntTracks);
	void foldTrans();
	void placeTrans(bool ep, int saquality, int nrAttempts, int wC, int gmC, int rC, int congC, int ngC);
	
    void route(bool hPoly, bool increaseIntTracks, bool optimize);
    void compact(string lpSolverFile, int diffStretching, int griddedPolly, int rdCntsCost, int maxDiffConts, int alignDiffConts, int reduceLturns, bool enableDFM, bool experimental, bool debug, int timeLimit);
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
	float getPSize(){return float(pSize/currentCircuit->getRules()->getScale());};
	float getNSize(){return float(nSize/currentCircuit->getRules()->getScale());};
	CellNetlst* getTmpNetlist(){return &currentNetList;};
	CLayout getLayout(){return currentLayout;};
	Pathfinder2* getRouting(){return rt;};
	void printGraph();
	
};

#endif
