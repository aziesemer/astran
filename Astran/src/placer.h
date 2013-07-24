/*
 *  placer.h
 *  ICPD
 *
 *  Created by Adriel Mota Ziesemer Jr. on 3/19/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PLACER_H
#define PLACER_H

#include "circuit.h"
#include "vector"
#include "list"
#include "compaction.h"
#include "router.h"

/**
Place the cells
@author Adriel Mota Ziesemer Jr.
*/

using namespace std;

class Placer{
	protected:
	Circuit* currentCircuit;
	int nrSites;
	float utilization;
	vector<list<string> > rows;
	map<string, int> cellSpace;
	map<string, int> cellConflicts;
	
	public:
	Placer();
	~Placer(){};
	CLayout* getLayoutFromInstance(string instanceName);
	bool checkPlacement();

	void setCircuit(Circuit* c){currentCircuit=c;};
	bool checkWL();
	bool incrementalPlacement(Router* rt, string lpSolverFile);
	bool autoFlip();
	bool setArea(int nrrows, float utilization);
	int getNrRows(){return rows.size();};
	float getNrSites(){return nrSites;};
	float getUtilization();
	int getVSize();
	int getHSize();
	bool placeInterfaceTerminals();
	bool writeBookshelfFiles(string fileName, bool dotPLOnly);
	bool writeCadende(string fileName);
	bool readMangoParrotPlacement(string fileName);
	bool readBookshelfPlacement(string fileName);
};

#endif