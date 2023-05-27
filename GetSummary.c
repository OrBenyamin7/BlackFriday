#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 256
#define BUFF 1024

//print an error to the screen and close the program
void Error(char* msg) { fprintf(stderr, "Error:%s\n", msg); exit(1); }

int main(int argc, char* argv[]) {
    int fd_file, rbytes, wbytes, i=0, j=0;
	int pid, pid1;
	char buff[BUFF];
	char *nameComp[MAX];
	char path_nameComp[MAX]="/home/braude/Black_Friday/";
	char path_GetNumComp[MAX]="/home/braude/";
	char path_OrderNum[MAX]="/home/braude/";
	char *line;

	if (argc!=1)
		Error("wrong number of paramaTers");

    strcat(path_nameComp, "camp_partic.txt");
	strcat(path_GetNumComp, "GetNumComp");
	strcat(path_OrderNum, "GetOrderNum");

	pid=fork();
	if (pid==0){ // The Child Process 
        if (execv(path_GetNumComp, 0) == -1){		 
            perror("error"); 
            return(-1); 
        } 
    }

    else if (pid==-1){
		perror("error"); //fork error
		return(-1);
	}
	else{ //parent Process
		wait();
		if ((fd_file = open(path_nameComp, O_RDONLY)) == -1){ //open camp partics file
			perror("open"); 
        	return(-1);
		}

		if ((rbytes = read(fd_file, buff, BUFF)) == -1){ //reading from camp partics file
			perror("read 1"); 
        	return(-1);
		}

        while(rbytes>0){
			line = strtok(buff, "\n"); 
			while( (line != NULL) && (i<MAX) ){
				nameComp[i]=line;
				line = strtok(NULL, "\n");	
				i = i+1;
			}
			if ((rbytes = read(fd_file, buff, BUFF)) == -1){ // keep reading from the file
				perror("read 2"); return(-1);
			}
		}

        while (i-1>0){ 
			pid1=fork();
			if (pid1==0){ //child Process
				if(execl(path_OrderNum, "GetOrderNum", nameComp[j], 0) == -1){ 
					perror("error"); 
            		return(-1); 
				}
			} 
			else if (pid1==-1){
				perror("error"); //fork error
				return(-1);
			}

			else{ //parent Process
				wait();
				i=i-1;
				j=j+1;
			} 
		}
	}

    close(fd_file);
    return 0;
}