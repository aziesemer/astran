/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Cristiano Lazzari     *
 *   (amziesemerj,clazz)@inf.ufrgs.br                                      *
 ***************************************************************************/
#pragma once

/**
 Generate the CIF file to the layout
 
 @author Adriel Mota Ziesemer Jr., Cristiano Lazzari
 */

#include <fstream>
#include <iostream>
#include <time.h>
#include <map>
#include <list>
#include "clayout.h"
#include "util.h"
#include "rules.h"

class Cif{
public:
    Cif(string fname, Rules &actual_rules);
    ~Cif();
    bool cellCif(map<string, CLayout>& layouts, string top);
    bool cif2Cadence(string designName, string top);
    
private:
    ofstream file;
    Rules *rules;
    int idx;
    int cifmetric;
    map<string, int> instances;
    string filename;
};