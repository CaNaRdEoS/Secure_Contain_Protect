all: build run clean clear

build:
	g++ main.cpp -lraylib -o scp
run:
	./scp
clean:
	rm scp
clear:
	clear 