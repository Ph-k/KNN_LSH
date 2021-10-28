CC = g++
cflags = -Wall -g3

valgrindFlags = --leak-check=full

pflags = -i ../input_small_id -q ../query_small_id -o ./output.lsh -N 3 -R 2 -k 4 -L 5

program = lsh

sourcePath = ./

mainObjects = $(sourcePath)main.o

DataStructuresLocation = $(sourcePath)
DataStructuresObjects = $(DataStructuresLocation)HashTable.o $(DataStructuresLocation)SimpleList.o

UtilitiesLocation = $(sourcePath)
UtilitiesObjects = $(UtilitiesLocation)Utilities.o $(UtilitiesLocation)FileReader.o  $(UtilitiesLocation)Point.o $(UtilitiesLocation)LSH.o

HashFuncsLocation = $(sourcePath)
HashFuncsObjects = $(HashFuncsLocation)Ghashing.o $(HashFuncsLocation)Hhashing.o

AllObjects = $(mainObjects) $(DataStructuresObjects) $(UtilitiesObjects) $(HashFuncsObjects)

$(program): $(AllObjects)
	$(CC) $(cflags) $(AllObjects) -o $@


$(sourcePath)%.o: $(sourcePath)%.cpp
	$(CC) $(cflags) -c $< -o $@

run: $(program)
	./$(program) $(pflags)

val: $(program)
	valgrind $(valgrindFlags) ./$(program) $(pflags)

clean:
	rm $(AllObjects) $(program)
