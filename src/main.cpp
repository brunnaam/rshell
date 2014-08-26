#include <iostream>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>
#include <stdlib.h>
#include <fcntl.h>
#include <dirent.h>
#include <signal.h>

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
			str[i] = '\0';
		}
	}
	return bg;
}

int input(char *str[], int n) {
	int res1 = -1;
        for (int i = 0; i < n; i ++) {
		res1 = i;
		if (strstr(str[i], "<") != NULL) {
			break;
                } else {
			res1 = -1;
		}
        }
        return res1;
}


int output(char *str[], int n) {
	int res = -1;
	for (int i = 0; i < n-1; i++) {
		res = i;
		if ((strstr(str[i], ">") != NULL) && (strstr(str[i+1], ">") == NULL)) {
			break;
		} else {
			res = -1;
		}
	}
	return res;
}


int output2(char *str[], int n) {
        int res = -1;
	for (int i = 0; i < n-1; i++) {
		res = i;
		if (strstr(str[i], ">>") != NULL) {
			str[i] = " ";
			break;
		} else {
			res = -1;
		}
	}
	return res;
}


int findPipe(char *str[], int n) {
	int res = -1;
        for (int i = 0; i < n; i++) {
                res = i;
                if (strstr(str[i], "|") != NULL) {
                        break;
                } else {
                        res = -1;
                }
        }
        return res;
}

void myexec(char **path, char *result[]) {
	int erro;
	char *p = new char[1024];

        for(int i = 0; path[i] != NULL; i++) {
        	strcpy(p, path[i]);
                strcat(p, "/");
                strcat(p, result[0]);

                cout << "";
                erro = execv(p, result);
        }

	if (erro == -1) perror("Execv failed.");

}

void in(int index, char *str[], char **path) {

	int fd = open(str[index+1],O_RDONLY );
	if(fd == -1)
		perror("open failed");
	if(-1 == dup2(fd,0))
		perror("dup2 failed");

	if (index == 1) {
//                if (execvp(str[0], NULL) == -1) {
//                        perror("execvp failed");
//                }
		myexec(path, str);
        } else {

                char *newStr[256];
                for (int i = 0; i < index; i++) {
                        newStr[i] = str[i];
                } 
               	//if (execvp(str[0], newStr) == -1) perror("execv failed");
		myexec(path, newStr);
        }

}

void out(bool append, int index, char *str[]) {

	int fd;

	if (append) {

		fd=open(str[index+1],O_RDWR|O_CREAT|O_APPEND, 0666);
		if (fd==-1) {
			perror("open failed");
			exit(1);
		}

	} else {
		fd=open(str[index+1],O_RDWR|O_CREAT, 0666);
		if (fd==-1) {
			perror("open failed");
			exit(1);
		}
	}


	if (-1 == dup2(fd,1)) perror ("dup2 failed"); 

	if (index == 1) {
		if (execvp(str[0], NULL) == -1) {
                        perror("execvp failed");
                }
	} else {

		char *newStr[256];
		for (int i = 0; i < index; i++) {
			newStr[i] = str[i];
		} 
		if (execvp(str[0], newStr) == -1) perror("execv failed");
	}

}


void piping2(char *str[], int len) {

	int pid = fork();
	if(pid == -1) {
		perror("Fork failed");
	}
	else if(pid == 0) { 
		if (len == 1) {
			if(execvp(str[0], NULL) == -1) {
                	        perror("Execvp failed.");
	                }
        	        exit(1);

		} else {
			if(execvp(str[0], str) == -1) {
				perror("Execvp failed.");
			}
			exit(1);
		}
	} else {
		if(wait(0) == -1) {
         	       perror("Wait failed");
        	}
	}

}


void piping(int index, int strLength, char *str[]) {

	char *part1[256] = {0};
	char *part2[256] = {0};

	int end = 0;
	int end2 = 0;

        for (int i = 0; i < index; i++) {
                part1[i] = str[i];
		end = i;
        }
	part1[end+1] = '\0';
	for (int i = index+1; i < strLength; i++) {
		part2[end2] = str[i];
		end2++;
	}
	part2[end2+1] = '\0';

	int fd[2];
	if (pipe(fd)==-1) {
		perror ("pipe failed");
		exit(1);
	}


	int pid = fork();

	if (pid == -1) {

		perror("Fork failed.");

	} else if(pid == 0) {
		if(close(fd[0]) == -1) {
			perror("Close failed.");
		}
		if(dup2(fd[1],1) == -1) {
			perror("Dup failed.");
		}
		if(-1 == execvp(part1[0], part1)) {
			perror("Execvp failed.");
		}
		exit(1);

	} else { 

		int c_in = dup(0);
		if(c_in == -1) {
			perror("Dup failed.");
		}
		if(close(fd[1]) == -1) {
			perror("Close failed.");
		}
		if(dup2(fd[0],0) == -1) {
			perror("Dup failed.");
		}
		if(wait(0) == -1) {
			perror("Wait failed");
		}
	
		int chain = findPipe(part2, end2);
		if (chain != -1) {
			piping(chain, end2, part2);
		} else {
			piping2(part2, end2);
		}

		if (dup2(c_in,0) == -1) {
			perror("Dup failed.");
		}
		cout << flush;	
	}
}

void prompt(char str[], int n) {
	char host[50];
        if ((gethostname(host, sizeof(host)-1))==-1) {
		host[0] = 'h';
		host[1] = 'o';
		host[2] = 's';
		host[3] = 't';
		host[4] = '\0';
		perror("Error getting hostname");
	}

	char login[50];
	
	if (getlogin_r(login, sizeof(login)-1)) {
		login[0] = 'u';
		login[1] = 's';
		login[2] = 'r';
		login[3] = '\0';
		perror("Error getting user login");
	}

	cout << login << "@" << host << "$ ";

        cin.getline(str,n);

	//Check if some comment has been made in the input
        while (str[0] == '\0' || str[0] == '#') {
                cout << login << "@" << host << "$ ";
                cin.getline(str,n);
	}										 
}


int mystrcmp(char const *p, char const *q)
{
        int i = 0;
        for(i = 0;q[i];i++) {
                if(p[i] != q[i])
                        return -1;
        }
        return 0;
}

void handler(int sig) {
      	signal(SIGINT,SIG_IGN);
}


int main(int argc, char** argv) {

int index2 = 0;
string key = "PATH";
char *pch2;
char *env = getenv(key.c_str());
pch2 = strtok(env, ":");
char **path;
path = new char*[2048];

while (pch2 != NULL) {
        path[index2] = pch2;
        index2++;
        pch2 = strtok (NULL, ":\n");
}

path[index2] = NULL;


while (true) {

	char currentDir[1024];
        if(!getcwd(currentDir, 1024)) perror("Getcwd failed. ");


	char str[256];
	prompt(str, 256);
	
	
	char * result[256];
	char * pch;

	pch = strtok (str," ");

	int index = 0;

	comment(str, 256);
	bool bgProcess = background(str, 256);
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


	signal(SIGINT, handler);

        if (mystrcmp(result[0],"cd") == 0) {
                if (index == 1) {
                        char *home = getenv("HOME");
                        if(chdir(home) == -1) perror("Chdir failed. ");
                } else {
                        if(-1 == chdir(result[1])) perror("Chdir failed. ");
                }
        }



	//Executes the command
	int pid = fork();
	
	if (pid < 0) {
		perror("fork failed");
		exit(1);
	}
	if (pid == 0) {

		int inputVar = input(result, index);
		int outputVar = output(result, index);
		int output2Var = output2(result, index);
		int pipeVar = findPipe(result, index);

		if (inputVar != -1) { 
			in(inputVar, result, path);
		}
		else if (output2Var != -1) { 
			out(true, output2Var, result);
		}
		else if (outputVar != -1) { 
			out(false, outputVar, result);
		}
                else if (pipeVar != -1) { 
			piping(pipeVar, index, result);
		}
		else {

			if (mystrcmp(result[0],"cd") != 0) {
                                int erro;
                                char *p = new char[1024];

				erro = execv(result[0], result);

                                for(int i = 0; path[i] != NULL; i++) {
                                        strcpy(p, path[i]);
                                        strcat(p, "/");
                                        strcat(p, result[0]);

                                        cout << "";
                                        erro = execv(p, result);
                                }

                                if (erro == -1) perror("execv failed");
                                delete[] p;
			//	myexec(path, result);
                        }

		}
		exit(0);

	} else {
		if (!bgProcess) {
			if (wait(0) == -1) {
				perror("wait failed");
			}
		}
	}
	cout << currentDir << endl;
}

return 0;

}
