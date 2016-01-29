/**************************************************************************
 *   Copyright (C) 2005 by Adriel Mota Ziesemer Jr.                        *
 *   amziesemerj[at]inf.ufrgs.br                                           *
 **************************************************************************/
#include "graphrouter.h"

GraphRouter::GraphRouter(){}

GraphRouter::~GraphRouter(){}

//Create a new node in the graph
unsigned int GraphRouter::createNode(){
	static Node tmp;
	graph.push_back(tmp);
	return(static_cast<int>(graph.size()-1));
};

//Clear data structures
void GraphRouter::clear(){
	netlist.clear();
	graph.clear();
	arcs.clear();
}

//Create a graph of a given size
unsigned int GraphRouter::createGraph(int size){
	graph.resize(size);
	return 1;
};

//Add a new node to the net
void GraphRouter::addNodetoNet(unsigned int net, unsigned int node){
	if(graph[node].net==-1){ 
		netlist[net].nodes.push_back(node);
		graph[node].net=net;
		graph[node].nrNets=0;
		graph[node].source=true;
	}
};

//Connect 2 nodes and give a cost to it
void GraphRouter::addArc(unsigned int node1, unsigned int node2, unsigned int cost){
	t_arc tmp;
	tmp.cost=cost;
	tmp.node1=node1;
	tmp.node2=node2;
	tmp.label=-1;
	tmp.lockNet=-1;
	arcs.push_back(tmp);
	graph[node1].arcs.push_back(&arcs.back());
	graph[node2].arcs.push_back(&arcs.back());
};

//Remove arcs connected to a given node
void GraphRouter::remArcs(unsigned int node){
	int n;
	
	for(list<t_arc*>::iterator arc_it = graph[node].arcs.begin(); arc_it != graph[node].arcs.end(); arc_it++){
		if((*arc_it)->node1!=node)    n=(*arc_it)->node1;
		else n=(*arc_it)->node2;
		for(list<t_arc*>::iterator arc_it2 = graph[n].arcs.begin(); arc_it2 != graph[n].arcs.end(); arc_it2++)
			if(*arc_it2==*arc_it){
				graph[n].arcs.erase(arc_it2);
				break;
			}
	}
	graph[node].arcs.clear();
};

void GraphRouter::clearHistory(){
   	for(unsigned int x=0; x<graph.size(); x++)
		graph[x].history=0;
}

int GraphRouter::prim(list<unsigned int> tmp_netnodes,unsigned int actualAttempt,unsigned int net) {
	unsigned int c=1;
   	int cost=0;
	int ok;		
	
	list<unsigned int> x;
	x.push_back(tmp_netnodes.front());
	while(c++<tmp_netnodes.size()){
		ok=bfsRoute2(x,net,actualAttempt,false,0,actualAttempt,false);//:   //Route the tree to the closest node in the graph
		if(ok!=-1)
			cost = cost + ok;
		else return -1;
	}
	return cost;
	
}

bool GraphRouter::routeNets(unsigned int nrAttempts){
	int ok=0;
	int actualAttempt=0, minConflict=0, cntConflict=-1;
	int conflicts;
	clock_t start,finish;
	float elapsed;
	bool AntConflict;
	
	
	list<unsigned int>::iterator netTree_it;
	map<unsigned int,t_nets>::iterator nets_it;
	list<t_arc>::iterator arcs_it;	
	map<int,int>::iterator Solved_it;
	
//	cout << "Routing graph..." << endl;
	reset();
	start=clock(); 
	
	//GraphRouter Routing
	do{
		actualAttempt++;
		finalNet=actualAttempt;
		if(actualAttempt%100==0) cerr << ".";		
		// Loop over all nets
		nets_it=netlist.begin();
		while((ok!=-1) && nets_it!=netlist.end()){
			if(nets_it->second.conflict || actualAttempt==1){
				//Rip-up net 
				for(netTree_it=(nets_it->second).netTree.begin();netTree_it!=(nets_it->second).netTree.end();netTree_it++){
					graph[*netTree_it].nrNets--;
					if((!graph[*netTree_it].source) && (graph[*netTree_it].net==nets_it->first))  graph[*netTree_it].net=-1; 
				}
				//Clear netTree	of the net
				(nets_it->second).netTree.clear();
				
				//Insert the first node to the tree
				(nets_it->second).netTree.push_back(*((nets_it->second).nodes.begin()));
				
				//Loop until all sinks have been found
				unsigned int c=1;
				while(c++<(nets_it->second).nodes.size() && (ok!=-1)){
					ok=bfsRoute((nets_it->second).netTree,nets_it->first,actualAttempt,false,0,actualAttempt,true);//:   //Route the tree to the closest node in the graph
				}
				for(netTree_it=(nets_it->second).netTree.begin();netTree_it!=(nets_it->second).netTree.end();netTree_it++){
					graph[*netTree_it].nrNets++;
					graph[*netTree_it].net=nets_it->first;
					if(graph[*netTree_it].nrNets>1)  graph[*netTree_it].history++;
				}
				if(ok==-1) cout << endl <<"-> Impossible to find a path to route net: " << nets_it->first << endl;
			}
			nets_it++;
		}
		
		conflicts=0;
		for(nets_it=netlist.begin();nets_it!=netlist.end(); nets_it++){
			AntConflict=nets_it->second.conflict;
			nets_it->second.conflict=false;
			for(netTree_it=nets_it->second.netTree.begin();netTree_it!=nets_it->second.netTree.end();netTree_it++){
				if(graph[*netTree_it].nrNets>1){
					conflicts+=graph[*netTree_it].nrNets;
					nets_it->second.conflict=true;
				}
			}
			if(nets_it->second.conflict==false && (AntConflict || actualAttempt==1)) {
				solvedNets[(nets_it->first)]=actualAttempt;
		    }	
	    }
		if(conflicts<minConflict || cntConflict==-1){
			cntConflict=0;
			minConflict=conflicts;
		}else{
			cntConflict++;
		}
		if(cntConflict>50000) break;
	}while((ok!=-1) && conflicts && actualAttempt<nrAttempts);
	
	//Update the arcs costs for non-conflicted nets
	for(arcs_it = arcs.begin(); arcs_it != arcs.end(); arcs_it++)	 
		if(arcs_it->label==solvedNets[graph[arcs_it->node1].net])
			arcs_it->label=finalNet;
	
	finish=clock();
	elapsed=(finish-start)/(CLOCKS_PER_SEC/1000);
	elapsed=elapsed/1000;
	
	if(conflicts){
		cout << endl <<"-> Unable to route the circuit after " << actualAttempt << " attempts."<< endl;
	}else{
		cout << endl <<"-> Routing finished in " << actualAttempt << " attempts after " << elapsed << " s" << endl;
	}
	return(ok && !conflicts);
	
}	
int GraphRouter::addSteiner(unsigned int actualAttempt,unsigned int net) {
	list<int> stCandidates;
	list<int>::iterator stCandidates_it,bestCandidate_it;
	list<unsigned int> tmp_netnodes=netlist[net].nodes;
	list<unsigned int>::iterator tmp_iter;
	int nrSteiners=0;
	int bestCost = prim(tmp_netnodes,actualAttempt,net);
	int cost=0;
	//Build steiner candidates list
	for(unsigned int x=0; x<graph.size(); x++){
		if(graph[x].net==-1 && graph[x].arcs.size()>2){ 
			stCandidates.push_back(x);  
	    }	
	}
	bool ok=true;
	while (ok) {	
		bestCandidate_it=stCandidates.end();
		for(stCandidates_it=stCandidates.begin();stCandidates_it!=stCandidates.end();stCandidates_it++) { 
			tmp_netnodes=netlist[net].nodes;
			tmp_netnodes.push_back(*stCandidates_it); 
			graph[*stCandidates_it].source=true;//Add candidate to the net
			graph[*stCandidates_it].net=net;
			netlist[net].netTree.clear(); //Clear netTree of the net
			cost = prim(tmp_netnodes,actualAttempt,net);//Route the net adding the steiner point
			if ((cost<bestCost)&&(cost!=-1)) { 
				bestCandidate_it = stCandidates_it;
				bestCost = cost; 
			}
			graph[*stCandidates_it].source=false;//Remove candidate from net
			graph[*stCandidates_it].net=-1;
		}
		if(bestCandidate_it!=stCandidates.end()) {
			addNodetoNet(net,*bestCandidate_it);
			graph[*bestCandidate_it].source=2;
			nrSteiners++;
			stCandidates.erase(bestCandidate_it);  			
		}
		else {
			ok=false;
		}
	}
	return nrSteiners;
}


bool GraphRouter::optimize(){
	list<unsigned int>::iterator netTree_it;
	map<unsigned int,t_nets>::iterator nets_it;
	clock_t start,finish;
	float elapsed;
	int nrSteiners=1;
	
	cout << endl;
	cout << "-> Cost before optimization:" << getCost() << endl;
	cout << "-> Optimizing routing graph...";
	
	start=clock();
	//Clear history				
	for(int x=0; x<graph.size(); x++){
		graph[x].history=0;
	}
	//Routing the steiner nets
	while(nrSteiners!=0){
		nrSteiners=0;
		finalNet++;
		nets_it=netlist.begin();
		while(nets_it!=netlist.end()) {
			//Rip-up net 
			for(netTree_it=(nets_it->second).netTree.begin();netTree_it!=(nets_it->second).netTree.end();netTree_it++){
				graph[*netTree_it].nrNets--;
				if(!graph[*netTree_it].source) graph[*netTree_it].net=-1;
			}
			//Clear netTree	of the net
			(nets_it->second).netTree.clear();
			
			if(nets_it->second.nodes.size()>2) nrSteiners+=addSteiner(finalNet,nets_it->first) ;
			
			//Insert the first node to the tree
			(nets_it->second).netTree.push_back(*((nets_it->second).nodes.begin()));
			
			//Loop until all sinks have been found
			unsigned int c=1;
			while((c++<(nets_it->second).nodes.size())){
				bfsRoute2((nets_it->second).netTree,nets_it->first,finalNet,false,0,finalNet,true);//:   //Route the tree to the closest node in the graph
			}
			for(netTree_it=(nets_it->second).netTree.begin();netTree_it!=(nets_it->second).netTree.end();netTree_it++)
				graph[*netTree_it].nrNets++;
			nets_it++;	
		}
	}
	finish =clock();
	elapsed=(finish-start)/(CLOCKS_PER_SEC/1000);
	elapsed=elapsed/1000;
	cout << endl;
	cout << "-> Cost after optimization: " << getCost() << endl;
	cout << "-> Runtime = " << elapsed << " s" << endl;
	return true;
}

//Find the shortest path using a maze router
int GraphRouter::bfsRoute(list<unsigned int>& sourceNodes, const unsigned int targetNet, const unsigned int label, const bool checkConflict, const unsigned int maxCost, unsigned int actualAttempt, bool definitive ){
	
	static vector<t_trace> trace;
	trace.resize(graph.size());
	for (int n=0; n<trace.size(); n++) trace[n].clear();
	
	list<t_arc*>::iterator arc_it;
	t_tmp tmp, actualNode;
 	
	//Initialize priority queue PQ with the sourceNodes
	priority_queue<t_tmp> pq;
	tmp.costAccumulated=0;
	tmp.father=-1;
	for(list<unsigned int>::iterator nodes_it = sourceNodes.begin(); nodes_it != sourceNodes.end(); nodes_it++){
		tmp.node=*nodes_it;
//		tmp.costAccumulated=calcDistance(*nodes_it, net->second.nodes.front() )/arbFactor ;
		pq.push(tmp);
		trace[*nodes_it].visited=true;
	}
	
	actualNode=pq.top();
	
	//Lool until new sink is found
	while(!pq.empty() && (graph[actualNode.node].net!=targetNet || actualNode.father==-1)){
		//Remove lowest cost node from PQ
		actualNode=pq.top();
		pq.pop();
		if(actualNode.father!=-1 && trace[actualNode.node].visited) continue;
		//Save the path
		trace[actualNode.node].setFather(actualNode.father,actualNode.link);
		
		//Put the adjacent nodes in the Priority Queue ordened by the costs from the source 
		for(arc_it = graph[actualNode.node].arcs.begin(); arc_it != graph[actualNode.node].arcs.end(); arc_it++){
			// if is not visited yet,  isn't a source from other net, dont have blockage if checkConflict is enabled neither is walking from the same net, add it to the queue
			if(!trace[(*arc_it)->node1].visited && !(graph[(*arc_it)->node1].source && graph[(*arc_it)->node1].net!=targetNet) && !(checkConflict && ((graph[(*arc_it)->node1].net!=-1 && graph[(*arc_it)->node1].net!=targetNet) || (*arc_it)->label==label)) && ((*arc_it)->lockNet==targetNet || (*arc_it)->lockNet==-1)){
				tmp.node=(*arc_it)->node1;
				tmp.father=actualNode.node;
				tmp.link=*arc_it;
				tmp.costAccumulated=calcCost(*arc_it, (*arc_it)->node1, targetNet, actualAttempt)+actualNode.costAccumulated;
				if((maxCost==0 || tmp.costAccumulated<=maxCost)){
					pq.push(tmp);
				}
			}
			else if(!trace[(*arc_it)->node2].visited && !(graph[(*arc_it)->node2].source && graph[(*arc_it)->node2].net!=targetNet) && !(checkConflict && ((graph[(*arc_it)->node2].net!=-1 && graph[(*arc_it)->node2].net!=targetNet) || (*arc_it)->label==label)) && ((*arc_it)->lockNet==targetNet || (*arc_it)->lockNet==-1)){
				tmp.node=(*arc_it)->node2;
				tmp.father=actualNode.node;
				tmp.link=*arc_it;
				tmp.costAccumulated=calcCost(*arc_it, (*arc_it)->node2, targetNet, actualAttempt)+actualNode.costAccumulated;
				if((maxCost==0 || tmp.costAccumulated<=maxCost)){
					pq.push(tmp);
				}	
			}
		}
	}
	
	//Trace the path back to the source node 
	if(graph[actualNode.node].net==targetNet && actualNode.father!=-1){
		int node = actualNode.node;
		while (trace[node].father!=-1) {
			sourceNodes.push_back(node);  //Put these new nodes in the Routing Tree
			if (definitive){ 
				trace[node].link->label=label;
			}
			node = trace[node].father;
		}
		return actualNode.costAccumulated;
	}
	return -1;
}

//Find the shortest path using a maze router
int GraphRouter::bfsRoute2(list<unsigned int>& sourceNodes, const unsigned int targetNet, const unsigned int label, const bool checkConflict, const unsigned int maxCost, unsigned int actualAttempt, bool definitive ){
	
	vector<t_trace> trace(graph.size());
	for (int n=0; n<trace.size(); n++) trace[n].clear();
	
	list<t_arc*>::iterator arc_it;
	t_tmp tmp, actualNode;
 	
	//Initialize priority queue PQ with the sourceNodes
	priority_queue<t_tmp> pq;
	tmp.costAccumulated=0;
	tmp.father=-1;
	for(list<unsigned int>::iterator nodes_it = sourceNodes.begin(); nodes_it != sourceNodes.end(); nodes_it++){
		tmp.node=*nodes_it;
		pq.push(tmp);
		trace[*nodes_it].visited=true;
	}
	
	actualNode=pq.top();
	
	//Lool until new sink is found
	while(!pq.empty() && (graph[actualNode.node].net!=targetNet || actualNode.father==-1)){
		//Remove lowest cost node from PQ
		actualNode=pq.top();
		pq.pop();
		if(actualNode.father!=-1 && trace[actualNode.node].visited) continue;
		
		//Save the path
		trace[actualNode.node].setFather(actualNode.father,actualNode.link);
		
		//Put the adjacent nodes in the Priority Queue ordened by the costs from the source
		for(arc_it = graph[actualNode.node].arcs.begin(); arc_it != graph[actualNode.node].arcs.end(); arc_it++){
			// if is not visited yet,  isn't a source from other net, dont have blockage if checkConflict is enabled neither is walking from the same net, add it to the queue
			if(!trace[(*arc_it)->node1].visited && !(graph[(*arc_it)->node1].source && graph[(*arc_it)->node1].net!=targetNet) && !(checkConflict && ((graph[(*arc_it)->node1].net!=-1 && graph[(*arc_it)->node1].net!=targetNet) || (*arc_it)->label==label)) && ((*arc_it)->lockNet==targetNet || (*arc_it)->lockNet==-1)){
				tmp.node=(*arc_it)->node1;
				tmp.father=actualNode.node;
				tmp.link=*arc_it;
				tmp.costAccumulated=calcCost(*arc_it, (*arc_it)->node1, targetNet, 1)+actualNode.costAccumulated;
				if((maxCost==0 || tmp.costAccumulated<=maxCost) && ((graph[tmp.node].net==-1) || (graph[tmp.node].net==targetNet))) {
					pq.push(tmp);
				}
				
			}
			else if(!trace[(*arc_it)->node2].visited && !(graph[(*arc_it)->node2].source && graph[(*arc_it)->node2].net!=targetNet) && !(checkConflict && ((graph[(*arc_it)->node2].net!=-1 && graph[(*arc_it)->node2].net!=targetNet) || (*arc_it)->label==label)) && ((*arc_it)->lockNet==targetNet || (*arc_it)->lockNet==-1)){
				tmp.node=(*arc_it)->node2;
				tmp.father=actualNode.node;
				tmp.link=*arc_it;
				tmp.costAccumulated=calcCost(*arc_it, (*arc_it)->node2, targetNet, 1)+actualNode.costAccumulated;
				if((maxCost==0 || tmp.costAccumulated<=maxCost) && ((graph[tmp.node].net==-1) || (graph[tmp.node].net==targetNet))) {
					pq.push(tmp);
				}
				
			}
		}
	}
	
	//Trace the path back to the source node 
	if(graph[actualNode.node].net==targetNet && actualNode.father!=-1){
		int node = actualNode.node;
		while (trace[node].father!=-1) {
			sourceNodes.push_back(node);  //Put these new nodes in the Routing Tree
			if (definitive){ 
				graph[node].net=targetNet; 
				trace[node].link->label=label;
			}
			node = trace[node].father;
		}
		return actualNode.costAccumulated;
	}
	return -1;
}


unsigned int GraphRouter::calcCost(const t_arc* arc, const unsigned int targetNode, const unsigned int targetNet, unsigned int actualAttempt){
 	if(actualAttempt!=1)
		return arc->cost+(graph[targetNode].history*(graph[targetNode].nrNets+1));
	else
		return arc->cost+graph[targetNode].history;
}



int GraphRouter::getNrFinalArcs(int n){
	int cnt=0;
	for(list<t_arc*>::iterator arc_it = graph[n].arcs.begin(); arc_it != graph[n].arcs.end(); arc_it++){
		if((*arc_it)->label==finalNet)
			cnt++;
	}
	return cnt;
}	

int GraphRouter::getCost(){
	unsigned int finalCost=0;
	for(list<t_arc>::iterator arc_it = arcs.begin(); arc_it != arcs.end(); arc_it++)
		if(arc_it->label==finalNet)
			finalCost+=arc_it->cost;
	return finalCost;
}

void GraphRouter::showResult(){
	unsigned int finalCost=0,wlCost=0;
	for(list<t_arc>::iterator arc_it = arcs.begin(); arc_it != arcs.end(); arc_it++){
		if(arc_it->label==finalNet){
//			cout << arc_it->node1 << " - " << arc_it->node2 << endl; 
			finalCost+=arc_it->cost;
			wlCost++;
		}
	}
	cout << "-> Cost =" << finalCost << endl;
//	cout << "-> Wirelength =" << wlCost << endl;
}

bool GraphRouter::areConnected(unsigned int n1, unsigned int n2){
	for(list<t_arc*>::iterator arc_it = graph[n1].arcs.begin(); arc_it != graph[n1].arcs.end(); arc_it++){
		if((*arc_it)->label==finalNet){
			if(((*arc_it)->node1==n1 && (*arc_it)->node2==n2) || ((*arc_it)->node1==n2 && (*arc_it)->node2==n1))
				return true;
		}
	}
	return false;
}

int GraphRouter::areConnected2(unsigned int n1, unsigned n2){
	return graph[n1].net==graph[n2].net && graph[n1].nrNets>=1 && graph[n2].nrNets>=1;
}

bool GraphRouter::connect(int net, unsigned int n1, unsigned n2){
	for(list<t_arc*>::iterator arc_it = graph[n1].arcs.begin(); arc_it != graph[n1].arcs.end(); arc_it++){
		if(((*arc_it)->node1==n1 && (*arc_it)->node2==n2) || ((*arc_it)->node1==n2 && (*arc_it)->node2==n1)){
			(*arc_it)->label=finalNet;
			if(graph[n1].net!=net){
				graph[n1].net=net;
				graph[n1].nrNets++;
				netlist[net].netTree.push_front(n1);
			}
			if(graph[n2].net!=net){
				graph[n2].net=net;
				graph[n2].nrNets++;
				netlist[net].netTree.push_front(n2);
			}
			return true;
		}
	}
	return false;
}

bool GraphRouter::isSource(unsigned int x){
	return graph[x].source==1;
}

int GraphRouter::getArcCost(int n1, int n2){
	for(list<t_arc*>::iterator arc_it = graph[n1].arcs.begin(); arc_it != graph[n1].arcs.end(); arc_it++){
		if(((*arc_it)->node1==n1 && (*arc_it)->node2==n2) || ((*arc_it)->node1==n2 && (*arc_it)->node2==n1))
			return (*arc_it)->cost;
	}
	return -1;
}

int GraphRouter::setArcCost(int n1, int n2, int cost){
	for(list<t_arc*>::iterator arc_it = graph[n1].arcs.begin(); arc_it != graph[n1].arcs.end(); arc_it++){
		if(((*arc_it)->node1==n1 && (*arc_it)->node2==n2) || ((*arc_it)->node1==n2 && (*arc_it)->node2==n1))
			return (*arc_it)->cost=cost;
	}
	return -1;
}

//Lock arc for specified net
bool GraphRouter::lockArc(int n1, int n2, int net){
	for(list<t_arc*>::iterator arc_it = graph[n1].arcs.begin(); arc_it != graph[n1].arcs.end(); arc_it++){
		if(((*arc_it)->node1==n1 && (*arc_it)->node2==n2) || ((*arc_it)->node1==n2 && (*arc_it)->node2==n1)){
			(*arc_it)->lockNet=net;
			return true;
		}	
	}
	return false;
}

void GraphRouter::reset(){
	for(map<unsigned int,t_nets>::iterator nets_it=netlist.begin(); nets_it!=netlist.end(); nets_it++){
		for(list<unsigned int>::iterator nodes_it=nets_it->second.nodes.begin(); nodes_it!=nets_it->second.nodes.end();nodes_it++)
			if(graph[*nodes_it].source==2) nets_it->second.nodes.erase(nodes_it);
		(nets_it->second).netTree.clear();
	}
	for(int x=0; x<graph.size(); x++){
		graph[x].history=0;
		if(graph[x].source==2) graph[x].source=0;
		if(!graph[x].source) graph[x].net=-1;
		graph[x].nrNets=0;
	}
	for(list<t_arc>::iterator arc_it = arcs.begin(); arc_it != arcs.end(); arc_it++)
		arc_it->label=-1;
	
}
