CC = g++
cflags = -Wall -g3 -std=c++11

valgrindFlags = --leak-check=full

search_flags_lsh = -i ../input.csv -q ../query.csv -o ./output.LSHsearch -N 3 -R 300 -k 3 -L 3 -algorithm LSH
search_flags_cube = -i ../input.csv -q ../query.csv -o ./output.CUBEsearch -N 3 -R 300 -k 3 -probes 3 -M 150 -algorithm Hypercube
search_flags_dfr = -i ../input.csv -q ../query.csv -o ./output.DFRsearch -N 3 -R 300 -k 3 -L 3 -algorithm Frechet -metric discrete -delta 6

cluster_flags = -i ../input_small_id -c ./cluster.conf -o ./output.clustering -m LSH

search_exe = search
cluster_exe = cluster

sourcePath = ./

OperationControllersLocation = $(sourcePath)OperationControllers/
OperationControllersObjects = $(OperationControllersLocation)MappingMethod.o $(OperationControllersLocation)LSH.o $(OperationControllersLocation)HyperCube.o $(OperationControllersLocation)ClusterInterface.o $(OperationControllersLocation)ClusterLSH.o $(OperationControllersLocation)ClusterHC.o $(OperationControllersLocation)ClusterLloyds.o

ObjectsA = $(sourcePath)mainA.cpp

# ObjectsB = $(sourcePath)mainCluster.cpp

DataStructuresLocation = $(sourcePath)DataStructures/
DataStructuresObjects = $(DataStructuresLocation)HashTable.o $(DataStructuresLocation)SimpleList.o

UtilitiesLocation = $(sourcePath)Utilities/
UtilitiesObjects = $(UtilitiesLocation)Utilities.o $(UtilitiesLocation)FileReader.o $(UtilitiesLocation)TimeSeries.o

LSHHashFuncsLocation = $(sourcePath)HashFunctions/
LSHHashFuncsObjects = $(LSHHashFuncsLocation)HashLSH.o

CubeHashFuncsLocation = $(sourcePath)HashFunctions/
CubeHashFuncsObjects = $(CubeHashFuncsLocation)HashHC.o

HashInterfacesLocation = $(sourcePath)HashFunctions/
HashInterfacesObjects = $(HashInterfacesLocation)HashInterface.o $(HashInterfacesLocation)HashDF.o $(HashInterfacesLocation)Hhashing.o

CommonObejects =  $(DataStructuresObjects) $(UtilitiesObjects) $(HashInterfacesObjects) $(LSHHashFuncsObjects) $(CubeHashFuncsObjects) $(OperationControllersObjects)

includePaths = -I./  -I$(DataStructuresLocation) -I$(CubeHashFuncsLocation) -I$(LSHHashFuncsLocation) -I$(UtilitiesLocation) -I$(OperationControllersLocation)

all: $(search_exe) #$(cluster_exe)

$(search_exe): $(CommonObejects) $(ObjectsA)
	$(CC) $(cflags) $(includePaths) $(CommonObejects) $(ObjectsA) -o $@

$(cluster_exe): $(CommonObejects) $(ObjectsB)
	$(CC) $(cflags) $(includePaths) $(CommonObejects) $(ObjectsB) -o $@

$(sourcePath)%.o: $(sourcePath)%.cpp
	$(CC) $(cflags) $(includePaths) -c $< -o $@

$(DataStructuresLocation)%.o: $(DataStructuresLocation)%.cpp
	$(CC) $(cflags) $(includePaths) -c $< -o $@

$(UtilitiesLocation)%.o: $(UtilitiesLocation)%.cpp
	$(CC) $(cflags) $(includePaths) -c $< -o $@

$(CubeHashFuncsLocation)%.o: $(CubeHashFuncsLocation)%.cpp
	$(CC) $(cflags) $(includePaths) -c $< -o $@

$(LSHHashFuncsLocation)%.o: $(LSHHashFuncsLocation)%.cpp
	$(CC) $(cflags) $(includePaths) -c $< -o $@

$(OperationControllersLocation)%.o: $(OperationControllersLocation)%.cpp
	$(CC) $(cflags) $(includePaths) -c $< -o $@

rSlsh: $(search_exe)
	./$(search_exe) $(search_flags_lsh)

rScube: $(search_exe)
	./$(search_exe) $(search_flags_cube)

rSdfr: $(search_exe)
	./$(search_exe) $(search_flags_dfr)

rCluster: $(cluster_exe)
	./$(cluster_exe) $(cluster_flags)

val: $(search_exe)
	valgrind $(valgrindFlags) ./$(search_exe) $(search_flags)

gdb: $(search_exe)
	gdb ./$(search_exe)

clean:
	rm $(CommonObejects) $(search_exe)
