//C++11 Standard Libraries
#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <fstream>

// Custom Libraries
#include <nodestructures.h>
#include <readnodes.h>
#include <readways.h>
#include <astar.h>
#include <closestpoint.h>

using std::endl;
using std::cout;
using std::getline;
using std::cin;

inline bool fexists(const std::string& filename);

int main(int argc, char * argv[]) {

	const unsigned long  start = (argc > 2) ? std::stoul(argv[1]) : 240949599;
	const unsigned long  goal = (argc > 2) ? std::stoul(argv[2]) : 195977239;

	const int  option = (argc > 3) ? std::atoi(argv[3]) : 0;	
	const std::string filename = (argc > 4) ? argv[4] : "spain.csv";
	const std::string folder = (argc > 5) ? argv[5] : "./share";
	const std::string outputfile = (argc > 6) ? argv[6] : "results.csv";
	

	if(option == 0){
		bool existence=fexists(folder+"/"+filename);
			 existence&=fexists(folder+"/Nodes.dat");
			 existence&=fexists(folder+"/Connections.dat");

		if(existence){
				astarfinding(outputfile, folder, start , goal);
		}
		else{
				readingnodes(filename, folder);
				readingways(filename, folder);
				astarfinding(outputfile, folder, start, goal);
		}
	}else if (option == 1){
		readingnodes(filename, folder);
		readingways(filename, folder);
		astarfinding(outputfile, folder, start, goal);	
	}else if (option ==2){
		cout<<"Starting from closest Latitude, Longitude."<<endl;
		std::string lat1= "0.0000";
		std::string lon1= "0.0000";
		std::string lat2= "0.0000";
		std::string lon2= "0.0000";
		
		cout<<"Enter Latitude of the starting point: "<<endl;
		getline(cin, lat1);
		cout<<"Enter Longitud of the starting point: "<<endl;
		getline(cin, lon1);
		cout<<"Enter Latitude of the goal point: "<<endl;
		getline(cin, lat2);
		cout<<"Enter Longitude of the goal point: "<<endl;
		getline(cin, lon2);

		unsigned long startLL= closepoint(folder, std::stod(lat1,nullptr), std::stod(lon1,nullptr) );
		unsigned long goalLL= closepoint(folder, std::stod(lat2,nullptr), std::stod(lon2,nullptr) );
		cout<<"Closest ID to goal point: "<<goalLL<<endl;
		cout<<"Closest ID to initial point: "<< startLL<< endl;
		bool existence=fexists(folder+"/"+filename);
			 existence&=fexists(folder+"/Nodes.dat");
			 existence&=fexists(folder+"/Connections.dat");

		if(existence){
				astarfinding(outputfile, folder, startLL, goalLL);
		}
		else{
				readingnodes(filename, folder);
				readingways(filename, folder);
				astarfinding(outputfile, folder, startLL, goalLL);
		}

	}

	return(0);
}

inline bool fexists(const std::string& filename) {
  std::ifstream ifile(filename.c_str());
  return (bool)ifile;
}
