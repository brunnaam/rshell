all: rshell ls

rshell: main.o
	mkdir bin
	g++ main.o -o ./bin/rshell

ls: ls.o
	g++ ls.o -o ./bin/ls

main.o: src/main.cpp
	g++ -c -Wall -Werror -ansi -pedantic src/main.cpp

ls.o: src/ls.cpp
	g++ -c -Wall -Werror -ansi -pedantic src/ls.cpp

