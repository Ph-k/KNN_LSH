CC = g++
cflags = -Wall -g3 -std=c++11

valgrindFlags = --leak-check=full

lsh_flags = -i ../input_small_id -q ../query_small_id -o ./output.lsh -N 3 -R 300 -k 4 -L 5
cube_flags = -i ../input_small_id -q ../query_small_id -o ./output.cube -k 4 -M 5 -probes 1 -N 3 -R 300
cluster_flags = -i ../input_small_id -q ../query_small_id -c ../cluster.conf -o ./output.clustering -m Classic

lsh_exe = lsh
cube_exe = cube
cluster_exe = cluster

sourcePath = ./

mainObjects = $(sourcePath)Point.o 


OperationControllersLocation = $(sourcePath)OperationControllers/
OperationControllersObjects = $(OperationControllersLocation)LSH.o $(OperationControllersLocation)HyperCube.o $(OperationControllersLocation)ClusterComplex.o

LshObjects = $(sourcePath)mainLSH.cpp
CubeObjects = $(sourcePath)mainCube.cpp
ClusterObjects = $(sourcePath)mainCluster.cpp

DataStructuresLocation = $(sourcePath)DataStructures/
DataStructuresObjects = $(DataStructuresLocation)HashTable.o $(DataStructuresLocation)SimpleList.o

UtilitiesLocation = $(sourcePath)Utilities/
UtilitiesObjects = $(UtilitiesLocation)Utilities.o $(UtilitiesLocation)FileReader.o

LSHHashFuncsLocation = $(sourcePath)HashFunctions/
LSHHashFuncsObjects = $(LSHHashFuncsLocation)HashLSH.o

CubeHashFuncsLocation = $(sourcePath)HashFunctions/
CubeHashFuncsObjects = $(CubeHashFuncsLocation)HashHC.o

InterfacesLocation = $(sourcePath)
InterfacesObjects = $(InterfacesLocation)HashInterface.o $(InterfacesLocation)Hhashing.o 

CommonObejects =  $(mainObjects) $(DataStructuresObjects) $(UtilitiesObjects) $(InterfacesObjects) $(LSHHashFuncsObjects) $(CubeHashFuncsObjects) $(OperationControllersObjects)

includePaths = -I./  -I$(DataStructuresLocation) -I$(CubeHashFuncsLocation) -I$(LSHHashFuncsLocation) -I$(UtilitiesLocation) -I$(OperationControllersLocation)

all: $(lsh_exe) $(cube_exe) $(cluster_exe)

$(lsh_exe): $(CommonObejects) $(LshObjects)
	$(CC) $(cflags) $(includePaths) $(CommonObejects) $(LshObjects) -o $@

$(cube_exe): $(CommonObejects) $(CubeObjects)
	$(CC) $(cflags) $(includePaths) $(CommonObejects) $(CubeObjects) -o $@

$(cluster_exe): $(CommonObejects) $(ClusterObjects)
	$(CC) $(cflags) $(includePaths) $(CommonObejects) $(ClusterObjects) -o $@

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

rlsh: $(lsh_exe)
	./$(lsh_exe) $(lsh_flags)

rcube: $(cube_exe)
	./$(cube_exe) $(cube_flags)

rcluster: $(cluster_exe)
	./$(cluster_exe) $(cluster_flags)

val: $(cluster_exe)
	valgrind $(valgrindFlags) ./$(cluster_exe) $(cluster_flags)

gdb: $(cluster_exe)
	gdb ./$(cluster_exe)

clean:
	rm $(CommonObejects) $(lsh_exe) $(cube_exe) $(cluster_exe)