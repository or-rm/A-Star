#define nnodes 23895681

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>



#include <nodestructures.h>
#include <nodebinsearch.h>
#include <nodequicksort.h>



#include <heuristic.h>
#include <readnodes.h>



int readingnodes(std::string filename, std::string pathname) {

	std::string csvpath=pathname+"/"+filename;	
	
	std::string str;
	std::string delimiter = "|";

	std::string token_type;
	std::string token_id;
	std::string token_name;
	std::string token_info;


	unsigned long nodeIndex=0;
	node *nodes;

	//Trying allocating memory
	printf("Allocating Memory\n");
	if((nodes = (node*)malloc(nnodes*(sizeof(node))))==NULL){
    	std::cerr << " Node. Cannot allocate enough memory.\n" << std::endl;
		return 1;
	}

	printf("Node. Succesful Allocating Memory\n");

	std::ifstream in(csvpath);
  	if(!in) {
    	std::cerr << "Cannot open input file." << csvpath << std::endl;
		free(nodes);
    	return 1;
  	}

	size_t pos=0;
	unsigned char field_splitter=0;
	
	while (std::getline(in, str)) {
		// output the line
		token_type= "";
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
		/*********************************************************************************************************/
		/*********************************************************************************************************/
			if(token_type.compare("node")==0){

				(*(nodes+nodeIndex)).id=std::stoul(token_id, nullptr, 10);

				for (field_splitter=0; field_splitter <6; field_splitter++){
					pos=str.find(delimiter);
					str.erase(0, pos + delimiter.length());
				}
				
				pos=str.find(delimiter);
				token_info = str.substr(0, pos);
				str.erase(0, pos + delimiter.length());
				(*(nodes+nodeIndex)).lat=d2r(std::stod(token_info, nullptr));

				pos=str.find(delimiter);
				token_info = str.substr(0, pos);
				str.erase(0, pos + delimiter.length());	
				(*(nodes+nodeIndex)).lon=d2r(std::stod(token_info, nullptr));

				nodeIndex++;

			}
	/**********************************************************************************************************************/
		}	
	}

	std::FILE *Nodes_outfile;
	std::string binfilepath =pathname+"/Nodes.dat";
	if((Nodes_outfile=std::fopen(binfilepath.c_str(),"wb")) == NULL){ 
    	std::cerr << "The output binary data file cannot be opened.\n" << std::endl;
		in.close();
 		free(nodes);
		return 1;
	}
	else{
		if(std::fwrite(&nodeIndex, sizeof(unsigned long), 1, Nodes_outfile)!=1){
			in.close();
	 		free(nodes);
			std::cerr << "The output binary data file cannot be opened.\n" << std::endl;
			return 1;
		}
		else
		{
			if(std::fwrite(nodes, sizeof(node), nodeIndex, Nodes_outfile)!=nodeIndex){
				in.close();
		 		free(nodes);
				std::cerr << "The output binary data file cannot be opened.\n" << std::endl;
				return 1;
			}
		}
		std::fclose(Nodes_outfile);
	}
 	free(nodes);
  	return 0;
}
