#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define BUFFER_SIZE 256

void createDirOrder(); //function that create new dir

int main(int argc,char* argv[])
{
	char input[BUFFER_SIZE]; //buffer for getting input from user

	char place[BUFFER_SIZE];

	char* printToFile;

	char* arrayOfData[3];

	int pid, i,status;

	int fd_to, wbytes;

	createDirOrder(); //calling function to create new dir
	
	//opening new file for saving companies names
	if( ( fd_to = open( "/home/braude/Black_Friday/camp_partic.txt", O_WRONLY | O_CREAT, 0664  ) ) == -1 )
	{ 
		perror( "open from" );
	 
		return -1 ; 
	}
	
	printf("//Black_Friday dir created, camp_partic.txt created\n");

	printf("AdvShell>");

	fgets(input, sizeof input,stdin);

	while(strcmp(input,"exit\n")!=0) //doing loop until user enter "exit""
	{

		if(strcmp(input,"\n")!=0) //in case not empty line
		{

			input[strlen(input)-1] = '\0'; //ingoring the \n in the end of the input

			for(i = 0; i < 3; i++)
	 		 	arrayOfData[i]=NULL;

    		arrayOfData[0] = strtok(input," ");

			for(i = 1; i < 3; i++)  //saving the arguments to array
			{
     		 	arrayOfData[i]=strtok(NULL," ");
			}

			if(strtok(NULL," ")!=NULL)
			{
				printf("Error\n");
			}

			else
			{

				pid=fork();	//creating new process

				switch(pid)
				{
					case -1: //in case the fork failed
						perror("Error \n");
						exit(1);
			
					case 0: //in case the son process
	
						if((strcmp(arrayOfData[0],"CreateFlyer")==0) || (strcmp(arrayOfData[0],"GetFlyer")==0)
							|| (strcmp(arrayOfData[0],"MakeOrder")==0) || (strcmp(arrayOfData[0],"GetNumComp")==0)
								|| (strcmp(arrayOfData[0],"GetOrderNum")==0)
								|| (strcmp(arrayOfData[0],"GetSummary")==0))
						{
								strcpy(place,"/home/braude/");
						}
						else
							strcpy(place,"/bin/");
	

						strcat(place,input);

						execl(place,arrayOfData[0],arrayOfData[1],arrayOfData[2],0); //calling the function
				
						printf("Not Supported \n");

						exit(1);

					default:
						break;
				}

				wait(&status);
			}
		}
		printf("AdvShell>");

		fgets(input, sizeof input,stdin);
		
	}

	close(fd_to); //closing the file

	pid=fork();	//creating new process

	if(pid==0)
	{
		strcpy(place,"/home/braude/");
		strcat(place,"Exit");

		execl(place,"Exit",0);

		printf("Exit order failed \n");

		exit(1);
	}

	else if(pid==-1) //failed to fork
	{
		perror("error"); //fork error
		return(-1);
	}

	else //parent process
	{
		wait();
	}

	return 0;

}

void createDirOrder() //function that create dir
{
	//creating the directory with the given company name
	char titleOrder[256] = "/home/braude/Black_Friday";
	if (mkdir(titleOrder, 0777) == -1) { //making the dir
		perror("create folder"); exit(1);
	}
}