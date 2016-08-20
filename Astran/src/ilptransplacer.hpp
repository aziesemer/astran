//
//  ilptransplacer.hpp
//  Astran
//
//  Created by Adriel Ziesemer Jr. on 15/08/16.
//
//

#ifndef ilptransplacer_hpp
#define ilptransplacer_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include <iostream>
#include "/Library/gurobi650/mac64/include/gurobi_c++.h"
#include "cellnetlst.h"

using namespace std;

class IlpTransPlacer{
    
public:
    bool transPlacement(CellNetlst &netlist, int wC, int gmC, int rC, int congC, int ngC, string vddNet, string gndNet);
};



#endif /* ilptransplacer_hpp */
