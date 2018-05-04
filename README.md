#A Star Algorithm

This is an academic project developed for the course of Discrete Optimization taken in UAB (Universitat Autònoma de Barcelona) as part of the third semester in Mathmods program.

This course has been taught by prof. Alseda, here you can find a link to the official website of the course:
 
- [Official website](http://mat.uab.cat/~alseda/MasterOpt/index.html)
- [Project Statement](http://mat.uab.cat/~alseda/MasterOpt/Assignment-AStar-2017.pdf)

## The problem:

We have a file [spain.csv](http://mat.uab.cat/~alseda/MasterOpt/spain.csv.zip), containing information of the latitude and longitud of connected nodes (places with connections to roads, highways or something else) around Spain. The information is structured in the following way: 


- <sub> node|@id|@name|@place|@highway|@route|@ref|@oneway|@maxspeed|node_lat|node_lon|... </sub>

- <sub> way|@id|@name|@place|@highway|@route|@ref|@oneway|@maxspeed|membernode|membernode|membernode|... </sub>

- <sub> relation|@id|@name|@place|@highway|@route|@ref|@oneway|@maxspeed|rel_type|type;@id;@role|... </sub>


Nodes give us the information of places in the map. The list of nodes is already sorted with respect to the keys. Ways are lists of nodes (each one is from 2 to around 2000) and give us the information about how the nodes connecto among themselves. For example ...|@oneway||A|B|C|.. means that node there is a connection from A to B and from B to C by the same way. If the field @oneway is empty means that there is also a connection to the opposite direction for each pair of members. 

The project consisted in organizing the information and implementing the A-Star algorithm in order to find the optimal path (in a practical sense a good approximation of the optimal) from Basílica de Santa María del Mar (in Barcelona) to the Giralda (Calle Mateos Gago) in Sevilla. That is organize the information in the file in such a way to construct structures that represent a connected graph and in such a graph to find an optimal path from the node whose (@id) is 240949599 to the node with (@id) 195977239.

## Final Solution

As result of this project we have an executable called main that needs to be invoked with 6 parameters:

- **First**: The ID of the source.

- **Second**: The ID of the goal.

- **Third**: Accepts the following values 0 if we want the *normal execution* of the program, that is we want the program to check if exists the binary files, otherwise the program reads the .csv file and execute AStar. If this option is set to be 1, forces to read the .csv and overwriting the binaries. If this option is set to be 2, it will ask for latitude and longitud of the starting point and the goal respectively, and the it will proceed in *normal execution* executing the A-Star.

- **Forth**: Name of the csv file containing the graph information.

- **Fifth**: Path to the share folder. In this folder will be saved the resulting binary files from the reading process, the .csv file with the information required to construct the graph and also inside this folder will be saved the resulting file with the nodes in case the A-Star algorithm finds a solution.

- **Sixth**: Name of the file that will contain the solution, in case A-star finds one.


We structure the code in the following way.

- *libs*: Inside this folder there are basic libraries created to make easier the development of the project.

- *src*: Inside this folder you can find the core codes, they form the basic logic behind the project.

- *obj*: This folder contains the objects used for the linker to create the executable main.

- *share*: Folder with *.csv* and *.dat* files created from the project. In this folder we have to save the .csv file that we want to be read.

- *include*: Definitions handled by the programs.

- *main.cpp*: Main file that joins all the parts of the project in one executable file. Also in this file we configure the way we require the parameters for the execution of the program.

- *compile.sh*: g++ commands to compile every file needed to build the final executable. 

- *makefile*: Makefile for easier development. This script has some bugs and it still needs some development in order to make it work properly. It works fine for the first compile. Since it is not a crucial part of the project, we use during the development of the project the bash script *compile.sh*.
