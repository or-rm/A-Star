#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h> 
#include <cstring>
#include <climits>
#include <cfloat>
#include <cmath>
#include <omp.h>
#include <heuristic.h>

#include <nodestructures.h>
#include <nodebinsearch.h>
#include <astarqueues.h>
#include <monitoring.h>

#include <astar.h>

int astarfinding(std::string filename, std::string pathname, unsigned long startID, unsigned long goalID) {


/*******************************************Variable Definitions***********************************************/
node *nodes;
connection *conns;

AStarNode *astar;

unsigned long nodeIndex=0;
unsigned long connsCounter=0;

unsigned long* openListBase;
unsigned long* closedListBase;

unsigned long MaxListOSize=0;
unsigned long MaxListCSize=0;

QueueStar openList;
QueueStar closedList;

unsigned long CurrentNode;
bool flag_for_break=false;



/*******************************************Reading the files***********************************************/
	std::FILE *Conns_infile;
	std::FILE *Nodes_infile;

	std::string nodesfile= pathname+"/Nodes.dat";
	std::string connsfile= pathname+"/Connections.dat";

	if((Nodes_infile=std::fopen(nodesfile.c_str(),"rb")) == NULL){ 
    	std::cerr << "The input binary data file cannot be opened.\n" << std::endl;
		return 2;
	}
	else{
		if (fread(&nodeIndex, sizeof(unsigned long), 1, Nodes_infile)!=1){
			std::cerr << "Impossible to read the size.\n" << std::endl;
			return 2;			
		}
		if((nodes = (node*)malloc(nodeIndex*(sizeof(node))))==NULL){
			std::cerr << "Cannot allocate enough memory.\n" << std::endl;
			return 2;
		}
		else if (fread(nodes, sizeof(node), nodeIndex, Nodes_infile)!=nodeIndex){
			std::cerr << "Impossible to read the nodes.\n" << std::endl;
			free(nodes);
			std::fclose(Nodes_infile);
			return 2;			
		}
		std::fclose(Nodes_infile);
	}

	if((Conns_infile=std::fopen(connsfile.c_str(),"rb")) == NULL){ 
    	std::cerr << "The input binary data file cannot be opened.\n" << std::endl;
		return 2;
	}
	else{
		if (fread(&connsCounter, sizeof(unsigned long), 1, Conns_infile)!=1){
			std::cerr << "Impossible to read the size.\n" << std::endl;
			return 2;			
		}
		if((conns = (connection*)malloc(connsCounter*(sizeof(connection))))==NULL){
			std::cerr << "Cannot allocate enough memory.\n" << std::endl;
			return 2;
		}
		else if (fread(conns, sizeof(connection), connsCounter, Conns_infile)!=connsCounter){
			std::cerr << "Impossible to read the nodes.\n" << std::endl;
			free(nodes);
			free(conns);
			std::fclose(Conns_infile);
			return 2;			
		}
		std::fclose(Conns_infile);
	}

	printf("Number of Nodes: %lu\n", nodeIndex);
	printf("Number of Relations: %lu\n", connsCounter);
/*******************************************Reading the files***********************************************/
/****************************************Constructing A* First Conditions***********************************/
	if((astar = (AStarNode*)malloc(nodeIndex*(sizeof(AStarNode))))==NULL){
			std::cerr << "Cannot allocate enough memory.\n" << std::endl;
			free(nodes);
			free(conns);
			return 2;
	}

	for (int counter=0; counter<nodeIndex; counter++){
		(astar+counter)->whq=NONE;
		(astar+counter)->PosInQueue=0;
	}
	if((openListBase = (unsigned long *)malloc(nodeIndex*(sizeof(unsigned long))))==NULL){
			std::cerr << "Cannot allocate enough memory.\n" << std::endl;
			free(nodes);
			free(conns);
			return 2;
	}
	if((closedListBase = (unsigned long *)malloc(nodeIndex*(sizeof(unsigned long))))==NULL){
			std::cerr << "Cannot allocate enough memory.\n" << std::endl;
			free(nodes);
			free(conns);
			free(openListBase);
			return 2;
	}


	unsigned long start = binarySearch(nodes, 0, nodeIndex-1,  startID);
	unsigned long goal = binarySearch(nodes, 0, nodeIndex-1,  goalID);

	if((start>=nodeIndex)){
		printf("Start Node ID: %lu does not exist\n", startID);
		return 2;			
	}
	if((goal>=nodeIndex)){
		printf("Goal Node ID: %lu does not exist\n", goalID);
		return 2;			
	}

/****************************************Constructing A* First Conditions***********************************/
/***********************************************Initializing Queues*****************************************/
    double tm = omp_get_wtime();  
	printf("Beginning of the A* Algorithm.\n");  
	openList.base=openListBase;
	openList.QSize=0;
	openList.MaxQSize=nodeIndex;
	openList.whq=OPEN;

	closedList.base=closedListBase;
	closedList.QSize=0;
	closedList.MaxQSize=nodeIndex;
	closedList.whq=CLOSED;
/***********************************************Initializing Queues*****************************************/
/***********************************************  A Estrella       *****************************************/
	double goalLat=(nodes+goal)->lat;
	double goalLon=(nodes+goal)->lon;
	//Setting start node in OpenList

	(astar+start)->g=0.0;
	(astar+start)->h=HarvesineRad((nodes+start)->lat, (nodes+start)->lon, goalLat, goalLon);

	(astar+start)->f=(astar+start)->h;
	//Push after updating since updates the minimum
	push2Queue(&openList, astar, start);

	/*****************************************SUPER WHILE****************************************************/
	double current_succesor_cost=DBL_MAX;	
	unsigned long NodeSuccesor=CurrentNode;


	while (!flag_for_break){
		//printf("%lu\n", CurrentNode);
		set_Maximum_node_Descendant_N(openList.QSize, &MaxListOSize);
		set_Maximum_node_Descendant_N(closedList.QSize, &MaxListCSize);
		if((CurrentNode==goal) || (openList.QSize==0)){
			flag_for_break=true;
		}
		else{

			CurrentNode=ExtractMinimum(&openList);
			extract4Open(&openList, astar, CurrentNode);
			for(unsigned long nnsuc=nodes[CurrentNode].offset; nnsuc<(nodes[CurrentNode].offset+nodes[CurrentNode].nsucc); nnsuc++){
				
				NodeSuccesor=conns[nnsuc].destination;

				current_succesor_cost=(astar+CurrentNode)->g;
				current_succesor_cost+=HarvesineRad((nodes+CurrentNode)->lat, (nodes+CurrentNode)->lon, (nodes+NodeSuccesor)->lat, (nodes+NodeSuccesor)->lon);
				if((astar+NodeSuccesor)->whq==NONE){

					(astar+NodeSuccesor)->h=HarvesineRad( (nodes+NodeSuccesor)->lat, (nodes+NodeSuccesor)->lon,  goalLat,  goalLon);
					(astar+NodeSuccesor)->g=current_succesor_cost;
					(astar+NodeSuccesor)->f=(astar+NodeSuccesor)->h+(astar+NodeSuccesor)->g;
					(astar+NodeSuccesor)->parent=CurrentNode;
					push2Queue(&openList, astar, NodeSuccesor);

				}
				else if ((astar+NodeSuccesor)->g>current_succesor_cost){
					
					if((astar+NodeSuccesor)->whq==CLOSED){

						extract4Closed(&closedList, astar , NodeSuccesor);

						(astar+NodeSuccesor)->g=current_succesor_cost;
						(astar+NodeSuccesor)->f=(astar+NodeSuccesor)->h+(astar+NodeSuccesor)->g;
						(astar+NodeSuccesor)->parent=CurrentNode;
										
						push2Queue(&openList, astar, NodeSuccesor);

					}else if ((astar+NodeSuccesor)->whq==OPEN){
						extract4Open(&openList, astar , NodeSuccesor);
						(astar+NodeSuccesor)->g=current_succesor_cost;
						(astar+NodeSuccesor)->f=(astar+NodeSuccesor)->h+(astar+NodeSuccesor)->g;
						(astar+NodeSuccesor)->parent=CurrentNode;
						push2Queue(&openList, astar, NodeSuccesor);
					}
				}
			}
			push2Queue(&closedList, astar, CurrentNode);
		}
	}

	printf("End of A* Algorithm, total time: %f\n", omp_get_wtime() - tm);

	std::string AStarPATH="";

	if(CurrentNode!=goal){
		printf("No Solutions found to this node.\n");
		return 1;
	}
	else{

		printf("Maximum number of members in OpenList: %lu\n", MaxListOSize);
		printf("Maximum number of members in ClosedList: %lu\n", MaxListCSize);

		printf("Total Distance from start to goal: %.1f meters, %.3f km.\n", astar[goal].g, astar[goal].g/1000.0);
		unsigned long traveler=goal;
		unsigned long travelerDad=0;

		unsigned long Ncount=0;

		while(traveler!=start){
			AStarPATH=std::to_string(traveler)+">"+AStarPATH;
			travelerDad=astar[traveler].parent;
			traveler=travelerDad;
			Ncount++;
		}
		//std::cout<< AStarPATH <<std::endl;
		printf("Number of nodes %lu\n", Ncount);
	}
/*****************************************SUPER WHILE****************************************************/
/***********************************************  A Estrella       *****************************************/

	free(conns);

	free(openListBase);
	free(closedListBase);

	astarprinting(filename, pathname, AStarPATH, nodes, astar);

	free(astar);
	free(nodes);

	return 0;
}


int astarprinting (std::string filename, std::string pathname, std::string AStarPATH, node *pnode, AStarNode *pasnode){

	std::string delimiter=">";	
	size_t pos=0;
	std::ofstream results;
	results.open(pathname+"/"+filename);
	unsigned long counter=0;
	results<<"Node ID,Latitude,Longitude,Distance to the starting point"<<std::endl;
	while((pos=AStarPATH.find(delimiter)) != std::string::npos){
	/*********************************************************************************************************/
		std::string token = AStarPATH.substr(0, pos);
		AStarPATH.erase(0, pos + delimiter.length());	
		unsigned long position=std::stoul(token.c_str(), nullptr, 10);	
		results<<pnode[position].id<< ","<< ((180/PI_C)*(pnode[position].lat)) <<"," << ((180/PI_C)*(pnode[position].lon))<<","<< pasnode[position].g <<std::endl;			
		counter++;	
	/**********************************************************************************************************************/
	}		
	results.close();
	return 0;
}

