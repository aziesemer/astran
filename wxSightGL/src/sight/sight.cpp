#include "sight.h"

//#define USE_ACCUMULATION_BUFFER

// -----------------------------------------------------------------------------

SightGL::SightGL() {
	clsStates.resize( SIGHT_NUM_STATES );
	clsStates[	SIGHT_SELECT    ] = new SightSelectState( *this );
	clsStates[	SIGHT_LINE      ] = new SightLineState( *this );
	clsStates[	SIGHT_WIRE      ] = new SightWireState( *this );
	clsStates[	SIGHT_RECTANGLE ] = new SightRectangleState( *this );
	clsStates[	SIGHT_RULER     ] = new SightRulerState( *this );
	clsStates[	SIGHT_DRAG      ] = new SightDragState( *this );
	clsStates[	SIGHT_LABEL     ] = new SightLabelState( *this );
	setState( SIGHT_SELECT );
	
	selectedObject = NULL;
	previousSelectedObject = NULL;
	selectedRuler = NULL;
	previousRuler = NULL;
	
	scale = 100;
	stepScale = scale/10;
	
	camera = Camera();
	repository = Repository(&camera);
	clsFile = File( (&repository) );
	
	clsCurrentLayer = 2;

	clsException = SIGHT_NONE;
		
	unsigned char c = (unsigned char)1.0f;
	clsColorGuide = Color(c,c,c);			
	
 } // end constructor

// -----------------------------------------------------------------------------

void SightGL::restartAll(){
	repository.reset();
	camera.setCameraPosition( 0, 0 );
	camera.setZoom(1.0f);
	render();
}//end function

// -----------------------------------------------------------------------------

void SightGL::mouseMove( const float x, const float y){
	camera.mouseMove(x,y);
}//atualize the cursor

// -----------------------------------------------------------------------------



//+++FILE
// -----------------------------------------------------------------------------

void SightGL::newFile(){
	restartAll();
}//create a empty project to edit

// -----------------------------------------------------------------------------

void SightGL::loadCIF( const char * const filename ) {
	ifstream fileA( filename );
	if ( !fileA ) {
		cerr << "[ERROR] Input file (" << filename << ") could not be opened!\n";
		return;
	} // end if

		repository.reset();

		camera.setCameraPosition( 0, 0 );

		clsFile.loadFileCIF( filename , 1/*ID*/ );
		
			checkGLErrors();
		if(repository.setAbsoluteBoudingBox()){cerr<<"Bouding box error\n";};		
			checkGLErrors();
		const float W = abs(repository.getMax().x - repository.getMin().x) ;
		const float H = abs(repository.getMax().y - repository.getMin().y) ;
			checkGLErrors();
	
		camera.mouseZoomIn( W , H ,
							W/2 + min( repository.getMax().x, repository.getMin().x) , 
							H/2 + min( repository.getMax().y, repository.getMin().y)
						   );	
		checkGLErrors();		
			
		camera.setInitialZoom( camera.getZoom() );
		camera.setInitialCameraCenterX(W/2 + min( repository.getMax().x, repository.getMin().x));
		camera.setInitialCameraCenterY(H/2 + min( repository.getMax().y, repository.getMin().y));
		cerr<< "ZOOM OUT\n";
		camera.zoomOut( 0.1f );//*/
		
			checkGLErrors();
	cameraMin.x = (int)camera.getBeginGridX();	
	cameraMin.y = (int)camera.getBeginGridY();
	cameraMax.x = (int)camera.getEndGridX();		
	cameraMax.y = (int)camera.getEndGridY();
} // load a cif file

// -----------------------------------------------------------------------------

void SightGL::saveFile(const char * const  filename){
	clsFile.SaveCIF(filename);
}//save a cif file

// -----------------------------------------------------------------------------



//+++LAYERS
// -----------------------------------------------------------------------------

void SightGL::setLayerVisibility( const int layer, const bool visible ) {
	repository.getLayer( layer ).setVisible( visible );	
} // set the layer visibility

// -----------------------------------------------------------------------------

string SightGL::getLayerName( const int index ) {
	return repository.getLayer( index ).getName( ); 
}//get the layer name/material

// -----------------------------------------------------------------------------

StippleMask SightGL::getLayerMask( const int index ) {
	return repository.getLayer( index ).getStippleMask( );
}//get the layer mask // fill Properties

// -----------------------------------------------------------------------------

Color SightGL::getLayerFillColor(const int index){
	return repository.getLayer( index ).getFillColor( );
}//get the layer fill color

// -----------------------------------------------------------------------------

Color SightGL::getLayerLineColor(const int index){
	return repository.getLayer( index ).getBorderColor( );
}//get the layer edge color

// -----------------------------------------------------------------------------



//+++KEYBOARD
// -----------------------------------------------------------------------------

void SightGL::handleKeyEvent(unsigned char key, bool ctrl){
	cout << "Key '" << key << " int " << (int) key<< "' was pressed.\n";

	if( ctrl ){
	/*	switch ( key ) {
			case  59: doMove(UP); break;
			case  61: doMove(DOWN); break;
			case  60: doMove(RIGHT); break;
			case  58: doMove(LEFT); break;
		}//end switch*/
	}else{
		switch ( key ) {
			case 'Z': printme(camera.getZoom()); break;
			case 'B': repository.clearRulers(); render(); break;
			case '+': camera.zoomIn ( 0.1f  ); render(); break;
			case '-': camera.zoomOut( 0.1f  ); render(); break;
			case  3 : repository.copySelection(); break;
			case  22: repository.pasteSelection(0, 0); render(); break;
			case  24: repository.cuteSelection(); render(); break;
			case  27: restartDraw(); break;
			//case  13: down = !down; handleClickLD(currentCursorX, currentCursorY, down); break;		
		} // end switch		
	}//end else
}//end method

// -----------------------------------------------------------------------------


//+++EDIT FUNCTIONS
// -----------------------------------------------------------------------------

void SightGL::copy(){
	repository.copySelection();
}//end method

// -----------------------------------------------------------------------------

void SightGL::cut(){
	repository.cuteSelection(); render();
}//end method

// -----------------------------------------------------------------------------

void SightGL::paste(){
	repository.pasteSelection(0, 0); render();
}//end method

// -----------------------------------------------------------------------------

void SightGL::deleteObj(){//temporariamente desativado
	handleKeyEvent( 8, true );//8 = delete? backspace?
}//end method

// -----------------------------------------------------------------------------

void SightGL::clearRulers(){
	handleKeyEvent( 'B', true );
}//end method

// -----------------------------------------------------------------------------



//+++OBJECT FUNCTIONS
// -----------------------------------------------------------------------------

void SightGL::addLabel( Object * newObj){
	const int lbl = repository.getNumLayers() - 1;
	repository.addObject( newObj, lbl );
}//add a new object label in the last layer

// -----------------------------------------------------------------------------

void SightGL::addObject( Object * newObj){
	repository.addObject( newObj, clsCurrentLayer );
}//add a new object in the current layer 

// -----------------------------------------------------------------------------

void SightGL::drawCurrentLayer(){
	repository.drawCurrentLayer(clsCurrentLayer);
}//draw only the current layer

// -----------------------------------------------------------------------------

void SightGL::addRuler(Ruler ruler){
	repository.addRuler(ruler);
}//add a new ruler

// -----------------------------------------------------------------------------

void SightGL::group(){
	repository.groupObjects();
}//end method

// -----------------------------------------------------------------------------

void SightGL::ungroup(){
	repository.ungroupObjects();
}//end method

// -----------------------------------------------------------------------------

void SightGL::restartDraw(){
	repository.resetAbsoluteBoudingBox();
	if( repository.setAbsoluteBoudingBox() ) { return; }
	const float W = abs(repository.getMax().x - repository.getMin().x) ;
	const float H = abs(repository.getMax().y - repository.getMin().y) ;
	camera.setCameraPosition(
		W/2 + min(repository.getMax().x, repository.getMin().x),
		H/2 + min(repository.getMax().y, repository.getMin().y));					
	camera.resetZoomAndPosition(W, H); 
}//end function

// -----------------------------------------------------------------------------


//+++CAMERA FUNCTIONS
// -----------------------------------------------------------------------------

void SightGL::zoom(const bool up){
	if(up){
		camera.zoomIn ( 0.1f  ); render();
	}else{
		camera.zoomOut( 0.1f  ); render();
	}
}//end method

// -----------------------------------------------------------------------------

void SightGL::fitIn( ){
	restartDraw();
	render();
}

// -----------------------------------------------------------------------------

bool SightGL::zoomIn(){
	const float width = abs(clsPrevTouch.x - clsCursorPos.x);
	const float height = abs(clsPrevTouch.y - clsCursorPos.y);
	if( (abs(clsPrevTouch.x - clsCursorPos.x ) * abs(clsPrevTouch.y - clsCursorPos.y)) > scale){  
		camera.mouseZoomIn( width, height, 
							width/2  + min( clsPrevTouch.x, clsCursorPos.x), 
							height/2 + min( clsPrevTouch.y, clsCursorPos.y) );
		render();
		return true;
	}else{//only click = menu
		return false;
	}
}//ens method

// -----------------------------------------------------------------------------

void SightGL::applyColorGuide(){
	glColor3f( clsColorGuide.r, clsColorGuide.g, clsColorGuide.b );
}

// -----------------------------------------------------------------------------

void SightGL::renderBlendingBox(Vertex first, Vertex last){

	restoreScreen();
	
	glDisable(GL_DEPTH_TEST);
	glEnable( GL_BLEND );
	
	glColor4f( 192 / 255.0f, 192 / 255.0f, 192 / 255.0f, 0.35f );
	
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	glBegin( GL_QUADS );	
		glVertex3f( first.x, first.y, maximo );
		glVertex3f( last.x, first.y, maximo );
		glVertex3f( last.x, last.y, maximo );
		glVertex3f( first.x, last.y, maximo );
	glEnd();
	glDisable( GL_BLEND );	

	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glBegin( GL_QUADS );
		glVertex3f( first.x, first.y, maximo );
		glVertex3f( last.x, first.y, maximo );
		glVertex3f( last.x, last.y, maximo );
		glVertex3f( first.x, last.y, maximo );
	glEnd();	
	
	glEnable(GL_DEPTH_TEST);			
	
	//if ( flush )
		glFlush();
}

// -----------------------------------------------------------------------------

void SightGL::move(){
	camera.mouseMove(clsCursorPos.x - clsPrevTouch.x, clsCursorPos.y - clsPrevTouch.y );
	render();
}



// =============================================================================
// Touches.
// =============================================================================

void SightGL::setState( const SightStateName sightState ) {
	clsCurrentStateName = sightState;
	clsStates[ clsCurrentStateName ]->onReset( false );
} // end method

// -----------------------------------------------------------------------------

void SightGL::resetState() {
	clsStates[ clsCurrentStateName ]->onReset( true );
} // end method

// -----------------------------------------------------------------------------

bool SightGL::touch( const bool stop ) {
	switch(clsException){
		case SIGHT_NONE:  clsStates[ clsCurrentStateName ]->onTouch( clsCursorPos, stop ); return true; break;
		case SIGHT_ZOOM:  return zoomIn(); break;
		case SIGHT_MOVE:  return true; break;
	}//end switch
} // end method

// -----------------------------------------------------------------------------

void SightGL::moveCursor( const int xScreen, const int yScreen ) {
	clsPreviousCursorPos = clsCursorPos;
	
	clsCursorPos.x = discretise( camera.xScreenToCamera( xScreen ), stepScale );
	clsCursorPos.y = discretise( camera.yScreenToCamera( yScreen ), stepScale );
	
	if ( clsPreviousCursorPos.x != clsCursorPos.x || clsPreviousCursorPos.y != clsCursorPos.y ){
		switch( clsException ){
			case SIGHT_NONE: clsStates[ clsCurrentStateName ]->onCursorMove( clsCursorPos ); break;
			case SIGHT_ZOOM: renderBlendingBox(clsPrevTouch, clsCursorPos); break;
			case SIGHT_MOVE: move(); break;
		}//end switch
	} // end if

#ifdef USE_ACCUMULATION_BUFFER
	if ( clsStates[ clsCurrentStateName ]->getStatus() != SIGHT_IDLE )
		(*clsSwapBuffersFunction)( clsSwapBuffersFunctionData );
#endif
} // end method

// -----------------------------------------------------------------------------

void SightGL::restoreScreen() {
	cerr << "Screen restored...\n";
#ifdef USE_ACCUMULATION_BUFFER
	glAccum( GL_RETURN,1.0f );
	checkGLErrors();
#else
	copyPixels(0, 0, clsWindowWidth, clsWindowHeight, GL_BACK, GL_FRONT);
#endif
} // end method

// -----------------------------------------------------------------------------

void SightGL::saveScreen() {
	cerr << "Screen saved...\n";
#ifdef USE_ACCUMULATION_BUFFER
	glAccum( GL_LOAD,1.0f );
	checkGLErrors();
#else
	copyPixels(0, 0, clsWindowWidth, clsWindowHeight, GL_FRONT, GL_BACK);
#endif
} // end method

// -----------------------------------------------------------------------------

void SightGL::restoreScreen( const int xSpace, const int ySpace, const int wSpace, const int hSpace ) {
	cerr << "Screen partially restored...\n";
#ifdef USE_ACCUMULATION_BUFFER
	glAccum( GL_RETURN,1.0f );
	checkGLErrors();
#else
	const int x = camera.xCameraToScreen( xSpace );
	const int y = camera.yCameraToScreen( ySpace );
	const int w = camera.distCameraToScreen( wSpace );
	const int h = camera.distCameraToScreen( hSpace );
	copyPixels(x, y, w, h, GL_BACK, GL_FRONT);
#endif	
} // end method

// -----------------------------------------------------------------------------

void SightGL::saveScreen( const int xSpace, const int ySpace, const int wSpace, const int hSpace ) {
	cerr << "Screen partially saved...\n";
#ifdef USE_ACCUMULATION_BUFFER
	glAccum( GL_LOAD,1.0f );
	checkGLErrors();
#else
	const int x = camera.xCameraToScreen( xSpace );
	const int y = camera.yCameraToScreen( ySpace );
	const int w = camera.distCameraToScreen( wSpace );
	const int h = camera.distCameraToScreen( hSpace );
	copyPixels(x, y, w, h, GL_FRONT, GL_BACK);
#endif
} // end method

// -----------------------------------------------------------------------------

void SightGL::reshapeWindow( const int width, const int height ) {
	clsWindowWidth = width;
	clsWindowHeight = height;

	cameraMin.x = (int)camera.getBeginGridX();	
	cameraMin.y = (int)camera.getBeginGridY();
	cameraMax.x = (int)camera.getEndGridX();		
	cameraMax.y = (int)camera.getEndGridY();

	camera.setViewportDimension( width, height );

	render();
}//end method

//-----------------------------------------------------------------------------

void SightGL::changeBlackgroundColor(const float r, const float g, const float b){
	glClearColor( r, g, b, 1.0f );
	render();
}

//-----------------------------------------------------------------------------

void SightGL::changeGuideColor(const float r, const float g, const float b){
	unsigned char a = (unsigned char) ( r * 255 );
	unsigned char c = (unsigned char) ( g * 255 );
	unsigned char d = (unsigned char) ( b * 255 );
	clsColorGuide = Color(a, c, d);
	render();
}


// -----------------------------------------------------------------------------
// Setup SightGL.
// -----------------------------------------------------------------------------

void SightGL::setup() {
	// Setups OpenGL.
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );		
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
		
	glClearDepth( 1.0f );
	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );		

	// Setup camera.
	camera.setViewportDimension( clsWindowWidth, clsWindowHeight );
	camera.setCameraPosition( 0, 0 );
	camera.setZoom(1.0f);
	
	repository.setScale(scale);
	repository.resetAbsoluteBoudingBox();
} // end function


//-----------------------------------------------------------------------------
// do move
//-----------------------------------------------------------------------------
void SightGL::doMove(const int direction){
	/*switch(direction){
		case UP   : clsCursorPos.y = clsCursorPos.y + scale/10; break;
		case DOWN : clsCursorPos.y = clsCursorPos.y - scale/10; break;
		case LEFT : clsCursorPos.x = clsCursorPos.x - scale/10;break;
		case RIGHT: clsCursorPos.x = clsCursorPos.x + scale/10;break;
	}
	*/
//	refreshCursor();
} // step scale idea
//-----------------------------------------------------------------------------

Object * SightGL::objectAtPosition( Vertex pos ){
	return repository.objectAtPosition( pos );
}

//-----------------------------------------------------------------------------

bool SightGL::findSelectedBoundingBox(Vertex min, Vertex max){
	return repository.findSelectedBoundingBox(min, max);
}
//-----------------------------------------------------------------------------

void SightGL::drawSelectedBoundingBox(const int maximo){
	repository.drawSelectedBoundingBox(maximo);
}

//-----------------------------------------------------------------------------

bool SightGL::existsSelection(){
	return repository.existsSelection();
}

//-----------------------------------------------------------------------------

Ruler * SightGL::rulerAtPosition(Vertex pos){
	return repository.rulerAtPosition(pos);
}

//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// delete the current selected object
//-----------------------------------------------------------------------------

void SightGL::deleteSelectedObject() {
/*	if( currentState = STATE_RULER ){
		if(selectedRuler != NULL){
			repository.deleteRuler(selectedRuler);
			render();
		}//end if
	}else{*/	
		repository.deleteSelecteds();  		
		selectedObject = NULL;
		previousSelectedObject = NULL;

		render();
//	}//end else
} // end function


// -----------------------------------------------------------------------------
// Render grid -> render the guide grid
// -----------------------------------------------------------------------------

void SightGL::renderGrid(){	

	if(camera.getZoom() < (float)10/scale ){ return; }
	applyColorGuide();
	
	const int inicialX = discretise(camera.getBeginGridX(), scale);
	const int inicialY = discretise(camera.getBeginGridY(), scale);		

	int gridX;
	int gridY;
	
	glPointSize (0.1f);
	glDisable( GL_DEPTH_TEST );
	glBegin(GL_POINTS);
		gridX = inicialX;
		gridY = inicialY;		
		for( int i=0; i < camera.getWindowHeight()/scale + 1; i++ ){
			for( int j=0; j < camera.getWindowWidth()/scale + 1; j++ ){
				//if ( i % largeStep == 0 && j % largeStep == 0 ) continue ;
				glVertex3f( gridX , gridY, 0 );	
				gridX = gridX + scale;		
			}//end for
			gridX = inicialX;
			gridY = gridY + scale;
		}//end for	
	glEnd();
	glEnable( GL_DEPTH_TEST );
	glPointSize (1.0f);
}//end method


// -----------------------------------------------------------------------------
// Render x and y axes.
// -----------------------------------------------------------------------------
void SightGL::renderAxes() {
	applyColorGuide();

	glDisable( GL_DEPTH_TEST );
	glLineWidth( 1.0f );
	glBegin( GL_LINES );
		glVertex2f( camera.xScreenToCamera(-clsWindowWidth), 0.0f );
		glVertex2f( camera.xScreenToCamera(clsWindowWidth), 0.0f );

		glVertex2f( 0.0f, camera.yScreenToCamera(-clsWindowHeight) );
		glVertex2f( 0.0f, camera.yScreenToCamera(clsWindowHeight) );
	glEnd();
	glEnable( GL_DEPTH_TEST );
} // end function


// -----------------------------------------------------------------------------
// Render.
// -----------------------------------------------------------------------------
void SightGL::render() {
	cerr << "REEEEEENDERRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRsight\n";
	checkGLErrors();

	// Clear color and depth buffers.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
	checkGLErrors();
	
	// Draw guides.
	renderAxes();
	renderGrid(); 
	
	// Draw objetcs.
	repository.drawAll();
	checkGLErrors();

	// Draw rulers.
	repository.drawRulers(maximo, scale, camera.getZoom());

	glFinish();
	glFlush();

	saveScreen();
		
	repository.drawSelectedBoundingBox(maximo);
	
	glFinish();
	glFlush();

#ifdef USE_ACCUMULATION_BUFFER
	(*clsSwapBuffersFunction)( clsSwapBuffersFunctionData );
#endif

	checkGLErrors();
} // end function

// -----------------------------------------------------------------------------
void SightGL::refreshScreen() {
	restoreScreen();
#ifdef USE_ACCUMULATION_BUFFER
	(*clsSwapBuffersFunction)( clsSwapBuffersFunctionData );
#endif
} // end method

//-----------------------------------------------------------------------------
// Maps the coordinates 
//-----------------------------------------------------------------------------
void SightGL::window_pos( GLfloat x, GLfloat y, GLfloat z, GLfloat w ){
   GLfloat fx, fy;

   /* Push current matrix mode and viewport attributes */
   glPushAttrib( GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );

   /* Setup projection parameters */
   glMatrixMode( GL_PROJECTION );
   glPushMatrix();
   glLoadIdentity();
   glMatrixMode( GL_MODELVIEW );
   glPushMatrix();
   glLoadIdentity();

   glDepthRange( z, z );
   glViewport( (int) x - 1, (int) y - 1, 2, 2 );

   /* set the raster (window) position */
   fx = x - (int) x;
   fy = y - (int) y;
   glRasterPos4f( fx, fy, 0.0, w ); // [Guilherme: Talvez trocar o 0.0 por z.]

   /* restore matrices, viewport and matrix mode */
   glPopMatrix();
   glMatrixMode( GL_PROJECTION );
   glPopMatrix();

   glPopAttrib();
}

//-----------------------------------------------------------------------------
// Copy squares from buffer to buffer
//-----------------------------------------------------------------------------
void SightGL::copyPixels( GLint x, GLint y, GLint width, GLint height, GLenum sourceBuffer, GLenum targetBuffer ){
	glDisable(GL_DEPTH_TEST);
	glReadBuffer( sourceBuffer );
	glDrawBuffer( targetBuffer );
	glDisable(GL_DEPTH_TEST);
	
	cerr<< "COPYPIXELS\n";
	
	window_pos(x, y, maximo, 1); 
	
	glCopyPixels( x, y, width, height, GL_COLOR );
	glDrawBuffer( GL_FRONT );

	GLboolean validd;
	
	glGetBooleanv( GL_CURRENT_RASTER_POSITION_VALID, &validd);
	if( !validd ){ cout<<"RASTER PANIC*****************\n"; }
	glEnable(GL_DEPTH_TEST);	
	glFinish();

	//repository.drawRulers(maximo, scale, camera.getZoom());
	
}


// -----------------------------------------------------------------------------
// Discretise -> discretize de mouse moves
// -----------------------------------------------------------------------------
int SightGL::discretise( const float f, const int step){
	return ((int)round(f) / step ) * step; 
}//end function

// -----------------------------------------------------------------------------
unsigned char* SightGL::getPixels() const {

	float viewport[4];
	glGetFloatv( GL_VIEWPORT, viewport );

	GLubyte * pixels = new GLubyte[clsWindowWidth * clsWindowHeight * 3];
	
	glReadPixels (0, 0, clsWindowWidth, clsWindowHeight, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	return pixels;
}

// -----------------------------------------------------------------------------

void SightGL::toUp(){
	camera.toUp(0.1f);
	render();
}

// -----------------------------------------------------------------------------

void SightGL::toDown(){
	camera.toDown(0.1f);
	render();
}

// -----------------------------------------------------------------------------

void SightGL::toLeft(){
	camera.toLeft(0.1f);
	render();
}

// -----------------------------------------------------------------------------

void SightGL::toRight(){
	camera.toRight(0.1f);
	render();
}

// -----------------------------------------------------------------------------

void SightGL::moveObjects(Vertex click){
	restoreScreen();
	repository.translateSelecteds( clsCursorPos.x - click.x, clsCursorPos.y - click.y );
	render();
}

// -----------------------------------------------------------------------------

void SightGL::moveSelection(Vertex click){
	//printme(click);
	//printme(clsCursorPos);
	restoreScreen();
	repository.moveSelecteds( maximo, clsCursorPos.x - click.x, clsCursorPos.y - click.y );
	glFlush();
}

// -----------------------------------------------------------------------------

bool SightGL::isClickInSelection(){
	return repository.isClickInSelection(clsCursorPos.x, clsCursorPos.y);
}

// -----------------------------------------------------------------------------

string SightGL::getCurrentLabel(){
	return clsCurrentLabel;
}

// -----------------------------------------------------------------------------

void SightGL::setLabel(string label){
	clsCurrentLabel = label;
}

// -----------------------------------------------------------------------------

void SightGL::drawLabelLayer(){
	const int lbl = repository.getNumLayers() - 1;
	printme(lbl);
	repository.drawCurrentLayer(lbl);
}

// -----------------------------------------------------------------------------

void SightGL::hierarchy(bool down){
	if(down){
		cerr<<"DOWN H\n\n";
		repository.downHierarchy();
		restoreScreen();
	}else{
		cerr<<"UP H\n\n";
		repository.upHierarchy();
		restoreScreen();
	}//end else
} // level hierarchy navigation

// -----------------------------------------------------------------------------

string SightGL::getModelName(int i){
	return repository.getModelName(i);
}

// -----------------------------------------------------------------------------

int SightGL::getNumModels(){
	return repository.getNumModels();
}

// -----------------------------------------------------------------------------

void SightGL::setCurrentModel(string name){
	repository.drawModel(name);
	fitIn();
}

// -----------------------------------------------------------------------------

void SightGL::createComponent(){//from model

}

// -----------------------------------------------------------------------------

bool SightGL::createNewComponent(const char* name){//with new model
		return repository.createNewComponent( name);
}//wrong*?

// -----------------------------------------------------------------------------

bool SightGL::createNewModel(const char* name){
	return repository.createNewModel(name);
}

// -----------------------------------------------------------------------------

void SightGL::saveModel(){
	repository.saveCurrentModel();
}

// -----------------------------------------------------------------------------

void SightGL::saveModelAs(const char* name){
	repository.saveNewModel(name);
}

// -----------------------------------------------------------------------------

void SightGL::loadGDSII( const char* const filename ){
	ifstream fileA( filename );
	if ( !fileA ) {
		cerr << "[ERROR] Input file (" << filename << ") could not be opened!\n";
		return;
	} // end if

		repository.reset();

		camera.setCameraPosition( 0, 0 );

		clsFile.loadFileGDS2( filename );
		
			checkGLErrors();
		if(repository.setAbsoluteBoudingBox()){cerr<<"Bouding box error\n";};		
			checkGLErrors();
		const float W = abs(repository.getMax().x - repository.getMin().x) ;
		const float H = abs(repository.getMax().y - repository.getMin().y) ;
			checkGLErrors();
	
		camera.mouseZoomIn( W , H ,
							W/2 + min( repository.getMax().x, repository.getMin().x) , 
							H/2 + min( repository.getMax().y, repository.getMin().y)
						   );	
		checkGLErrors();		
			
		camera.setInitialZoom( camera.getZoom() );
		camera.setInitialCameraCenterX(W/2 + min( repository.getMax().x, repository.getMin().x));
		camera.setInitialCameraCenterY(H/2 + min( repository.getMax().y, repository.getMin().y));
		cerr<< "ZOOM OUT\n";
		camera.zoomOut( 0.1f );//*/
		
			checkGLErrors();
	cameraMin.x = (int)camera.getBeginGridX();	
	cameraMin.y = (int)camera.getBeginGridY();
	cameraMax.x = (int)camera.getEndGridX();		
	cameraMax.y = (int)camera.getEndGridY();
}

// -----------------------------------------------------------------------------

void SightGL::mirror(bool Xmirror){
	if(repository.mirror(Xmirror)){
		render();
	}//end if
}//end mthod

// -----------------------------------------------------------------------------
void SightGL::connection(const int ID){
	if(repository.findConnection(ID)) 
		render();
}

// -----------------------------------------------------------------------------



// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------














