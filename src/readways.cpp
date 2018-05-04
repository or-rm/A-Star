//#define connNumber 28394533
#define connNumber 48685269

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h> 
#include <cstring>
#include <climits>

#include <nodestructures.h>
#include <nodebinsearch.h>
#include <nodequicksort.h>
#include <monitoring.h>

#include <readways.h>

int readingways(std::string filename, std::string pathname){


node *nodes;
connection *conns;

unsigned long nodeIndex=0;
unsigned long connscounter=0;

std::string str;
std::string delimiter = "|";

std::string token_type;
std::string token_id;
std::string token_name;
std::string token_info;
std::string token_oneway;
std::string node1, node2, node3;

size_t pos=0;
unsigned char field_splitter=0;


unsigned long indexForNodes, indexForSuccesors;
unsigned long binary_search_result, idwaymember;
unsigned long connections[5306];
unsigned long connectionsID[5306];
unsigned long nodemembercounter=0;
unsigned long  pos1Node, pos2Node; 
unsigned long  pos1NodeSucc, pos2NodeSucc;
unsigned long NConnections=0;



/*******************************************Reading the Nodes files***********************************************/
	std::FILE *Nodes_infile;
	std::string nodesfile= pathname+"/Nodes.dat";

	if((Nodes_infile=std::fopen(nodesfile.c_str(),"rb")) == NULL){ 
    	std::cerr << "The input binary data file cannot be opened.\n" << std::endl;
		return 1;
	}
	else{
		if (fread(&nodeIndex, sizeof(unsigned long), 1, Nodes_infile)!=1){
			std::cerr << "Impossible to read the size.\n" << std::endl;
			return 1;			
		}
		printf("%lu\n", nodeIndex);
		if((nodes = (node*)malloc(nodeIndex*(sizeof(node))))==NULL){
			std::cerr << "Cannot allocate enough memory.\n" << std::endl;
			return 1;
		}
		else if (fread(nodes, sizeof(node), nodeIndex, Nodes_infile)!=nodeIndex){
			std::cerr << "Impossible to read the nodes.\n" << std::endl;
			free(nodes);
			std::fclose(Nodes_infile);
			return 1;			
		}
		std::fclose(Nodes_infile);
	}
	printf("Succesful Memory Allocation for Nodes\n");
/*******************************************Reading the Nodes files***********************************************/

	if((conns = (connection *)malloc(connNumber*(sizeof(connection))))==NULL){
    	std::cerr << "Cannot allocate enough memory.\n" << std::endl;
		free(nodes);
		return 1;
	}
	printf("Succesful Allocating Memory\n");

/*******************************************Reading the Spain files***********************************************/
	std::string csvfilepath= pathname + "/" + filename;

	std::ifstream in(csvfilepath);
  	if(!in) {
    	std::cerr << "Cannot open input file.\n" << std::endl;
		free(nodes);
    	return 1;
  	}
/*******************************************Reading the Spain files***********************************************/

	while (std::getline(in, str)) {
		// output the line
		token_type= "";
		//std::cout << str << std::endl;
	/*************************************************************************************************************/
		if((pos=str.find(delimiter)) != std::string::npos){
		/*********************************************************************************************************/
			token_type = str.substr(0, pos);	
			str.erase(0, pos + delimiter.length());	
			
			pos=str.find(delimiter);
			token_id = str.substr(0, pos);
			str.erase(0, pos + delimiter.length());	

			pos=str.find(delimiter);
			token_name = str.substr(0, pos);
			str.erase(0, pos + delimiter.length());	
/*****************************************WAY CLASIFICATION***************************************************************/
			if (token_type.compare("way")==0){
	/**********************************************************************************************************************/
				for (field_splitter=0; field_splitter < 4; field_splitter++){
					pos=str.find(delimiter);
					str.erase(0, pos + delimiter.length());
				}
	/**********************************************************************************************************************/
				pos=str.find(delimiter);
				token_oneway=str.substr(0, pos);
				str.erase(0, pos + delimiter.length());

				pos=str.find(delimiter);
				str.erase(0, pos + delimiter.length());

				indexForNodes=0;
	/**********************************************************************************************************************/
			/*************************************************************************************************************/
				while((pos=str.find(delimiter))!=std::string::npos){
					token_info=str.substr(0,pos);
					str.erase(0, pos + delimiter.length());					
					
					idwaymember=std::stoul(token_info,nullptr, 10);
					binary_search_result=binarySearch(nodes, 0, nodeIndex-1, idwaymember);

					if (binary_search_result<nodeIndex){
						connectionsID[indexForNodes]=idwaymember;
						connections[indexForNodes]=binary_search_result;
						indexForNodes++;					
					}					
				}
			/*************************************************************************************************************/	
				token_info=str;
				idwaymember=std::stoul(token_info, nullptr, 10);
				binary_search_result=binarySearch(nodes, 0, nodeIndex-1, idwaymember);	
				
				if (binary_search_result<nodeIndex){

					connectionsID[indexForNodes]=idwaymember;
					connections[indexForNodes]=binary_search_result;
					indexForNodes++;

				}
			/*************************************************************************************************************/				
				if(indexForNodes>=2){
					if(token_oneway.compare("oneway")==0){

						for(indexForSuccesors=0; indexForSuccesors < indexForNodes-1; indexForSuccesors++){
							if(connections[indexForSuccesors]!=connections[indexForSuccesors+1]){
								pos1Node=connections[indexForSuccesors];
								pos1NodeSucc=nodes[pos1Node].nsucc;

								nodes[pos1Node].nsucc = nodes[pos1Node].nsucc+1;

								(conns+connscounter)->source=connections[indexForSuccesors];
								(conns+connscounter)->destination=connections[indexForSuccesors+1];
								connscounter++;

								set_Maximum_node_Descendant_N(pos1NodeSucc+1, &nodemembercounter);  // Debugging Instruction: Determine the maximum number of succesors of a node 
							}
						}
						NConnections=NConnections+(indexForNodes-1);
					}
					else{

						for(indexForSuccesors=0; indexForSuccesors<indexForNodes-1; indexForSuccesors++){
							if(connections[indexForSuccesors]!=connections[indexForSuccesors+1]){
								pos1Node=connections[indexForSuccesors];
								pos1NodeSucc=nodes[pos1Node].nsucc;
	
								pos2Node=connections[indexForSuccesors+1];
								pos2NodeSucc=nodes[pos2Node].nsucc;

								nodes[pos1Node].nsucc = pos1NodeSucc+1;
								nodes[pos2Node].nsucc = pos2NodeSucc+1;
								/*************************************************************************************************************/
								/*************************************************************************************************************/
								(conns+connscounter)->source=connections[indexForSuccesors];
								(conns+connscounter)->destination=connections[indexForSuccesors+1];
								connscounter++;
								(conns+connscounter)->source=connections[indexForSuccesors+1];
								(conns+connscounter)->destination=connections[indexForSuccesors];
								connscounter++;
			/*************************************************************************************************************/
			/*************************************************************************************************************/
								set_Maximum_node_Descendant_N(nodes[pos1Node].nsucc, &nodemembercounter);// Debugging Instruction: Determine the maximum number of succesors of a node
								set_Maximum_node_Descendant_N(nodes[pos2Node].nsucc, &nodemembercounter);// Debugging Instruction: Determine the maximum number of succesors of a node
							}
			/*************************************************************************************************************/
			/*************************************************************************************************************/
						}
						/*************************************************************************************************************/
						NConnections=NConnections+2*(indexForNodes-1);
					}
				}
			/*****************************************WAY CLASIFICATION***************************************************************/
			}			
		}

	}
	/**********************************Closed Connections*****************************************************************/
	in.close();
	/**********************************Closed Connections*****************************************************************/
	/**********************************Ordering Connections*****************************************************************/
	printf("Reaching QuickSort\n");
	printf("Number of descendants %lu\n", nodemembercounter);
	quickSort(conns, 0, connscounter-1);
	printf("QuickSortPassed\n");
	/**********************************Ordering Connections*****************************************************************/
	/**********************************Writing Connections*****************************************************************/

	std::string connfilename=pathname+"/Connections.dat";	
	std::FILE *Conns_outfile;


	if((Conns_outfile=std::fopen(connfilename.c_str(),"wb")) == NULL){ 
    	std::cerr << "The output binary data file cannot be opened.\n" << std::endl;
		in.close();
 		free(nodes);
		free(conns);
		return 1;
	}
	else{
		if(std::fwrite(&connscounter, sizeof(unsigned long), 1, Conns_outfile)!=1){
			in.close();
	 		free(nodes);
			free(conns);
			std::cerr << "The output binary data file cannot be opened.\n" << std::endl;
			return 1;
		}
		else
		{
			if(std::fwrite(conns, sizeof(connection), connscounter, Conns_outfile)!=connscounter){
				in.close();
		 		free(nodes);
				free(conns);
				std::cerr << "The output binary data file cannot be opened.\n" << std::endl;
				return 1;
			}
		}
		std::fclose(Conns_outfile);
	}
	/**********************************Writing Connections*****************************************************************/

	/**********************************Setting Node Graph*****************************************************************/
	for(unsigned long CCounter=0; CCounter<connscounter; CCounter=CCounter+(nodes+(conns+CCounter)->source)->nsucc){
		nodes[conns[CCounter].source].offset=CCounter;
	}
	/**********************************Setting Node Graph*****************************************************************/
	/**********************************Writing Node Graph****************************************************************/
	std::FILE *Nodes_outfile;
	if((Nodes_outfile=std::fopen(nodesfile.c_str(),"wb")) == NULL){ 
    	std::cerr << "The output binary data file cannot be opened.\n" << std::endl;
		in.close();
 		free(nodes);
		free(conns);
		return 1;
	}
	else{
		if(std::fwrite(&nodeIndex, sizeof(unsigned long), 1, Nodes_outfile)!=1){
			in.close();
	 		free(nodes);
			free(conns);
			std::cerr << "The output binary data file cannot be opened.\n" << std::endl;
			return 1;
		}
		else
		{
			if(std::fwrite(nodes, sizeof(node), nodeIndex, Nodes_outfile)!=nodeIndex){
				in.close();
		 		free(nodes);
				free(conns);
				std::cerr << "The output binary data file cannot be opened.\n" << std::endl;
				return 1;
			}
		}
		std::fclose(Nodes_outfile);
	}

	printf("%lu\n", nodemembercounter);
	printf("%lu\n", NConnections);
	printf("%lu\n", connscounter);
	printf("%lu\n", nodemembercounter);
	free(nodes);
	free(conns);
	return 0;
}
