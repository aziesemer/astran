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
private:
    //Variables to ILP Model
    GRBVar **P;// = new GRBVar*[Cnum]; //Transistor P Placement
    GRBVar **N;// = new GRBVar*[Cnum]; //Transistor N Placement
    GRBVar *O_P;// = new GRBVar[Cnum]; //Transistor P orientation (S-G-D)
    GRBVar *O_N;// = new GRBVar[Cnum]; //Transistor N orientation (S-G-D)
    GRBVar *A_P;// = new GRBVar[Cnum]; //Transistor P Abutment
    GRBVar *A_N;// = new GRBVar[Cnum]; //Transistor N Abutment
    
    GRBEnv *env = new GRBEnv();
    GRBModel model = GRBModel(*env);
    
    unsigned long Cnum;
    
    //Auxiliar variables
    vector<TransitorTerminal> orderingP, orderingN;
    CellNetlst &netlist;
    
    //Functions to placement
    bool transistorPlacement();
    //bool transistorPairing(CellNetlst &netlist);
    //bool transistorAbutment(CellNetlst &netlist);
    bool transistorPairing();
    bool transistorAbutment();
public:
    IlpTransPlacer(CellNetlst &net);
    bool transPlacement(int wC, int gmC, int rC, int congC, int ngC, string vddNet, string gndNet);
    
    
    
};



#endif /* ilptransplacer_hpp */
