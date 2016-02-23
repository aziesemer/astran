/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                       *
 *   [amziesemerj[at]@inf.ufrgs.br                                        *
 **************************************************************************/
#pragma once

/**
 Data Structure of the cell netlist
 
 @author Adriel Mota Ziesemer Jr.
 */

#include <vector>
#include <list>
#include <map>
#include <string>
#include <iostream>
#include <strstream>
#include <sstream>
#include "math.h"
#include "thresholdaccept.h"
#include "util.h"

enum TransType {PMOS, NMOS};
enum TransTerminalType {DRAIN, GATE, SOURCE, GAP};

// Points the net to a transistor
struct TransitorTerminal{
	int link;
	TransTerminalType type;
};

struct InstancePin{
	string targetCellInst;
	int targetPin;
};

struct Net{
	list<TransitorTerminal> trans;
	list<InstancePin> insts;	
	string name;
};

struct Transistor{
	string name;
	bool visited;
    int drain, gate, source;
	TransType type;
	float length,width;	
};

struct CellInstance{
	string subCircuit;
	vector<int> ports;
};

class CellNetlst{
public:
    void setName(string n){name=n;};
    string getName(){return name;};
    vector<int>& getInouts(){return inouts;};
    string getInout(int n);
    Net& getNet(int n){return nets[n];};
    vector<Net>& getNets(){return nets;};
    bool setIOType(string name, IOType t);
    IOType getIOType(int net){return inouts_type[net];};
    void appendNetNameSuffix(string n);
    string& getNetName(int n){return nets[n].name;};
    bool insertInOut(string name);
    int insertNet(string& name, TransTerminalType type, int t);
    void insertInstance(string instName, string subCircuit, vector<string> &ports);
    map<string,CellInstance>& getInstances(){return instances;};
    bool check();
    void folding(float pSize, float nSize, string VddNet_Name, string GndNet_Name);
    void insertTrans(string name, string d, string g, string s, TransType t, float l, float w);
    unsigned long size(){return trans.size();};
    int pSize();
    int nSize();
    Transistor& getTrans(int ID){ return trans[ID];};
    void print();
    void clear();
    bool transPlacement(bool ep, int saquality, int nrattempts, int wC, int gmC, int rC, int congC, int ngC, string vddNet, string gndNet);
    void printPlacement();
    unsigned int calcWeight(vector<TransitorTerminal>& eulerPathP,vector<TransitorTerminal>& eulerPathN);
    int getMaxCongestioning(){return maxCong;};
    int getWidth(){return posPN;};
    void move(const vector<TransitorTerminal>& ref, vector<TransitorTerminal>& cp);
    vector<TransitorTerminal>& getOrderingP(){return orderingP;};
    vector<TransitorTerminal>& getOrderingN(){return orderingN;};
    void setOrderingP(vector<TransitorTerminal>& ordering){orderingP=ordering;};
    void setOrderingN(vector<TransitorTerminal>& ordering){orderingN=ordering;};
    
    int perturbation(); //Retorna o custo da perturbacao
    int getCost(); //Retorna o custo
    int getProblemSize(); //Retorna um numero qualquer que indica o tamanho do problem (ex. numero de transistores)
    void undoPerturbation(); //Desfaz a ultima perturbacao
    bool isIO(string n);
    
    //Métodos para o algoritmo de folging nas series
    bool defineIOToGraph(string Vdd_Name, string Gnd_Name);
    bool isOut(int out_id);
    bool isGate(int gate_id);
    bool wasVisit(int indexNet);
    int getSeriesToFolding(int currentNet, int currentTrans, int nextNet);
    void findSeriesToFolding(int startNet);
    void foldingSeries(float pSize, float nSize);
    bool seriesFolding(int numSequence, int numTrans, int numLegs, int biggerTrans);

protected:
	string name;
	vector<Transistor> trans;
	map<string,CellInstance> instances;
	vector<int> inouts;
	vector<IOType> inouts_type;
	vector<Net> nets;		//List of transistors and instances that are connected to each net
	vector<TransitorTerminal> orderingP, orderingN, cpP, cpN, tmpP, tmpN;
	int PorN, custo_atual, custo_anterior;
	int wGaps, mismatchesGate, wRouting, maxCong, posPN, wCost, gmCost, rCost, congCost, ngCost;
        string vddNet, gndNet;
    
    //Atributos para novo folding
    vector<int> gates; //Vetor que armazenará as nets que são gates
    vector<int> outs; //Vetor que armazenara o indice das nets que sao saidas
    int gnd; // Net que e GND
    int vcc; // Net que e VCC/VDD
    vector<int> visitToFolding; //Transistores que ja foram visitados na busca dos transistores em serie
    int **transToFolding; /*Matriz que armazena os transistores que estao em serie.
                           *Cada linha sera uma sequencia de transistores em serie, e cada coluna desta linha, sera um transistor desta serie*/
    int totalTrans; //Quantida total de transistores da celula
    int lineToFolding; //Iterador das linhas da matriz dos transistores que estao em serie
    int columnToFolding; //Iterador das colunas da matriz dos transistores que estao em serie
    
	bool eulerpath();
	bool visit(int nDifs, TransitorTerminal& transP, TransitorTerminal& transN);
	bool visit(int nDifs);
	bool verifyCnt(int pos);
	TransitorTerminal point(int link, TransTerminalType type);
	void move(vector<TransitorTerminal>& ref);
};
