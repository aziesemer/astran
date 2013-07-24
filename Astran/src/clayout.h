/***************************************************************************
*   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Cristiano Lazzari      *
*   amziesemerj[at]inf.ufrgs.br                                            *
***************************************************************************/
#ifndef CLayout_H
#define CLayout_H

/**
Class that has the circuit layout

@author Adriel Mota Ziesemer Jr.
*/

#include "draw.h"
#include "util.h"

using namespace std;

class Pin: public Point{
	private:
	layer_name layer;
	public:
	Pin(){};
	void setLayer(layer_name l){layer=l;};
	layer_name getLayer(){return layer;};
};

class CLayout;

class Instance: public Point{
	private:
	string targetCell;
	bool mx,my;
	
	public:
	Instance(){};
	Instance(string n){posx=posy=mx=my=0;targetCell=n;};
	~Instance(){};
	
	void setMX(bool m){mx=m;};
	void setMY(bool m){my=m;};
	bool getMX(){return mx;};
	bool getMY(){return my;};
	string getTargetCell(){return targetCell;};

	void print();
};


class CLayout: public Draw, public Box {	
	protected:
	string name;
	multimap<string,Pin> pins;	// Position of the ports
	map<string, Instance> instances;

	public:
	CLayout(string n);
	CLayout();
	~CLayout();
	
	string getName(){return name;};
	void setName(string n){name=n;};
	void setPin(string name, Pin p);
	multimap<string,Pin>& getPins(){ return pins;};
	Instance* getInstance(string n);
	map<string, Instance>* getInstances(){return &instances;};
	void clear();
	bool placeCell(string instanceName, int posX, int posY, bool mx, bool my);
	bool addInstance(string instName, string cellName);
};

#endif

