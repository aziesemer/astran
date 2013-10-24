#include <cmath>

#include "BinaryFileReader.h"

BinaryFileReader::BinaryFileReader( const string &filename)
{
	Open( filename );
}

// -----------------------------------------------------------------------------

BinaryFileReader::~BinaryFileReader() {
}
// -----------------------------------------------------------------------------

bool BinaryFileReader::Open( const string &filename ){
	clsFile.open( filename.c_str() );

	end = clsFile.eof();

	bufferRead=0;
	bufferPos=0;
	noswap = false; //QSysInfo::ByteOrder == QSysInfo::BigEndian;

	lastBlock=false;
	return end;
} // end method

// -----------------------------------------------------------------------------

void BinaryFileReader::ReadBuffer(){
	int newPos=0;
	for (newPos=0;bufferPos<bufferRead;newPos++){
	   buffer[newPos]=buffer[bufferPos];
	   bufferPos++;	
	 }
	clsFile.read((char *)&buffer[newPos],bufferSize-newPos);
	int i=clsFile.gcount();
	bufferRead=i+newPos;
	if (i<(bufferSize-newPos)) lastBlock=true;
	bufferPos=0;
} // end method

// -----------------------------------------------------------------------------

int32 BinaryFileReader::ReadInt32(){
bufferPos+=4;
if (bufferRead<bufferPos) {
	bufferPos-=4;
	ReadBuffer();
	bufferPos+=4;
	}
if (lastBlock) if (bufferPos>=bufferRead) end=true;
if (noswap) {
	int32 *i;
	i=(int32 *)&buffer[bufferPos-4];
        return *i;
    } else {
	int32 i;
        register uint8 *p = (uint8 *)(&i);
            *p++ = buffer[bufferPos-1];;
	    *p++ = buffer[bufferPos-2];;
	    *p++ = buffer[bufferPos-3];;
            *p = buffer[bufferPos-4];
	return i;
    }
}

// -----------------------------------------------------------------------------

int16 BinaryFileReader::ReadInt16(){
bufferPos+=2;
//printf("bufferPos %d %d\n",bufferPos,bufferRead);
if (bufferRead<bufferPos) {
	bufferPos-=2;
	ReadBuffer();
	bufferPos+=2;
	}
if (lastBlock) if (bufferPos>=bufferRead) end=true;
if (noswap) {
	int16 *i;
	i=(int16 *)&buffer[bufferPos-2];
        return *i;
    } else {
	int16 i;
        register uint8 *p = (uint8 *)(&i);
            *p++ = buffer[bufferPos-1];;
            *p = buffer[bufferPos-2];
	return i;
    }
}

// -----------------------------------------------------------------------------

uint16 BinaryFileReader::ReadUInt16(){
bufferPos+=2;
//printf("bufferPos %d %d\n",bufferPos,bufferRead);
if (bufferRead<bufferPos) {
	bufferPos-=2;
	ReadBuffer();
	bufferPos+=2;
	}
if (lastBlock) if (bufferPos>=bufferRead) end=true;
if (noswap) {
	uint16 *i;
	i=(uint16 *)&buffer[bufferPos-2];
        return *i;
    } else {
	uint16 i;
        register uint8 *p = (uint8 *)(&i);
            *p++ = buffer[bufferPos-1];;
            *p = buffer[bufferPos-2];
	return i;
    }
}

// -----------------------------------------------------------------------------

uint8 BinaryFileReader::ReadUInt8(){
uint8 *i;
i=(uint8 *)&buffer[bufferPos];
bufferPos++;
//printf("bufferPos %d %d\n",bufferPos,bufferRead);
if (bufferRead<bufferPos) {
	bufferPos--;
	ReadBuffer();
	i=(uint8 *)&buffer[bufferPos];
	bufferPos++;
	}
if (lastBlock) if (bufferPos>=bufferRead) end=true;
//printf("uint8 %d \n",*i);
return *i;
}

// -----------------------------------------------------------------------------

int8 BinaryFileReader::ReadInt8(){
int8 *i;
i=(int8 *)&buffer[bufferPos];
bufferPos++;
//printf("bufferPos %d %d\n",bufferPos,bufferRead);
if (bufferRead<bufferPos) {
	bufferPos--;
	ReadBuffer();
	i=(int8 *)&buffer[bufferPos];
	bufferPos++;
	}
if (lastBlock) if (bufferPos>=bufferRead) end=true;
return *i;
}

// -----------------------------------------------------------------------------

double BinaryFileReader::Read8ByteReal(){
  int exp ; // help 
  int sig ; // help
  int i;
  uint8 help;   //help
  double double_;
  help=ReadUInt8();
  exp = help & 0x7f;
  sig = help & 0x80 ? -1 : 1;
  double_=0;
  for (i=0; i<7; i++) {
  	help=ReadUInt8();
	double_=double_*256+help; }
  double_ = sig * (double)double_ * pow((double)(16.0),(exp-64))/256/256/256/256/256/256/256;
  return double_;
} // end method

// -----------------------------------------------------------------------------

string BinaryFileReader::ReadString (int items){
  string s,s1;
  uint8 help;
  int i;
  s1.resize( items );
  for (i=0;i<items;i++){
  	help=ReadUInt8();
	if (help!=0){
		s1[i]=help;}
	}
  return s1;
} // end method
