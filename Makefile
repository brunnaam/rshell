all: rshell ls

bin:
	[ ! -d bin ] && mkdir bin

rshell: bin main.o
	g++ main.o -o ./bin/rshell

ls: bin ls.o
	g++ ls.o -o ./bin/ls

main.o: src/main.cpp
	g++ -c -Wall -Werror -ansi -pedantic src/main.cpp

ls.o: src/ls.cpp
	g++ -c -Wall -Werror -ansi -pedantic src/ls.cpp
