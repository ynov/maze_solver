# Lazy Makefile
# Just compile all the .cpp!

CPP=g++
BIN=maze_solver

all:
	$(CPP) src/*.cpp -o bin/$(BIN)

run: all
	bin/$(BIN) -sv maps/map_1.txt

clean:
	rm -vf bin/*
	touch bin/.empty
