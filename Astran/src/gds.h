/*
 * gds.h
 * veja gds.cpp
 * Bernardo Honaiser - bphonaiser@inf.ufrgs.br
 */

#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "util.h"

class Gds {
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

private:
    union conv_2b{
        char c[2];
        int l;
    };


    union conv_4b{
        char c[4];
        long int l;
    };

   ofstream file;
   string filename;
   vector<char> f;
   time_t tempo;
   tm ts;
   long int x1, x2, y1, y2;
   short int layer;
   void write();
};
