//
//  ilptransplacer.cpp
//  Astran
//
//  Created by Adriel Ziesemer Jr. on 15/08/16.
//
//

#include "ilptransplacer.hpp"

IlpTransPlacer::IlpTransPlacer(CellNetlst &net): netlist(net){
    TransitorTerminal tmp;
    //this->netlist = netlists;
    tmp.type=DRAIN;
    unsigned long i;
    for(i=0;i < netlist.getTrans().size();++i){
        tmp.link= static_cast<int>(i);
        if(netlist.getTrans()[i].type==PMOS)
            orderingP.push_back(tmp);
        else
            orderingN.push_back(tmp);
    }
    
    tmp.type=DRAIN;
    tmp.link=-1;
    unsigned long width=max(orderingP.size(), orderingN.size());
    for(i=orderingP.size();i<width;i++)
        orderingP.push_back(tmp);
    for(i=orderingN.size();i<width;i++)
        orderingN.push_back(tmp);
    
    //unsigned long Cnum = width;
    Cnum = width;
}


bool IlpTransPlacer::transistorPlacement(){
    // Each transistor i should be placed in exactly 1 column j
    try{
        for (int i = 0; i < Cnum; i++) {
            GRBLinExpr exprP = 0;
            GRBLinExpr exprN = 0;
            for (int j = 0; j < Cnum; j++){
                exprP += P[i][j];
                exprN += N[i][j];
            }
            model.addConstr(exprP == 1, "C1_"+to_string(i));
            model.addConstr(exprN == 1, "C3_"+to_string(i));
        }
        
        // Each column should contein exactly 1 transistor
        for (int j = 0; j < Cnum; j++) {
            GRBLinExpr exprP = 0;
            GRBLinExpr exprN = 0;
            for (int i = 0; i < Cnum; i++){
                exprP += P[i][j];
                exprN += N[i][j];
            }
            model.addConstr(exprP == 1, "C2_"+to_string(j));
            model.addConstr(exprN == 1, "C4_"+to_string(j));
        }
    } catch (...) {
        cout << "Error during placement optimization" << endl;
        cout << "Transistor Placement" << endl;
        return false;
    }
    return true;
}

bool IlpTransPlacer::transistorPairing(){

    try{
        cout << "Transistor Pariring" << endl;
        
        GRBVar nrPairs = model.addVar(0.0, Cnum, -10, GRB_CONTINUOUS, "Pairs");
        GRBLinExpr expr = -nrPairs;
        for (int x = 0; x < Cnum; x++){
            for (int y = 0; y < Cnum; y++){
                if(orderingP[x].link!=-1 && orderingN[y].link!=-1 && netlist.getTrans()[orderingP[x].link].gate==netlist.getTrans()[orderingN[y].link].gate){
                    for (int j = 0; j < Cnum; j++){
                        GRBVar pair = model.addVar(0.0, 1.0, 0, GRB_BINARY, "Pair_"+to_string(x)+"_"+to_string(y)+"_"+to_string(j));
                        model.update();
                        model.addConstr(pair - P[x][j] - N[y][j] >= -1, "D1_"+to_string(x)+"_"+to_string(y)+"_"+to_string(j)); // Logical AND operation
                        model.addConstr(pair - P[x][j]  <= 0, "D2_"+to_string(x)+"_"+to_string(y)+"_"+to_string(j));
                        model.addConstr(pair - N[y][j]  <= 0, "D3_"+to_string(x)+"_"+to_string(y)+"_"+to_string(j));
                        expr += pair;
                    }
                }
            }
        }
        model.addConstr(expr == 0, "D");
    } catch (...) {
        cout << "Error during placement optimization" << endl;
        cout << "Transistor Pairing" << endl;
        return false;
    }
    return true;
}

// Transistor abutment is not working now
bool IlpTransPlacer::transistorAbutment(){
    cout << "Transistor Abutment" << endl;
    try{
        for (int j = 0; j < Cnum-1; j++) {
            for (int x = 0; x < Cnum; x++) {
                
                //Get the signals connected in the drain/source of transistor Px
                int SD_source = netlist.getTrans( orderingP[x].link ).source;
                int SD_drain = netlist.getTrans( orderingP[x].link ).drain;
                
                vector<TransitorTerminal> B_Px; // Vector with transistors that can abut diffusion with trasistor X
                
                for (list<TransitorTerminal>::iterator it = netlist.getNet(SD_source).trans.begin(); it != netlist.getNet(SD_source).trans.end(); it++) {
                    if (netlist.getTrans(it->link).type == PMOS)
                        B_Px.push_back(*it);
                }
                for (list<TransitorTerminal>::iterator it = netlist.getNet(SD_drain).trans.begin(); it != netlist.getNet(SD_drain).trans.end(); it++) {
                    if (netlist.getTrans(it->link).type == PMOS){
                        //Pesquisa se o transistor jah esta no conjunto B
                        unsigned int b_i = 0;
                        for (b_i = 0; b_i < B_Px.size(); b_i++) {
                            if (netlist.getTrans(it->link).name == netlist.getTrans(B_Px[b_i].link).name)
                                break;
                        }
                        if (b_i == B_Px.size())
                            B_Px.push_back(*it);
                    }
                }
                
                //                B_Px.insert(B_Px.end(), netlist.getNet(SD_source).trans.begin(),netlist.getNet(SD_source).trans.end());
                //                B_Px.insert(B_Px.end(), netlist.getNet(SD_drain).trans.begin(),netlist.getNet(SD_drain).trans.end());
                
                GRBLinExpr expr_Ap = 0;
                //model.update();
                for (int xi = 0; xi < B_Px.size(); xi++) {
                    GRBVar Ox_xi = model.addVar(0.0, 1.0, 0, GRB_BINARY, "O_x_"+to_string(x)+"_xi_"+to_string(xi));
                    GRBVar Oxi_x = model.addVar(0.0, 1.0, 0, GRB_BINARY, "O_xi_"+to_string(xi)+"_x_"+to_string(x));
                    model.update();
                    
                    //model.addConstr(Ox_xi == (1 - O_P[x])*O_P[xi], "E1_Ox_xi_"+to_string(x)+"_"+to_string(xi));
                    //model.addConstr(Oxi_x == (1 - O_P[xi])*O_P[x], "E1_Oxi_x_"+to_string(xi)+"_"+to_string(x));
                    
                    // Logical AND Operation - (1 - O_P[x])*O_P[xi] --- P(x,j+1)P(xi,j)
                    //y ≥ x1 + x2 − 1, y ≤ x1, y ≤ x2
                    
                    model.addConstr(Ox_xi >= (1 - O_P[x]) + O_P[xi] -1, "E1_OR_Ox_xi_"+to_string(x)+"_"+to_string(xi));
                    model.addConstr(Ox_xi <= (1 - O_P[x]), "E1_Ox_xi_"+to_string(x)+"_"+to_string(xi)+"_OR_1-Op_x_" + to_string(x));
                    model.addConstr(Ox_xi <= O_P[xi], "E1_Ox_xi_"+to_string(x)+"_"+to_string(xi)+"_OR_Op_xi_" + to_string(xi));
                    
                    
                    model.addConstr(Oxi_x == (1 - O_P[xi]) + O_P[x] - 1, "E1_OR_Oxi_x_"+to_string(xi)+"_"+to_string(x));
                    model.addConstr(Oxi_x <= (1 - O_P[xi]), "E1_Oxi_x_"+to_string(xi)+"_"+to_string(x)+"_OR_1_Op_xi_" + to_string(xi));
                    model.addConstr(Oxi_x <= O_P[x], "E1_Oxi_x_"+to_string(xi)+"_"+to_string(x)+"_OR_Op_x_" + to_string(x));
                    
                    
                    //    for (int j = 0; j < Cnum-1; j++) {
                    
                    GRBVar Aux_firstTerm = model.addVar(0.0, 1.0, 0, GRB_BINARY, "Aux_firstTerm_E_x_xi_j_"+to_string(x)+"_"+to_string(xi)+"_"+to_string(j));
                    model.update();
                    
                    // Logical AND Operation - P(x,j)P(xi,j+1)O(x,xi)
                    //y ≥ x1 + x2 + x3 − 2, y ≤ x1, y ≤ x2, y <= x3
                    model.addConstr( Aux_firstTerm - P[x][j] - P[xi][j+1] - Ox_xi >= -2, "E2_j_"+to_string(j)+"_x_"+to_string(x)+"_xi_"+to_string(xi));
                    model.addConstr(Aux_firstTerm - P[x][j]  <= 0, "E3_x_j_"+to_string(x)+"_"+to_string(j));
                    model.addConstr(Aux_firstTerm - P[xi][j+1]  <= 0, "E4_xi_j+1_"+to_string(xi)+"_"+to_string(j+1));
                    model.addConstr(Aux_firstTerm - Ox_xi <= 0, "E5_Ox_xi_j_"+to_string(x)+"_"+to_string(xi)+"_"+to_string(j));
                    //model.update();
                    GRBVar Aux_secondTerm = model.addVar(0.0, 1.0, 0, GRB_BINARY, "Aux_secondTerm_E_x_xi_j_"+to_string(x)+"_"+to_string(xi)+"_"+to_string(j));
                    model.update();
                    // Logical AND Operation - P(x,j+1)P(xi,j)O(xi,x)
                    //y ≥ x1 + x2 + x3 − 2, y ≤ x1, y ≤ x2, y <= x3
                    model.addConstr( Aux_secondTerm - P[x][j+1] - P[xi][j] - Oxi_x >= -2, "E6_j_"+to_string(j)+"_x_"+to_string(x)+"_xi_"+to_string(xi));
                    model.addConstr(Aux_secondTerm - P[x][j+1]  <= 0, "E7_x_j+1_"+to_string(x)+"_"+to_string(j+1));
                    model.addConstr(Aux_secondTerm - P[xi][j]  <= 0, "E8_xi_j_"+to_string(xi)+"_"+to_string(j));
                    model.addConstr(Aux_secondTerm - Oxi_x <= 0, "E9_Oxi_x_j_"+to_string(xi)+"_"+to_string(x)+"_"+to_string(j));
                    model.update();
                    
                    // Logical OR Operation- P(x,j)P(xi,j+1)O(x,xi) + P(x,j+1)P(xi,j)O(xi,x)
                    //y ≤ x1 + x2, y ≥ x1, y ≥ x2
                    model.addConstr( A_P[j] - Aux_firstTerm - Aux_secondTerm <= 0, "E10_j_"+to_string(j)+"_1term_2term_x_"+to_string(x)+"_xi_"+to_string(xi));
                    model.addConstr(A_P[j] - Aux_firstTerm >= 0, "E11_1term_x_"+to_string(x)+"_xi_"+to_string(xi)+"_j_"+to_string(j));
                    model.addConstr(A_P[j] - Aux_secondTerm >= 0, "E12_2term_x_"+to_string(x)+"_xi_"+to_string(xi)+"_j_"+to_string(j));
                     expr_Ap += A_P[j];
                    model.update();
                    //}
                    
                }
                model.addConstr(expr_Ap == 1, "E_AP_j_"+to_string(j));
                model.update();
                
            }
            
        }
    } catch (...) {
        cout << "Error during placement optimization" << endl;
        cout << "Transistor Pairing" << endl;
        return false;
    }
    return true;
}

bool IlpTransPlacer::transPlacement(int wC, int gmC, int rC, int congC, int ngC, string vddNet, string gndNet){

    try {
        
        // Create binary decision variables
        this->P = new GRBVar*[Cnum]; //Transistor P Placement
        this->N = new GRBVar*[Cnum]; //Transistor N Placement
        O_P = new GRBVar[Cnum]; //Transistor P orientation (S-G-D)
        O_N = new GRBVar[Cnum]; //Transistor N orientation (S-G-D)
        A_P = new GRBVar[Cnum]; //Transistor P Abutment
        A_N = new GRBVar[Cnum]; //Transistor N Abutment
        
        
        for (int i = 0; i < Cnum; i++){
            P[i] = new GRBVar[Cnum];
            N[i] = new GRBVar[Cnum];
            
            O_P[i] = model.addVar(0.0, 1.0, 0, GRB_BINARY, "O_P_"+to_string(i));
            O_N[i] = model.addVar(0.0, 1.0, 0, GRB_BINARY, "O_N_"+to_string(i));
            A_P[i] = model.addVar(0.0, 1.0, 0, GRB_BINARY, "A_P_"+to_string(i));
            A_N[i] = model.addVar(0.0, 1.0, 0, GRB_BINARY, "P_N_"+to_string(i));
        }
        
        for (int i = 0; i < Cnum; i++) {
            for (int j = 0; j < Cnum; j++) {
                P[i][j] = model.addVar(0.0, 1.0, 0, GRB_BINARY, "P_"+to_string(i)+"_"+to_string(j));
                N[i][j] = model.addVar(0.0, 1.0, 0, GRB_BINARY, "N_"+to_string(i)+"_"+to_string(j));
            }
        }
        
        model.update();
        
        // C. Transistor Placement
        transistorPlacement();
        model.update();
        
        // D. Transistor Pairing
        transistorPairing();
        model.update();
        
        //E. Transistor Abutment (it is not working now)
        //transistorAbutment();
        
        //...
        
        // Optimize model
        model.update();
        model.write("result_place.lp");
        model.optimize();
        model.write("result_places.sol");

        //Put the corect transistor ordering in OrderingP vector
        vector<TransitorTerminal> orderingAux(Cnum);
        for (int i = 0; i < Cnum; i++) {
            for (int j = 0; j < Cnum; j++) {
                //If have number 1 in this variable, transistor i is placed in column j
                if (P[i][j].get(GRB_DoubleAttr_X) > 0.5){
                    cout << "P:" << i << "->" << j << endl;
                    orderingAux[j] = orderingP[i];
                    //To define the order of transistor terminal
//                    if (A_P[j].get(GRB_DoubleAttr_X)>0.5){
//                        orderingAux[j].type = SOURCE;
//                    } else {
//                        orderingAux[j].type = DRAIN;
//                    }
                }
            }
        }
        netlist.setOrderingP(orderingAux);
        
        //Put the corect transistor ordering in OrderingN vector
        for (int i = 0; i < Cnum; i++) {
            for (int j = 0; j < Cnum; j++) {
                //If have number 1 in this variable, transistor i is placed in column j
                if (N[i][j].get(GRB_DoubleAttr_X) > 0.5){
                    cout << "N:" << i << "->" << j << endl;
                    orderingAux[j] = orderingN[i];
                }
            }
        }
        netlist.setOrderingN(orderingAux);
        
        delete[] P;
        delete[] N;
    } catch (...) {
        cout << "Error during optimization" << endl;
    }
    
    delete env;
    
    return true;
}


