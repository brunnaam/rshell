#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

void comment(char str[], int n) {
	for (int i = 0; i < n; i++) {
		if (str[i] == '#') {
			str[i] = '\0';
		}
	}
}

void prompt(char str[], int n) {
	 char name[50];
         gethostname(name, sizeof name);

         cout << getlogin() << "@" << name << "$ ";

         cin.getline(str,n);

         while (str[0] == '\0' || str[0] == '#') {
                 cout << getlogin() << "@" << name << "$ ";
                 cin.getline(str,n);
	}										 
}

int main(int argc, char** argv) {


while (true) {

	char str[256];
	prompt(str, 256);
	
	
	char * result[256];
	char * pch;

	pch = strtok (str," ");

	int index = 0;

	comment(str, 256);

	while (pch != NULL)
	{
	 	result[index] = pch;
	 	index++;
		pch = strtok (NULL, " \n");
	}

	result[index] = NULL;
	
	
	if (strcmp(result[0], "exit") == 0) {

		exit(0);
	}

	
	int pid = fork();
	if (pid == 0) {
		if (execvp(result[0], result) == -1) {
			perror("execl failed");
		}
		exit(0);
	} else {
		wait(0);
	}

}

return 0;

}
