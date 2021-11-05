CC = g++
cflags = -Wall -g3 -std=c++11

valgrindFlags = --leak-check=full

lsh_flags = -i ../input_small_id -q ../query_small_id -o ./output.lsh -N 3 -R 300 -k 4 -L 5
cube_flags = -i ../input_small_id -q ../query_small_id -o ./output.lsh -N 3 -R 300 -k 4 -M 5 1000 -probes 1

lsh_exe = lsh
cube_exe = cube

sourcePath = ./

mainObjects = $(sourcePath)main.o $(sourcePath)Point.o $(sourcePath)LSH.o

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

AllObejects =  $(mainObjects) $(DataStructuresObjects) $(UtilitiesObjects) $(InterfacesObjects) $(LSHHashFuncsObjects) $(CubeHashFuncsObjects)

includePaths = -I./  -I$(DataStructuresLocation) -I$(CubeHashFuncsLocation) -I$(LSHHashFuncsLocation) -I$(UtilitiesLocation)

all: $(lsh_exe) $(cube_exe)

$(lsh_exe): $(AllObejects)
	$(CC) $(cflags) $(includePaths) $(AllObejects) -o $@

$(cube_exe): $(AllObejects)
	$(CC) $(cflags) $(includePaths) $(AllObejects) -o $@

$(sourcePath)%.o: $(sourcePath)%.cpp
	$(CC) $(cflags) $(includePaths) -c $< -o $@

$(DataStructuresLocation)%.o: $(DataStructuresLocation)%.cpp
	$(CC) $(cflags) $(includePaths) -c $< -o $@

$(UtilitiesLocation)%.o: $(UtilitiesLocation)%.cpp
	$(CC) $(cflags) $(includePaths) -c $< -o $@

rlsh: $(lsh_exe)
	./$(lsh_exe) $(lsh_flags)

rcube: $(cube_exe)
	./$(cube_exe) $(cube_flags)

val: $(lsh_exe)
	valgrind $(valgrindFlags) ./$(lsh_exe) $(lsh_flags)

gdb: $(lsh_exe)
	gdb ./$(lsh_exe)

clean:
	rm $(AllObejects) $(lsh_exe) $(cube_exe)