/*
 *  verilog.cpp
 *  ICPD
 *
 *  Created by Adriel Mota Ziesemer Jr. on 6/26/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "verilog.h"

Verilog::Verilog(){}

Verilog::~Verilog(){}

bool Verilog::readFile(const string& nome, Circuit& netlist){
	vector<string> tokens;
	int a, b, instance=0;
	CellNetlst cell;
	parserFile(nome, tokens);
	int n=0;
	string modName="";
	while(n<tokens.size()){
		if (tokens[n] == "MODULE"){
			wires.clear();
			inouts.clear();
			inputs.clear();
			outputs.clear();
			parameters.clear();
			assigns.clear();
			cell.clear();
			n++;
			modName=tokens[n++];
			if(!compareChars(tokens[n], "(")) return false;
			while(tokens[n]!=")"){
				inouts.push_back(tokens[++n]);
				if(!compareChars(tokens[++n], ",", ")")) return false;
			}
			++n;
			if(!compareChars(tokens[n++], ";")) return false;
			while(n<tokens.size()){
				
				if(tokens[n] == "PARAMETER"){
					n++;
					if(!compareChars(tokens[n+1], "=")) return false;
					parameters[tokens[n]]=tokens[n+2];
					n+=3;
					if(!compareChars(tokens[n++], ";")) return false;
				}else if(tokens[n] == "WIRE"){
					if(tokens[n+1]=="["){
						readRange(tokens,++n,a,b);
						while(tokens[n++]!=";"){
							for(int i=b; i<=a; ++i){
								wires.push_back(tokens[n]+ "[" + to_string(i) + "]");
							}
							if(!compareChars(tokens[++n], ",", ";")) return false;
						}
					}else{
						while(tokens[n++]!=";"){
							inputs.push_back(tokens[n++]);
							if(!compareChars(tokens[n], ",", ";")) return false;
						}				
					}
					if(!inouts.size()) return compareChars(tokens[n], ";");
				}else if(tokens[n] == "INPUT"){
					if(tokens[n+1]=="["){
						readRange(tokens,++n,a,b);
						while(tokens[n++]!=";"){
							for(int i=b; i<=a; ++i){
								inputs.push_back(tokens[n]+ "[" + to_string(i) + "]");
							}
							if(!compareChars(tokens[++n], ",", ";")) return false;
						}
					}else{
						while(tokens[n++]!=";"){
							inputs.push_back(tokens[n++]);
							if(!compareChars(tokens[n], ",", ";")) return false;
						}				
					}
					if(!inouts.size()) return compareChars(tokens[n], ";");
				}else if(tokens[n] == "OUTPUT"){
					if(tokens[n+1]=="["){
						readRange(tokens,++n,a,b);
						while(tokens[n++]!=";"){
							for(int i=b; i<=a; ++i){
								outputs.push_back(tokens[n]+ "[" + to_string(i) + "]");
							}
							if(!compareChars(tokens[++n], ",", ";")) return false;
						}
					}else{
						while(tokens[n++]!=";"){
							inputs.push_back(tokens[n++]);
							if(!compareChars(tokens[n], ",", ";")) return false;
						}				
					}
					if(!inouts.size()) return compareChars(tokens[n], ";");
				}else if(tokens[n] == "ASSIGN"){
					string v1, v2;
					v1=tokens[++n];
					if(tokens[n+1]=="["){
						to_string(readRange(tokens,++n,a,b));
						v1+="["+to_string(a)+"]";				
					}
					if(!compareChars(tokens[++n], "=")) return false;
					v2=tokens[++n];
					if(tokens[n+1]=="["){
						to_string(readRange(tokens,++n,a,b));
						v2+="["+to_string(a)+"]";
					}
//					cout << v1 << "-" << v2 << endl;
					assigns[v1]=v2;
					++n;
					if(!compareChars(tokens[n++], ";")) return false;
				}else if(tokens[n] == "ENDMODULE"){
					string v2;
					cell.setName(modName);
					for(int c=0; c<inouts.size(); ++c){
						for(int d=0; d<inputs.size(); ++d){
							v2=inputs[d];
							if(inouts[c]==v2 || inouts[c]+"["==v2.substr(0,inouts[c].size()+1)){
								if(assigns.find(v2)!=assigns.end()) v2=assigns[v2];
								cell.insertInOut(v2);
							}
						}
						for(int d=0; d<outputs.size(); ++d){
							v2=outputs[d];
							if(inouts[c]==v2 || inouts[c]+"["==v2.substr(0,inouts[c].size()+1)){
								if(assigns.find(v2)!=assigns.end()) v2=assigns[v2];
								cell.insertInOut(v2);
							}
						}
					}
					cell.insertInOut(netlist.getVddNet());
					cell.insertInOut(netlist.getGndNet());
					netlist.insertCell(cell);
					n++;
					break;
				}else{
					string mod,v1, v2;
					int a,b, save_n;
					mod=tokens[n];
					v1=tokens[++n];
					if(tokens[n+1]=="["){
						readRange(tokens,++n,a,b);
						if(!compareChars(tokens[++n], "(")) return false;
						save_n=n;
						for(int x=b;x<=a;++x){
							n=save_n;
							vector<string> net_ids;
							while(tokens[n]!=")"){
								v2=tokens[++n];
								if(v2=="~"){
									v2=tokens[++n];
									vector<string> net_ids2;
									net_ids2.push_back(v2+"_NEG");
									net_ids2.push_back(v2);
									net_ids2.push_back(netlist.getVddNet());
									net_ids2.push_back(netlist.getGndNet());
									v2=net_ids2.front();
									wires.push_back(v2);
									cell.insertInstance("X"+to_string(instance++), "INV", net_ids2);
								}
								if(tokens[n+1]=="["){
									int c,d;
									to_string(readRange(tokens,++n,c,d));
									v2+="["+to_string(d+x)+"]";				
								}
								else{ //no brakets
									if(!findNet(v2))
										v2+="["+to_string(x)+"]";				
								}
								if(assigns.find(v2)!=assigns.end()) v2=assigns[v2];
								if(!findNet(v2))
                                    throw AstranError("Parameter " + v2 + " not initialized");
                                net_ids.push_back(v2);
								if(!compareChars(tokens[++n], ",", ")")) return false;
							}
							net_ids.push_back(netlist.getVddNet());
							net_ids.push_back(netlist.getGndNet());
							cell.insertInstance("X"+to_string(instance++), mod, net_ids);
						}
					}else{
						if(!compareChars(tokens[n], "(")) return false;
						vector<string> net_ids;
						while(tokens[n]!=")"){
							v2=tokens[++n];
							if(v2=="~"){
								v2=tokens[++n];
								vector<string> net_ids2;
								net_ids2.push_back(v2+"_NEG");
								net_ids2.push_back(v2);
								net_ids2.push_back(netlist.getVddNet());
								net_ids2.push_back(netlist.getGndNet());
								v2=net_ids2.front();
								wires.push_back(v2);								
								cell.insertInstance("X"+to_string(instance++), "INV", net_ids2);
							}
							if(tokens[n+1]=="["){
								int c,d;
								to_string(readRange(tokens,++n,c,d));
								v2+="["+to_string(c)+"]";				
							}
							if(assigns.find(v2)!=assigns.end()) v2=assigns[v2];
							if(!findNet(v2))
                                throw AstranError("Parameter " + v2 + " not initialized");
							net_ids.push_back(v2);
							if(!compareChars(tokens[++n], ",", ")")) return false;
						}
						net_ids.push_back(netlist.getVddNet());
						net_ids.push_back(netlist.getGndNet());
						cell.insertInstance("X"+to_string(instance++), mod, net_ids);
					}					
					n++;
					if(!compareChars(tokens[n++], ";")) return false;
				}
			}
			
		}
	}
    return true;
}

int Verilog::findNet(const string v){
	vector<string>::iterator tmp;

	for(tmp=inputs.begin(); tmp!=inputs.end(); ++tmp)
		if(*tmp==v) return true;
	for(tmp=outputs.begin(); tmp!=outputs.end(); ++tmp)
		if(*tmp==v) return true;
	for(tmp=wires.begin(); tmp!=wires.end(); ++tmp)
		if(*tmp==v) return true;
	return false;
}
	
int Verilog::getValue(const string v){
	if(parameters.find(v)!=parameters.end())
		return strToInt(parameters[v]);
	return strToInt(v);
}

bool Verilog::readRange(vector<string>& tokens, int& n,  int& a, int& b){
	if(!compareChars(tokens[n++], "[")) return false;
	a=0;
	while(tokens[n]!=":" && tokens[n]!="]"){
		switch(tokens[n][0]){
			case '+':
				a+=getValue(tokens[++n]);
				break;
			case '-':
				a-=getValue(tokens[++n]);
				break;
			default: 
				a=getValue(tokens[n]);
				break;
		}
		n++;
	}
	b=a;
	if(tokens[n]==":"){
		n++;
		b=0;
		while(tokens[n]!="]"){
			switch(tokens[n][0]){
				case '+':
					b+=getValue(tokens[++n]);
					break;
				case '-':
					b-=getValue(tokens[++n]);
					break;
				default: 
					b=getValue(tokens[n]);
					break;
			}
			n++;
		}
	}
	return true;	
}

bool Verilog::compareChars(const string a, const string b){
	if (a == b) return true;
	cout << "-> ERROR: It was expecting " << b << " but get " << a << endl;
	return false;
}

bool Verilog::compareChars(const string a, const string b, const string c){
	if (a == b | a == c) return true;
	cout << "-> ERROR: It was expecting " << b << " or " << c << " but get " << a << endl;
	return false;
}

int Verilog::parserFile(const string& filename, vector<string>& words){
	int fileline=0;
	bool comment=false;
	string str_tmp;
	words.clear();
	
	ifstream arq (filename.c_str());
	if (!arq.is_open()){
		cerr << "\t-> File " << filename << " could not be opened." << endl;
		return false;
	}	
	words.push_back("");
	while(getline(arq, str_tmp)) {
		if(words.back()!="") words.push_back("");
		fileline++;
		str_tmp=upcase(str_tmp);
		for(int n=0; n<str_tmp.size(); ++n){
			if(str_tmp[n]=='/' && n+1<str_tmp.size() && str_tmp[n+1]=='/'){ break;}
			else if(str_tmp[n]=='/' && n+1<str_tmp.size() && str_tmp[n+1]=='*'){ comment=true; ++n; continue;}
			else if(str_tmp[n]=='*' && n+1<str_tmp.size() && str_tmp[n+1]=='/'){ comment=false; ++n; continue;}
			
			if(!comment){
				switch(str_tmp[n]){
					case ',':
					case '~':
					case '(':
					case ')':
					case '[':
					case ':':
					case ']': 
					case '-': 
					case '+': 
					case '=': 
					case ';': 
						if(words.back()!="") words.push_back("");
						words.back()=str_tmp[n];
						words.push_back("");
						break;
					case ' ': 
					case '\t':
					case '\f':
					case '\r':
					case '\v':
						/*      case '\\': */
					case '\b':
						if(words.back()!="") words.push_back("");
						break;
					default:
						words.back()+=str_tmp[n];	
						break;
				}
			}
		}
	}
	arq.close();
	return static_cast<int>(words.size());
}
