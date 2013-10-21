#include <iostream>
	using std::cout;

#include "../util/util.h"

#include "wire_line.h"
#include "opengl.h"
#include "object.h"
#include "vertex.h"

// -----------------------------------------------------------------------------

void SightLine::draw( const float z ) const {
	glBegin( GL_LINE_STRIP );
		for(size_t i = 0; i < clsVecLine.size(); i++)
			glVertex3f( clsVecLine[i].x, clsVecLine[i].y, z );
	glEnd();
} // end method

// -----------------------------------------------------------------------------

Vertex SightLine::getMin() const{ 
	int x = clsVecLine[0].x;
	int y = clsVecLine[0].y;
	
	for( size_t i= 1; i < clsVecLine.size(); i++){
		if ( x > clsVecLine[i].x ) { x = clsVecLine[i].x; }
		if ( y > clsVecLine[i].y ) { y = clsVecLine[i].y; }
	}
	
	Vertex min;
	min.x = x;
	min.y = y;
	
	return min;
}//end function

// -----------------------------------------------------------------------------

Vertex SightLine::getMax() const{ 
	int x = clsVecLine[0].x;
	int y = clsVecLine[0].y;
	
	for( size_t i= 1; i < clsVecLine.size(); i++){
		if ( x < clsVecLine[i].x ) { x = clsVecLine[i].x; }
		if ( y < clsVecLine[i].y ) { y = clsVecLine[i].y; }
	}
	
	Vertex max;
	max.x = x;
	max.y = y;
	
	return max;
}//end function

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
/*void SightLine::drawEditMode(const int x, const int y) {
	Vertex point;
	point.x = x;	point.y = y;
	clsVecEditLine.push_back(point);
	
	for( int i = 0; i < clsVecEditLine.size(); i++ ){
		glBegin(GL_LINE_STRIP);
		glColor3f(1 , 1 , 1);
		glVertex3f( clsVecEditLine[i++].x, clsVecEditLine[i].y, getLayer());
	}//end for
	
	glVertex3f( mainCurrentX, maiinCurrentY, getLayer());
	
	glEnd();
}*/

// -----------------------------------------------------------------------------
// WIRE functions
// -----------------------------------------------------------------------------

void wireAux::whatIF( int x0, int y0, int x1, int y1) {
	int dx = x0 - x1;
	int dy = y0 - y1;
	
	if( abs(dx) > abs(dy) ){ //horizontal   
            clsCurrentDir = HORIZONTAL;   
		if( dx > 0 ){// up   
		    clsPrevY = 1; 
			return;
		} else {// rigth
            clsPrevY = -1; 
			return;
		}//end else	
	} else {//vertical
            clsCurrentDir = VERTICAL;
		if( dy > 0 ){// up
            clsPrevX = -1; 
			return;
		} else {// down
            clsPrevX = 1; 
			return;
		}//end else
	}//end else
}//lock wire aux

// -----------------------------------------------------------------------------

void wireAux::whatthehell(int x0, int y0, int x1, int y1){
	int dx = x0 - x1;	int dy = y0 - y1;
	int total = abs(dx) + abs(dy);
	
	clsPrevX = (float)dy / (float)total;	clsPrevY = (float)dx / (float)total;
	
//	if( abs(dx) < abs(dy) ) clsPrevX = (float)dy / (float)total;
//	else clsPrevY = (float)dx / (float)total;

}
	
// -----------------------------------------------------------------------------
/*void SightWire::draw( const float z ) const {

	wireAux aux;

	glBegin(GL_LINE_STRIP);
		for( size_t i = 0; i < clsVecWire.size(); i++ ){
			glVertex3f( clsVecWire[i].x, clsVecWire[i].y, maximo );
		}//end for	
	glEnd();//end lines 

	glBegin(GL_POLYGON);
		//aux.clsPrevX = 0; aux.clsPrevY = 0;  

		glColor3f(255 , 0 , 0);
		for ( size_t i = 0; i < clsVecWire.size() -1; i++){
			//aux.whatIF( clsVecWire[i].x, clsVecWire[i].y, clsVecWire[i+1].x, clsVecWire[i+1].y);
			aux.whatthehell( clsVecWire[i].x, clsVecWire[i].y, clsVecWire[i+1].x, clsVecWire[i+1].y);
			glVertex3f( clsVecWire[i].x + aux.clsPrevX * getThickness(), clsVecWire[i].y + aux.clsPrevY * getThickness(), maximo);

		}//end for IDA
		   
		//aux.clsPrevX = 0;	aux.clsPrevY = 0;
		//aux.whatIF(clsVecWire[clsVecWire.size()-2].x, clsVecWire[clsVecWire.size()-2].y, clsVecWire.back().x, clsVecWire.back().y);
		aux.whatthehell(clsVecWire[clsVecWire.size()-2].x, clsVecWire[clsVecWire.size()-2].y, clsVecWire.back().x, clsVecWire.back().y);
		glVertex3f(clsVecWire.back().x + aux.clsPrevX * getThickness(), clsVecWire.back().y + aux.clsPrevY * getThickness(), maximo);
		glColor3f(0 , 0 , 255);
		
		
		for ( int i = clsVecWire.size()-1; i > 0  ;i--){// VOLTA
		  // aux.whatIF( clsVecWire[i].x, clsVecWire[i].y, clsVecWire[i-1].x, clsVecWire[i-1].y);
		   aux.whatthehell( clsVecWire[i].x, clsVecWire[i].y, clsVecWire[i-1].x, clsVecWire[i-1].y);
		   glVertex3f( clsVecWire[i].x + aux.clsPrevX * getThickness(), clsVecWire[i].y + aux.clsPrevY * getThickness(), maximo);
		}//end for  

		//aux.clsPrevX = 0;	aux.clsPrevY = 0;
		//aux.whatIF( clsVecWire[1].x, clsVecWire[1].y, clsVecWire[0].x, clsVecWire[0].y );
		aux.whatthehell( clsVecWire[1].x, clsVecWire[1].y, clsVecWire[0].x, clsVecWire[0].y );
		glVertex3f( clsVecWire[0].x + aux.clsPrevX * getThickness(), clsVecWire[0].y + aux.clsPrevY * getThickness(), maximo);
	glEnd(); //end wire
	
}//end function*/
// -----------------------------------------------------------------------------

Vertex SightWire::getMin() const{ 
	int x = clsVecWire[0].x;
	int y = clsVecWire[0].y;
	
	for( size_t i= 1; i < clsVecWire.size(); i++){
		if ( x > clsVecWire[i].x ) { x = clsVecWire[i].x; }
		if ( y > clsVecWire[i].y ) { y = clsVecWire[i].y; }
	}
	
	Vertex min;
	min.x = x;
	min.y = y;
	
	return min;
}//end function

// -----------------------------------------------------------------------------

Vertex SightWire::getMax() const{ 
	int x = clsVecWire[0].x;
	int y = clsVecWire[0].y;
	
	for( size_t i= 1; i < clsVecWire.size(); i++){
		if ( x < clsVecWire[i].x ) { x = clsVecWire[i].x; }
		if ( y < clsVecWire[i].y ) { y = clsVecWire[i].y; }
	}
	
	Vertex max;
	max.x = x;
	max.y = y;
	
	return max;
}//end function

bool bug = false;

// -----------------------------------------------------------------------------

float2 findIntersection(float2 p1, float2 p2, float2 d1, float2 d2){
	const float a = p1.x;	const float e = p1.y;
	const float b = d1.x;	const float f = d1.y;
	const float c = p2.x;	const float g = p2.y;
	const float d = d2.x;	const float h = d2.y;

	float t;
	if( (b*h - d*f)  == 0 )	t = 0;
	else					t = ( (c-a)*h + (e-g)*d ) / ( b*h - d*f );
	float2 q = p1 + (d1 * t);

	if( (t < 0 || t > 1) ){ bug = true; }else{bug = false;}
printme(t);
	return q;
}//end function

// -----------------------------------------------------------------------------

bool findMyPoint(float2 v0, float2 v1, float2 v2, const int thickness, const float z){//ab cd
	cout<<"ENTER FUN\n";
	// Direction vectors.
	float2 d1 = v1 - v0;
	float2 d2 = v2 - v1;

	// Points over the lines.
	float2 p1 = v1 + d1.perpendicular().normalize() * thickness ; 
	float2 p2 = v1 + d2.perpendicular().normalize() * thickness ;
	
	float m1 = (v0.x - v1.x) == 0 ? (v0.y - v1.y) : (v0.y - v1.y)/(v0.x - v1.x);
	float m2 = (v1.x - v2.x) == 0 ? (v1.y - v2.y) : (v1.y - v2.y)/(v1.x - v2.x);
	
	float tg = (m1 - m2) / ( 1 + m1 * m2 ) ;
	float distancia = sqrt( pow( (p1.x - p2.x), 2 ) + pow( (p1.y - p2.y), 2 ) );
	float limite = sqrt( 2 * pow(thickness, 2.0f));
	
	float2 u1 = d1.normalize();	float2 u2 = d2.normalize();
		
		
	if( distancia > limite && tg > 0.0f  ){//dois
		glVertex3f( p1.x, p1.y, z);
		glVertex3f( p2.x, p2.y, z);
		return true;
	} else {		
		/*float A = (v1.y - v0.y)/(v1.x - v0.x);	
		float B = v0.y - ((v1.y - v0.y)/(v0.x - v1.x)) * v0.x;

			
		float D = fabs( A*p2.x + B*p2.y ) / sqrt(pow(A,2) + pow(B,2));
		float D2 = thickness;
		printme(D);			printme(D2);
		//if(D < D2){
		//if(bug){
		//if( tg < 0.0f && cos < -0.707107f){
			
			//BUAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAHH Dö8		
			float2 q = findIntersection(p1,p2,d1,d2);//um
			
			// q->p1?
		//	glVertex3f( q.x, q.y, z);
			//float2 k0 = v2 + float2( v1 - v2 ).perpendicular().normalize() * thickness ;
			//float2 k1 = v2 + float2( v2 - v1 ).perpendicular().normalize() * thickness ;
			
			//float2 d3 = k0-k1;			
			//float2 q = findIntersection(p1,v2,d1,d3);//um
		//float2 q1 = v1 + float2( v1-v2 ).perpendicular().normalize() * getThickness() ;
		//	glColor3f(1,1,1);
		//glVertex3f( p2.x, p2.y, z);	
		//glColor3f(0,0,1);	
		const float s = ( (p1.x-p2.x)*d1.y + (p2.y-p1.y)*d1.x ) / ( d2.x*d1.y - d1.x*d2.y );
		float2 g1 = d1.invert();
		float2 g2 = d2.invert();
		
		
	
	//q = p2 + ( d2 * s );
	
	if( (s < 0 || s > 1) ){ bug = true; }else{bug = false;}
	printme(s);
	//printme(d1.modulo());
	//printme(d2.modulo());					
			
		if(bug){
		glColor3f(1,1,1);
			glVertex3f( q.x, q.y, z);	
		glColor3f(0,0,1);	 
			cout << "FATAL ERROR hahahahahahaha\n";	
			return false;	
			//return true;	
		}else{ */
			float2 q = findIntersection(p1,p2,d1,d2);//um
			glVertex3f( q.x, q.y, z);
			return true;
	//	}//end else
	}//end else
}//end function

// -----------------------------------------------------------------------------

void SightWire::findTwoPoints(float2 const v0, float2 const v1, float2 const v2, const float z)const{//ab cd

	float limite = sqrt( 2 * pow(clsThickness, 2.0f));
	float2 d1;	float2 d2;	float2 p1;	float2 p2;
	
	// Direction vectors.
	d1 = v1 - v0;
	d2 = v2 - v1;

	// Points over the lines.
	p1 = v1 + d1.perpendicular().normalize() * clsThickness ; 
	p2 = v1 + d2.perpendicular().normalize() * clsThickness ;
	
	float m1 = (v0.x - v1.x) == 0 ? (v0.y - v1.y) : (v0.y - v1.y)/(v0.x - v1.x);
	float m2 = (v1.x - v2.x) == 0 ? (v1.y - v2.y) : (v1.y - v2.y)/(v1.x - v2.x);
	
	float tg = (m1 - m2) / ( 1 + m1 * m2 ) ;
	float distancia = sqrt( pow( (p1.x - p2.x), 2 ) + pow( (p1.y - p2.y), 2 ) );
	
	if( distancia > limite && tg > 0.0f ){
	//	glColor3f(0,1,0);
		glVertex3f( p1.x, p1.y, z);
		
		float2 d1b = float2( -d1.x, -d1.y);
		float2 d2b = float2( -d2.x, -d2.y);	
		float2 p1b = v1 + d1b.perpendicular().normalize() * clsThickness ; 
		float2 p2b = v1 + d2b.perpendicular().normalize() * clsThickness ;
		float2 l2 = findIntersection(p1b,p2b,d1b,d2b);
	//	glColor3f(1,0,1);	
		
		glVertex3f( l2.x, l2.y, z);
		
		glVertex3f( p1.x, p1.y, z);//"triangle"
		glVertex3f( p2.x, p2.y, z);//"triangle"
		glVertex3f( l2.x, l2.y, z);//"triangle"		
		glVertex3f( l2.x, l2.y, z);
		
		glVertex3f( l2.x, l2.y, z);
		glVertex3f( p2.x, p2.y, z);
		
		
		cout<<"DOISSSSSSSSSSSS\n";
	} else {
		cout<<"UUUUUUUUUUUUUUUM\n";
		float2 q1 = findIntersection(p1,p2,d1,d2);
		
		glVertex3f( q1.x, q1.y, z);	
		
		d1 = float2( -d1.x, -d1.y);
		d2 = float2( -d2.x, -d2.y);	
		p1 = v1 + d1.perpendicular().normalize() * clsThickness ; 
		p2 = v1 + d2.perpendicular().normalize() * clsThickness ;
		  m2 = (v0.x - v1.x) == 0 ? (v0.y - v1.y) : (v0.y - v1.y)/(v0.x - v1.x);
		  m1 = (v1.x - v2.x) == 0 ? (v1.y - v2.y) : (v1.y - v2.y)/(v1.x - v2.x);
		  tg = (m1 - m2) / ( 1 + m1 * m2 ) ;
		  distancia = sqrt( pow( (p1.x - p2.x), 2 ) + pow( (p1.y - p2.y), 2 ) );
		 
		  if( distancia > limite && tg > 0.0f ){
		  		glVertex3f( p1.x, p1.y, z);		
				float2 d1b = float2( -d1.x, -d1.y);
				float2 d2b = float2( -d2.x, -d2.y);	
				float2 p1b = v1 + d1b.perpendicular().normalize() * clsThickness ; 
				float2 p2b = v1 + d2b.perpendicular().normalize() * clsThickness ;
				float2 l2 = findIntersection(p1b,p2b,d1b,d2b);					
				
				glVertex3f( l2.x, l2.y, z);
			//	glColor3f(1,0,1);
				glVertex3f( p1.x, p1.y, z);//"triangle"
				glVertex3f( p2.x, p2.y, z);//"triangle"
				glVertex3f( l2.x, l2.y,z);//"triangle"				
			
			//	glColor3f(0,0,1);
				glVertex3f( p2.x, p2.y, z);
			//	glColor3f(1,0,0);				
				glVertex3f( l2.x, l2.y, z);
			} else{		
				float2 q2 = findIntersection(p1,p2,d1,d2);
			//	glColor3f(1,1,0);	
				glVertex3f( q2.x, q2.y, z);
			//	glColor3f(1,0,0);
				glVertex3f( q2.x, q2.y, z);
				glVertex3f( q1.x, q1.y, z);	
			}//end else
	}	//end else	
}//end function

// -----------------------------------------------------------------------------

void SightWire::draw(const float z) const {
	cout<<"DRAW WIRE\n";
	glBegin(GL_QUADS);
//	glColor3f(1,1,1);
		//first points
		float2 v1 = float2( clsVecWire[0] ) + float2( float2( clsVecWire[0] ) - float2( clsVecWire[1] ) ).perpendicular().normalize() * getThickness() ;
		glVertex3f( v1.x, v1.y, z);			
		
		v1 = float2( clsVecWire[0] ) + float2( float2( clsVecWire[1] ) - float2( clsVecWire[0] ) ).perpendicular().normalize() * getThickness() ;
		glVertex3f( v1.x, v1.y, z);
		
		for( size_t i = 1; i < clsVecWire.size() - 1; i++){ 	
			findTwoPoints( clsVecWire[i-1], clsVecWire[i], clsVecWire[i+1], z );
		}//ida
//glColor3f(0,1,0);
		v1 = float2(clsVecWire.back()) + float2( float2( clsVecWire.back() ) - float2( clsVecWire[clsVecWire.size()-2] ) ).perpendicular().normalize() * getThickness() ;
		glVertex3f( v1.x, v1.y, z);	
//		glColor3f(1,1,1);
		float2 v2 = float2( clsVecWire.back() ) + float2( float2( clsVecWire[clsVecWire.size()-2] ) - float2( clsVecWire.back() ) ).perpendicular().normalize() * getThickness();
		glVertex3f( v2.x, v2.y, z);	

	glEnd();
}

// -----------------------------------------------------------------------------

void SightWire::drawEdges(const float z) const {

	glBegin(GL_LINE_STRIP);
		for( size_t i = 0; i < clsVecWire.size(); i++ )
			glVertex3f( clsVecWire[i].x, clsVecWire[i].y, maximo );
	glEnd();//end LINE 


//	glColor3f(1,1,1);
	glBegin(GL_LINE_STRIP);

		//first point
		float2 v1 = float2( clsVecWire[0] ) + float2( float2( clsVecWire[1] ) - float2( clsVecWire[0] ) ).perpendicular().normalize() * getThickness() ;
		glVertex3f( v1.x, v1.y, z);
		
		bool control = true;
		for( size_t i = 1; i < clsVecWire.size() - 1; i++){ 	
			control = findMyPoint( clsVecWire[i-1], clsVecWire[i], clsVecWire[i+1], clsThickness, z );			
		}//ida

	//	glColor3f(0, 1, 0);
		if(control){	
			v1 = float2(clsVecWire.back()) + float2( float2( clsVecWire.back() ) - float2( clsVecWire[clsVecWire.size()-2] ) ).perpendicular().normalize() * getThickness() ;
			glVertex3f( v1.x, v1.y, z);		
		}
		v1 = float2( clsVecWire.back() ) + float2( float2( clsVecWire[clsVecWire.size()-2] ) - float2( clsVecWire.back() ) ).perpendicular().normalize() * getThickness();
		glVertex3f( v1.x, v1.y, z);
		
		for( int i = clsVecWire.size() - 2; i >= 1; i--){ 		
			findMyPoint( clsVecWire[i+1], clsVecWire[i], clsVecWire[i-1], clsThickness, z );
		}//volta			
		
		v1 = float2( clsVecWire[0] ) + float2( float2( clsVecWire[0] ) - float2( clsVecWire[1] ) ).perpendicular().normalize() * getThickness() ;
		glVertex3f( v1.x, v1.y, z);	
		v1 = float2( clsVecWire[0] ) + float2( float2( clsVecWire[1] ) - float2( clsVecWire[0] ) ).perpendicular().normalize() * getThickness() ;
		glVertex3f( v1.x, v1.y, z);

		//for( size_t i = 0; i < clsVecWire.size(); i++ ) 
		//	printme(clsVecWire[i]);
		cout<<"\n"; 


	glEnd();
	
//	preencher(z);
}

// -----------------------------------------------------------------------------










