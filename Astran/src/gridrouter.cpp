/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
 *   amziesemerj[at]inf.ufrgs.br                                           *
 **************************************************************************/

#include "gridrouter.h"

//Clear data structures
void GridRouter::clear(){
	for(int x=0; x<graph.size(); x++)
		graph[x].net=0;
    netlist.clear();
	graph.clear();
	costs.clear();
	blockageNet=0;
}

void GridRouter::reset(){
	graph.clear();
	graph.resize(sizeXY*sizeZ);
	for(map<int,t_nets>::iterator nets_it=netlist.begin(); nets_it!=netlist.end(); ++nets_it){
		nets_it->second.netTree.clear();
		nets_it->second.conflict=false;
		for(vector<int>::iterator netTree_it=nets_it->second.nodes.begin();netTree_it!=nets_it->second.nodes.end(); ++netTree_it){
			nets_it->second.netTree.push_back(*netTree_it);
			graph[*netTree_it].net=-nets_it->first;
			graph[*netTree_it].nrNets=1;
		}
	}
}

//Set the size of the grid 
void GridRouter::setSize(int sX, int sY, int sZ){
	sizeX=sX;
	sizeY=sY;
	sizeZ=sZ;
	sizeXY=sX*sY;
	graph.clear();
	graph.resize(sizeXY*sZ);
	costs.resize(3*sZ);
	direction[RT_EAST]=+1;
	direction[RT_WEST]=-1;
	direction[RT_NORTH]=+sizeX;
	direction[RT_SOUTH]=-sizeX;
	direction[RT_DOWN]=-sizeXY;
	direction[RT_UP]=+sizeXY;
}

//Add a new node to the net
bool GridRouter::addNodetoNet(int net, int node){
	if((net>0) & !graph[node].net){
		netlist[net].nodes.push_back(node);
		netlist[net].netTree.push_back(node);
		netlist[net].conflict=false;
		graph[node].net=-net;
		graph[node].nrNets=1;
		return true;
	}
	return false;
}

//Set the arc Costs
void GridRouter::setLayerCosts(int layer, int hCost, int vCost, int upCost){
	costs[layer*3]=hCost;
	costs[layer*3+1]=vCost;
	costs[layer*3+2]=upCost;
}

//Calculate manhattan distance between nodes
inline int GridRouter::calcDistance(int node1, int node2){
	//	return 0;
	return 5*(abs(getPosX(node1)-getPosX(node2))+abs(getPosY(node1)-getPosY(node2))+abs(getPosZ(node1)-getPosZ(node2)));
}


//Clear node congestion history
void GridRouter::clearHistory(){
   	for(int x=0; x<graph.size(); x++)
		graph[x].history=0;
}

//Astar Search
bool GridRouter::aStar(map<int,t_nets>::iterator& net, list<int>& targetNodes){	
	static t_tmp actualNode;
	int node;
	
	++trace_id;
	//Initialize priority queue PQ with the sourceNodes
	priority_queue<t_tmp> pq;
	
	actualNode.costAccumulated=0;
	actualNode.father=-1;
	for(list<int>::iterator nodes_it = net->second.netTree.begin(); nodes_it !=  net->second.netTree.end(); ++nodes_it){
		actualNode.aStarCost=getClosestNodeDistance(*nodes_it, targetNodes);
//		actualNode.costAccumulated=calcDistance(*nodes_it, net->second.nodes.front() )/arbFactor ;
		actualNode.node=*nodes_it;
		pq.push(actualNode);
		graph[*nodes_it].setFather(trace_id, -1);
	}
	//	int center=getCenter(net->second.nodes);
	
	//Loop until new sink is found
	while(!(pq.empty() || targetNodes.empty())){
		//Remove lowest cost node from PQ
		actualNode=pq.top();
		pq.pop();
		if(actualNode.father!=-1){
			if(graph[actualNode.node].isVisited(trace_id)) continue;
			graph[actualNode.node].setFather(trace_id, actualNode.father);
			if(getNet(actualNode.node) == net->first) break;
		}
		//		cout << ++visited << ": visitando (" << getPosX(actualNode.node) << " " <<getPosY(actualNode.node)<< " " <<getPosZ(actualNode.node)<< ") " << actualNode.costAccumulated << "+" << actualNode.aStarCost-actualNode.costAccumulated << "=" << actualNode.aStarCost << " vindo de (" << getPosX(actualNode.father) << " " <<getPosY(actualNode.father)<< " " <<getPosZ(actualNode.father)<< ")"<<endl;
		//Test the neighbours and add to queue the valids
		if(getPosX(actualNode.node)+1 < sizeX)
			pqAddto(actualNode,pq, net->first,targetNodes,RT_EAST);
		if(getPosY(actualNode.node)+1 < sizeY)
			pqAddto(actualNode,pq, net->first,targetNodes,RT_NORTH);	
		if(getPosX(actualNode.node)) 
			pqAddto(actualNode,pq, net->first,targetNodes,RT_WEST);
		if(getPosY(actualNode.node))
			pqAddto(actualNode,pq, net->first,targetNodes,RT_SOUTH);
		if(getPosZ(actualNode.node))
			pqAddto(actualNode,pq, net->first,targetNodes,RT_DOWN);
		if(getPosZ(actualNode.node)+1 < sizeZ)
			pqAddto(actualNode,pq, net->first,targetNodes,RT_UP);	
	}
	//	cout << ++visited << ": visitando (" << getPosX(actualNode.node) << " " <<getPosY(actualNode.node)<< " " <<getPosZ(actualNode.node)<< ") " << actualNode.costAccumulated << "+" << actualNode.aStarCost-actualNode.costAccumulated << "=" << actualNode.aStarCost << " vindo de (" << getPosX(actualNode.father) << " " <<getPosY(actualNode.father)<< " " <<getPosZ(actualNode.father)<< ")"<<endl;
	
	//Trace the path back to the source node
	if(getNet(actualNode.node)==net->first && actualNode.father!=-1){
		node = actualNode.node;
		targetNodes.remove(node);
		while (graph[node].getFather(trace_id)!=-1) {
			//Put these new nodes in the Routing Tree
			net->second.netTree.push_back(node); 
			net->second.routeResult.push_back(node);
			if(!isSource(node)){
				if(graph[node].nrNets){
					bool &tmp=netlist[graph[node].net].conflict;
					if(!tmp){
						tmp=true;
						++conflicts;
					}
					net->second.conflict=true;
				}
				graph[node].net=net->first;
				++graph[node].nrNets;
			}
			node = graph[node].getFather(trace_id);
		}
		net->second.routeResult.push_back(node);
		net->second.routeResult.push_back(-1);
		return true;
	}
	return false;
}

void GridRouter::routeNets(int nrAttempts){
	list<int> targetNodes;
	map<int,t_nets>::iterator nets_it;
	list<int>::iterator netTree_it;
	vector<int>::iterator netTree_it2;
	vector<int>::iterator nodes_it;
	
	//Initializations
	clock_t start=clock(); 
	actualAttempt=0;
	trace_id=0;
	conflicts=0;
	visited=0;
	netlist.erase(blockageNet);
	for(nets_it=netlist.begin(); nets_it!=netlist.end(); ++nets_it){
		//cout << (nets_it->second).nodes[0] << endl;
		if(rand()%2==0) getCenter((nets_it->second).nodes);	
		//cout << (nets_it->second).nodes[0] << endl;
	}	
	//GridRouter Routing
	cout << "-> Routing graph..." << endl;
	do{		
		// Loop over all nets
		for(nets_it=netlist.begin(); nets_it!=netlist.end(); ++nets_it){
			if(nets_it->second.conflict){
				//If conflict exists, check it out and do rip-upand re-route. Otherwise jump net.
				--conflicts;
				nets_it->second.conflict=false;
				for(netTree_it=nets_it->second.netTree.begin();netTree_it!=nets_it->second.netTree.end(); ++netTree_it){
					if(graph[*netTree_it].nrNets>1){
						nets_it->second.conflict=true;
						break;
					}
				}
			}
			if(nets_it->second.conflict | !actualAttempt){
				//Rip-up net 
				for(netTree_it=(nets_it->second).netTree.begin(); netTree_it!=(nets_it->second).netTree.end(); ++netTree_it){
					if(!isSource(*netTree_it)){
						--graph[*netTree_it].nrNets;
						if(getNet(*netTree_it)==nets_it->first)  graph[*netTree_it].net=0; 
					}
				}
				//Clear netTree	of the net
				(nets_it->second).netTree.clear();
				(nets_it->second).routeResult.clear();
				
				//Create list of target nodes 
				targetNodes.clear();
				for(nodes_it=(nets_it->second).nodes.begin();nodes_it!=(nets_it->second).nodes.end(); ++nodes_it){
					targetNodes.push_back(*nodes_it);
					//cout << *nodes_it << endl;
				}		
				//Insert the first node to the tree
				(nets_it->second).netTree.push_back(*targetNodes.begin());
				targetNodes.erase(targetNodes.begin());
				//Loop until all sinks have been found
				while(!targetNodes.empty() && aStar(nets_it, targetNodes)){}  //Route the tree to the closest node in the graph
				
				if(!targetNodes.empty()) break;
				if(nets_it->second.conflict) ++conflicts;
			}
		}
		//update H cost
		++trace_id;
		for(nets_it=netlist.begin(); nets_it!=netlist.end(); ++nets_it){
			if(nets_it->second.conflict){
				for(netTree_it=nets_it->second.netTree.begin();netTree_it!=nets_it->second.netTree.end(); ++netTree_it){
					if(graph[*netTree_it].nrNets>1 && graph[*netTree_it].idNr!=trace_id){
						++graph[*netTree_it].history;
						graph[*netTree_it].idNr=trace_id;
					}
				}
			}
		}
		
		if(!(++actualAttempt%15)) cerr << "." << conflicts << ".";				
		//		cout << conflicts << endl;
	}while(targetNodes.empty() && conflicts && actualAttempt<nrAttempts);
	//	cout << visited << endl;
		cout << endl << "-> Runtime = " << static_cast<float>((clock()-start)/(CLOCKS_PER_SEC/1000))/1000 << "s" << endl;
	if(!targetNodes.empty()) cout << "-> Impossible to route net: " << nets_it->first << endl;		
	if(conflicts || !targetNodes.empty()){
		cout <<"-> Unable to route the circuit after ";
		cout << actualAttempt << " attempts."<< endl;
	}else{
		cout <<"-> Routing finished in ";
		cout << actualAttempt << " attempts."<< endl;
		showResult();
	}
	if(!targetNodes.empty() || conflicts)
        throw AstranError("Could not finish routing");
}


rt_dir GridRouter::dirFromPos(int antPos, int pos){
	if(getDir(antPos,RT_WEST)==pos) return RT_WEST;
	else if(getDir(antPos,RT_EAST)==pos) return RT_EAST;
	else if(getDir(antPos,RT_SOUTH)==pos) return RT_SOUTH;
	else if(getDir(antPos,RT_NORTH)==pos) return RT_NORTH;
	else if(getDir(antPos,RT_UP)==pos) return RT_UP;
	else if(getDir(antPos,RT_DOWN)==pos) return RT_DOWN;
    else return RT_INVALID;
}

void GridRouter::optimize(){
}

void GridRouter::pqAddto(t_tmp& actualNode, priority_queue<t_tmp>& pq, const int& targetNet, list<int>& targetNodes, rt_dir direction){
	t_tmp tmp;
	int lCost, nextNode;
	list<int>::iterator target_it;
	
	nextNode=getDir(actualNode.node, direction);
	if(!(graph[nextNode].isVisited(trace_id) || (isSource(nextNode) && graph[nextNode].net!=-targetNet))){
		tmp.node=nextNode;
		tmp.father=actualNode.node;
		tmp.costAccumulated=actualNode.costAccumulated+calcCost(nextNode, direction);
		lCost=calcDistance(nextNode, targetNodes.front());
		for(target_it=++targetNodes.begin(); target_it!=targetNodes.end(); ++target_it)
			lCost=min(lCost,calcDistance(nextNode, *target_it));
		tmp.aStarCost=tmp.costAccumulated+lCost;
		//		cout << "(" << getPosX(actualNode.node) << " " <<getPosY(actualNode.node)<< " " <<getPosZ(actualNode.node)<< ")  --(" << tmp.aStarCost <<")-->  (" << getPosX(tempNode.node) << " " <<getPosY(tempNode.node)<< " " <<getPosZ(tempNode.node)<< ") " << endl;
		pq.push(tmp);
	}	
}
//Put the closest to center node in the vector front
int GridRouter::getCenter(vector<int>& Nodes){
	int x=0,y=0,z=0;
	int centerNode,temp,closestPos;
	vector<int>::iterator target_it;
	for(target_it=Nodes.begin(); target_it!=Nodes.end(); ++target_it){
		x+=getPosX(*target_it);
		y+=getPosY(*target_it);
		z+=getPosZ(*target_it);
	}
	centerNode=getPos(round(x/Nodes.size()),round(y/Nodes.size()), round(z/Nodes.size()));
	//cout << "CenterNode: " << centerNode << endl;
	closestPos=getClosestNodePos(centerNode,Nodes);
	temp=Nodes[0];
	Nodes[0]=Nodes[closestPos];
	Nodes[closestPos]=temp;
	return 0;
}

list<int>::iterator GridRouter::getClosestNode(int node,list<int>& targetNodes){
	int lCost=calcDistance(node, targetNodes.front());
    int tempCost;
    list<int>::iterator closestNode=targetNodes.begin();
    for(list<int>::iterator target_it=targetNodes.begin();target_it!=targetNodes.end();target_it++){
		tempCost=calcDistance(node, *target_it);
		if(lCost>tempCost){
			closestNode=target_it;
			lCost=tempCost;
		}	
    }	
    return closestNode;
}	

int GridRouter::getClosestNodePos(int node,vector<int>& targetNodes){
	int lCost=calcDistance(node, targetNodes[0]);
	int closestNodePos=0;
	for(int pos=0;pos<targetNodes.size();pos++){
		//cout << "CALC: " << calcDistance(node, targetNodes[pos]) << endl;
		if(lCost>calcDistance(node, targetNodes[pos])){
			closestNodePos=pos;
			lCost=calcDistance(node, targetNodes[pos]);
		}	
	}	
	return closestNodePos;
}	

int GridRouter::getClosestNodeDistance(int& node, list<int>& targetNodes){
	int lCost;
	list<int>::iterator target_it;
	lCost=calcDistance(node, targetNodes.front());
	for(target_it=++targetNodes.begin(); target_it!=targetNodes.end(); ++target_it)
		lCost=min(lCost,calcDistance(node, *target_it));
	return lCost;
}

inline int GridRouter::calcCost(int node2, rt_dir dir){
	int tmp = (actualAttempt ? graph[node2].history*(graph[node2].nrNets+1) : 0);
	switch (dir){
		case RT_WEST: return costs[getPosZ(node2)*3] + tmp; break;
		case RT_SOUTH: return costs[getPosZ(node2)*3+1] + tmp; break;
		case RT_DOWN: return costs[(getPosZ(node2)+1)*3+2] + tmp; break;
		case RT_EAST: return costs[getPosZ(node2)*3] + tmp; break;
		case RT_NORTH: return costs[getPosZ(node2)*3+1] + tmp; break;
		case RT_UP: return costs[(getPosZ(node2)-1)*3+2] + tmp;  break;
        default: assert(false);
    }
	return 0;
}

void GridRouter::showResult(){
	int finalCost=0,wlCost=0,pins=0;
	rt_dir UP=RT_UP;
	for(map<int,t_nets>::iterator nets_it=netlist.begin(); nets_it!=netlist.end();++nets_it){
		for(list<int>::iterator nodes_it=nets_it->second.netTree.begin(); nodes_it!=nets_it->second.netTree.end();++nodes_it){
			if(areConnected(*nodes_it, RT_EAST)){
				++wlCost;
				finalCost+=costs[getPosZ(*nodes_it)*3];	
				//cout << *nodes_it << " -> " << (*nodes_it)+1 << endl; 
			}
			if(areConnected(*nodes_it, RT_NORTH)){
				++wlCost;
				finalCost+=costs[getPosZ(*nodes_it)*3+1];
				//cout << *nodes_it << " -> " << (*nodes_it)+sizeX << endl;
			}
			if(areConnected(*nodes_it, RT_UP)){
				if(areConnected(getDir(*nodes_it, UP), RT_UP)) 
					++pins;
				else pins+=2;  
				++wlCost;
				finalCost+=costs[getPosZ(*nodes_it)*3+2];
				//cout << *nodes_it << " -> " << (*nodes_it)+sizeXY << endl;
			}
			//			cout << "(" << getPosX(*nodes_it) << " " <<getPosY(*nodes_it)<< " " <<getPosZ(*nodes_it)<< ")  --("<< graph[*nodes_it].net << " " << nets_it->second.finalNet <<")-->  (" << graph[*nodes_it].rightIter << " " <<graph[*nodes_it].upIter<< " " <<graph[*nodes_it].upLayerIter << ") " << endl;
		}
	}
	cout << "-> Cost =" << finalCost << endl;
	cout << "-> Wirelength =" << wlCost << endl;
	cout << "-> # Pins =" << pins << endl;
}

bool GridRouter::areConnected(int node1, rt_dir dir){
	switch(dir){
		case RT_WEST:	return getPosX(node1)>0 && getNet(node1-1)==getNet(node1) && getNet(node1) && getNet(node1)!=blockageNet;
		case RT_SOUTH:	return getPosY(node1)>0 && getNet(node1-sizeX)==getNet(node1) && getNet(node1) && getNet(node1)!=blockageNet;
		case RT_DOWN:	return getPosZ(node1)>0 && getNet(node1-sizeXY)==getNet(node1) && getNet(node1) && getNet(node1)!=blockageNet;
		case RT_EAST:	return getPosX(node1)+1<sizeX && getNet(node1+1)==getNet(node1) && getNet(node1) && getNet(node1)!=blockageNet;
		case RT_NORTH:	return getPosY(node1)+1<sizeY && getNet(node1+sizeX)==getNet(node1) && getNet(node1) && getNet(node1)!=blockageNet;
		case RT_UP:		return getPosZ(node1)+1<sizeZ && getNet(node1+sizeXY)==getNet(node1) && getNet(node1) && getNet(node1)!=blockageNet;
        default: assert(false);
	}
	return false;
}

bool GridRouter::connect(int net, int n1, int n2){		
	//cout << n1 << ", " << getNet(n1) << endl;
	//cout << n2 << ", " << getNet(n2) << endl;
    if(!getNet(n1)){
        //cout << "(" << n1 << ", ";
		graph[n1].net=net;
		graph[n1].nrNets++;
		netlist[net].netTree.push_front(n1);
	}		
	if(!getNet(n2)){
        //cout <<  n2 << ")" << endl;
		graph[n2].net=net;
		graph[n2].nrNets++;
		netlist[net].netTree.push_front(n2);
	}  
	return true; //TODO: TEST TO RETURN FALSE
}		

//Write to a designated file the routing result
void GridRouter::pathTree(int net, ofstream& froute){	
	int antPos,pos,desloc;
	rt_dir dir;
	int r=0;
	
	if(netlist[net].routeResult.size()>0){
		froute << "Net " << net << "\t";
		froute << "( " << getPosX(netlist[net].routeResult[(r)]) << " "	<< getPosY(netlist[net].routeResult[(r)]) << " " << getPosZ(netlist[net].routeResult[(r)]) << " ) ";
		desloc=1;
		r=1;
		while(r<(netlist[net].routeResult.size()-1)){
			if(netlist[net].routeResult[r]==-1){
				froute << " ( " << getPosX(netlist[net].routeResult[(r+1)]) << " "	<< getPosY(netlist[net].routeResult[(r+1)]) << " " << getPosZ(netlist[net].routeResult[(r+1)]) << " ) ";
				r+=2;
				desloc=1;
			}
			else {
				antPos=netlist[net].routeResult[r-1];
				pos=netlist[net].routeResult[r];
				dir=dirFromPos(antPos,pos);
				if(netlist[net].routeResult[r+1]==-1 || !(getDir(pos,dir)==netlist[net].routeResult[r+1])){
					switch (dir){
						case RT_WEST:
							froute << "[ " << desloc <<  " WEST ] ";
							break; 
						case RT_EAST:
							froute << "[ " << desloc <<  " EAST ] ";
							break;
						case RT_SOUTH:	
							froute << "[ " << desloc <<  " SOUTH ] ";
							break;
						case RT_NORTH:
							froute << "[ " << desloc <<  " NORTH ] ";
							break;
						case RT_DOWN:
							froute << "[ " << desloc <<  " DOWN ] ";
							break;
						case RT_UP:
							froute << "[ " << desloc <<  " UP ] ";
							break;						
                        default: assert(false);
					}
					desloc=1;	
				}
				else desloc++;
				r++;
			}	
		}
		froute << endl;
	}
}
