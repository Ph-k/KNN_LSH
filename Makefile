CC = g++
cflags = -Wall -g3 -std=c++11

valgrindFlags = --leak-check=full

lsh_flags = -i ../input_small_id -q ../query_small_id -o ./output.lsh -N 3 -R 300 -k 4 -L 5
cube_flags = -i ../input_small_id -q ../query_small_id -o ./output.lsh -N 3 -R 300 -k 4 -M 5 1000 -probes 1

lsh_exe = lsh
cube_exe = cube

sourcePath = ./

mainObjects = $(sourcePath)main.o

DataStructuresLocation = $(sourcePath)
DataStructuresObjects = $(DataStructuresLocation)HashTable.o $(DataStructuresLocation)SimpleList.o

UtilitiesLocation = $(sourcePath)
UtilitiesObjects = $(UtilitiesLocation)Utilities.o $(UtilitiesLocation)FileReader.o  $(UtilitiesLocation)Point.o $(UtilitiesLocation)LSH.o

HashFuncsLocation = $(sourcePath)
HashFuncsObjects = $(HashFuncsLocation)HashLSH.o $(HashFuncsLocation)Hhashing.o $(HashFuncsLocation)HashHC.o

InterfacesLocation = $(sourcePath)
InterfacesObjects = $(InterfacesLocation)HashInterface.o

AllObjects = $(mainObjects) $(DataStructuresObjects) $(UtilitiesObjects) $(HashFuncsObjects) $(InterfacesObjects)

$(lsh_exe): $(AllObjects)
	$(CC) $(cflags) $(AllObjects) -o $@

$(cube_exe): $(AllObjects)
	$(CC) $(cflags) $(AllObjects) -o $@

$(sourcePath)%.o: $(sourcePath)%.cpp
	$(CC) $(cflags) -c $< -o $@


run lsh: $(lsh_exe)
	./$(lsh_exe) $(lsh_flags)

run cube: $(cube_exe)
	./$(cube_exe) $(cube_flags)

val: $(lsh_exe)
	valgrind $(valgrindFlags) ./$(lsh_exe) $(lsh_flags)

gdb: $(lsh_exe)
	gdb ./$(lsh_exe)

clean:
	rm $(AllObjects) $(lsh_exe)
