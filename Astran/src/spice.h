/**************************************************************************
*   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Renato Hentschke      *
*   (amziesemerj,renato)[at]@inf.ufrgs.br                                 *
***************************************************************************/

#ifndef SPICE_H
#define SPICE_H

/**
Read a netlist SPICE file

@author Adriel Mota Ziesemer Jr., Renato Hentschke
*/

#include <iostream>
#include <fstream>
#include <strstream>
#include <string>
#include "circuit.h"
#include "util.h"

using namespace std;

class Spice{
private:
	float getFactor(char s);

public:
	bool readFile(string nome, Circuit& netlist, bool reading_cadence);
	bool saveFile(string filename, Circuit& netList);
};

#endif
