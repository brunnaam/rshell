#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <dirent.h>
#include <errno.h>
#include <iostream>
#include <grp.h>
#include <iomanip>

using namespace std;

void details(struct stat fileStat) {
    
    struct passwd *pw;
    if(!(pw = getpwuid(fileStat.st_uid)))
	perror("error: getpwuid. ");
    struct group *pg;
    if(!(pg = getgrgid(fileStat.st_gid)))
	perror("Erro: getgrgid ");

    char mBuf[200];

    struct tm *mytm = localtime(&fileStat.st_mtime);
    strftime(mBuf, 18, "%b %d %I:%M ", mytm);

    printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
    printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
    printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
    cout << " " << fileStat.st_nlink << " ";
    cout << pw->pw_name <<" ";
    cout << pg->gr_name << " ";
    cout << setw(7) << right << fileStat.st_size<<" ";

    cout << " " << mBuf;

}


void rCommand(dirent *entry, string curDir) {

	if (entry->d_type == DT_DIR){	// if entry is a directory
		string fname = entry->d_name;
		if (fname != "." && fname != "..")
			rCommand(entry, entry->d_name);
	}
	else if (entry->d_type == DT_REG){	// if entry is a regular file
		string fname = entry->d_name;	// filename
		cout << entry->d_name << endl;

	}

}


bool ignore(string command) { //if the file starts with .
	bool ignore = false;
	char c = '.';
	char init = command[0];
	if (init == c) {
		ignore = true;
	}
	return ignore;
} 

void lCommand(string name, struct stat fileStat, bool a) {
	if (a) {
		details(fileStat);
                cout << name << endl;
	} else {
	       	if (!ignore(name)) {
	                details(fileStat);
		      	cout << name << endl;
		}
	}

}

void aCommand(string name, struct stat fileStat) {
	cout << name << endl;
}

//tratar chamadas a files e dir
void ls(char **argv, int argc, string directory) {
	//char *dirName = ".";
	
        DIR *dirp;
        if (!(dirp = opendir(directory.c_str())))
        {
                cerr << "Error(" << errno << ") opening " << directory.c_str() << std::endl;
                //return errno;
		return;
        }

        dirent *direntp;
        while ((direntp = readdir(dirp))) {

                struct stat fileStat;
                if(stat(direntp->d_name,&fileStat) < 0)
                        return;

		bool flagA = false;
		bool flagL = false;
		bool flagR = false;

		for (int i = 1; i < argc; i++) {
			if (strcmp(argv[i], "-a") == 0) { flagA = true; }
			if (strcmp(argv[i], "-l") == 0) { flagL = true; }
			if (strcmp(argv[i], "-R") == 0) { flagR = true; }
			if ((strcmp(argv[i], "-al") == 0) || (strcmp(argv[i], "-la") == 0) ) { flagA = true; flagL = true;}
			if ((strcmp(argv[i], "-aR") == 0) || (strcmp(argv[i], "-Ra") == 0)) { flagR = true; flagA = true; }
			if ((strcmp(argv[i], "-lR") == 0) || (strcmp(argv[i], "-Rl") == 0)) { flagR = true; flagL = true; }
			if ((strcmp(argv[i], "-alR") == 0) || (strcmp(argv[i], "-aRl") == 0) || (strcmp(argv[i], "-lRa") == 0) || (strcmp(argv[i], "-laR") == 0) || (strcmp(argv[i], "-Ral") == 0) || (strcmp(argv[i], "-Rla") == 0) ) { flagA = true; flagL = true; flagR = true;}
		}

		//ls
		if (argc == 1) {
               	        if (!ignore(direntp->d_name)) {
                       	        cout << direntp->d_name << endl;
       	                } 
                } else {
		
			//-a
			if(flagA && !flagL && !flagR) { aCommand(direntp->d_name, fileStat); }
			//-l
			if(!flagA && flagL && !flagR) {  lCommand(direntp->d_name, fileStat, false); }
			//-R
			if(!flagA && !flagL && flagR) { 
				if (direntp->d_type == DT_DIR){   // if entry is a directory
                           		string fname = direntp->d_name;
                                        if (fname != "." && fname != "..") {
						cout << "\n./" << fname << endl;
						ls(argv, argc, direntp->d_name);
                                              	// rCommand(entry, entry->d_name);
					}
                               	}
                        	else if (direntp->d_type == DT_REG){      // if entry is a regular file
                	                string fname = direntp->d_name;   // filename
        	                        cout << direntp->d_name << endl;
	
        	                }
	
		
			} 
	
			//-l -a 
			if(flagA && flagL && !flagR) { lCommand(direntp->d_name, fileStat, true); }
			// -a -R
			if(flagA && !flagL && flagR) {}
			// -l -R
			if(!flagA && flagL && flagR) {

				if (direntp->d_type == DT_DIR){   // if entry is a directory
                                        string fname = direntp->d_name;
                                        if (fname != "." && fname != "..") {
                                                
						lCommand(direntp->d_name, fileStat, false);
						cout << "" << endl;
                                                ls(argv, argc, direntp->d_name);
                                        }
                                }
                                else if (direntp->d_type == DT_REG){      // if entry is a regular file
                                        string fname = direntp->d_name;   // filename
					lCommand(direntp->d_name, fileStat, false);
                                }	

			}
			// -l -a -R
			if(flagA && flagL && flagR) {}

			if (direntp->d_type == DT_REG) {
                                if ((strcmp(argv[1], direntp->d_name) == 0) && !ignore(direntp->d_name)) {
                                        cout << direntp->d_name << endl;
                                }
                        }

		}
        }

        closedir(dirp);

}
 
int main(int argc, char **argv)
{
 
	string currDir = get_current_dir_name();

	ls(argv, argc, currDir);

	return 0;
}
