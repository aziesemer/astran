/*
 *  util.h
 *  Jungle Parrot
 *
 *  Created by Adriel Mota Ziesemer Jr on 1/12/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#pragma once

#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <wx/textctrl.h>
#include <wx/wx.h>
#include <wx/process.h>

#define printme( e ) { cerr << #e << " = " << ( e ) << "\n"; };
#define VERSION "3.20"

enum IOType {IOTYPE_INPUT, IOTYPE_OUTPUT, IOTYPE_INOUT};
enum HorV {V, H};

using namespace std;

inline void printHeader (ostream& target, string begin, string end) {
	time_t timer = time(NULL);
	string date_time = asctime(localtime(&timer));
	date_time=date_time.substr(0,date_time.size()-1);
	target << begin << "ASTRAN - IC Physical Design Tool v" << VERSION << " - " << date_time << end << endl;
	target << begin << "--------------------------------------------------------" << end << endl;
	target << begin << "UFRGS - Universidade Federal do Rio Grande do Sul" << end << endl;
	target << begin << "GME - Microelectronics Group" << end << endl;
	target << begin << "WEBPAGE - http://aziesemer.github.io/astran/" << endl << endl;
	target << begin << "Original Author" << end << endl;
	target << begin << " - Adriel Ziesemer <amziesemerj[at]inf.ufrgs.br>" << end << endl;
	target << begin << "Project Advisor" << end << endl;
	target << begin << " - Ricardo Reis <reis[at]inf.ufrgs.br>" << end << endl;	
	target << begin << "Contributors:" << end << endl;
	target << begin << " - Leandro Heck, Gustavo Smaniotto, Charles Capella Leonhardt, Felipe Avila Nesello, Cristiano Lazzari, Glauco Santos, Guilherme Flach, Gracieli Posser, Renato Hentschke" << end << endl << endl;
}

inline bool isNumber(string number){
	istringstream test_num(number.c_str());
	double value;
	test_num >> value;
    return !test_num.fail();
}

inline bool fileExist(string filename){
	ifstream file(filename.c_str());
    return file.good();
}

inline string upcase(string s){
	transform(s.begin(), s.end(), s.begin(), (int(*)(int)) toupper);
	return s;
}

inline int strToInt( const string &s){
	istringstream myStream(s);
	int i;
	myStream>>i;
	return i;
}

inline string removeExt(const string& fileName){
	return fileName.substr(0,fileName.find_last_of("."));
}

inline string getFileName(const string& fileName){
	return fileName.substr(fileName.find_last_of("/\\")+1);
}

inline string getExt(const string& fileName){
	return fileName.substr(fileName.find_last_of(".")+1);
}

inline string getPath(const string& fileName){
	return fileName.substr(0,fileName.find_last_of("/\\")+1);
}

inline void icpdExecute(const string& exec_cmd){
#if defined (_MSC_VER)
	wxExecute(exec_cmd);
#else
	wxProcess* process = new wxProcess();
	process->Open(wxString::From8BitData(exec_cmd.c_str()));
#endif
}

static map<string, string> config;

class Redirector : public streambuf {
private:
	streambuf *clsOriginalStreamBuffer;
	ostream * clsOutStream;
	wxTextCtrl *clsTextBox;
	bool clsFlush;
	
public:
	Redirector( wxTextCtrl *textbox, ostream &out, bool flush ) {
		clsFlush=flush;
		clsOutStream = &out;
		clsOriginalStreamBuffer = out.rdbuf();
		clsTextBox = textbox;
		clsOutStream->rdbuf( this );
	} // end constructor
	
	~Redirector() {
		clsOutStream->rdbuf( clsOriginalStreamBuffer );
	} // end destructor
	
	int overflow( int ch ) {
		clsTextBox->WriteText( wxChar( ch ) );
		if(clsFlush || ch=='\n') {
			clsTextBox->Update();
			clsTextBox->SetInsertionPointEnd();			
		}
		return ch;
	} // end overloaed method
}; // end class

class AstranError{
    string message;
public:
    AstranError(string msg){
        this->message=msg;
    }
    string what(){
        return message;
    }
};

#if defined(_MSC_VER)
inline float round( const float f ) {
    return floor( f + 0.5f );
} // end function

#if _MSC_VER == 1200
template< typename T > inline T min( const T &a, const T &b )
{ return ( a < b ) ? a : b; }

template< typename T > inline T max( const T &a, const T &b )
{ return ( a > b ) ? a : b; }

#define for if( false ); else for // solves the variable redifinition problem on VC6
#else
#include <algorithm>
using std::max;
using std::min;
#endif
#else
#define _popen popen
#define _pclose pclose
#endif