/*
 *  Model.h
 *  SightGL
 *
 *  Created by Lenna on 4/26/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MODEL_H
#define MODEL_H

#include <utility>
	using std::pair;
#include <string>
	using std::string;
#include <vector>
	using std::vector;
#include <fstream>
	using std::ofstream;

#include "object.h"
#include "opengl.h"
#include "stipple.h" 
#include "layer.h"


#include "txGpu.h"

//__________________________________________________________________________________________________
class ObjectReference {
	private:
		Object * clsObject;
		int clsLayer;

	public:
		ObjectReference( Object * object, int layer) : clsObject( object ), clsLayer( layer ) {}
	//	ObjectReference( Object * object, int layer, int group) : clsObject( object ), clsLayer( layer ), clsGroup(group) {}
		ObjectReference() : clsObject( NULL ), clsLayer( -1 ) {}
		
		Object * operator->() const { return clsObject; }
	
		Object *getObject() const { return clsObject; }
		
		int getLayer() const { return clsLayer; }
		int getGroup() const { return clsObject->getGroup( ); }
		void setGroup( const int g){ clsObject->setGroup( g ); }
	
};

// -----------------------------------------------------------------------------
//picture is the photo of the model
//when we draw, we transform the photo :p
// -----------------------------------------------------------------------------
class Model {
	private:
		vector <Layer> * clsLayers;
		
		int clsModelId;
		int clsScale;
		int clsLayerIndex;
		
		TXmatrix clsPicture;
		
		vector < ObjectReference > clsContents;//object + layer
		string clsName;
		
		Vertex clsMin;
		Vertex clsMax;
		
		int clsDividendo, clsDivisor;
		
	public:		
		Model(){ clsLayerIndex = 0;  }
	
		void setModelId( int ID ){ clsModelId = ID; }
		int getModelId(){ return clsModelId; }
		
		void setLayers(vector <Layer> * layers){ clsLayers = layers; }
		
		int getLayer(){ return clsLayerIndex; }
		
		void setName(string name){ clsName = name; }
		string getName(){ return clsName; }		
		
		void setScale( int dvd, int dvs ){ clsScale = dvd/dvs; clsDividendo = dvd; clsDivisor = dvs; }
		int getScale(){ return clsScale; }
		
		int getDVD(){return clsDividendo;}
		int getDVS(){return clsDivisor;}
		
		void setMinMax(Vertex min, Vertex max){clsMin = min; clsMax = max;}
		void setMinMax();
		
		Vertex getMax(){return clsMax;}
		Vertex getMin(){return clsMin;}
		
		void gimmeObjects( vector <ObjectReference> & vec );
		void gimmeObjectsOnly( vector <ObjectReference> & vec );
		void saveCallsOnly( ofstream &file);
		
		void saveChanges();
		
		void picMe();
		
		bool empty(){ return clsContents.empty();}
		
		bool addContent( Object *newObject, int layer );

		vector <ObjectReference > getContents(){ return clsContents; }	
		
		void draw();
		void drawFillFromModel( const Vertex translation , const Vertex rotation, const bool mirrorX, bool hasmirror );
		void drawEdgeFromModel( const Vertex translation , const Vertex rotation, const bool mirrorX, bool hasmirror );
		void drawFromModel(const Vertex translation, const Vertex Rotation, const bool mirrorX, const bool hasmirror );
		void drawLayerFromModel( const Vertex translation, const Vertex Rotation, const bool mirrorX, const bool hasmirror, int layer );		
		void drawFromPicture( const Vertex translation , const Vertex rotation, const bool mirrorX, bool hasmirror );
};

// -----------------------------------------------------------------------------

enum sgtHierarchy { SGT_ACTIVE, SGT_IDLE, SGT_OPEN };
//active: cam be selected
//idle: no selection
//open: sons cam be selected

// -----------------------------------------------------------------------------

class sgtComponent : public Object {
	private:
		Model* clsModelIndex;
		Vertex clsTranslation; //translaton from model
		Vertex clsRotation;
		bool clsMirrorX;
		bool hasMirror;
		
		bool clsDirty;//to see
		
		sgtHierarchy clsActiveHierarchy;
		
		Vertex clsMax, clsMin;
		
		vector < Object * > clsObjects;//object + layer
	
	public:
	
		sgtComponent();
		sgtComponent(Vertex t, Model* m);
		
		bool mirrorX(){ return clsMirrorX; }
		bool hMirror(){ return hasMirror; }
		bool hasTranslation(){ return (clsTranslation.x != 0 || clsTranslation.y != 0); }
		
		void setModelPt( Model * model );
		Model* getModelPt(){ return clsModelIndex; }
		int getModelId(){ return clsModelIndex->getModelId(); }
		
		void drawMe(){ clsModelIndex->drawFromModel( clsTranslation, clsRotation, clsMirrorX, hasMirror ); }
		void drawMe(int layer){ clsModelIndex->drawLayerFromModel( clsTranslation, clsRotation, clsMirrorX, hasMirror, layer ); }
		void drawPic(){clsModelIndex->drawFromPicture( clsTranslation, clsRotation, clsMirrorX, hasMirror ); }
		//void drawMe(){ clsModelIndex->drawFromPicture( clsTranslation, clsRotation, clsMirrorX, hasMirror ); }
		
		void gimmeObjects( vector <ObjectReference> & vec );

		Vertex getTranslation(){ return clsTranslation; }
		
		void reflatenize();
		
		void setMinMax();
		
		void setTranslation(int x, int y);
		void setMirror(bool X);
		void setRotation(int x, int y){ clsRotation = Vertex(x,y); }
		void mirror(){ hasMirror = true; }
		
		void setHierarchy(sgtHierarchy h);	
		sgtHierarchy getHierarchyStatus(){return clsActiveHierarchy; }	
		
		virtual void draw( const float z ) const;
		virtual void drawEdges( const float z ) const;
		virtual void drawEditMode( const int z, const int x1, const int y1) const;
		virtual void resize( const int x1, const int y1, const int x2, const int y2) {}	

		virtual Object* clone()const{ return new sgtComponent(*this); }
		
		virtual Vertex getMin()const { return clsMin; }
		virtual Vertex getMax()const { return clsMax; }
				
		virtual	void setGroup( int g ){ }
		virtual int getGroup() const { return 0; }	
		
		int getLayer(){ return clsModelIndex->getLayer(); }
				
		virtual void translate(const int x, const int y);
		virtual void rotate(const int x, const int y) {}
		virtual void mirroring(const bool inX) {}
		
		Object* contains( Vertex point );//return a pointer to this or a son

		virtual bool contains( const int x, const int y ) const;// return true if the object contains the vertex
		virtual bool inn(Vertex min, Vertex max) const;// return true if the object is all into the box
		virtual bool intersected(Vertex min, Vertex max) const;// return true if the object is intersected by the line
		
		virtual bool intersected(Object * obj) const{ return false; }

};


#endif
