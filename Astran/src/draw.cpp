/**************************************************************************
*   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
*   amziesemerj[at]inf.ufrgs.br                                           *
***************************************************************************/
#include "draw.h"

Draw::Draw() {}

Draw::~Draw() {}

Box& Draw::addLayer(Box& bx, layer_name l){
	layers[l].push_front(bx);
	return layers[l].front();
}

Box& Draw::addLayer(int ptx, int pty, int width, int height, layer_name l){
	Box tmp(ptx, pty, width, height);  
	return addLayer(tmp ,l);
}

Box& Draw::addBox(int ptx, int pty, int width, int height, layer_name l){
	Point pt(ptx, pty);
	return addBox(pt,width,height,l);
}

Box& Draw::addBox(Point& pt, int width, int height, layer_name l){
	Box tmp((pt.getX()+width)/2,(pt.getY()+height)/2, abs(width), abs(height));
	return addLayer(tmp,l);
}

Box& Draw::addPolygon(int ptx_ini, int pty_ini, int ptx_end, int pty_end, layer_name l){
	Point p1(ptx_ini, pty_ini), p2(ptx_end, pty_end);
	return addPolygon(p1, p2, l);
}

Box& Draw::addPolygon(Point& pt1, Point& pt2, layer_name& l){
	Box tmp;
	tmp.setX((pt1.getX()+pt2.getX())/2);
	tmp.setY((pt1.getY()+pt2.getY())/2);
	tmp.setWidth(abs(pt2.getX()-pt1.getX()));
	tmp.setHeight(abs(pt2.getY()-pt1.getY()));
	return addLayer(tmp,l);
}

Box& Draw::addEnc(Box tmp, int border, layer_name l){
	tmp.setWidth(tmp.getWidth()+(2*border));
	tmp.setHeight(tmp.getHeight()+(2*border));  
	return addLayer(tmp,l);
}

Box& Draw::addPath(int ptx, int pty, int width, int length, bool border, direction d, layer_name l){
	Box tmp;
	switch(d){
		case N:	tmp.setY(pty+length/2);
		tmp.setX(ptx);
		tmp.setWidth(width);
		if(border) length+=width;
		tmp.setHeight(length);
		break;
		case S:	tmp.setY(pty-length/2);
		tmp.setX(ptx);
		if(border) length+=width;
		tmp.setWidth(width);
		tmp.setHeight(length);
		break;
		case E:	tmp.setX(ptx+length/2);
		tmp.setY(pty);
		if(border) length+=width;
		tmp.setWidth(length);
		tmp.setHeight(width);
		break;
		case W:	tmp.setX(ptx-length/2);
		tmp.setY(pty);
		if(border) length+=width;
		tmp.setWidth(length);
		tmp.setHeight(width);
		break;
	}
	return addLayer(tmp,l);
}

Box& Draw::addPath(Point& pt, int width, int length, bool border, direction d, layer_name l){
	return addPath(pt.getX(), pt.getY(), width, length, border, d, l);
}

void Draw::addLabel(string label, Point pt){
	Label tmp;
	tmp.pt=pt;
	tmp.text=label;
	labels.push_back(tmp);
}

void Draw::merge(){
	map <layer_name , list<Box> >::iterator layers_it; // iterador das camadas
	list <Box>::iterator layer_it,layer_it2;
	bool restart;
	int tmp;
	for(layers_it = layers.begin(); layers_it != layers.end(); layers_it++) {
		do{
			layer_it = layers_it->second.begin(); 
			restart=false;
			while(!restart && layer_it != layers_it->second.end()){
				layer_it2 = layer_it;
				layer_it2++;
				while(!restart && layer_it2 != layers_it->second.end()){
					//verify if layer_it2 is inside layer_it           
					if((static_cast<int>(layer_it2->getX()-layer_it2->getWidth()/2)>=static_cast<int>(layer_it->getX()-layer_it->getWidth()/2)) &&
					   (static_cast<int>(layer_it2->getX()+layer_it2->getWidth()/2)<=static_cast<int>(layer_it->getX()+layer_it->getWidth()/2)) &&
					   (static_cast<int>(layer_it2->getY()+layer_it2->getHeight()/2)<=static_cast<int>(layer_it->getY()+layer_it->getHeight()/2)) &&
					   (static_cast<int>(layer_it2->getY()-layer_it2->getHeight()/2)>=static_cast<int>(layer_it->getY()-layer_it->getHeight()/2))){
						layers_it->second.erase(layer_it2);
						restart=true;
					}
					//verify if layer_it is inside layer_it2           
					else if((static_cast<int>(layer_it2->getX()-layer_it2->getWidth()/2)<=static_cast<int>(layer_it->getX()-layer_it->getWidth()/2)) &&
					        (static_cast<int>(layer_it2->getX()+layer_it2->getWidth()/2)>=static_cast<int>(layer_it->getX()+layer_it->getWidth()/2)) &&
					        (static_cast<int>(layer_it2->getY()+layer_it2->getHeight()/2)>=static_cast<int>(layer_it->getY()+layer_it->getHeight()/2)) &&
					        (static_cast<int>(layer_it2->getY()-layer_it2->getHeight()/2)<=static_cast<int>(layer_it->getY()-layer_it->getHeight()/2))){
						*layer_it=*layer_it2;
						layers_it->second.erase(layer_it2);
						restart=true;
					}
					//verify if layer_it2 is beside layer_it           
					else if(static_cast<int>(layer_it2->getX()+layer_it2->getWidth()/2)>=static_cast<int>(layer_it->getX()-(layer_it->getWidth()/2)) &&
						(static_cast<int>(layer_it2->getX()+layer_it2->getWidth()/2)<=static_cast<int>(layer_it->getX()+layer_it->getWidth()/2)) &&
					layer_it2->getHeight()==layer_it->getHeight() && layer_it->getY()==layer_it2->getY()){
						tmp=((layer_it2->getX()-layer_it2->getWidth()/2)+(layer_it->getX()+layer_it->getWidth()/2))/2;
						layer_it->setWidth((layer_it->getX()+layer_it->getWidth()/2)-(layer_it2->getX()-layer_it2->getWidth()/2));
						layer_it->setX(tmp);
						layers_it->second.erase(layer_it2);
						restart=true;
					}
					else if(static_cast<int>(layer_it->getX()+layer_it->getWidth()/2)>=static_cast<int>(layer_it2->getX()-(layer_it2->getWidth()/2)) &&
						(static_cast<int>(layer_it->getX()+layer_it->getWidth()/2)<=static_cast<int>(layer_it2->getX()+layer_it2->getWidth()/2)) &&
					layer_it->getHeight()==layer_it2->getHeight() && layer_it->getY()==layer_it2->getY()){
						tmp=((layer_it->getX()-layer_it->getWidth()/2)+(layer_it2->getX()+layer_it2->getWidth()/2))/2;
						layer_it->setWidth((layer_it2->getX()+layer_it2->getWidth()/2)-(layer_it->getX()-layer_it->getWidth()/2));
						layer_it->setX(tmp);
						layers_it->second.erase(layer_it2);
						restart=true;
					}
					//verify if layer_it is above layer_it2           
					else if(static_cast<int>(layer_it2->getY()+layer_it2->getHeight()/2)>=static_cast<int>(layer_it->getY()-(layer_it->getHeight()/2)) &&
						(static_cast<int>(layer_it2->getY()+layer_it2->getHeight()/2)<=static_cast<int>(layer_it->getY()+layer_it->getHeight()/2)) &&
					layer_it2->getWidth()==layer_it->getWidth() && layer_it->getX()==layer_it2->getX()){
						tmp=((layer_it2->getY()-layer_it2->getHeight()/2)+(layer_it->getY()+layer_it->getHeight()/2))/2;
						layer_it->setHeight((layer_it->getY()+layer_it->getHeight()/2)-(layer_it2->getY()-layer_it2->getHeight()/2));
						layer_it->setY(tmp);
						layers_it->second.erase(layer_it2);
						restart=true;
					}   
					//verify if layer_it is below layer_it2           
					else if(static_cast<int>(layer_it->getY()+layer_it->getHeight()/2)>=static_cast<int>(layer_it2->getY()-layer_it2->getHeight()/2) &&
						(static_cast<int>(layer_it->getY()+layer_it->getHeight()/2)<=static_cast<int>(layer_it2->getY()+layer_it2->getHeight()/2)) &&
					layer_it2->getWidth()==layer_it->getWidth() && layer_it->getX()==layer_it2->getX()){
						tmp=((layer_it->getY()-layer_it->getHeight()/2)+(layer_it2->getY()+layer_it2->getHeight()/2))/2;
						layer_it->setHeight((layer_it2->getY()+layer_it2->getHeight()/2)-(layer_it->getY()-layer_it->getHeight()/2));
						layer_it->setY(tmp);
						layers_it->second.erase(layer_it2);
						restart=true;
					}
					else layer_it2++;
				}
				if(!restart) layer_it++;            
			}  
		}while(restart);          
	}
}

void Draw::clear(){
	layers.clear();
	labels.clear();
}