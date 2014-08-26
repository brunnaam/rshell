rshell
======

SUMMARY

Rshell is a project for the cs100 course that performs some commands of a Linux Shell.

When started, rshell prints the user name and the hostname along with '$', telling the user that the shell is ready to receive input. It takes a command name and a list of arguments.


HOW TO BUILD

Rshell has a Makefile to be build. To use it, clone the repository and type 'make'. This will create a folder /bin, where the executable file, rshell, is. The bin folder will also have a ls executable, which is a implementation of the linux command 'ls'.


BUGS:

1. Commands like cd or '>' are not implemented. 
2. The 'echo' command works, but when echo "hello world" the output will be "hello world" instead of just hello world.
3. There is a limitation on the size of the user input, where the maximum number of characters supported are 256. If the user type more that it, the rshell will exit.
4. Don't handle files with spaces in the name.


LS BUGS

1. The flag -R identifies some directories and some files, but not all as should be.

2. The flag -R can be used with -l, but do not with -a

3. Ls does not list files of directories passed by argument.

4. Flags don't work for files passed by argument. A valid entry will be: bin/ls example.txt


RSHELL V.2 BUGS

1. The 'history' and 'ps aux' commands don't work while using piping.
2. The user input has to be separated by spaces, or it will return error, i.e: command [args] <'>>','>','<','|'> output.
3. It's not possible use input redirection, output redirection and piping in the same user input.
4. When piping, it is possible to use mutiple piping, but with input and output redirection it isn't.

RSHELL V.3 BUGS

1. Sometimes, input and output redirection don't work.
2. Ctrl+C will exit rshell if there is no process running in the shell.
