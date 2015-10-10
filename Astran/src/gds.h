/*
 * gds.h
 * veja gds.cpp
 * Bernardo Honaiser - bphonaiser@inf.ufrgs.br
 */

#ifndef _GDS_H_

#define _GDS_H_

#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


using namespace std;

class Gds {
private:
   ofstream file;
   string filename;
   vector<char> f;
   time_t tempo;
   tm ts;
   long int x1, x2, y1, y2;
   short int layer;
   void write();

public:
   Gds(string);
   ~Gds();
   bool open();
   void generateHeader(int); 
   void generateUnits();
   void generateLibrary(); 
   void generateLibname(char*);
   void generateStruct(); 
   void generateStructname(char*);
   void generateString(const char* a);
   void add_Layer(short int &);
   void add_Boxtype(short int &);
   void add_XY(long int&, long int&, long int&, long int&);
   void generateBox(short int,long int, long int, long int, long int);
   void generateLabel(short int layer, long int x, long int y, const char* text);
   void generateEndelement();
   void generateEndStruct();
   void generateEndLibrary();
   void add_longint(long int&);
};


#endif