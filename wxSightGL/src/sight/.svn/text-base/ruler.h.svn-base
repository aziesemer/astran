

#ifndef RULER_H
#define RULER_H

#include <vector>
	using std::vector; 
#include <iostream>
	using std::cout;
#include "vertex.h"
#include "opengl.h"

#include "../util/util.h"
 	
const int d = 30;//boundig box aux

class Ruler{
	private:
		Vertex clsFirst;
		Vertex clsLast;		
		
	public:
		Ruler(Vertex first, Vertex last): clsFirst(first), clsLast(last) { }
		void setRuler(const Vertex first, const Vertex last){ clsFirst = first; clsLast = last;	}		
		Vertex getFirst(){ return clsFirst; }
		Vertex getLast(){ return clsLast;}	
		void DesenhaTexto(char *string, const float x, const float y, void * font)const;
		void divideH(const int z, const int scale, void * font, const float trace, const int k)const;
		void divideV(const int z, const int scale, void * font, const float trace, const int k)const;
		void drawRuler(const int z, int scale, const float zoom)const;
		void drawBB(const int z)const;
		
		void lock();
		
		//selection fun
		bool contains(const int x, const int y)const;
		
};

#endif
