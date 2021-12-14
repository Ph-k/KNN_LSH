CC = g++
cflags = -march=native -Ofast -std=c++14 -fpermissive -fPIC -ffast-math -fno-trapping-math -ftree-vectorize

objects = config.o curve.o frechet.o interval.o point.o simplification.o
LibLocation = ./

all: $(objects)

$(LibLocation)%.o: $(LibLocation)%.cpp $(LibLocation)%.hpp
	$(CC) $(cflags) -c $< -o $@

clean:
	rm $(objects)