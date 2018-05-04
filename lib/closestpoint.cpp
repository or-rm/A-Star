
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <omp.h>
#include <closestpoint.h>
#include <nodestructures.h>
#include <heuristic.h>


#pragma omp declare reduction(minimum : unsigned long  : omp_out = omp_in < omp_out ? omp_in : omp_out)

unsigned long closepoint (std::string pathname, double lat, double lon){

	unsigned long nodeIndex=0;
	std::FILE *Nodes_infile;
	std::string nodesfile= pathname+"/Nodes.dat";
	node *nodes;

	if((Nodes_infile=std::fopen(nodesfile.c_str(),"rb")) == NULL){
		throw std::runtime_error("Impossible to open file nodes."); 
	}
	else{
		if (fread(&nodeIndex, sizeof(unsigned long), 1, Nodes_infile)!=1){
			throw std::runtime_error("Impossible to read nodes number."); 
		}
		if((nodes = (node*)malloc(nodeIndex*(sizeof(node))))==NULL){
			throw std::runtime_error("Impossible to read nodes.");
		}
		else if (fread(nodes, sizeof(node), nodeIndex, Nodes_infile)!=nodeIndex){
			free(nodes);
			std::fclose(Nodes_infile);
			throw std::runtime_error("No enough memory.");
		}
	}
	std::fclose(Nodes_infile);
	lat=d2r(lat);
	lon=d2r(lon);
	unsigned long index = 0;
	double min = HarvesineRad(nodes[0].lat, nodes[0].lon, lat, lon);

	#pragma omp parallel
	{
		unsigned long int index_local = index;
		double min_local = min;  
		#pragma omp for nowait
		for (auto i = 1; i < nodeIndex; i++) {
			double cmptmp=HarvesineRad(nodes[i].lat, nodes[i].lon, lat, lon);        
		    if ((cmptmp < min_local)&& (nodes[i].nsucc>0)) {
		        min_local = cmptmp;
		        index_local = i;
		    }
		}
		#pragma omp critical 
		{
				index = (min_local < min) ? index_local : index;
		}
	}
printf("Lat: %f, Lon: %f\n", 180.0*nodes[index].lat/PI_C, 180.0*nodes[index].lon/PI_C);
return nodes[index].id;

}
