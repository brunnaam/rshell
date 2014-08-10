rshell
======

SUMMARY

Rshell is a project for the cs100 course that performs some commands of a Linux Shell.

When started, rshell prints the user name and the hostname along with '$', telling the user that the shell is ready to receive input. It takes a command name and a list of arguments.


HOW TO BUILD

Rshell has a Makefile to be build. To use it, clone the repository and type 'make'. This will create a folder /bin, where the executable file, rshell, is. 


BUGS:

1. Commands like cd or '>' are not implemented. 
2. The 'echo' command works, but when echo "hello world" the output will be "hello world" instead of just hello world.
3. There is a limitation on the size of the user input, where the maximum number of characters supported are 256. If the user type more that it, the rshell will exit.
4. Don't handle files with spaces in the name.

