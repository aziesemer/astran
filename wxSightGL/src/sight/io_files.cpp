#include <cstring>
#include "io_files.h"


enum State{ MODEL , FREE };
State state = FREE;

const int LABEL_ID = 31;

#include "../util/util.h"

// -----------------------------------------------------------------------------
File::File(Repository * repository) {
	clsRepository = repository;

	clsRepository->setNumLayers( 34 ); //deveria ser 33!!!!

	clsRepository->getLayer( 32 ).configure( "CELLBOX", 32, Color( 155, 0, 255 ), Color( 155,   0, 255 ), STIPPLE_MASK_EMPTY		 );
	clsRepository->getLayer( 31 ).configure( "LABEL", 31, Color( 255, 255, 255 ), Color( 255, 255, 255 ), STIPPLE_MASK_FILL			 );
	clsRepository->getLayer( 30 ).configure( "MET10", 30, Color( 255, 218, 185 ), Color( 255, 218, 185 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer( 29 ).configure( "VIA9" , 29, Color( 127, 255, 212 ), Color( 127, 255, 212 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer( 28 ).configure( "MET9" , 28, Color( 255, 250, 205 ), Color( 255, 250, 205 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer( 27 ).configure( "VIA8" , 27, Color( 139, 131, 120 ), Color( 139, 131, 120 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer( 26 ).configure( "MET8" , 26, Color( 165,  42,  42 ), Color( 165,  42,  42 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer( 25 ).configure( "VIA7" , 25, Color( 205, 192, 176 ), Color( 205, 192, 176 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer( 24 ).configure( "MET7" , 24, Color( 255, 255,   0 ), Color( 255, 255,   0 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer( 23 ).configure( "VIA6" , 23, Color( 238, 223, 204 ), Color( 238, 223, 204 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer( 22 ).configure( "MET6" , 22, Color(   0, 255,   0 ), Color(   0, 255,   0 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer( 21 ).configure( "VIA5" , 21, Color( 255, 239, 219 ), Color( 255, 239, 219 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer( 20 ).configure( "MET5" , 20, Color( 216, 191, 216 ), Color( 216, 191, 216 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer( 19 ).configure( "VIA4" , 19, Color( 193, 205, 193 ), Color( 193, 205, 193 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer( 18 ).configure( "MET4" , 18, Color(  67, 110, 238 ), Color(  67, 110, 238 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer( 17 ).configure( "VIA3" , 17, Color( 255, 192, 203 ), Color( 255, 192, 203 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer( 16 ).configure( "MET3P", 16, Color(  50, 255, 255 ), Color(  50, 255, 255 ), STIPPLE_MASK_CROSS2		 );
	clsRepository->getLayer( 15 ).configure( "MET3" , 15, Color(  50, 255, 255 ), Color(  50, 255, 255 ), STIPPLE_MASK_DIAGONAL_UP_2 );
	clsRepository->getLayer( 14 ).configure( "VIA2" , 14, Color(  57, 191, 255 ), Color( 255,   0, 255 ), STIPPLE_MASK_CROSS         );
	clsRepository->getLayer( 13 ).configure( "MET2P", 13, Color( 255,   0, 255 ), Color( 255,   0, 255 ), STIPPLE_MASK_CROSS2		 );
	clsRepository->getLayer( 12 ).configure( "MET2" , 12, Color( 255,   0, 255 ), Color( 255,   0, 255 ), STIPPLE_MASK_DOT           );
	clsRepository->getLayer( 11 ).configure( "VIA1" , 11, Color( 255,   0, 255 ), Color(   0,   0, 255 ), STIPPLE_MASK_CROSS         );
	clsRepository->getLayer( 10 ).configure( "MET1P", 10, Color(   0,   0, 255 ), Color(   0,   0, 255 ), STIPPLE_MASK_CROSS2		 );
	clsRepository->getLayer(  9 ).configure( "MET1" ,  9, Color(   0,   0, 255 ), Color(   0,   0, 255 ), STIPPLE_MASK_DIAGONAL_DOWN );
	clsRepository->getLayer(  8 ).configure( "POL2" ,  8, Color( 238,  99,  99 ), Color( 238,  99,  99 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer(  7 ).configure( "CONT" ,  7, Color( 185, 211, 238 ), Color(   5,   5,   5 ), STIPPLE_MASK_FILL          );
	clsRepository->getLayer(  6 ).configure( "POL1" ,  6, Color( 255,   0,   0 ), Color( 255,   0,   0 ), STIPPLE_MASK_HALFTONE      );
	clsRepository->getLayer(  5 ).configure( "PDIF" ,  5, Color( 255, 128,   0 ), Color( 255, 128,   0 ), STIPPLE_MASK_CROSS         );
	clsRepository->getLayer(  4 ).configure( "NDIF" ,  4, Color(   0, 204, 102 ), Color(   0, 204, 102 ), STIPPLE_MASK_CROSS         );
	clsRepository->getLayer(  3 ).configure( "PSEL" ,  3, Color( 255, 128,   0 ), Color( 255, 128,   0 ), STIPPLE_MASK_EMPTY         );
	clsRepository->getLayer(  2 ).configure( "NSEL" ,  2, Color(   0, 204, 102 ), Color(   0, 204, 102 ), STIPPLE_MASK_EMPTY         );
	clsRepository->getLayer(  1 ).configure( "NWEL" ,  1, Color(   0, 204, 102 ), Color(   0, 204, 102 ), STIPPLE_MASK_DIAGONAL_UP   );
	clsRepository->getLayer(  0 ).configure( "PWEL" ,  0, Color( 255, 128,   0 ), Color( 255, 128,   0 ), STIPPLE_MASK_DIAGONAL_UP   );

	layer["CBX"] = 32;
	layer["LABEL"] = 31;//94
	layer["MET10"] = 30;
	layer["VIA9" ] = 29;
	layer["MET9" ] = 28;
	layer["VIA8" ] = 27;
	layer["MET8" ] = 26;
	layer["VIA7" ] = 25;
	layer["MET7" ] = 24;
	layer["VIA6" ] = 23;
	layer["MET6" ] = 22;
	layer["VIA5" ] = 21;
	layer["MET5" ] = 20;	layer["CMP"  ] = 20;
	layer["VIA4" ] = 19;	layer["CVQ"  ] = 19;
	layer["MET4" ] = 18;	layer["CMQ"  ] = 18;
	layer["VIA3" ] = 17;	layer["CVT"  ] = 17;
	layer["MET3P"] = 16;	layer["CPT"  ] = 16;
	layer["MET3" ] = 15;	layer["CMT"  ] = 15;
	layer["VIA2" ] = 14;	layer["CVS"  ] = 14;
	layer["MET2P"] = 13;	layer["CPS"  ] = 13;
	layer["MET2" ] = 12;	layer["CMS"  ] = 12;
	layer["VIA1" ] = 11;	layer["CVA"  ] = 11;
	layer["MET1P"] = 10;	layer["CPF"  ] = 10;
	layer["MET1" ] =  9;	layer["CMF"  ] =  9;
	layer["POL2" ] =  8;	
	layer["CCG"  ] =  7;	layer["CONT" ] =  7;
	layer["POL1" ] =  6;	layer["CPG"  ] =  6;	layer["POLY" ] =  6;
	layer["PDIF" ] =  5;	layer["CAP"  ] =  5;	layer["CAA"  ] =  5;
	layer["NDIF" ] =  4;	layer["CAN"  ] =  4;	
	layer["PSEL" ] =  3;	layer["CSP"  ] =  3;
	layer["NSEL" ] =  2;	layer["CSN"  ] =  2;
	layer["NWEL" ] =  1;	layer["CWN"  ] =  1;
	layer["PWEL" ] =  0;	layer["CWP"  ] =  0;

} // end constructor

// -----------------------------------------------------------------------------
// Parse LAYER.
// -----------------------------------------------------------------------------
void File::parseLayerCIF( istringstream &input ){    
	string camada;
	input >> camada;
		
	if ( camada[camada.size()-1] == ';')
		camada.resize(camada.size()-1);
	
  map<string,int>::iterator iter = layer.find(camada);

	if ( iter != layer.end() ) { 
		currentLayer =  iter->second;
	} // end if
	else {
		currentLayer =  0;
		bool already = false;
		for( size_t i = 0; i < undefineds.size(); i++){
			if ( undefineds[i] == camada ){ already = true; }
		}
		if( !already ){	
			undefineds.push_back(camada);
			cerr << "[ERROR] Undefined layer " << camada << ".\n";
		}		
		
	} // end if
} //end parselayer

// -----------------------------------------------------------------------------
// Parse SUB ROTINE.
// -----------------------------------------------------------------------------
void File::parseModelCIF( istringstream &input ){  

	int ID, divd, divs;	
	input >> ID >> divd >> divs;
	
	char buffer[5];

	sprintf(buffer,"%i", ID);
	const char* str2  = "untitle ";
	char* str3;	
	str3 = (char *)calloc(strlen(str2) +7, sizeof(char));	
	strcat(str3, str2);
	strcat(str3, buffer);	
	
	Model newModel;
	newModel.setModelId(ID);
	newModel.setScale(divd , divs);	
	newModel.setName(str3);
	
	clsRepository->addModel( newModel );
}

// -----------------------------------------------------------------------------
// Parse ROTINE NAME.
// -----------------------------------------------------------------------------
void File::parseModelName( istringstream &input ){  

	string name;	
	input >>name;
	if( name[name.size()-1] == ';' ) name.erase(name.size()-1);
	
	clsRepository->clsModels[ clsRepository->currentModel ].setName(name);
}

// -----------------------------------------------------------------------------
// Parse label.
// -----------------------------------------------------------------------------
void File::parseLabelCIF( istringstream &input ){
	int x, y;
	string txt;	
	input >> txt >> x >> y;
	
	SightLabel *newLabel = new SightLabel(txt, x, y);
	
	if( state == FREE ){
		clsRepository->addObject( newLabel, LABEL_ID );
	} else {
		clsRepository->clsModels[ clsRepository->currentModel ].addContent( newLabel, LABEL_ID );	
	}//end else
}//end parse label
// -----------------------------------------------------------------------------
// Parse rectangle.
// -----------------------------------------------------------------------------
void File::parseRectangleCIF( istringstream &input , const int ID) {
	int x0, y0, x1, y1, length, width;
	input >> length >> width >> x0 >> y0;
	
	//cout<<"adding rectangle\n";
    x1 = x0 + (length / 2);
    y1 = y0 - (width / 2);
    x0 = x0 - (length / 2);
    y0 = y0 + (width / 2);

	SightRectangle *newRectangle = new SightRectangle( Vertex( x0, y0 ), Vertex( x1, y1 ) );

	if( state == FREE ){
		//cerr << "Add free...\n"; 
		clsRepository->addObject( newRectangle, ID );
	} else {
		//cerr << "Add model...\n";
		clsRepository->clsModels[ clsRepository->currentModel ].addContent( newRectangle, currentLayer );
	}		
				
} // end function

// -----------------------------------------------------------------------------
// Parse WIRE
// -----------------------------------------------------------------------------
/*void File::parseWireCIF(){	
	SightWire *newWire = new SightWire( ID+1, mouseClicks );
	newWire->setColor( Color( 0, 255, 255 ) );
	newWire->setLayer( layer );
	newWire->setThickness( thickness );
	clsRepository->addObject( newWire );
}//end function
//*/

// -----------------------------------------------------------------------------
// Parse CALL.
// -----------------------------------------------------------------------------

void File::parseCallCIF( istringstream &input, const int ID ){

	if(state == FREE){
		cerr<<"**ignorando call fora de DF**\n\n";
		return;
	}//*/

	//cout << "Parsng Call: "<< input.str() << "\n";
cerr<<"CALL\n\n";
	char in = ' '; 
	int mod;
	input>> mod;
	//cerr<< mod << " MODELO\n";
	Model *model;
	model = clsRepository->getModel( mod );
	if ( model == NULL ) {
		cerr << "[WARNING] Subrotine " << mod << " was not declared.\n";
		return;
	} // end if
	
/*	vector < ObjectReference > contents;
	
	contents = model->getContents();
	
	printme(contents.size());
	
	for (size_t i=0; i< contents.size(); i++){
		contents[i] = ObjectReference(contents[i]->clone(), contents[i].getLayer());
	}//*/
	
	sgtComponent * c = new sgtComponent();
	
	c->setModelPt(model);
	
	input >> in;

	while (in != ';'){
	//cout << "Leu: "  << in << "\n";
		
		int x;	int y;
		
		switch (in){
			case 'T' :	input >> x >> y;
						c->setTranslation(x,y);
				/*		for( size_t i=0; i<contents.size(); i++){
							contents[i]->translate(x, y);						
						}//*/
						break;
			case 'R' :	input >> x >> y;
						c->setRotation(x,y);
					/*	for(size_t i=0; i<contents.size(); i++){
							contents[i]->rotate(x, y);
						}//*/
						break;
			case 'M' :	input >> in;
						c->setMirror(in == 'X');
						c->mirror();
					/*	if( in == 'X' ){
							for(size_t i=0; i< contents.size(); i++){
								contents[i]->mirroring(true);
							}
						}else{
							for(size_t i=0; i< contents.size(); i++){
								contents[i]->mirroring(false);
							}
						}//*/
						break;		
		}
		input >> in;
	}//end while
	
	
/*	for (size_t i=0; i< contents.size(); i++){
		clsRepository->addObject( contents[i].getObject(), contents[i].getLayer() );
	}//end for*/

	clsRepository->clsModels[ clsRepository->currentModel ].addContent( c, currentLayer );	
//	clsRepository->addComponent(c); 
}

// -----------------------------------------------------------------------------
// Load file .CIF
// -----------------------------------------------------------------------------

void File::loadFileCIF( const string filename , const int ID) {
	cerr<<"opening file into iofile\n";
	ifstream file( filename.c_str() );
	if ( !file ) {
		cerr << "[ERROR] Input file (" << filename << ") could not be opened!\n";
		exit( 1 );
	} // end if
  //  cout<<"loadCIF\n";
	const int MAX_BUFFER_SIZE = 10000;
	char buffer[ MAX_BUFFER_SIZE ];
	string whatIS;

	Color color;
	clsRepository->currentModel = 0;
	
	undefineds.clear();
	
	while ( true ) {
		file.getline( buffer, MAX_BUFFER_SIZE, '\n' );
		//if ( file.eof() ) {cout<< "EOF\n"; break; }
		if ( !file ) {
			cerr << "[WARNING] Premature end of file.\n";
			return;
		} // end if

		istringstream inputLine( buffer );
		//cout << inputLine.str() << "\n";

		inputLine >> whatIS;
		if ( !inputLine ) // linha vazia
			continue;

		if ( whatIS == "94"  ) { parseLabelCIF( inputLine ); continue; } // SightLabel
		if ( whatIS == "B"   ) { parseRectangleCIF( inputLine , ID); continue; } // Box
		if ( whatIS == "L"   ) { parseLayerCIF( inputLine ); continue; } // Layer
		if ( whatIS == "W"   ) { continue; } // Wire
		if ( whatIS == "C"   ) { parseCallCIF( inputLine, ID ); continue; } // Call
		if ( whatIS == "R"   ) { continue; } // Roundflash
		if ( whatIS == "P"   ) { continue; } // Polygon
		if ( whatIS == "DS"  ) { state = MODEL; parseModelCIF( inputLine ); continue; } // Define Subrotine
		if ( whatIS == "9"   ) { parseModelName( inputLine ); continue;} // define model name
		if ( whatIS == "DF;" ) { state = FREE;  continue; } // Define Finished
		if ( whatIS == "DD"  ) { continue; } // Delete Subrotine

		if ( whatIS == "E;" ||  whatIS == "END;" || whatIS == "E" || whatIS == "END" ) {
			clsRepository->start();
			return; 
		}// End of CIF File

		
		if ( whatIS[0] == '(' ) continue;

		cerr << "[INFO] Ignoring line: " << inputLine.str() << "\n";
		
	} // end while
	
} // end function

//==============================================================================================================
//SAVE
/*
void File::SaveCIF(const string filename){
	
	ofstream file;
	file.open(filename.c_str());

	
	file<<"\t(UFRGS - Universidade Federal do Rio Grande do Sul)\n"
		<<"\t(GME - Group of Microeletronic)\n"
		<<"\t(CIF generated by SightGL)\n"
		<<"\t(Marilena Maule - mmaule@inf.ufrgs.br)\n"
		<<"\t(Guilherme Flach- gaflach@inf.ufrgs.br)\n\n";	
		
	file << "DS 1 1 1;\n";

	for( int i = 0; i < clsRepository->getNumLayers() -1; i++){
		Layer layer = clsRepository->getLayer(i);
		if( !(layer.getNumObjects() == 0) ){ 
			file << "L " << layer.getName() << ";\n";
		}
		for( int j = 0; j < layer.getNumObjects(); j++){
			//B width height x y [rx ry];
			Object * obj = layer.getObject( j );
			Vertex min = obj->getMin();
			Vertex max = obj->getMax();
			int width = abs(max.x - min.x);
			int height = abs(max.y-min.y);
			int x = width/2 + min.x;
			int y = height/2 + min.y;
			file << "B " << width << " " << height << " " << x << " " << y << ";\n";
		}//end for objects		
	}//end for layers	
	
	Layer layer = clsRepository->getLayer(clsRepository->getNumLayers() - 1);
	for(int i = 0; i < layer.getNumObjects();  i++){
		Object * obj = layer.getObject( i );
		Vertex min = obj->getMin();
		string lbl = obj->getText();
		
		file << "94 "<< lbl << " " << min.x << " " << min.y << "\n";
	}//for for labels
	
	file	<< "DF;\n"	
			<< "C 1;\n"
			<< "E;";
	
	file.close();

}//end function */





string obtainExtension( const string &str ) {
	for ( int i = str.size() - 1; i > 0; i-- )
		if ( str[ i ] == '.' )
			return str.substr( i + 1, str.size() - i );
	return "";
} // end method



//==============================================================================================================
//{TODO} LABELS!!
void File::SaveCIF(string filename){
	
	ofstream file;
	

	if(obtainExtension(filename) != "cif"){ filename = filename + ".cif"; }
	
	file.open(filename.c_str());

	
	file<<"\t(UFRGS - Universidade Federal do Rio Grande do Sul)\n"
		<<"\t(GME - Group of Microeletronic)\n"
		<<"\t(CIF generated by SightGL)\n"
		<<"\t(Marilena Maule - mmaule@inf.ufrgs.br)\n"
		<<"\t(Guilherme Flach- gaflach@inf.ufrgs.br)\n\n";	
		
	vector <Model> * models;
	
	models = clsRepository->getModelsPT();
	
	for(size_t i = 0; i < models->size(); i++ )	{
		file << "DS "<< (*models)[i].getModelId() << " " << (*models)[i].getDVD() << " " << (*models)[i].getDVS() <<";\n";
		file << "9 " << (*models)[i].getName() << ";\n";
		
		//find layers
		vector <ObjectReference> objs;
		(*models)[i].gimmeObjectsOnly(objs);

		while( !objs.empty() ){
			int currentLayer = objs[0].getLayer();
			if (currentLayer != 28 )//labels layer = 28 **CAUTION**
				file << "L " << clsRepository->getLayer(currentLayer).getName() << ";\n";	
			
			bool stop = false;	
			for(size_t l = 0; l <  objs.size() && !stop; l++){
				if(objs[l].getLayer() == currentLayer){
					Object * obj  = objs[l].getObject();
					stop = false;
					switch (obj->getType()){					
						case OBJECT_RECTANGLE: {
							Vertex min = obj->getMin();
							Vertex max = obj->getMax();
							int width = abs(max.x - min.x);
							int height = abs(max.y-min.y);
							int x = width/2 + min.x;
							int y = height/2 + min.y;
							file << "B " << width << " " << height << " " << x << " " << y << ";\n";
						} break;
						
						case OBJECT_LABEL: {
							Vertex min = obj->getMin();
							string lbl = obj->getText();							
							file << "94 "<< lbl << " " << min.x << " " << min.y << "\n";					
						} break;
						
						case OBJECT_WIRE:{}break;
						case OBJECT_LINE:{}break;
						case OBJECT_COMPONENT: break;
					};// end switch
					
					objs[i] = objs.back();
					objs.pop_back();
				} else {
					stop = true;
				}
				printme(objs.size());
				printme(l);
			}//end for save objects
		}//end while

		(*models)[i].saveCallsOnly(file);

		file<<"DF;\n";
	}//end for
		
	file	<< "E;";
	
	file.close();

}//end function


















