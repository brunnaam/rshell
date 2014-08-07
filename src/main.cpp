#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

using namespace std;

//Search for '#' in str[] (user input)
void comment(char str[], int n) {
	for (int i = 0; i < n; i++) {
		if (str[i] == '#') {
			str[i] = '\0'; 
		}
	}
}

//Search for '&' in str[] (user input)
bool background(char str[], int n) {
	bool bg = false;
	for (int i = 0; i < n; i ++) {
		if (str[i] == '&') {
			bg = true;
		}
	}
	return bg;
}

void prompt(char str[], int n) {
	char host[50];
        if ((gethostname(host, sizeof(host)-1))==-1) {
		host[0] = 'h';
		host[1] = 'o';
		host[2] = 's';
		host[3] = 't';
		host[4] = '\0';
//		perror("hostname error");
	}

	char login[50];
	
	if (getlogin_r(login, sizeof(login)-1)-1) {
		login[0] = 'u';
		login[1] = 's';
		login[2] = 'r';
		login[3] = '\0';
//		perror("user error");
	}
	cout << login << "@" << host << "$ ";

        cin.getline(str,n);

	//Check if there is '&' in the input. If its true, the prompt will be called again
	if(background(str, n) == true) {
		cout << login  << "@" << host << "$ ";
		cin.getline(str,n);
	}

	//Check if some comment has been made in the input
        while (str[0] == '\0' || str[0] == '#') {
                cout << login << "@" << host << "$ ";
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

	//Initialize *result[] with the user input, splited by spaces 
	while (pch != NULL)
	{
	 	result[index] = pch;
	 	index++;
		pch = strtok (NULL, " \n");
	}

	result[index] = NULL;
	
	//Exits the rshell
	if (strcmp(result[0], "exit") == 0) {
		exit(0);
	}

	//Executes the command
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
