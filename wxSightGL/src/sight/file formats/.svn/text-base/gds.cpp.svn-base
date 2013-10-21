#include <vector>
using std::vector;
/*
#include "../general/Position.h"
#include "../general/Types.h"
#include "../elements/Element.h"
#include "../elements/ComponentInstance.h"
#include "../elements/LayeredBox.h"//*/
#include "BinaryFileReader.h"

#include "gds.h"

#define MYDEBUG 1

class drawingfield
{
public:

//! active layer
/*!
This is the default layer, on which most of the operation take place.
*/  
   int activeLayer;
//! actuell grid in X
/*!
This member holds the actuell X-grid in drawinguntis
*/
   int gridX;
//! actuell grid in Y
/*!
This member holds the actuell X-grid in drawinguntis
*/
   int gridY;
//! actuell grid-offset in X
/*!
This member holds the actuell grid-offset in X in drawinguntis
*/
   int gridOffsetX;
//! actuell grid-offset in Y
/*!
This member holds the actuell grid-offset in Y in drawinguntis
*/
   int gridOffsetY;

public:
//! automatic grid
/*!
If this member is true, the grid is automaticly change after changing the scale.
*/
   bool gridauto;
//! userunits
/*!
All values in the main widget are displayed in this unit.
*/
   double userunits;
//! databaseunits
/*!
All coordinates are integer. This value is the factor of these integers to the real value.
*/
   double databaseunits;
//! libname
/*!
Name of this libary as used in GDS-file.
*/
   string libname;
   bool modified;
   bool autorepaint;
// macrorecording
   bool record;
   string macro;
};

void GDSII::load( const string fileName, Repository &repository ){
	drawingfield *d = new drawingfield();
	
	cerr<<"READING FILE GDS2\n\n";
	
	string s;
	/*
	if (typ==fileImport) {
		report.setTitle(tr("Import of GDSII-File \"")+fileName+"\"");}
	else  if (typ==fileOpen){
		report.setTitle(tr("Open of GDSII-File \"")+fileName+"\"");}
	else  if (typ==fileUpdate){
		report.setTitle(tr("Update with GDSII-File \"")+fileName+"\"");}
	*/
	try { 

		//cellList *firstcellhelp=d->first_cell;
		double databaseunitshelp=d->databaseunits; // [WARNING]

		BinaryFileReader f;
		if ( f.Open( fileName ) ) {
			throw string("Can not open File.");}

		int16 reclen; //bytes pro eintrag
		int8 record; //type of record
		int8 type;   //datatype

		// statistics
		int unused=0; // not used records
		int used=0; // used records
		int items=0; //element pro eintrag

		// import variables
		int8 help;   //help
		int16 int16,
			year,month,day,hour,min,sec,  //date/time load
			help16; //help
		int32 int32,int32x,int32y;

		int anzx=0,anzy=0;
		int layer=1;
		int presentation=0;
		float float_=0;
		double double_=0;
		double angle=0,mag=1;
		bool mirror_x=false;
		bool rotate=false;
		bool mag_=false;
		int beginExt=0,endExt=0;
		int width=0;
		int cap=0;
		string s,  //help
			sname;
		int elementmode=0;
		int datatype=0;
		int layer_max = -1;
		Object *element_=NULL;
		vector< Vertex > point_array;
		Vertex point;

		//cellList *cell_list;
		Model * cell_; //cell *cell_;
		cell_=NULL;
		//cell_list=NULL;
		int recordCount=0;

		while (!(f.end)) {   
			recordCount++;
			reclen=f.ReadInt16();
			record=f.ReadInt8(); /* Record Type */
			type=f.ReadInt8(); /* Data Type */
			if ((record>5)&&(record<31)&&(cell_==NULL)) throw string(("File corrupt."));
			switch (type) {
				case 0:	                      break; // No Data
				case 1: items = (reclen-4)/2; break; // Bit Array
				case 2: items = (reclen-4)/2; break; // Two Byte signed int
				case 3:	items = (reclen-4)/4; break; // Four Byte signed int
				case 4: items = (reclen-4)/4; break; // Four byte real
				case 5: items = (reclen-4)/8; break; // Eight byte real
				case 6: items = (reclen-4);   break; // ASC
			} // end switch type

			//if (MYDEBUG){printf("record %d :\n",record);}
			switch (record){
				case 0: //HEADER
					help16=f.ReadInt16();
					used++;
					if (MYDEBUG){printf("HEADER %d \n",help16);}
					break;
				case 1: //BGNLIB
					year=f.ReadInt16();
					month=f.ReadInt16();
					day=f.ReadInt16();
					hour=f.ReadInt16();
					min=f.ReadInt16();
					sec=f.ReadInt16();
					//ts>>year>>month>>day>>hour>>min>>sec;
					//d->date_modification.setYMD(year+1900,month,day);
					//d->time_modification.setHMS(hour,min,sec);
					if (MYDEBUG){printf("BGNLIB\n");
					printf("Modification: %d:%d:%d,%d:%d:%d\n",year+1900,month,day,hour,min,sec);}
					year=f.ReadInt16();
					month=f.ReadInt16();
					day=f.ReadInt16();
					hour=f.ReadInt16();
					min=f.ReadInt16();
					sec=f.ReadInt16();
					//ts>>year>>month>>day>>hour>>min>>sec;
					//d->date_access.setYMD(year+1900,month,day);
					//d->time_access.setHMS(hour,min,sec);
					if (MYDEBUG){printf("Access: %d:%d:%d,%d:%d:%d\n",year+1900,month,day,hour,min,sec);}
					used++;
					break;
				case 2: //LIBNAM
					//d->libname=ReadString(&f,items);
					//if (MYDEBUG){printf("LIBNAM %s\n",d->libname.toLatin1().data());}
					if (MYDEBUG){printf("LIBNAM\n");}
					used++;
					break;
				case 3: //UNITS 
					double_=f.Read8ByteReal();
					d->userunits = double_; //if (typ==fileOpen) d->userunits = double_;
					d->databaseunits = f.Read8ByteReal();
					if (MYDEBUG){
						printf("UNITS\n");
						printf("userunits %e\n",d->userunits);
						printf("databaseunits %e\n",d->databaseunits);
					}
					used++;
					break;
				case 4: //ENDLIB
					if (MYDEBUG){
						printf("ENDLIB\n");
					}
					f.end=true;
					used++;
					break;
				case 5: //BGNSTR
					if (MYDEBUG){
						printf("BGNSTR\n");
					}
					used++;
//TODO .addcomponent?					repository.addModel(); //cell_list=d->addCell();
					cell_ = repository.getLastModel(); //cell_=cell_list->this_cell;
					year=f.ReadInt16();
					month=f.ReadInt16();
					day=f.ReadInt16();
					hour=f.ReadInt16();
					min=f.ReadInt16();
					sec=f.ReadInt16();
					//ts>>year>>month>>day>>hour>>min>>sec;
					//cell_->date_modification.setYMD(year+1900,month,day);
					//cell_->time_modification.setHMS(hour,min,sec);
					if (MYDEBUG){printf("Modification: %d:%d:%d,%d:%d:%d\n",year+1900,month,day,hour,min,sec);}
					year=f.ReadInt16();
					month=f.ReadInt16();
					day=f.ReadInt16();
					hour=f.ReadInt16();
					min=f.ReadInt16();
					sec=f.ReadInt16();
					//ts>>year>>month>>day>>hour>>min>>sec;
					//cell_->date_access.setYMD(year+1900,month,day);
					//cell_->time_access.setHMS(hour,min,sec);
					if (MYDEBUG){printf("Access: %d:%d:%d,%d:%d:%d\n",year+1900,month,day,hour,min,sec);}
					break;
				case 6: //STRNAM
					cell_->setName( f.ReadString( items ) ); //cell_->cellName=ReadString(&f,items);
					if (MYDEBUG){
						printf("STRNAM\n");
						//printf("Libname %s\n",cell_->cellName.toLatin1().data());
					}
					used++;
					break;
				case 7: //ENDSTR
					if (MYDEBUG){
						printf("ENDSTR\n");
					}
					used++;
					break;
				case 8: //BONDRY ->Polygon
					elementmode=110;
					if (MYDEBUG){
						printf("BONDRY\n");
					}
					used++;
					break;
				case 9: //PATH 
					elementmode=150;
					angle=0;
					mag=1;
					mirror_x=false;
					width=0;
					cap=0;
					endExt=0;
					beginExt=0;
					if (MYDEBUG){
						printf("PATH\n");
					}
					used++;
					break;
				case 10: //SREF
					elementmode=120;
					angle=0;
					mag=1;
					mirror_x=false;
					rotate=false;
					mag_=false;
					if (MYDEBUG){
						printf("SREF\n");
					}
					used++;
					break;
				case 11: //AREF
					elementmode=130;
					angle=0;
					mag=1;
					mirror_x=false;
					rotate=false;
					mag_=false;
					if (MYDEBUG){
						printf("AREF\n");
					}
					used++;
					break;
				case 12: //TEXT
					elementmode=140;
					angle=0;
					mag=1;
					width=0;
					mirror_x=false;
					rotate=false;
					mag_=false;
					presentation=0;
					if (MYDEBUG){
						printf("TEXT\n");
					}
					used++;
					break;
				case 13: //LAYER
					//ts>>int16;
					layer=f.ReadInt16();
					if (MYDEBUG){
						printf("LAYER %d\n",layer);
					}
					if (layer<0){layer=layer_max-1;}
					if (layer>=layer_max){layer=layer_max-1;}
					used++;
					break;
				case 14: //DTATYP
					//ts>>int16;
					used++;
					datatype=f.ReadInt16();
					break;
				case 15: //WIDTH 
					//ts>>int32;
					width=f.ReadInt32();
					if (MYDEBUG){
						printf("WIDTH %d\n",width);
					}
					//if (width<0) {width=-width;}
					//if (width<0) {report.addItem("Element(s) with negative (=absolute) width.",4,s.setNum(width));}
					used++;
					break;
				case 16: //XY
					if (MYDEBUG){
						printf("XY %d\n",(items/2));
					}
					point_array.resize((int)(items/2));
					for (int g=0;g<(int)(items/2);g++){
						int32x=f.ReadInt32();
						int32y=f.ReadInt32();
						//ts>>int32x>>int32y;
						if (MYDEBUG){
							printf("%d: %d %d %d\n",g,int32x,int32y,(int)(items/2));
						}
						point_array[ g ] = Vertex( int32x, int32y ); //point_array.setPoint(g,int32x,int32y);
					}
					used++;
					break;
				case 17: //ENDEL
					if (MYDEBUG){
						printf("ENDEL\n");
						printf("element %d\n",elementmode);
					}
					used++;
					switch (elementmode){
						case 100:
							//element_=cell_->addBox(point_array,layer);
							//element_->setDatatype(datatype);
							element_ = new SightRectangle( point_array[ 0 ], Vertex(point_array[ 0 ].x+ 100, point_array[ 0 ].y + 100 )); 
							cell_->addContent( element_, layer );
							break;
						case 110: 
							//element_=cell_->addPolygon(point_array,layer);
							//element_->setDatatype(datatype);
							break;
						case 120:
							element_ = new sgtComponent( point_array[ 0 ], repository.FindModelPt( sname ) );
							
							//element_=cell_->addCellref();
							//element_->setPos(point_array.point(0));
							//element_->setCellRef(d->findCell(sname));
							//element_->setName(sname);
							//if(mirror_x)element_->rotate(-angle);
							//else element_->rotate(angle);
							//element_->scale(mag);
							//if(mirror_x)element_->setMirrorx();
							sname="";
							mirror_x=false;
							rotate=false;
							break;
						case 130:
							/*if (mirror_x){
							point=point_array.point(1);
							point.setY(-point.y());
							point_array.setPoint(1,point);
							point=point_array.point(2);
							point.setY(-point.y());
							point_array.setPoint(2,point);};*/
							
							//element_=cell_->addCellrefArray(d->findCell(sname),point_array,anzx,anzy);
							//if(mirror_x)element_->rotate(-angle);
							//else element_->rotate(angle);
							//element_->scale(mag);
							//element_->setName(sname);
							//if(mirror_x)element_->setMirrorx();
							sname="";
							mirror_x=false;
							rotate=false;
							mag_=false;
							anzx=0;
							anzy=0;
							break;
						case 140:
							//if (mag<=1&&width==0){width=-10;}
							//if (width==0){width=-10;}
							//if (width==1){width=-10;}
							//element_=cell_->addText(layer,point_array.point(0),sname);
							//if(mirror_x)element_->rotate(-angle);
							//else element_->rotate(angle);
							//element_->scale(mag);
							//element_->setName(sname);
							//if(mirror_x)element_->setMirrorx();
							//element_->setWidth(width);
							//element_->setPresentation(presentation);
							mirror_x=false;
							rotate=false;
							mag_=false;
							sname="";
							break;
						case 150:
							//element_=cell_->addPath(point_array,layer);
							//element_->setWidth(width);
							//if (cap!=4) element_->setCap(cap);
							//else element_->expandCaps(beginExt,endExt);
							break;
					} // switch endmodule

					elementmode=0;
					datatype=0;
					break;
				case 18: //SNAME
					sname= f.ReadString(items);
					if (MYDEBUG){
						printf("SNAME %s\n",sname.c_str());
					}
					used++;
					break;
				case 19: //COLROW
					//ts>>int16;
					anzx=f.ReadInt16();
					//ts>>int16;
					anzy=f.ReadInt16();
					if (MYDEBUG){
						printf("COLROW\n");
					}
					used++;
					break;
					/*case 20: //TXTNOD
					break;
					case 21: //NODE
					break;*/
				case 22: //TXTTYP
					//ts>>int16;
					used++;
					datatype=f.ReadInt16();
					break;
				case 23: //PRSTTN
					//ts>>int16;
					presentation=(f.ReadInt16()&0x000F);
					if (MYDEBUG){
						printf("PRSTIN\n");
					}
					used++;
					break;
					/*case 24: //SPACNG
					break;*/
				case 25: //STRING
					sname=f.ReadString(items);
					if (MYDEBUG){
						printf("STRING %s\n",sname.c_str());
					}
					used++;
					break;
					break;
				case 26: //STRANS
					//ts>>int16;
					int16=f.ReadInt16();
					if ((int16& 0x8000)!=0){mirror_x=true;}else{mirror_x=false;};
					if ((int16& 0x0002)!=0){rotate=true;}else{rotate=false;};
					if ((int16& 0x0004)!=0){mag_=true;}else{mag_=false;};
					if (MYDEBUG){
						printf("STRANS\n");
					}
					used++;
					break;
				case 27: //MAG
					mag = f.Read8ByteReal();
					if (MYDEBUG){
						printf("MAG\n");
					}
					used++;
					break;
				case 28: //ANGLE 
					angle = f.Read8ByteReal();
					if (MYDEBUG){
						printf("ANGLE\n");
					}
					used++;
					break;
					/*case 29: //intEG
					break;
					case 30: //USTRNG
					break;
					case 31: //REFLIB
					s=ReadString(&ts,items);
					if (MYDEBUG){
					printf("REFLIB %s\n",s.toLatin1().data());
					}
					break;
					case 32: //FONTS 
					break;*/
				case 33: //PTHTYP
					//ts>>int16;
					cap=f.ReadInt16();
					if (MYDEBUG){
						printf("PATHTYPE %d\n",cap);
					}
					used++;
					break;
					break;
					/*case 34: //GENRTS
					break;
					case 35: //ATRTBL
					break;
					case 36: //STPTBL
					break;
					case 37: //STRTYP
					break;
					case 38: //EFLAGS
					break;
					case 39: //ELKEY 
					break;
					case 40: //LNKTYP
					break;
					case 41: //LNKKEYa
					break;
					case 42: //NODTYP
					break;
					case 43: //PROATR 
					break;
					case 44: //PROVAL
					break;*/
				case 45: //BOX 
					elementmode=100; 
					if (MYDEBUG){
						printf("BOX\n");
					}
					used++;
					break;
				case 46: //BOXTYP
					//ts>>int16;
					if (MYDEBUG){
						printf("BOXTYPE\n");
					}
					used++;
					datatype=f.ReadInt16();
					break;
					/*case 47: //PLEX  
					break;*/
				case 48: //BGNEXTN
					beginExt=f.ReadInt32();
					if (MYDEBUG){
						printf("BGNEXTN %d\n",beginExt);
					}
					used++;
					break;
				case 49: //ENDEXTN
					endExt=f.ReadInt32();
					if (MYDEBUG){
						printf("ENDEXTN %d\n",endExt);
					}
					used++;
					break;
					/*case 50: //TAPNUM
					break;
					case 51: //TAPCOD
					break;
					case 52: //STRCLS
					break;
					case 53: //RESRVD
					break;
					case 54: //FORMAT
					break;
					case 55: //MASK
					break;
					case 56: //ENDMSK
					break;
					case 57: //LDIRSZ
					break;
					case 58: //SRFNAM
					break;
					case 59: //LIBSCR
					break;*/
				default: 
					{
						//if (MYDEBUG){printf("unused record %d, type %d :",record,type); }
						 //report.addItem(tr("unknown/unsupported record(s)."),3,s.setNum(record));
						 unused++;
						 for (int i=0;i<items;i++){
							 switch (type) {
								case 0:
									break;	/* No Data */
								case 1:			/* Bit Array */
									int16=f.ReadInt16();
									if (MYDEBUG){ printf("%d ",int16);}
									break;
								case 2: 			/* Two Byte signed int */
									int16=f.ReadInt16();
									if (MYDEBUG){ printf("%d ",int16);}
									break;
								case 3: 			/* Four Byte signed int */
									int32=f.ReadInt32();
									if (MYDEBUG){ printf("%d ",int32);}
									break;
								case 4: 			/* Four byte real */
									//ts>>float_;
									int32=f.ReadInt32();
									if (MYDEBUG){ printf("%f ",float_);}
									break;
								case 5: 			/* Eight byte real */
									int32=f.ReadInt32();
									int32=f.ReadInt32();
									if (MYDEBUG){ printf("%f ",double_);}
									break;
								case 6: 			/* ASC */
									help=f.ReadInt8();
									if (MYDEBUG){ printf("%s ",&help);}
									break;
							 } // end switch type 
						 } // end for i < items
						 if (MYDEBUG){ printf(" (ende)\n");}
						 break;
					} // end default case
			} // end switch (default)
			if (((recordCount%10000)==0)||(f.end) ){
				//QString sh;
				//sh.setNum(recordCount);
				//d->showMessage(tr("%1 records loaded.").arg(sh));
			} // end if
		} // end while
		f.Close();
		//printf("time: %d ms\n", t.elapsed());
		//printf("1: %d ms\n", t.elapsed());
		// resolving brocken cellrefs
		/*
		for (cellList *f =d->first_cell; f!=NULL;f=f->next_cell) {
			if (f->this_cell!=NULL){
				for (elementlist *e=f->this_cell->firstElement;e!=NULL;e=e->next_element){
					if (e->this_element->isCellref()||e->this_element->isCellrefArray()) {
						if (e->this_element->depend()==NULL){e->this_element->setCellRef(d->findCell(e->this_element->getName()));}
						if ((e->this_element->depend()==NULL)&&(typ==fileImport)){  //refs to before existing cells
							cellList *c=d->first_cell;
							d->first_cell=firstcellhelp;
							e->this_element->setCellRef(d->findCell(e->this_element->getName()));
							d->first_cell=c;}
						if (e->this_element->depend()==NULL){
							//add empty cells
							cellList *c;
							c=d->addCell();
							c->this_cell->cellName=e->this_element->getName();
							e->this_element->setCellRef(d->findCell(e->this_element->getName()));
							//f->this_cell->deleteElement(e->this_element);
							//e=f->this_cell->first_element;
							report.addItem(tr("Cellref(s) can not be resolved. Empty cell added."),1,e->this_element->getName());}
					}
					if (e==0) {break;}

				}
				f->this_cell->clean();
			}
		}
		
		//printf("2: %d ms\n", t.elapsed());
		//  
		d->aktuell_cell=d->findTopCell();
		//printf("3: %d ms\n", t.elapsed());
		if (d->aktuell_cell==NULL) d->aktuell_cell=d->first_cell->this_cell;
		if ((typ==fileImport)||(typ==fileUpdate)) {
			if (d->databaseunits!=databaseunitshelp) {
				report.addItem(tr("Databaseunits are different.Import is fitted."),3,s.setNum(d->databaseunits,'g',10));
				for (cellList *f =d->first_cell; f!=NULL;f=f->next_cell) {f->this_cell->resize(d->databaseunits/databaseunitshelp);}
				d->databaseunits=databaseunitshelp;}
		}
		//printf("4: %d ms\n", t.elapsed());
		if (typ==fileImport) {filegeneral::import(&report,d,firstcellhelp);}
		if (typ==fileUpdate) filegeneral::update(&report,d,firstcellhelp);
		*/
		if (MYDEBUG) {printf("Load complete\n");
		printf("%d unused records\n",unused);
		printf("%d used records\n",used);
		}
		//printf("5: %d ms\n", t.elapsed());
	} //end try
	catch (string s){
		//report.addItem(tr("Aborted. ")+s,0);
		//report.addItem(s,1);
		printf( "Catch: %s", s.c_str() );
	}
	
	//report.showReport(); 
	//s=report.getReport();
	//d->showReport(s,report.getLastRang());
} // end method
