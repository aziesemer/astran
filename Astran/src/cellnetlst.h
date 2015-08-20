/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
 *   [amziesemerj[at]@inf.ufrgs.br                                      *
 **************************************************************************/
#ifndef CELLNETLST_H
#define CELLNETLST_H

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

using namespace std;

enum transType {PMOS, NMOS};

enum t_DGorS {DRAIN, GATE, SOURCE, GAP};

// Point the net to a transistor
struct t_net2{
	int link;
	t_DGorS type;
};

struct t_inst{
	string targetCellInst;
	int targetPin;
};

// Lista encadeada com os transistores da mesma rede
struct t_net{
	list<t_net2> trans;
	list<t_inst> insts;	
	string name;
};

struct Trans{
	string name;
	bool visited;
	unsigned int drain, gate, source;
	transType type;
	float length,width;	
};

struct Inst{
	string subCircuit;
	vector<int> ports;
};

class CellNetlst{
protected:
	string name;
	vector<Trans> trans;
	map<string,Inst> instances;
	vector<int> inouts;
	vector<IOType> inouts_type;
	vector<t_net> nets;		//List of transistors and instances that are connected to each net
	vector<t_net2> orderingP, orderingN, cpP, cpN, tmpP, tmpN;
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
	bool visit(int nDifs, t_net2& transP, t_net2& transN);
	bool visit(int nDifs);
	bool verifyCnt(int pos);
	t_net2 point(int link, t_DGorS type);
	
	void move(vector<t_net2>& ref);
	
public:
	void setName(string n){name=n;};
	string getName(){return name;};
	vector<int>& getInouts(){return inouts;};
	string getInout(int n);
	t_net& getNet(int n){return nets[n];};
	vector<t_net>& getNets(){return nets;};
	bool setIOType(string name, IOType t);
	IOType getIOType(int net){return inouts_type[net];};
	void appendNetNameSuffix(string n);
	string& getNetName(int n){return nets[n].name;};
	bool insertInOut(string name);
	int insertNet(string& name, t_DGorS type, int t);
	void insertInstance(string instName, string subCircuit, vector<string> &ports);
	map<string,Inst>& getInstances(){return instances;};
	bool check();
	void folding(float pSize, float nSize);
	void insertTrans(string name, string d, string g, string s, transType t, float l, float w);
	int size(){return trans.size();};
	int pSize();
	int nSize();
	Trans& getTrans(int ID){ return trans[ID];};
	void print();
	void clear();
	bool transPlacement(bool ep, int saquality, int nrattempts, int wC, int gmC, int rC, int congC, int ngC, string vddNet, string gndNet);
    void printPlacement();
	unsigned int calcWeight(vector<t_net2>& eulerPathP,vector<t_net2>& eulerPathN);
    int getMaxCongestioning(){return maxCong;};
    int getWidth(){return posPN;};
	void move(const vector<t_net2>& ref, vector<t_net2>& cp);	
	vector<t_net2>& getOrderingP(){return orderingP;};
	vector<t_net2>& getOrderingN(){return orderingN;};
    void setOrderingP(vector<t_net2>& ordering){orderingP=ordering;};
	void setOrderingN(vector<t_net2>& ordering){orderingN=ordering;};

	int Perturbation(); //Retorna o custo da perturbacao
	int GetCost(); //Retorna o custo
	int GetProblemSize(); //Retorna um numero qualquer que indica o tamanho do problem (ex. numero de transistores)
	void UndoPerturbation(); //Desfaz a ultima perturbacao
	bool isIO(string n);
    
    //Métodos para o algoritmo de folging nas series
    bool defineIOToGraph();
    bool isOut(int out_id);
    bool isGate(int gate_id);
    bool wasVisit(int indexNet);
    int getSeriesToFolding(int currentNet, int currentTrans, int nextNet);
    void findSeriesToFolding(int startNet);
    void foldingSeries(float pSize, float nSize);
    bool seriesFolding(int numSequence, int numTrans, int numLegs, int biggerTrans);
    
};

#endif
