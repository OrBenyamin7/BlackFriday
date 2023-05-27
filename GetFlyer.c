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
    int fd_file, rbytes, wbytes;
	char buff[BUFF];
	char nameComp[MAX];
	char path_nameComp[MAX]="/home/braude/Black_Friday/";

	if (argc>2)
		Error("wrong number of paramaters");

	strcat(path_nameComp, argv[1]);
	strcat(path_nameComp, ".txt");
	
	if ((fd_file = open(path_nameComp, O_RDONLY)) == -1){ //open company's flyer
		//printf("Flyer is not Found!\n"); 
        return(-1);
	}

	if ((rbytes = read(fd_file, buff, BUFF)) == -1){ //reading from company's flyer
		perror("read 1"); 
        return(-1);
	}

	while(rbytes>0){
		if ((wbytes = write(1, buff, rbytes)) == -1){ //write to the screen
			perror("write"); return(-1);
		}

		if (wbytes != rbytes){ //in case what readed isnt what written
			fprintf(stderr, "bad write\n"); return(-1);
		}
		if ((rbytes = read(fd_file, buff, BUFF)) == -1){ // keep reading from the file
			perror("read 2"); return(-1);
		}
	}

	close(fd_file);
    return 0;
}