CC = g++
cflags = -Wall -g3 -std=c++14

valgrindFlags = --leak-check=full

search_flags_lsh = -i ../nasd_input.csv -q ../nasd_query.csv -o ./output.LSHsearch -N 3 -R 300 -k 3 -L 3 -algorithm LSH
search_flags_cube = -i ../nasd_input.csv -q ../nasd_query.csv -o ./output.CUBEsearch -N 3 -R 300 -k 3 -probes 3 -M 150 -algorithm Hypercube
search_flags_dfr = -i ../nasd_input.csv -q ../nasd_query.csv -o ./output.DFRsearch -N 3 -R 300 -k 3 -L 3 -algorithm Frechet -metric discrete -delta 6.2
search_flags_cfr = -i ../nasd_input.csv -q ../nasd_query.csv -o ./output.CFRsearch -N 3 -R 300 -k 3 -L 3 -algorithm Frechet -metric continuous -delta 6.2 -no_brute_force

cluster_flags_llov = -i ../nasd_input.csv -c ./cluster.conf -o ./output.clusteringLLOV -assignment Classic -update Mean_Vector
cluster_flags_llof = -i ../nasd_input.csv -c ./cluster.conf -o ./output.clusteringLLOF -assignment Classic -update Mean_Frechet
cluster_flags_lshf = -i ../nasd_input.csv -c ./cluster.conf -o ./output.clusteringLSHF -assignment LSH -update Mean_Vector
cluster_flags_lshff = -i ../nasd_input.csv -c ./cluster.conf -o ./output.clusteringLSHFF -assignment LSH_Frechet -update Mean_Frechet
cluster_flags_hc = -i ../nasd_input.csv -c ./cluster.conf -o ./output.clusteringHC -assignment Hypercube -update Mean_Vector

search_exe = search
cluster_exe = cluster
unitTest_exe = unitTest

sourcePath = ./

OperationControllersLocation = $(sourcePath)OperationControllers/
OperationControllersObjects = $(OperationControllersLocation)MappingMethod.o $(OperationControllersLocation)LSH.o $(OperationControllersLocation)HyperCube.o $(OperationControllersLocation)ClusterInterface.o $(OperationControllersLocation)ClusterLSH.o $(OperationControllersLocation)ClusterHC.o $(OperationControllersLocation)ClusterLloyds.o

ObjectsA = $(sourcePath)mainA.cpp

ObjectsB = $(sourcePath)mainCluster.cpp

ObjectsUnitTesting = $(sourcePath)UnitTestingMain.cpp

DataStructuresLocation = $(sourcePath)DataStructures/
DataStructuresObjects = $(DataStructuresLocation)HashTable.o $(DataStructuresLocation)SimpleList.o $(DataStructuresLocation)CurveArray.o

UtilitiesLocation = $(sourcePath)Utilities/
UtilitiesObjects = $(UtilitiesLocation)Utilities.o $(UtilitiesLocation)FileReader.o $(UtilitiesLocation)TimeSeries.o

LSHHashFuncsLocation = $(sourcePath)HashFunctions/
LSHHashFuncsObjects = $(LSHHashFuncsLocation)HashLSH.o

CubeHashFuncsLocation = $(sourcePath)HashFunctions/
CubeHashFuncsObjects = $(CubeHashFuncsLocation)HashHC.o

HashInterfacesLocation = $(sourcePath)HashFunctions/
HashInterfacesObjects = $(HashInterfacesLocation)HashInterface.o $(HashInterfacesLocation)HashDF.o $(HashInterfacesLocation)Hhashing.o

CommonObejects =  $(DataStructuresObjects) $(UtilitiesObjects) $(HashInterfacesObjects) $(LSHHashFuncsObjects) $(CubeHashFuncsObjects) $(OperationControllersObjects) $(FredLibObjects)

FREDCC = g++
fred_cflags = -march=native -Ofast -static-libgcc -static-libstdc++ -std=c++14 -fpermissive -fPIC -ffast-math -fno-trapping-math -ftree-vectorize

FredLibLocation = $(sourcePath)FredLib/
FredLibObjects = $(FredLibLocation)config.o $(FredLibLocation)curve.o $(FredLibLocation)frechet.o $(FredLibLocation)interval.o $(FredLibLocation)point.o $(FredLibLocation)simplification.o $(FredLibLocation)FredLibWrapper.o

includePaths = -I./  -I$(DataStructuresLocation) -I$(CubeHashFuncsLocation) -I$(LSHHashFuncsLocation) -I$(UtilitiesLocation) -I$(OperationControllersLocation)

all: $(search_exe) $(cluster_exe)

$(search_exe): $(CommonObejects) $(ObjectsA)
	$(CC) $(cflags) $(includePaths) $(CommonObejects) $(ObjectsA) -o $@

$(cluster_exe): $(CommonObejects) $(ObjectsB)
	$(CC) $(cflags) $(includePaths) $(CommonObejects) $(ObjectsB) -o $@

$(unitTest_exe): $(CommonObejects) $(ObjectsUnitTesting)
	$(CC) $(cflags) $(includePaths) $(CommonObejects) $(ObjectsUnitTesting) -o $@ -lgtest -lpthread 

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

$(FredLibLocation)%.o: $(FredLibLocation)%.cpp
	$(FREDCC) $(fred_cflags) -c $< -o $@

$(OperationControllersLocation)%.o: $(OperationControllersLocation)%.cpp
	$(CC) $(cflags) $(includePaths) -c $< -o $@

rSlsh: $(search_exe)
	./$(search_exe) $(search_flags_lsh)

rScube: $(search_exe)
	./$(search_exe) $(search_flags_cube)

rSdfr: $(search_exe)
	./$(search_exe) $(search_flags_dfr)

rScfr: $(search_exe)
	./$(search_exe) $(search_flags_cfr)

rCllov: $(cluster_exe)
	./$(cluster_exe) $(cluster_flags_llov)

rCllof: $(cluster_exe)
	./$(cluster_exe) $(cluster_flags_llof)

rClshf: $(cluster_exe)
	./$(cluster_exe) $(cluster_flags_lshf)

rClshff: $(cluster_exe)
	./$(cluster_exe) $(cluster_flags_lshff)

rChc: $(cluster_exe)
	./$(cluster_exe) $(cluster_flags_hc)

rUnitTest: $(unitTest_exe)
	./$(unitTest_exe)

val: $(search_exe)
	valgrind $(valgrindFlags) ./$(search_exe) $(search_flags_cube)

gdb: $(cluster_exe)
	gdb ./$(cluster_exe)

clean:
	rm $(CommonObejects) $(search_exe) $(cluster_exe) $(unitTest_exe)
