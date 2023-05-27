#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h> // use directory functions

#define BUFFER_SIZE 256

//print an error to the screen and close the program
void Error(char* msg) 
{
    fprintf(stderr, "Error:%s\n", msg);
	exit(1);
}

//find the number of orders in an existing company
int main(int argc, char* argv[]) 
{
	int numOfFiles = 0;
	char resOrderDir[BUFFER_SIZE];
	struct dirent* dp;
	DIR* fd;

	if (argc != 2) Error("Invalid given arguments\n");

    sprintf(resOrderDir, "/home/braude/Black_Friday/%s_Order", argv[1]); //find the accordingly folder
	// opening order directory
	if ((fd = opendir(resOrderDir)) == NULL) {
		fprintf(stderr, "Company Not Found!\n");
		return -1;
	}

    while ((dp = readdir(fd)) != NULL) { //while there is still files in the folder
        if (strcmp(dp->d_name, ".") == 0 ||strcmp(dp->d_name, "..") == 0) continue;
        else
            numOfFiles++;
	}
	closedir(fd);
	// printing num of orders
	printf("%s ---- %d Orders\n", argv[1], numOfFiles);
	return 0;
}