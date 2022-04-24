compile: src/main.cpp
	g++ -g -Wall -Wextra src/*.cpp -o main

.PHONY:run

run: main
	./main

.PHONY: clean
clean:
	rm -f main

.PHONY: memcheck
memcheck: main
	valgrind --tool=memcheck --leak-check=full ./main