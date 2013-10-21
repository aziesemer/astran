// Adapted from Juergen Thies - LayoutEditor (layout@juergenthies.de)

#ifndef BINARY_FILE_READER_H
#define BINARY_FILE_READER_H

#include <fstream>
using std::ifstream;
#include <string>
using std::string;

#include "Types.h"

#define bufferSize 256000

class BinaryFileReader {
private:
	bool noswap;
	bool lastBlock;
	char buffer[bufferSize+1];
	int bufferPos,bufferRead;

	ifstream clsFile;

	void ReadBuffer();

public:
	BinaryFileReader(){}
    BinaryFileReader( const string &filename );
    
    ~BinaryFileReader();
    bool Open( const string &filename );
	void Close() { clsFile.close(); }
    bool end;

    int16  ReadInt16();
    uint16 ReadUInt16();
    uint8  ReadUInt8();
    int8   ReadInt8();
    int32  ReadInt32();
	
	string ReadString( int items );
	double Read8ByteReal();
};

#endif
