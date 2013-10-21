
#include "txGpu.h"


//______________________________________________________________________________

GLvoid TXmatrix::create(GLubyte * matrix, int width, int height){
	// Update texture dimensions.
	clsWidth = width;
	clsHeight = height;	
	
	clsImg = matrix;
	
	checkGLErrors() ;
	// Generate our a texture.
	glGenTextures( 1, &clsId );//gl gere uma textura, plz e me dê o id
	checkGLErrors() ;
	
	glGenFramebuffersEXT( 1,  &clsFrameBuffer  );//cria framebuffer

	//associa a textura ao framebuffer
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, clsFrameBuffer );
		
	// Bind the texture to a texture target.
	glBindTexture( textureType, clsId );
	checkGLErrors();
	// Turn off filtering and wrap modes. *evitar media e acesso indevido :p
	glTexParameteri( textureType, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( textureType, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( textureType, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( textureType, GL_TEXTURE_WRAP_T, GL_CLAMP );
	checkGLErrors();
	// Set the texture enviroment to replace instead of the defaut modulate. [???]
	glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
	checkGLErrors();
	//glTexImage2D( textureType, 0, textureComponents, clsWidth, clsHeight, 0, texturePropertyFormat, GL_FLOAT, clsData );
/*
	checkGLErrors();
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, textureType, clsId, 0 );
	checkGLErrors();	*/

	glTexImage2D( textureType, 0, textureComponents, clsWidth, clsHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, clsImg );

	glFinish();


checkGLErrors();
}
//______________________________________________________________________________

void TXmatrix::setData(GLubyte * data, const int width, const int height){
	clsImg = data;
	clsWidth = width;
	clsHeight = height;
	
	glBindTexture( textureType, clsId );
	glTexImage2D( textureType, 0, textureComponents, clsWidth, clsHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, clsImg );
}

//______________________________________________________________________________

void TXmatrix::setDimensions(const int width, const int height){
	clsWidth = width;
	clsHeight = height;
}


//______________________________________________________________________________

void TXmatrix::receiveFromGPU(float * data){
	// Bind the texture to a texture target.
	glBindTexture( textureType, clsId );

	//if(data != NULL){ delete(data); }

	//data = new GLfloat[ clsWidth * clsHeight * 4 ];
	glBindTexture( textureType, clsId );
	glGetTexImage( textureType, 0 , GL_RGBA, GL_FLOAT, data );
}

GLuint TXmatrix::getFBid()const{ return clsFrameBuffer; }

//______________________________________________________________________________

void TXmatrix::print(){
	GLfloat *data = new GLfloat[ clsWidth * clsHeight * 4 ];
	glBindTexture( textureType, clsId );
	glGetTexImage( textureType, 0 , textureFormat, GL_FLOAT, data );

	cerr << setprecision( 2 );
	for ( int i = 0; i < clsHeight; i++ ) {
		for ( int j = 0; j < clsWidth; j++ )
			cerr << data[ i * ( clsWidth * 4 ) + j * 4 ] << '\t';
		cerr << '\n';
	} // end for
	cerr << '\n';
	delete (data);
}

//______________________________________________________________________________
void TXmatrix::printFB(){
	GLfloat *data = new GLfloat[ clsWidth * clsHeight * 4 ];
	
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, clsFrameBuffer );
	
	glReadBuffer( attachment );
	checkGLErrors();
	glReadPixels( 0, 0, clsWidth, clsHeight, textureFormat, GL_FLOAT, data );
	checkGLErrors();

	cerr << setprecision( 2 );//<<fixed;
	for ( int i = 0; i < clsHeight; i++ ) {
		for ( int j = 0; j < clsWidth; j++ )
			cerr << data[ i * ( clsWidth * 4 ) + j * 4 ] << '\t';
		cerr << '\n';
	} // end for
	cerr << '\n';
	delete( data);
}
//______________________________________________________________________________
void TXmatrix::printRGB(){
	GLfloat *data = new GLfloat[ clsWidth * clsHeight * 4 ];
	glReadBuffer( attachment );
	glGetTexImage( textureType, 0 , textureFormat, GL_FLOAT, data );

	cerr << setprecision( 2 );// << fixed;
	for ( int i = 0; i < clsHeight; i++ ) {
		for ( int j = 0; j < clsWidth*4; j++ ) {
			cerr << data[ i * ( clsWidth*4 ) + j  ] << '\t';	
			//printme(i*width*4 + j);		
		} // end for
		cerr << '\n';
	} // end for
	cerr << '\n';
	delete (data);
}

//______________________________________________________________________________


//______________________________________________________________________________


//______________________________________________________________________________


//______________________________________________________________________________


//______________________________________________________________________________