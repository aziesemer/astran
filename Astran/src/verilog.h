/*
 *  verilog.h
 *  ICPD
 *
 *  Created by Adriel Mota Ziesemer Jr. on 6/26/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef VERILOG_H
#define VERILOG_H

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <strstream>
#include "util.h"
#include "circuit.h"

using namespace std;

class Verilog{
	map<string,string> parameters, assigns;
	vector<string> inputs, outputs, inouts, wires;
	public:
	Verilog();
	~Verilog();
	int getValue(string v);
	int findNet(string v);
	bool readRange(vector<string>& words, int& n,  int& a, int& b);
	bool compareChars(string a, string b);
	bool compareChars(string a, string b, string c);
	bool readFile(string nome, Circuit& netlist);
	int parserFile(string& filename, vector<string>& words);
};

#endif
