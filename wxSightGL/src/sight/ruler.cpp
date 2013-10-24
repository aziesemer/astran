#include <cstdio>
#include "ruler.h"

//all the ruler methods are very complex, and crazy and.. oh my god... 
// -----------------------------------------------------------------------------

void Ruler::DesenhaTexto(char *string, const float x, const float y, void * font) const{  
	// Posição no universo onde o texto será colocado          
	glRasterPos2f(x,y); 
	// Exibe caracter a caracter
	glDisable(GL_DEPTH_TEST);
	while(*string)
		 glutBitmapCharacter(font,*string++); 
}//draw the numbers	

// -----------------------------------------------------------------------------
// Divide -> auxs to render the ruler
// -----------------------------------------------------------------------------

void Ruler::divideV(const int z, const int scale, void * font, const float trace, const int k)const{
	
	char delta[5] = "";
	const char base[]="";
	int mult = - 1;
	int scale2 = scale;
	size_t i = scale;
	int i2 = mult * i;
	
	glBegin(GL_LINES);
		glVertex3f( clsFirst.x + trace , clsFirst.y	, z );//first trace
		glVertex3f( clsFirst.x - trace , clsFirst.y , z );
		
		glVertex3f( clsFirst.x + trace, clsLast.y, z );//last trace
		glVertex3f( clsFirst.x - trace, clsLast.y, z );	
	glEnd();//end lines
		
	sprintf(delta, "%s%.0f", base, 0.0);
	DesenhaTexto(delta, clsFirst.x + trace, clsFirst.y- 1.25f , font);
	
	sprintf(delta, "%s%.1f", base, (float)abs(clsFirst.y - clsLast.y)/scale*k);		
	DesenhaTexto(delta, clsLast.x+ trace , clsLast.y- 1.25f , font); 		
	
	if( 0 > (clsFirst.y - clsLast.y)){
		scale2 = - scale;
		mult = 1;
		i2 = mult * i;
	}//set direction parameters
	
	for(i; i<(abs(clsFirst.y - clsLast.y)); i+=scale){
		i2 = mult * i;

		sprintf(delta, "%s%.0f", base, (float)i*k/scale);
		glBegin(GL_LINES);
			glColor3f(1 , 1 , 1);

			glVertex3f( clsFirst.x + trace, clsFirst.y +i2, z ); //big trace
			glVertex3f( clsFirst.x - trace, clsFirst.y +i2, z );
		glEnd();//end line		
		DesenhaTexto(delta, clsFirst.x + trace , clsFirst.y +i2 - 0.5f, font);//digit						
	}//end for

	glBegin(GL_LINES);
	glColor3f(1,1,1);
	for(int j =  clsFirst.y;(mult > 0 && j < clsLast.y) || (mult < 0 && j > clsLast.y) ; j+=(mult*scale/10)){
		glVertex3f(  clsFirst.x + trace/2, j , z );
		glVertex3f(  clsFirst.x			 , j , z );	
	}//end for little traces
	glEnd();//end lines
}//end method

// -----------------------------------------------------------------------------

void Ruler::divideH( const int z, const int scale, void * font, const float trace, const int k)const{
	int mult = - 1;
	int scale2 = scale;
	size_t i = scale;
	int i2 = mult * i;	
	char delta[5] = "";
	const char base[]="";
	
	glBegin(GL_LINES);
		glVertex3f( clsFirst.x , clsFirst.y + trace, z );//first trace
		glVertex3f( clsFirst.x , clsFirst.y - trace, z );

		glVertex3f( clsLast.x, clsFirst.y + trace, z );//last trace
		glVertex3f( clsLast.x, clsFirst.y - trace, z );		
	glEnd();
	
	sprintf(delta, "%s%.0f", base, 0.0);//first digit
	DesenhaTexto(delta, clsFirst.x - 1.25f, clsFirst.y + trace, font);	
	
	sprintf(delta, "%s%.1f", base, (float)abs(clsFirst.x - clsLast.x)/scale*k);
	DesenhaTexto(delta, clsLast.x - 1.25f, clsLast.y + trace, font); //last digit
	
	if( 0 > (clsFirst.x - clsLast.x)){
		scale2 = - scale;
		mult = 1;
		i2 = mult * i;
	}//set direction parameters

	for(i; i<(abs(clsFirst.x - clsLast.x)); i+=scale){
		i2 = mult * i;
		sprintf(delta, "%s%.0f", base, (float)i*k/scale);
		glBegin(GL_LINES);
			glColor3f(1 , 1 , 1);
			glVertex3f( clsFirst.x +i2, clsFirst.y + trace, z ); //big trace
			glVertex3f( clsFirst.x +i2, clsFirst.y - trace, z );
		glEnd();//end line		
		DesenhaTexto(delta, clsFirst.x +i2 - 0.5f, clsFirst.y + trace, font);//digit						
	}//end for

	glBegin(GL_LINES);
		glColor3f(1,1,1);
		for(int j =  clsFirst.x;(mult > 0 && j < clsLast.x) || (mult < 0 && j > clsLast.x) ; j+=(mult*scale/10)){
			glVertex3f(  j, clsFirst.y + trace/2, z );
			glVertex3f(  j, clsFirst.y , z );	
		}//end for little traces
	glEnd();//end lines	
}//end method

// -----------------------------------------------------------------------------
	
void Ruler::drawRuler(const int z, int scale, const float zoom)const{	
//draw the background:
	glColor4f(0.0 , 0.0 , 0.0, 0.4);
	glEnable(GL_BLEND);
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glBegin(GL_QUADS);
		const int x0 = min(clsFirst.x, clsLast.x) - d;
		const int x1 = max(clsFirst.x, clsLast.x) + d;
		const int y0 = min(clsFirst.y, clsLast.y) - d;
		const int y1 = max(clsFirst.y, clsLast.y) + d;
		
		glVertex3f( x0, y0, z );
		glVertex3f( x0, y1, z );
		glVertex3f( x1, y1, z );
		glVertex3f( x1, y0, z );
	glEnd();//quads	
	glDisable(GL_BLEND); 

//draw the main line:
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glBegin(GL_LINES);
		glColor3f(255 , 255 , 0);				
		glVertex3f( clsFirst.x	, clsFirst.y, z );
		glVertex3f( clsLast.x	, clsLast.y	, z );
	glEnd();//end lines

//draw segments and digits	
	float trace;
	int k = 1;
	void* font;
		
	if( clsFirst.y == clsLast.y){//horizontal		

		if( zoom <= 1.0f){ 
			if( zoom <= 0.4f){ scale = scale * 2; k = 2; }
			trace = 4.0f;
			font = GLUT_BITMAP_HELVETICA_10;
		//	d = 30;
		}else{
			if( zoom > 1.0f && zoom <= 3.0f ){
				trace = 6.0f;
				font = GLUT_BITMAP_HELVETICA_12;
		//		d = 20;
			}else{
				trace = 8.0f;
				font = GLUT_BITMAP_HELVETICA_18;
		//		d = 10;
			}
		}	//set direction parameters				
		divideH( z, scale, font, trace, k );//divisions		
					
	}else{//vertical		
		if( zoom <= 1.0f){ 
			if( zoom <= 0.4f){ scale = scale * 2; k = 2; } 
		//	d = 30;
			trace = 4.0f;
			font = GLUT_BITMAP_HELVETICA_10;
		}else{
			if( zoom > 1.0f && zoom <= 3.0f ){
				trace = 6.0f;
		//		d = 20;
				font = GLUT_BITMAP_HELVETICA_12;
			}else{
		//		d = 10;
				trace = 8.0f;
				font = GLUT_BITMAP_HELVETICA_18;
			}
		}	//set direction parameters		
		divideV( z, scale, font, trace, k );
		
	}//end else	 	
}//end method

// -----------------------------------------------------------------------------	

bool Ruler::contains(const int x, const int y)const{
	const int x0 = min(clsFirst.x, clsLast.x) - d;
	const int x1 = max(clsFirst.x, clsLast.x) + d;
	const int y0 = min(clsFirst.y, clsLast.y) - d;
	const int y1 = max(clsFirst.y, clsLast.y) + d;
	return (x >= x0 && x <= x1 && y >= y0 && y <= y1);
}//end method

// -----------------------------------------------------------------------------	

void Ruler::drawBB(const int z)const{
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE);
	glLineStipple( 1, 0xf0f0 );
glEnable( GL_LINE_STIPPLE );
	glBegin(GL_LINE_LOOP);
	
		glColor3ub( 255, 255, 0 );
		const int x0 = min(clsFirst.x, clsLast.x) - d;
		const int x1 = max(clsFirst.x, clsLast.x) + d;
		const int y0 = min(clsFirst.y, clsLast.y) - d;
		const int y1 = max(clsFirst.y, clsLast.y) + d;
		
		glVertex3f( x0, y0, z );
		glVertex3f( x0, y1, z );
		glVertex3f( x1, y1, z );
		glVertex3f( x1, y0, z );
	glEnd();
glDisable( GL_LINE_STIPPLE );
	
	glFlush();
}//draw bounding box

// -----------------------------------------------------------------------------
	
void Ruler::lock(){
	if( (abs(clsFirst.x -  clsLast.x)) > (abs(clsFirst.y - clsLast.y))){//horizontal		
		setRuler(clsFirst, Vertex(clsLast.x, clsFirst.y));
	}else{ //vertical	
		setRuler(clsFirst, Vertex(clsFirst.x, clsLast.y));
	}//end else	
}//lock ruler direction

// -----------------------------------------------------------------------------












