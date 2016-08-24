/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr., Cristiano Lazzari     *
 *   (amziesemerj,clazz)@inf.ufrgs.br                                      *
 ***************************************************************************/
#pragma once

/**
 Route using a Pathfinder based algorithm
 Detailed router
 @author Adriel Mota Ziesemer Jr.
 */

#include <vector>
#include <list>
#include <queue>
#include <map>
#include <iostream>
#include <strstream>
#include <fstream>
#include <time.h>
#include <math.h>
#include <stdlib.h>
#include "util.h"

enum rt_dir {RT_EAST, RT_WEST, RT_NORTH, RT_SOUTH, RT_DOWN, RT_UP, RT_INVALID};

class GridRouter{
protected:
	class Node{
	public:
		int net;
		int nrNets;
		int history;
		int father;
		int idNr;
		
		void setFather(int i, int f){father=f;idNr=i;};
		int getFather(int i){ return idNr==i ? father :  0; };
		bool isVisited(int i){return idNr==i;};
		Node():net(0), nrNets(0), history(0), idNr(0){};
	};
	
	//Keep trace of the path in the bfs algorithm 
	struct t_tmp{
		int node;
		int father;
		int aStarCost;
		int costAccumulated;
		bool operator< (const t_tmp& x) const { return (x.aStarCost < aStarCost) | ((x.aStarCost == aStarCost) & (x.aStarCost-x.costAccumulated < aStarCost-costAccumulated)) ;};
		//		bool operator< (const t_tmp& x) const { return x.aStarCost < aStarCost;};
	};	
	
	//List of the Nets in the graph
	struct t_nets{
		vector<int> nodes;
		list<int> netTree;
		vector<int> routeResult;
		bool conflict;
	};

	int direction[6];
	vector<int> costs;
	vector<Node> graph;
	map<int,t_nets> netlist;
	int sizeX, sizeY, sizeZ, sizeXY;
	int visited;
	int actualAttempt;
	int trace_id;
	int conflicts;
	int blockageNet;
public:
	GridRouter(){};
	~GridRouter(){};
	
	int getDir(const int& node, const rt_dir& dir){return node+direction[dir];};
	int getDirN(int& node, rt_dir dir){return node+direction[dir];};
	void setBlockageNet(int net){blockageNet=net;};
	bool addNodetoNet(int net, int node);	
	int getNet(int node){return abs(graph[node].net);};
	int getNrNets(int node){return graph[node].nrNets;};
	vector<int>* getNetNodes(int net){return &netlist[net].nodes;};
	void setLayerCosts(int layer, int hCost, int vCost, int upCost);
	void routeNets(int nrAttempts);
	bool isSource(int x){ return graph[x].net<0;};
	void optimize();
	bool aStar(map<int,t_nets>::iterator& net, list<int>& targetNodes);
	void showResult();
	bool areConnected(int node1, rt_dir dir);
	bool connect(int net,int n1, int n2);
	int getSizeX(){return sizeX;};
	int getSizeY(){return sizeY;};
	int getSizeZ(){return sizeZ;};
	void setSize(int sX, int sY, int sZ);
	int getPos(int x, int y, int z){return (z*sizeXY)+(y*sizeX)+x;};
	int getPosX(int& pos){return (pos%sizeXY)%sizeX;};
	int getPosY(int& pos){return (pos%sizeXY)/sizeX;};
	int getPosZ(int& pos){return pos/sizeXY;};	
	int getCenter(vector<int>& Nodes);
	void clear();
	void reset();
	int calcDistance(int node1, int node2);
	inline int calcCost(int node, rt_dir direction);
	list<int>::iterator getClosestNode(int node,list<int>& targetNodes);
	int getClosestNodeDistance(int& node, list<int>& targetNodes);
	int getClosestNodePos(int nodePos,vector<int>& targetNodes);
	void pqAddto(t_tmp& actualNode, priority_queue<t_tmp>& pq, const int& targetNet, list<int>& targetNodes, rt_dir direction);
	void pathTree(int net, ofstream& froute);
	void clearHistory();
	unsigned long graphSize(){ return graph.size();};
	unsigned long netlistSize(){ return netlist.size();};
	int netTreeFirst(int net) { return *(netlist[net].netTree.begin());};
	rt_dir dirFromPos(int antPos, int pos);
};