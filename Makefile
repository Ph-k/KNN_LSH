CC = g++
cflags = -Wall -g3 -std=c++11

valgrindFlags = --leak-check=full

search_flags = -i ../input.csv -q ../query.csv -o ./output.search -N 3 -R 300 -k 4 -L 5 -algorithm LSH

search_exe = search

sourcePath = ./

OperationControllersLocation = $(sourcePath)OperationControllers/
OperationControllersObjects = $(OperationControllersLocation)LSH.o $(OperationControllersLocation)HyperCube.o $(OperationControllersLocation)ClusterComplex.o

Objects = $(sourcePath)mainA.cpp

DataStructuresLocation = $(sourcePath)DataStructures/
DataStructuresObjects = $(DataStructuresLocation)HashTable.o $(DataStructuresLocation)SimpleList.o

UtilitiesLocation = $(sourcePath)Utilities/
UtilitiesObjects = $(UtilitiesLocation)Utilities.o $(UtilitiesLocation)FileReader.o $(UtilitiesLocation)Point.o

LSHHashFuncsLocation = $(sourcePath)HashFunctions/
LSHHashFuncsObjects = $(LSHHashFuncsLocation)HashLSH.o

CubeHashFuncsLocation = $(sourcePath)HashFunctions/
CubeHashFuncsObjects = $(CubeHashFuncsLocation)HashHC.o

HashInterfacesLocation = $(sourcePath)HashFunctions/
HashInterfacesObjects = $(HashInterfacesLocation)HashInterface.o $(HashInterfacesLocation)Hhashing.o 

CommonObejects =  $(DataStructuresObjects) $(UtilitiesObjects) $(HashInterfacesObjects) $(LSHHashFuncsObjects) $(CubeHashFuncsObjects) $(OperationControllersObjects)

includePaths = -I./  -I$(DataStructuresLocation) -I$(CubeHashFuncsLocation) -I$(LSHHashFuncsLocation) -I$(UtilitiesLocation) -I$(OperationControllersLocation)

all: $(search_exe)

$(search_exe): $(CommonObejects) $(Objects)
	$(CC) $(cflags) $(includePaths) $(CommonObejects) $(Objects) -o $@

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

rsearch: $(search_exe)
	./$(search_exe) $(search_flags)

val: $(search_exe)
	valgrind $(valgrindFlags) ./$(search_exe) $(search_flags)

gdb: $(search_exe)
	gdb ./$(search_exe)

clean:
	rm $(CommonObejects) $(search_exe)
