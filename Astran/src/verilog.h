/*
 *  verilog.h
 *  ICPD
 *
 *  Created by Adriel Mota Ziesemer Jr. on 6/26/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <strstream>
#include "util.h"
#include "circuit.h"

class Verilog{
public:
    Verilog();
    ~Verilog();
    bool readFile(const string& nome, Circuit& netlist);
    
private:
	map<string,string> parameters, assigns;
	vector<string> inputs, outputs, inouts, wires;
    
    int getValue(const string v);
    int findNet(const string v);
    bool readRange(vector<string>& words, int& n,  int& a, int& b);
    bool compareChars(const string a, const string b);
    bool compareChars(const string a, const string b, const string c);
    int parserFile(const string& filename, vector<string>& words);
};
