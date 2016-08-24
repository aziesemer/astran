/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Cristiano Lazzari     *
 *   (amziesemerj,clazz)@inf.ufrgs.br                                      *
 ***************************************************************************/
#pragma once

/**
 Route the graph using a Pathfinder based algorithm
 Intracell router
 @author Adriel Mota Ziesemer Jr.
 */

#include <vector>
#include <list>
#include <queue>
#include <map>
#include <iostream>
#include <strstream>
#include <time.h>
#include "util.h"

class GraphRouter{
protected:
    //Adjacency list
    struct t_arc{
        unsigned int cost;
        int label;
        unsigned int node1;
        unsigned int node2;
        int lockNet;
    };
    
    class Node{
	public:
		list<t_arc*> arcs;
		int net;
		unsigned int nrNets;
		unsigned int history;
		int source; // 1=source 2=Steiner point
		Node():net(-1), nrNets(0), history(0), source(false){};
		~Node(){};
		bool verifyConflict(){return(nrNets>1);};
		void clearNets(){net=-1; nrNets=0;};
	};
	struct t_trace{
		int father;
		t_arc* link;
		bool visited;
		void clear(){father=-1;visited=false;};
		void setFather(int f, t_arc* l){link=l; father=f; visited=true;};
	};
	//Keep trace of the path in the bfs algorithm 
	struct t_tmp{
		unsigned int node;
		int father;
		t_arc* link;
		unsigned int costAccumulated;
		bool operator< (const t_tmp& x) const { return costAccumulated > x.costAccumulated;};
	};	
	//List of the Nets in the graph
	struct t_nets{
		list<unsigned int> nodes;
		list<unsigned int> netTree;
		bool conflict;
	};
	
	map<int,int> solvedNets;		
	map<unsigned int,t_nets> netlist;
	vector<Node> graph;
	list<t_arc> arcs;
	unsigned int finalNet;
	
public:
	GraphRouter();
	~GraphRouter();
	int prim(list<unsigned int> tmp_netnodes,unsigned int actualAttempt,unsigned int net);
	int addSteiner(unsigned int actualAttempt,unsigned int net);
	unsigned int createNode();
	void addNodetoNet(unsigned int net, unsigned int node);	
	int getNet(unsigned int node){return graph[node].net;};
	int getNrNets(unsigned int node){return graph[node].nrNets;};
	list<unsigned int>* getNetNodes(unsigned int net){return &netlist[net].nodes;};
	void addArc(unsigned int node1, unsigned int node2, unsigned int cost);
	void remArcs(unsigned int node);
	bool routeNets(unsigned int nrAttempts);
	bool optimize();
	int bfsRoute(list<unsigned int>& sourceNodes, const unsigned int targetNet, const unsigned int label, const bool checkConflict, const unsigned int maxCost, unsigned int actualAttempt,bool definitive);
	int bfsRoute2(list<unsigned int>& sourceNodes, const unsigned int targetNet, const unsigned int label, const bool checkConflict, const unsigned int maxCost, unsigned int actualAttempt,bool definitive);
	
	unsigned int calcCost(const t_arc* arc, const unsigned int targetNode, const unsigned int targetNet, unsigned int actualAttempt);
	void showResult();
	bool areConnected(unsigned int n1, unsigned int n2);
	int areConnected2(unsigned int n1, unsigned int n2);
	bool connect(int net, unsigned int n1, unsigned int n2);
	void clearHistory();
	int getNrAttempts(){return finalNet;};
	int getCost();
	int getArcCost(int a, int b);
	int setArcCost(int n1, int n2, int cost);
	unsigned int createGraph(int size);
	int getNrFinalArcs(int n);
	bool lockArc(int n1, int n2, int net);
	bool isSource(unsigned int x);
	void clear();
	void reset();
	
};
