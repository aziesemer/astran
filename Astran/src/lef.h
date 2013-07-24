/**************************************************************************
*   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
*   amziesemerj[at]@inf.ufrgs.br                                          *
***************************************************************************/

#ifndef LEF_H
#define LEF_H

/**
Read a LEF library of cells

@author Adriel Mota Ziesemer Jr.
*/

#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include "circuit.h"
#include "rules.h"
#include "clayout.h"
#include "util.h"
#include <iomanip>

using namespace std;

class Lef{
	private:
	ifstream arq;

	unsigned int fileline;
	vector<string> words;
	unsigned int convertFactor;
	unsigned int cellsHeight;
	Rules *currentRules;
	unsigned int manufGrid;
	bool readTech; //read Layers in CIF (false) or Tech (true) format
	int readLine(vector<string>& words);

	public:
	Lef();
	~Lef();
	void readPort(Pin& tmp);
	void readObs(Draw& tmp, string net);
	void readPin(CellNetlst& c, string name, CLayout& layout);
	void readMacro(CellNetlst& c, CLayout& tmp);
	void readUnits();
	bool readFile(string nome, Circuit& c, bool rTech);
	bool saveFile(string nome, Circuit& c);
};

#endif
