run: src/main.cpp
	g++ -g -Wall -Wextra src/main.cpp src/Instruction.cpp src/Auxiliar.cpp -o main
	./main