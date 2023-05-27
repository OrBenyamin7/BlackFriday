#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define MAXNUMBERINLINE 41
#define BUFFER_SIZE 256

void createDirOrder(char* name); //function that create dir for saving the company's orders

int checkLastInsert() //function to find the next free line in the file to insert new company
{
	int fd_from, rbytes, time=0;
	char buff[BUFFER_SIZE];


	if( ( fd_from= open( "/home/braude/Black_Friday/camp_partic.txt", O_RDONLY ) ) == -1 )
	{ 
		perror( "open from" );
 
		return( -1 ); 
	}

	if( ( rbytes= read( fd_from, buff, BUFFER_SIZE ) ) == -1 )
	{ 
			perror( "read 2" ); 

			return( -1 ); 
	}

	while( rbytes> 0 )
	{
		time=rbytes;
		if( ( rbytes= read( fd_from, buff, BUFFER_SIZE ) ) == -1 )
		{ 
			perror( "read 2" ); 

			return( -1 ); 
		}
	}
	close(fd_from);
	return time; //the place where the new company can be entered in the file
}


int writeToFile(int fd, char input[]) //function that get file describtor and input and write to the file
{
	int wbytes;
	if( ( wbytes = write( fd, input, strlen(input) ) ) == -1 )
	{ 		
		perror( "write" ); 
		return( -1 ); 
	}

	return wbytes;
}

// The function gets 2 parameters: first is the name of company, the second is percentage of discount
// argv[0]=name of program, argv[1]=name of Company, argv[2]=percentage of discount
int main(int argc,char* argv[])
{	
	int fd_to, wbytes, time, fd_new_file, i, j, firstTime=1;

	char* printToFile;

	char nameOfCompany[BUFFER_SIZE], haedLine[BUFFER_SIZE]="\t\t";

	char input[BUFFER_SIZE];
	char* stringFromInput;
	char productDetail[BUFFER_SIZE];
	int amountOfChars=0;

	char lineToPrint[BUFFER_SIZE]="\t\t";
	char choiceOfUser[BUFFER_SIZE];

	char createFolder[BUFFER_SIZE];

	if (argc != 3)
	{
		printf("Bad amount of arguments\n");
		return(-1);
	}

	strcpy(nameOfCompany,"/home/braude/Black_Friday/"); //creating new company flyer
	
	
	strcat(nameOfCompany,argv[1]);


	strcat(nameOfCompany,".txt");

	if ((fd_new_file = open(nameOfCompany, O_WRONLY| O_CREAT, 0644)) == -1) //creates file with company name
    {
		perror("open file"); 
		return (-1);
    }

	//printing the headline for the file (name of company + sale and percentage of discount)
	for(i=1;i<3;i++)
	{
		strcat(haedLine,argv[i]);
		if(i==1) //company name
			strcat(haedLine," Sale");
		else  //discount percentage
			strcat(haedLine,"% off");

		strcat(haedLine,"\n\n");

		if(writeToFile(fd_new_file,haedLine)==-1) //write to the file
			return -1;
		strcpy(haedLine,"\t\t");
	}

	printf("Insert item and item cost:\n");

	fgets(input, sizeof input,stdin); //getting from user input

	while(strcmp(input,"Stop\n")!=0) //getting from user the items and their prices
	{
		input[strlen(input)-1] = '\0';

		stringFromInput=strtok(input," ");
		while(stringFromInput!=NULL) //printing to the file the name of the product and its price
		{
			if(firstTime==0)
			{
				strcat(productDetail," ");
				amountOfChars++;
				if(writeToFile(fd_new_file,productDetail)==-1)
					return -1;
				strcpy(productDetail,stringFromInput);
				amountOfChars+=strlen(productDetail);
			}
			else
			{
				strcpy(productDetail,stringFromInput);
				amountOfChars+=strlen(productDetail);
				firstTime=0;
			}
		
			stringFromInput=strtok(NULL," ");
		}
		amountOfChars-=strlen(productDetail);
		strcat(productDetail,"NIS");
		if(lseek(fd_new_file , -1 , SEEK_CUR )==-1) //writing on the space after the last name of product
		{
			perror("lseek1 error");
			exit(1);
		}

		for(j=0;j<(MAXNUMBERINLINE-amountOfChars-(strlen(productDetail)));j++)
		{
			if(writeToFile(fd_new_file,".")==-1)
				return -1;
		}

		if(lseek(fd_new_file , -1 , SEEK_CUR )==-1) 
		{
			perror("lseek1 error");
			exit(1);
		}

		if(writeToFile(fd_new_file," ")==-1) //printing space after the last point before the price
			return -1;

		if(writeToFile(fd_new_file,productDetail)==-1)
			return -1;

		if(writeToFile(fd_new_file,"\n")==-1)
			return -1;

		firstTime=1;
		amountOfChars=0;
		fgets(input, sizeof input,stdin);
	}

	printf("What is your choice?\n"); 
	printf("1.Enjoyed \n2.Shop Now and have fun \n3.Have Fun \n4.Other\n");

	fgets(choiceOfUser, sizeof choiceOfUser,stdin); //printing message in the end of the file	
	if(atoi(choiceOfUser)==1)
	{
		strcat(lineToPrint,"Enjoyed");
	}
	else
	{
		if(atoi(choiceOfUser)==2)
		{
			strcat(lineToPrint,"Shop Now and have fun");
		}
		else
		{
			if(atoi(choiceOfUser)==3)
			{
				strcat(lineToPrint,"Have Fun");
			}
			else
			{
				printf("Enter your word:\n");
				fgets(input, sizeof input,stdin);
				strcat(lineToPrint,input);
			}
		}
	}

	strcat(lineToPrint,"\n");
	if(writeToFile(fd_new_file,lineToPrint)==-1) //write to the file
		return -1;


	
	time=checkLastInsert(); //calling the function to find the free line
	
	if( ( fd_to = open( "/home/braude/Black_Friday/camp_partic.txt", O_WRONLY, 0664  ) ) == -1 )
	{ 
		perror( "open from" );
	 
		return -1 ; 
	}
	

	lseek(fd_to, time, SEEK_SET ); //jumping in the file to the next free line
	
	printToFile=strtok(argv[1]," ");
	strcat(printToFile,"\n");
	

	if(writeToFile(fd_to,printToFile)==-1)
		return -1;

	close(fd_new_file);
	close(fd_to);

	//creating a folder for saving orders

	argv[1][strlen(argv[1])-1] = '\0';
	createDirOrder(argv[1]);
	
	
	printf("Successfully created // %s.txt created , %s_Order dir created.\n", argv[1], argv[1]);
	return 0;
	
}

void createDirOrder(char* name) {
	//creating the directory with the given company name
	char titleOrder[BUFFER_SIZE] = "/home/braude/Black_Friday/";
	strcat(titleOrder, name);
	strcat(titleOrder, "_Order");
	if (mkdir(titleOrder, 0777) == -1) { //making the dir
		perror("create folder"); exit(1);
	}
}

