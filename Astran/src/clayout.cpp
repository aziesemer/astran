/***************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Cristiano Lazzari     *
 *   (amziesemerj,clazz)@inf.ufrgs.br                                      *
 ***************************************************************************/
#include "clayout.h"


void Instance::print(){
	cout << "-> Position: POSX=" << posx << " POSY=" << posy << " MX=" << mx << " MY=" << my << endl;
}


CLayout::CLayout(string n):name(n) {
	width=0;
	height=0;
}

CLayout::CLayout(){
	width=0;
	height=0;
}

CLayout::~CLayout() {}

void CLayout::setPin(string name, Pin p){
	pins.insert(make_pair(name,p));
}

void CLayout::placeCell(string instanceName, int posX, int posY, bool mx, bool my){
	Instance *currentInstance=getInstance(instanceName);
	if(!currentInstance)
        throw AstranError("Instance not found: " + instanceName);
    
    currentInstance->setPos(posX, posY);
    currentInstance->setMX(mx);
    currentInstance->setMY(my);
}


bool CLayout::addInstance(string instName, string cellName){
	if(instances.find(instName)!=instances.end()){
		cout << "-> Cell Instance " << instName << " in " << name << " is duplicated" << endl;
		return false;
	}else{
		Instance tmp(cellName);
		instances[instName]=tmp;
		return true;
	}
} 

Instance* CLayout::getInstance(string n){
	map<string,Instance>::iterator tmp=instances.find(n);
	if(tmp!=instances.end())
		return &tmp->second;
	else
		return 0;
}

void CLayout::clear(){
	Draw::clear();
	pins.clear();
	instances.clear();
	name="";
}