#ifndef OPENGL_H
#define OPENGL_H

#include <iostream>
	using namespace std;
#if defined(__APPLE__) || defined(MACOSX)
  // Remember to include GLUT.framework and OpenGL.framework to your project.
  #include <OpenGL/gl.h>		// Header File For The OpenGL32 Library
  #include <OpenGL/glu.h>		// Header File For The GLu32 Library
  #include <GLUT/glut.h>		// Header File For The GLut Library
#else
	#include <GL/glew.h>
	#include <GL/glut.h>
#endif

#define checkGLErrors() \
	{ const GLenum errCode = glGetError(); \
	if ( errCode != GL_NO_ERROR) { \
		cerr << "[OpenGL ERROR] (" << errCode << ") " << __FILE__ \
			<< "(" << __LINE__ << ") : "<< gluErrorString( errCode ) << "\n"; } }

void drawRoundedRectangle( const float x, const float y, const float w, const float h, const GLclampf roundness );

//#define glBegin(x) { extern int beginCount; cerr << "glBeing( " << #x << " ): " << __FILE__ << "(" << __LINE__ << ") : "<< beginCount++ << "\n"; glBegin(x); } 
//#define glEnd() { extern int endCount; cerr << "glEnd(): " << __FILE__ << "(" << __LINE__ << ") : "<< endCount++ << "\n"; glEnd(); } 

//#define glBegin(x) { glBegin(x); checkGLErrors(); } 
//#define glEnd() { glEnd(); checkGLErrors(); } 


#endif
