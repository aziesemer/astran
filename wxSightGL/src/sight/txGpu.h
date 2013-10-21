#ifndef CG_TEXTURE_H
#define CG_TEXTURE_H

#include "opengl.h"
#include <iostream>
#include <iomanip>
	using std::setprecision;
#include <string>
	using std::string;	
	using std::cout;
	
	
#ifdef CP_USE_INVERTED_RGBA
	const GLenum texturePropertyFormat  = GL_ABGR_EXT; ;
#else
	const GLenum texturePropertyFormat  = GL_RGBA;
#endif
	
//texture properties
const GLenum textureType		= GL_TEXTURE_RECTANGLE_EXT;
const GLenum textureFormat		= GL_RGBA;
const GLint  textureComponents	= GL_RGBA8;

//o que seria isso?? :-/ n interessa :p
const GLenum attachment = GL_COLOR_ATTACHMENT0_EXT;	

class TXmatrix{
	private:
		GLuint clsId;
		int clsWidth, clsHeight;

		GLubyte* clsImg;
		
		GLuint clsFrameBuffer;		
		
	public:
		TXmatrix(){}
		TXmatrix(GLfloat * data, const int width, const int height);
		
		void create(GLubyte * matrix, int width, int height);
		
		GLuint getReference() const { return clsId; }

		void setData(GLubyte * data, const int width, const int height);
		
		void setDimensions(const int width, const int height);
		int getWidth() const { return clsWidth; }
		int getHeight() const { return clsHeight; }
		
		void sendToGPU(float * data);
		void receiveFromGPU(float * data);
		
		void atualize();
		void refresh();
		void print();
		void printRGB();
		void printFB();
		void zero();
		
		GLuint getFBid()const;
		
};

#endif