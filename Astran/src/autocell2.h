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
	int  center;
	int pDif_iniY, pDif_endY, nDif_iniY, nDif_endY;
	vector<int> trackPos, diffPini, diffNini;
	int state;
	
	list<Element> elements;
	map<string,int> inoutPins;
	//	void eletricalOtimization(Cell& currentCell, Pathfinder2 &rt);
	
	Element* createElement(int vcost, int nDiffIni, int pDiffIni);
	
	CellNetlst* currentCell;
	Rules* currentRules;
	Circuit* currentCircuit;
	
	Pathfinder2* rt;
	CellNetlst currentNetList;
	CLayout currentLayout;

    void insertCntPol(vector<Box*> &geometries, compaction &cpt, string cntPos, vector<string> polTracks, int pos);
    string insertCntDif(vector<Box*> &geometries, compaction &cpt, string cntPos, string &lastGatePos, string &lastDiff, layer_name l, bool endDiff);
    string insertCnt(vector<Box*> &geometries, compaction &cpt, list<Element>::iterator elements_it, vector<string>& metTracks, int pos);
    string insertGate(vector<Box*> &geometries, compaction &cpt, int transistor, list<Element>::iterator elements_it, vector<string> &currentPolTrack, string &lastContact, string lastContactDiff, string &lastGatePos, string currentDiff, layer_name l);
    string createGeometry(vector<Box*> &geometries, compaction &cpt, string netName, int priority, layer_name l);
    void createNode(vector<Box*> &geometries, compaction &cpt, list<Element>::iterator elements_it, int pos, vector<string> &currentMetNode, string netName, layer_name l);
    void createTrack(vector<Box*> &geometries, compaction &cpt, string lastMetNode, string currentMetNode, string netName, layer_name l, HorV dir);

    public:
	AutoCell();
	~AutoCell();
	void clear();
	bool calcArea(Circuit* c);
	bool foldTrans();
	bool placeTrans(bool ep, int saquality, int nrAttempts, int wC, int gmC, int rC, int congC, int ngC);
	
	bool route(int mCost, int pCost, int cCost, int ioCost);
	bool compact(int mPriority, int pPriority, int gsPriority, int wPriority, string lpSolverFile);	
	bool selectCell(string c);
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
	void showIOCost();
	
};

#endif
