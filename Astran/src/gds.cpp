/*
 * gds.cpp
 * Classe gds
 * Gera arquivo gds de celulas
 * Bernardo Honaiser - bphonaiser@inf.ufrgs.br
 */

#include "gds.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <cstdio>
#include <time.h>

Gds::Gds(string s) {
    filename = s;
    
    x1 = x2 = y1 = y2 = layer = 0;
    
    f.clear();
}

void Gds::add_longint(long int& l){
    conv_4b tmp;
    tmp.l=l;
    f.push_back(tmp.c[3]);
    f.push_back(tmp.c[2]);
    f.push_back(tmp.c[1]);
    f.push_back(tmp.c[0]);
}

bool Gds::open() {
    file.open(filename.c_str(), ios::binary);
    
    if(!file) {
        cerr << "ERROR: Cannot open file " << filename << endl;
        return false;
    }
    
    return true;
}

void Gds::generateHeader(int ver) {
    conv_2b tmp ;
    tmp.l = ver;
    
    f.push_back(0x00);
    f.push_back(0x06);
    f.push_back(0x00);
    f.push_back(0x02);
    f.push_back(tmp.c[1]);
    f.push_back(tmp.c[0]);
    
    write();
}


void Gds::generateUnits(){
    //  f.push_back(0x00);
    //   f.push_back(0x14);
    //   f.push_back(0x03);
    //   f.push_back(0x05);
    //   f.push_back(0x3F);
    //   f.push_back(0x28);
    //   f.push_back(0xF5);
    //   f.push_back(0xC2);
    //   f.push_back(0x8F);
    //   f.push_back(0x5C);
    //   f.push_back(0x28);
    //   f.push_back(0xF6);
    //   f.push_back(0x3A);
    //   f.push_back(0x2A);
    //   f.push_back(0xF3);
    //   f.push_back(0x1D);
    //   f.push_back(0xC4);
    //   f.push_back(0x61);
    //   f.push_back(0x18);
    //   f.push_back(0x74);
    
    f.push_back(0x00);
    f.push_back(0x14);
    f.push_back(0x03);
    f.push_back(0x05);
    f.push_back(0x3E);
    f.push_back(0x41);
    f.push_back(0x89);
    f.push_back(0x37);
    f.push_back(0x4B);
    f.push_back(0xC6);
    f.push_back(0xA7);
    f.push_back(0xEF);
    f.push_back(0x39);
    f.push_back(0x44);
    f.push_back(0xB8);
    f.push_back(0x2F);
    f.push_back(0xA0);
    f.push_back(0x9B);
    f.push_back(0x5A);
    f.push_back(0x51);
    
    write();
}

void Gds::generateLibrary() {
    time(&tempo);
    ts = *localtime(&tempo);
    
    conv_2b tmp,tmp2,tmp3,tmp4,tmp5,tmp6 ;
    
    tmp.l = ts.tm_year + 1900 ;
    tmp2.l = ts.tm_mon + 1 ;
    tmp3.l = ts.tm_mday ;
    tmp4.l = ts.tm_hour ;
    tmp5.l = ts.tm_min ;
    tmp6.l = ts.tm_sec ;
    
    f.push_back(0x00);
    f.push_back(0x1C);
    f.push_back(0x01);
    f.push_back(0x02);
    f.push_back(tmp.c[1]); // ano
    f.push_back(tmp.c[0]);
    f.push_back(tmp2.c[1]); // mes
    f.push_back(tmp2.c[0]);
    f.push_back(tmp3.c[1]); // dia
    f.push_back(tmp3.c[0]);
    f.push_back(tmp4.c[1]); // hora
    f.push_back(tmp4.c[0]);
    f.push_back(tmp5.c[1]); // min
    f.push_back(tmp5.c[0]);
    f.push_back(tmp6.c[1]); // seg
    f.push_back(tmp6.c[0]);
    f.push_back(tmp.c[1]); // ano
    f.push_back(tmp.c[0]);
    f.push_back(tmp2.c[1]); // mes
    f.push_back(tmp2.c[0]);
    f.push_back(tmp3.c[1]); // dia
    f.push_back(tmp3.c[0]);
    f.push_back(tmp4.c[1]); // hora
    f.push_back(tmp4.c[0]);
    f.push_back(tmp5.c[1]); // min
    f.push_back(tmp5.c[0]);
    f.push_back(tmp6.c[1]); // seg
    f.push_back(tmp6.c[0]);
    
    write();
}


void Gds::generateLibname(char* a) {
    int j = static_cast<int>(strlen(a)) ;
    conv_2b tmp;
    tmp.l = j + 4;
    bool x = false;
    
    if(j % 2 != 0){
        tmp.l++;
        x = true;
    }
    
    f.push_back(tmp.c[1]);
    f.push_back(tmp.c[0]);
    f.push_back(0x02);
    f.push_back(0x06);
    
    for(int i=0 ; i<j ; i++)
        f.push_back(a[i]);
    
    if(x)
        f.push_back(0x00);
    
    write();
}


void Gds::generateStruct() {
    time(&tempo);
    ts = *localtime(&tempo);
    
    conv_2b tmp,tmp2,tmp3,tmp4,tmp5,tmp6 ;
    
    tmp.l = ts.tm_year ;
    tmp2.l = ts.tm_mon ;
    tmp3.l = ts.tm_mday ;
    tmp4.l = ts.tm_hour ;
    tmp5.l = ts.tm_min ;
    tmp6.l = ts.tm_sec ;
    
    f.push_back(0x00);
    f.push_back(0x1C);
    f.push_back(0x05);
    f.push_back(0x02);
    f.push_back(tmp.c[1]); // ano
    f.push_back(tmp.c[0]);
    f.push_back(tmp2.c[1]); // mes
    f.push_back(tmp2.c[0]);
    f.push_back(tmp3.c[1]); // dia
    f.push_back(tmp3.c[0]);
    f.push_back(tmp4.c[1]); // hora
    f.push_back(tmp4.c[0]);
    f.push_back(tmp5.c[1]); // min
    f.push_back(tmp5.c[0]);
    f.push_back(tmp6.c[1]); // seg
    f.push_back(tmp6.c[0]);
    f.push_back(tmp.c[1]); // ano
    f.push_back(tmp.c[0]);
    f.push_back(tmp2.c[1]); // mes
    f.push_back(tmp2.c[0]);
    f.push_back(tmp3.c[1]); // dia
    f.push_back(tmp3.c[0]);
    f.push_back(tmp4.c[1]); // hora
    f.push_back(tmp4.c[0]);
    f.push_back(tmp5.c[1]); // min
    f.push_back(tmp5.c[0]);
    f.push_back(tmp6.c[1]); // seg
    f.push_back(tmp6.c[0]);
    
    write();
}


void Gds::generateStructname(char* a) {
    int j = static_cast<int>(strlen(a)) ;
    conv_2b tmp;
    tmp.l = j + 4;
    bool x = false;
    
    if(j % 2 != 0){
        tmp.l++;
        x = true;
    }
    
    f.push_back(tmp.c[1]);
    f.push_back(tmp.c[0]);
    f.push_back(0x06);
    f.push_back(0x06);
    
    for(int i=0 ; i<j ; i++)
        f.push_back(a[i]);
    
    if(x)
        f.push_back(0x00);
    
    
    write();
}

void Gds::generateString(const char* a) {
    int j = static_cast<int>(strlen(a)) ;
    conv_2b tmp;
    tmp.l = j + 4;
    bool x = false;
    
    if(j % 2 != 0){
        tmp.l++;
        x = true;
    }
    
    f.push_back(tmp.c[1]);
    f.push_back(tmp.c[0]);
    f.push_back(0x19);
    f.push_back(0x06);
    
    for(int i=0 ; i<j ; i++)
        f.push_back(a[i]);
    
    if(x)
        f.push_back(0x00);
}


void Gds::add_Layer(short int& layer){
    f.push_back(0x00);
    f.push_back(0x06);
    f.push_back(0x0D);
    f.push_back(0x02);
    f.push_back(0x00);
    f.push_back(layer);
}


void Gds::add_Boxtype(short int& boxtype){
    f.push_back(0x00);
    f.push_back(0x06);
    f.push_back(0x0D);
    f.push_back(0x02);
    f.push_back(0x00);
    f.push_back(boxtype);
    
    write();
}



void Gds::add_XY(long int& x1, long int& y1, long int& x2, long int& y2){
    add_longint(x1);
    add_longint(y1);
    add_longint(x2);
    add_longint(y1);
    add_longint(x2);
    add_longint(y2);
    add_longint(x1);
    add_longint(y2);
    add_longint(x1);
    add_longint(y1);
}


void Gds::generateBox(short int layer, long int x1, long int y1, long int x2, long int y2){
    f.push_back(0x00);
    f.push_back(0x04);
    f.push_back(0x2D);
    f.push_back(0x00);
    add_Layer(layer);
    
    f.push_back(0x00);
    f.push_back(0x2C);
    f.push_back(0x10);
    f.push_back(0x03);
    add_XY(x1, y1, x2, y2);
    
    write();
}

void Gds::generateLabel(short int layer, long int x, long int y, const char* text){
    f.push_back(0x00);
    f.push_back(0x04);
    f.push_back(0x0C);
    f.push_back(0x00);
    
    add_Layer(layer);
    
    f.push_back(0x00);
    f.push_back(0x06);
    f.push_back(0x16);
    f.push_back(0x02);
    f.push_back(0x00);
    f.push_back(0x00);
    
    f.push_back(0x00);
    f.push_back(0x0C);
    f.push_back(0x1B);
    f.push_back(0x05);
    f.push_back(0x3f);
    f.push_back(0xf0);
    f.push_back(0x00);
    f.push_back(0x00);
    f.push_back(0x00);
    f.push_back(0x00);
    f.push_back(0x00);
    f.push_back(0x00);
    
    f.push_back(0x00);
    f.push_back(0x0C);
    f.push_back(0x10);
    f.push_back(0x03);
    add_longint(x);
    add_longint(y);
    
    generateString(text);
    write();
}

void Gds::generateEndelement(){
    f.push_back(0x00);
    f.push_back(0x04);
    f.push_back(0x11);
    f.push_back(0x00);
    
    write();
}


void Gds::generateEndStruct(){
    f.push_back(0x00); 
    f.push_back(0x04); 
    f.push_back(0x07);
    f.push_back(0x00);
    
    write();
}


void Gds::generateEndLibrary(){
    f.push_back(0x00); 
    f.push_back(0x04); 
    f.push_back(0x04);
    f.push_back(0x00);
    
    write();
}

void Gds::write(){
    char *buffer = new char[f.size()];
    
    for(unsigned int i = 0; i < f.size(); i++)  
        //file.put(f[i]);
        buffer[i]=f[i];
    
    file.write(buffer,f.size());
    f.clear();
    
    delete[] buffer;
}

Gds::~Gds() {
    file.close();
}




