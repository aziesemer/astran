#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <vector>
	using std::vector;
#include <iostream>
	using std::cout;
	using std::endl;

#include <algorithm>
#include <cmath>
#include <climits>

#include "object.h"
#include "ruler.h"
#include "vertex.h"
#include "model.h"
#include "layer.h"
#include "stipple.h"

#include "camera.h"

#include "../util/util.h"

enum  {
	Z_NONE,
	Z_DELETE, 
	Z_MIRRORX,
	Z_MIRRORY,
	Z_TRANSLATE,
	Z_ROTATE,
	Z_PASTE,
	Z_CUTE
};

class Ctrl_z {//TODO
	private:
		vector <int> clsActions;		

	public:
		Ctrl_z(){}
		
		bool undo(){}
		bool redo(){}
};

//__________________________________________________________________________________________________
class Repository {
	private:
		vector< Layer > clsLayers;
		//vector< ObjectNode > clsObjectNodes;
		vector< Model > clsModels;
				
		vector <sgtComponent*> clsComponents;
		
		vector <sgtComponent*> clsHierarchyStak;
		
		int currentModel;
		int clsModelCounter;
		
		Ctrl_z clsUndo;
		
		bool clsDirty;
		
		bool modelMode;
		
		Camera * clsCamera;
		
		int clsCurrentId;
		int clsNgroup;
		
		bool deleteSel;

		Vertex clsMax;	Vertex clsMin; //absolute bounding box
		
		Vertex maxSelected; //selected bouding box
		Vertex minSelected;

		
		vector < ObjectReference > clsTransfer;//area de transferencia
		vector < ObjectReference > clsSelectedObjects;//objetos selecionados
		vector < Ruler > clsRulers;//cada regua eh um par de vertices
		
		int clsScale;
		
		enum Move { RIGHT, LEFT, UP, DOWN, RIGHT_UP, RIGHT_DOWN, LEFT_UP, LEFT_DOWN, NONE };
		
		Move direction;
		
		bool objectAtPosition( const int x, const int y, ObjectReference &objReference ) const;
		bool componentAtPosition( const int x, const int y, ObjectReference &objReference ) ;
		
	public:

		Repository(){modelMode = false; clsModelCounter = 0; deleteSel = false;}
		Repository(Camera * camera) : clsCurrentId( 0 ), clsNgroup( 1 ) { clsCamera = camera; modelMode = false; clsModelCounter =0; deleteSel = false;}
	
//++Rulers functions+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		void clearRulers(){ clsRulers.clear(); }
		void drawRulers(const int z, int scale, const float zoom);
		void deleteRuler(const int i){}//*************************	
		void deleteRuler(Ruler * ruler);	
		void addRuler(Vertex first, Vertex last){ 
						Ruler ruler = Ruler(first, last);	clsRulers.push_back(ruler ); }	
		void addRuler(Ruler ruler){ clsRulers.push_back(ruler); }
//++Rulers functions+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++Other functions++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		void setProperties( const int layer){ clsLayers[layer].setLayerFillProperties(); }		
		void setScale(const int scale){ clsScale = scale;}		
		void setDirection( Move what) { direction = what; }
		void reset();
		Vertex getMaxSB(){ return maxSelected; }
		Vertex getMinSB(){ return minSelected; }	
		float getCurrentZ(const int layer);				
	//Object++++++++++++++++++++++++++++++
		bool groupObjects();
		bool ungroupObjects();
		bool addObject( Object * newObject, const int idLayer );
		bool addObject( Object * newObject );
		bool delObject( const int id );		
	//Layers++++++++++++++++++++++++++++++++
		Layer &getLayer( const int index ) { return clsLayers[ index]; }	
		void setNumLayers(const int nLayers){ clsLayers.resize(nLayers); }		
		int getNumLayers(){ return clsLayers.size(); }
	//Models++++++++++++++++++++++++++++++++
		int getNumModels(){return clsModels.size();}
		Model * getModel(const int idx );
		Model * getLastModel(){ return &clsModels.back(); }
		bool addModel( Model newModel );
		bool createNewModel(const char* name);	
		
		vector<Model>* getModelsPT(){return &clsModels;}		
//++Other Functions++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++Draw Functions+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		void drawAll() ;
		void drawWiew(Vertex min, Vertex max) const;
		void drawCurrentLayer(const int Layer);	
		void drawSelectedBoundingBox(const int z);	
//++Draw Functions+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		
//++Selection Functions++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		Object * objectAtPosition( Vertex p ) const;
		Object * componentAtPosition( Vertex p ) ;
		Ruler * rulerAtPosition( Vertex p ) ;
		//Object *objectByIndex( const int id ) const;		
		bool findSelectedBoundingBox(Vertex p1, Vertex p2);
		bool findSelectedBB(Vertex p1, Vertex p2);
		void clearSelection(){ clsSelectedObjects.clear(); cout<<"CLEAR\n";}
		void setSelectedBoudingBox();		
		bool existsSelection(){return !clsSelectedObjects.empty(); }
		bool isClickInSelection(const int x, const int y);
	//Edit++++++++++++++++++++++++++++++++++
		void copySelection();
		void cuteSelection();		
		void pasteSelection(const int x, const int y);
//++Selection Functions++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		
//++Edit Functions+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		void moveSelecteds( const int z, const int difX, const int difY );
		void translateSelecteds( const int difX, const int difY );		
		bool letResize(Vertex p);
		void resizeSelecteds(const int x, const int y);		
		void editSelecteds(const int x, const int y, const int z);
		
		//[TODO] lembrar dos ponteiros next dos objectnode
		void deleteObj(const Object * deleteObject , const int layer );
		void deleteSelecteds();
		
		bool mirror(bool Xmirror);
//++Edit Functions+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++Absolute BoudingBox Functions++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		bool setAbsoluteBoudingBox();		
		void resetAbsoluteBoudingBox(){
			setMax( INT_MIN , INT_MIN );
			setMin( INT_MAX , INT_MAX );
		}				
		void setMax( const int x, const int y ){ clsMax.x = x; clsMax.y = y; }   
		void setMin( const int x, const int y ){ clsMin.x = x; clsMin.y = y; }  		
		Vertex getMax() { return clsMax; }
		Vertex getMin() { return clsMin; }
//++Absolute BoudingBox Functions++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//++COmponents++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		void addComponent(sgtComponent* cmp){ clsComponents.push_back(cmp); }
		
		bool createNewComponent(const char*);
		
		void flatenize();
//++COmponents++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++		
		
		Model* FindModelPt( const string &componentName );

		void upHierarchy();
		void downHierarchy();
		
		void startHierarchy();
		
		string getModelName(int i);
		Model * getModelByName(const char* name);
		void drawModel(string name);
		
		void saveNewModel(const char* name);
		
		void flatenizeModel();
		
		void saveCurrentModel();
		
		void start();//inicializa depois de ler arquivo
		
		bool dirty(){return clsDirty;}
		
		void seekAndAdd(int layer, vector <Object*> &object, size_t from);
		
		bool findConnection(const int ID);//selectConection
		
		
		
		
		
		
	friend class File;
}; // end class

#endif