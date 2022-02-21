compile: src/main.cpp
	g++ -g -Wall -Wextra src/*.cpp -o main

.PHONY:run

run: main
	./main