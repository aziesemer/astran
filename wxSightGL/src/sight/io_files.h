#ifndef IO_FILES_H
#define IO_FILES_H

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
	using std::cout;
	using std::cerr;
#include <sstream>
	using std::istringstream;
#include <string>
	using std::string;
#include <fstream>
	using std::ifstream;
	using std::ofstream;
#include <map>
    using std::map;


#include "opengl.h"
#include "camera.h"
#include "repository.h"
#include "rectangle.h"
#include "wire_line.h"
#include "vertex.h"
#include "model.h"
#include "label.h"
#include "fileformats/gds.h"
#define printme( e ) { cout << #e << " = " << ( e ) << "\n"; } 

// -----------------------------------------------------------------------------
class File{
    private:
		map<string, int> layer;
		
		vector <string> undefineds;
		
		Repository * clsRepository;
	
        int currentLayer;
        Color currentFulfilling;
        Color currentEmbroiders;
	
	protected:
		void parseLabelCIF( istringstream &input );
		
		void parseModelName( istringstream &input );
	
		void parseModelCIF( istringstream &input );
		
		void parseCallCIF( istringstream &input , const int ID );
		
		void parseLayerCIF( istringstream &input );
		
		void parseRectangleCIF( istringstream &input , const int ID) ;
		
		void parseComponent();
		
    public:
		File(){}
		File(Repository * repository);	
	
		void loadFileGDS2(const string filename){ GDSII file;	file.load(filename,(*clsRepository)); }

		void loadFileCIF( const string filename , const int ID);
		
		void SaveCIF(string filename);


};//end class

#endif
