/***************************************************************************
*   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Cristiano Lazzari     *
*   (amziesemerj,clazz)@inf.ufrgs.br                                      *
***************************************************************************/
#pragma once

/**
Estrutura de dados e diretivas de desenho

@author Adriel Mota Ziesemer Jr., Cristiano Lazzari
*/

#include <string>
#include <map>
#include <list>
#include "rules.h"
#include "util.h"

class Point {
	protected:
	int posx, posy;
	public:
	Point(){};
	Point(int x, int y):posx(x), posy(y){};
	void setX(int x){posx=x;};
	void setY(int y){posy=y;};
	void setPos(int x, int y){posx=x; posy=y;};		
	int getX(){return posx;};		
	int getY(){return posy;};
};

class Box: public Point {
	protected:
	int height, width;
	string net;
	public:
	Box(){};
	Box(int x, int y, int w, int h):height(h), width(w), Point(x,y), net(""){};
	void setHeight(int h){height=h;};
	void setWidth(int w){width=w;};
	int getHeight(){return height;};		
	int getWidth(){return width;};
	int getX1(){return posx-(width/2);};
	int getX2(){return posx+(width/2)+(width%2?1:0);};
	int getY1(){return posy-(height/2);};
	int getY2(){return posy+(height/2)+(height%2?1:0);};
	void setNet(string n){net=n;};
	string getNet(){return net;};
};

struct Label{
	Point pt;
	string text;	
};

enum direction {N, S, E, W};

class Draw{
//	private:
	public:
	map<layer_name , list<Box> > layers;
	list<Label> labels;
	
	Draw();
	~Draw();
	Box& addLayer(Box& bx, layer_name l);
	Box& addLayer(int ptx, int pty, int width, int height, layer_name l);
	Box& addBox(int ptx, int pty, int width, int height, layer_name l);
	Box& addBox(Point& pt, int width, int height, layer_name l);
	Box& addPolygon(int ptx_ini, int pty_ini, int ptx_end, int pty_end, layer_name l);
	Box& addPolygon(Point& pt1, Point& pt2, layer_name& l);
	Box& addEnc(Box tmp, int border, layer_name l);
	Box& addPath(Point& pt, int width, int length, bool border, direction d, layer_name l);
	Box& addPath(int ptx, int pty, int width, int length, bool border, direction d, layer_name l);
	void addLabel(string label, Point pt);
	void merge();
	void clear();
};
