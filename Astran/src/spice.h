/**************************************************************************
*   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Renato Hentschke      *
*   (amziesemerj,renato)[at]@inf.ufrgs.br                                 *
***************************************************************************/
#pragma once

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

class Spice{
public:
    virtual ~Spice()=0;
    
    static void readFile(const string& fileName, Circuit& netlist, const bool reading_cadence){
         ifstream arq (fileName.c_str());
         string linha;
        
         if (!arq.is_open())
             throw AstranError("Could not open Spice file: " + fileName );
        
         vector<string> palavras;
         string palavra;
        
         CellNetlst subcktCell,topCell;
         CellNetlst *currentCell=&topCell;
         topCell.setName(upcase(removeExt(getFileName(fileName))));
         string cellName;
         unsigned int lineNr=0;
         while (!arq.eof()){
             lineNr++;
             getline(arq,linha);
            
             palavras.clear();
            
             istrstream clin(linha.c_str());
            
             while (clin>>palavra)
                 palavras.push_back(upcase(palavra));
            
             if (palavras.size() == 0 || palavras[0] == ".GLOBAL") continue;
            
             if (palavras[0] == "*INTERFACE"){
                 if(palavras.size() == 4 || palavras.size() == 6){
                     IOType type;
                     direction orient;
                     switch(palavras[2][palavras[2].size()-1]){
                         case 'N': orient=N; break;
                         case 'S': orient=S; break;
                         case 'E': orient=E; break;
                         case 'W': orient=W; break;
                         default:
                             throw AstranError("Line" + to_string(lineNr) + ": Interface orientation unknown.");
                            
                     }
                     switch(palavras[3][0]){
                         case 'I': type=IOTYPE_INPUT; break;
                         case 'O': type=IOTYPE_OUTPUT; break;
                         default:
                             throw AstranError("Line" + to_string(lineNr) + ": Interface type unknown. Use INPUT or OUTPUT");
                     }
                     topCell.insertInOut(palavras[1]);
                     netlist.insertInterface(palavras[1], orient, type, 0, 0);
                 }
                 else
                     throw AstranError("Line" + to_string(lineNr) + ": Number of parameters for *interface is not correct");
                
                 continue;
             }
            
             if (reading_cadence && palavras[0] == "*" && palavras.size() == 5 && palavras[1] == "SUBCIRCUIT"){
                 currentCell->clear();
                 topCell.setName(palavras[4].substr(0,palavras[4].size()-1));
             }
            
             if (palavras[0][0] == '*' || palavras[0][0] == 'C' || palavras[0][0] == '+' || palavras[0][0] == 'D' || (palavras[0][0]=='X' && reading_cadence)) // corrigir aqui para ler o '+' e ignorar os par‚metros desnecess·rios
                 continue;
            
             if (palavras[0] == ".MODEL" || palavras[0] == ".END") break;
            
             if (palavras[0] == ".SUBCKT" && currentCell==&topCell && !reading_cadence){
                 // It's a new cell definition. . .
                 subcktCell.clear();
                 currentCell=&subcktCell;
                 cellName=palavras[1];
                
                 // compare if subcircuit name is the same as the top cell name
                 if(cellName == topCell.getName()){
                     string topname = topCell.getName() + "-TOP";
                     topCell.setName(topname);
                 }
                
                 for (int p=2; p<palavras.size(); p++)
                     currentCell->insertInOut(palavras[p]);
                
             }
             else if (palavras[0] == string(".INCLUDE")){
                 readFile(getPath(fileName)+palavras[1],netlist,reading_cadence);
                 //                throw AstranError("Could not read included file in line: " + intToStr(lineNr));
             }
            
             // declaring transistor in subcircuit read from Cadence
             else if (palavras[0][0] == 'M' && palavras.size()>=5){
                
                 // insert in and out pins
                 if (reading_cadence){
                     for (int p=1; p<5; ++p){
                         if (!isNumber(palavras[p]) || palavras[p] == "0")
                             currentCell->insertInOut(palavras[p]);
                     }
                 }
                
                 // identify transistor type
                 TransType type;
                 if(palavras[5]=="PMOS" || palavras[5]=="CMOSP" || palavras[5]=="MODP" || palavras[5]=="PMOS_VTL")
                     type=PMOS;
                 else if(palavras[5]=="NMOS" || palavras[5]=="CMOSN" || palavras[5]=="MODN" || palavras[5]=="NMOS_VTL")
                     type=NMOS;
                 else
                     throw AstranError("Line" + to_string(lineNr) + ": Parameter " + palavras[5] + " is incorrect. Use NMOS or PMOS");
                
                 // get parameters' values
                 float length=0, width=0;
                 for (int p=6; p<palavras.size(); p++){
                     int pos=static_cast<int>(palavras[p].find("="));
                     string parm=palavras[p].substr(0,pos++);
                     float  tam=atof((palavras[p].substr(pos)).c_str())*getFactor(palavras[p][palavras[p].size()-1])*getFactor(palavras[p][palavras[p].size()-2]);
                     if(parm=="L")
                         length=tam;
                     else if(parm=="W")
                         width=tam;
                     else if(parm!="AD" && parm!="PD" && parm!="AS" && parm!="PS" && parm!="NRD" && parm!="NRS" && parm!="M")
                         throw AstranError("Line" + to_string(lineNr) + ": Parameter " + parm + " not supported");
                 }
                
                 // insert transistor in cell
                 currentCell->insertTrans(palavras[0], palavras[1], palavras[2], palavras[3], type, length, width);
             }
            
             else if (palavras[0][0] == 'X' && !reading_cadence){
                 string instName=palavras[0];
                 vector<string> net_ids;
                 int p;
                 for (p=1; p<palavras.size()-1; p++)
                     net_ids.push_back(palavras[p]);
                 currentCell->insertInstance(instName, palavras[p], net_ids);
             }
             else if (currentCell==&subcktCell && palavras[0] == ".ENDS"){
                 currentCell->setName(cellName);
                 netlist.insertCell(*currentCell);
                 currentCell=&topCell;
             }
             else
                 throw AstranError("Errot in line " + to_string(lineNr) + " of: " + fileName);
         }
        
         if(topCell.getNets().size() != 0)
             netlist.insertCell(topCell);
     }
    
    static void saveFile(const string& fileName, Circuit& netList){
         ofstream file;
         file.open(fileName.c_str()); // Write
         if (!file)
             throw AstranError("Could not save file: " + fileName);
        
         printHeader (file, "* ", "");
        
         map<string, CellNetlst>::iterator cells_it;
         for ( cells_it = netList.getCellNetlsts()->begin(); cells_it != netList.getCellNetlsts()->end(); cells_it++ ){
             file << ".SUBCKT " <<  cells_it->first;
             for ( vector<int>::iterator inouts_it=cells_it->second.getInouts().begin(); inouts_it != cells_it->second.getInouts().end(); inouts_it++ )
                 file << " " << cells_it->second.getNetName(*inouts_it);
             file << endl;
             for(map<string,CellInstance>::iterator tmp=cells_it->second.getInstances().begin(); tmp!=cells_it->second.getInstances().end(); ++tmp){
                 file << tmp->first << " ";
                 for(vector<int>::iterator tmp2=tmp->second.ports.begin(); tmp2!=tmp->second.ports.end(); ++tmp2)
                     file << cells_it->second.getNetName(*tmp2) << " ";
                 file << tmp->second.subCircuit << endl;
             }		
             for(int x=0; x<cells_it->second.size(); x++){
                 file << cells_it->second.getTrans(x).name << " " << 
                 cells_it->second.getNetName(cells_it->second.getTrans(x).drain) << " " << 
                 cells_it->second.getNetName(cells_it->second.getTrans(x).gate) << " " << 
                 cells_it->second.getNetName(cells_it->second.getTrans(x).source) << " ";
                 if(cells_it->second.getTrans(x).type==PMOS) 
                     file << netList.getVddNet() << " PMOS";
                 else
                     file << netList.getGndNet() << " NMOS";
                 file << " L=" << cells_it->second.getTrans(x).length << "U W=" << cells_it->second.getTrans(x).width << "U"<< endl;			
             }
             file << ".ENDS " << cells_it->first << endl << endl;
         }
     }

private:
    static float getFactor(char s){
         switch(s){
             case 'U': return 1;
             case 'N': return 0.001;  //fill with the others
             default: return 1;
         }
     }
};