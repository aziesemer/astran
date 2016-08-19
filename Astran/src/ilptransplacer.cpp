//
//  ilptransplacer.cpp
//  Astran
//
//  Created by Adriel Ziesemer Jr. on 15/08/16.
//
//

#include "ilptransplacer.hpp"

bool IlpTransPlacer::transPlacement(CellNetlst &netlist, int wC, int gmC, int rC, int congC, int ngC, string vddNet, string gndNet){
    vector<TransitorTerminal> orderingP, orderingN;
    cout << " ---------      Entrou na funcao placement ILP ------- "<< endl;
    TransitorTerminal tmp;
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

    unsigned long Cnum = width;
    GRBEnv *env = new GRBEnv();

    try {
        GRBModel model = GRBModel(*env);
        model.set(GRB_IntAttr_ModelSense, -1);

        // Create binary decision variables
        GRBVar **P = new GRBVar*[Cnum];
        GRBVar **N = new GRBVar*[Cnum];
        for (int i = 0; i < Cnum; i++){
            P[i] = new GRBVar[Cnum];
            N[i] = new GRBVar[Cnum];
        }
        for (int i = 0; i < Cnum; i++) {
            for (int j = 0; j < Cnum; j++) {
                P[i][j] = model.addVar(0.0, 1.0, 0, GRB_BINARY, "P_"+to_string(i)+"_"+to_string(j));
                N[i][j] = model.addVar(0.0, 1.0, 0, GRB_BINARY, "N_"+to_string(i)+"_"+to_string(j));
            }
        }

        model.update();

        // C. Transistor Placement
        // Each transistor i should be placed in exactly 1 column j
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
            model.addConstr(exprP == 1, "C2_"+to_string(i));
            model.addConstr(exprN == 1, "C4_"+to_string(i));
        }
        model.update();
        // D. Transistor Pairing
        for (int x = 0; x < Cnum; x++){
            for (int y = 0; y < Cnum; y++){
                if(orderingP[x].link!=-1 && orderingN[y].link!=-1 && netlist.getTrans()[orderingP[x].link].gate==netlist.getTrans()[orderingN[y].link].gate){
                    GRBVar pair = model.addVar(0.0, 1.0, -10, GRB_BINARY, "Pair_"+to_string(x)+"_"+to_string(y));
                    model.update();
                    for (int j = 0; j < Cnum; j++){
                        model.addConstr(pair - P[x][j] - N[y][j] >= -1, "D1_"+to_string(x)+"_"+to_string(y)); // Logical AND operation
                        model.addConstr(pair - P[x][j]  <= 0, "D2_"+to_string(x)+"_"+to_string(y));
                        model.addConstr(pair - N[y][j]  <= 0, "D3_"+to_string(x)+"_"+to_string(y));
                    }
                }
            }
        }

//...

        // Optimize model
        model.update();
        model.write("result.lp");
        model.optimize();
        model.write("result.sol");

        // Extract solution
        if (model.get(GRB_IntAttr_SolCount) > 0) {
            for (int i = 0; i < Cnum; i++)
                for (int j  =0; j < Cnum; j++)
                    if(P[i][j].get(GRB_DoubleAttr_X) > 0.5)
                        cout << i << "->" << j << endl;
        }

        //Put the corect transistor ordering in OrderingP vector
        vector<TransitorTerminal> orderingAux(width);
        orderingAux.clear();
        tmp.type=DRAIN;
        tmp.link=-1;
        for(i=0;i<width;i++){
            orderingAux.push_back(tmp);
        }

        for (int i = 0; i < Cnum; i++) {
            for (int j = 0; j < Cnum; j++) {
                //If have number 1 in this variable, transistor i is placed in column j
                if (P[i][j].get(GRB_DoubleAttr_X) > 0.5)
                    orderingAux.at(j) = orderingP[i];
            }
        }
        orderingP.clear();
        orderingP = orderingAux;
        netlist.setOrderingP(orderingAux);
        
        //Put the corect transistor ordering in OrderingN vector
        orderingAux.clear();
        tmp.type=DRAIN;
        tmp.link=-1;
        for(i=0;i<width;i++)
            orderingAux.push_back(tmp);

        for (int i = 0; i < Cnum; i++) {
            for (int j = 0; j < Cnum; j++) {
                //If have number 1 in this variable, transistor i is placed in column j
                if (N[i][j].get(GRB_DoubleAttr_X) > 0.5)
                    orderingAux.at(j) = orderingN[i];
            }
        }
        orderingN.clear();
        orderingN = orderingAux;
        netlist.setOrderingN(orderingAux);


        cout << "Número de dominós em sequência: " << model.get(GRB_DoubleAttr_ObjVal) << endl;
        delete[] P;
        delete[] N;
    } catch (...) {
        cout << "Error during optimization" << endl;
    }
    delete env;

    return true;
}
