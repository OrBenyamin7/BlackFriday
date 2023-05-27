#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 256

int main(int argc,char* argv[]) //function that delete dir and its file
{
	int pid;

	char place[BUFFER_SIZE]; //the place where the dir is found

	strcpy(place,"/bin/");
	strcat(place,"rm");

	pid=fork();

	if(pid==0) //kid's process
	{
		execl(place,"rm","-rf","Black_Friday/",0); //calling the function deleting the dir and it's all contains
		exit(1);
	}

	if(pid==-1)
	{
		perror("Error \n");
		exit(1);
	}

	wait(); //waiting to the kid finished

	printf("GoodBye...\n"); //printing goodbye

	return 0;

	
}