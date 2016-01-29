/***************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                         *
 *   amziesemerj[at]inf.ufrgs.br                                            *
 ****************************************************************************/

#pragma once

/**
 Data Structs for the circuit
 
 @author Adriel Mota Ziesemer Jr.
 */

#include <list>
#include <map>
#include <string>
#include <strstream>

#include "cellnetlst.h"
#include "clayout.h"

class Circuit {
public:
    struct Interface{
        direction orient;
        IOType ioType;
        Point pos;
    };
    
    Circuit();
    ~Circuit(){};
    
    void clear();
    vector<string> getCellTemplatesList();
    void setTopCell(string cellName){topCell=cellName;};
    string getTopCell(){return topCell;};
    void setVddNet(string s){vddNet=s;};
    string getVddNet(){return vddNet;};
    void setGndNet(string s){gndNet=s;};
    string getGndNet(){return gndNet;};
    void setRules(Rules* r){currentRules=r;};
    Rules* getRules(){return currentRules;};
    void setHGridOffset(bool x){hGridOffset=x;};
    bool getHGridOffset(){return hGridOffset;};
    void setVGridOffset(bool x){vGridOffset=x;};
    bool getVGridOffset(){return vGridOffset;};
    void setHPitch(float x){hGrid=x;};
    float getHPitch(){return hGrid;};
    void setVPitch(float x){vGrid=x;};
    float getVPitch(){return vGrid;};
    void setSupplyVSize(float x){supplyVSize=x;};
    float getSupplyVSize(){return supplyVSize;};
    void setnWellPos(float x){nWellPos=x;};
    float getnWellPos(){return nWellPos;};
    void setnWellBorder(float x){nWellBorder=x;};
    float getnWellBorder(){return nWellBorder;};
    void setpnSelBorder(float x){pnSelBorder=x;};
    float getpnSelBorder(){return pnSelBorder;};
    void setCellTemplate(string x){cellTemplate=x;};
    string getCellTemplate(){return cellTemplate;};
    void setRowHeight(int h){cellsHeight=h;};
    int getRowHeight(){return cellsHeight;};
    bool setMargins(float l, float r, float t, float b);
    int getLMargin(){return c2lMargin;};
    int getRMargin(){return c2rMargin;};
    int getTMargin(){return c2tMargin;};
    int getBMargin(){return c2bMargin;};
    CLayout* getLayout(string n);
    map<string, CLayout>* getLayouts(){return &layouts;};
    void insertCell(CellNetlst& tmp);
    bool insertLayout(CLayout& tmp);
    bool insertEquation(string cellName, string& tmp);
    bool insertInterface(string name, direction orient, IOType type, int posx, int posy);
    unsigned int nrCells();
    CellNetlst* getCellNetlst(string n);
    CellNetlst* findCellNetlst(string n);
    string* getEquation(string n);
    Interface* getInterface(string n);
    CellNetlst getFlattenCell(string name);
    
    map<string, Interface>* getInterfaces(){return &interfaces;};
    map<string, CellNetlst>* getCellNetlsts(){return &cellNetlsts;};
    map<string, string>* getEquations(){return &equations;};
    void setCellsHeight(unsigned int h) { cellsHeight=h;};
    
    void writeCellSizesFile(string filename);
    void printInterface(string net);
    void printNet(string net);
    void printInstance(CLayout* l, string instance);
    void calculateCellsPins();
    
    //	void printCells();
    //	void printNetlist();
    //	bool checkCells();
    
protected:
    map<string, Interface> interfaces;
    map<string, CLayout> layouts;
    map<string, CellNetlst> cellNetlsts;
    map<string, string> equations;
    
    Rules* currentRules;
    int cellsHeight, c2lMargin, c2rMargin, c2tMargin, c2bMargin;
    float hGrid, vGrid, supplyVSize, nWellPos, nWellBorder, pnSelBorder;
    string topCell,vddNet, gndNet, cellTemplate;
    bool hGridOffset, vGridOffset;
};

