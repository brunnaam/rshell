all: rshell

rshell: main.o
	mkdir bin
	g++ main.o -o ./bin/rshell

main.o: src/main.cpp
	g++ -c -Wall -Werror -ansi -pedantic src/main.cpp
				
