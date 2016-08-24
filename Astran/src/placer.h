/*
 *  placer.h
 *  ICPD
 *
 *  Created by Adriel Mota Ziesemer Jr. on 3/19/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include <vector>
#include <list>
#include "circuit.h"
#include "compaction.h"
#include "router.h"

/**
 Place the cells
 @author Adriel Mota Ziesemer Jr.
 */

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
    void checkWL();
    void incrementalPlacement(Router* rt, string lpSolverFile);
    void autoFlip();
    void setArea(int nrrows, float utilization);
    int getNrRows(){return static_cast<int>(rows.size());};
    float getNrSites(){return nrSites;};
    float getUtilization();
    int getVSize();
    int getHSize();
    void placeInterfaceTerminals();
    void writeBookshelfFiles(string fileName, bool dotPLOnly);
    void writeCadende(string fileName);
    void readMangoParrotPlacement(string fileName);
    void readBookshelfPlacement(string fileName);
};