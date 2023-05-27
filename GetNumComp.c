#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX 256

int main(int argc, char* argv[])
{
	int fd_from,rbytes;
	char buff[MAX];
	char* line;
	int word=0;

	//open camp partics file
	if( ( fd_from = open( "/home/braude/Black_Friday/camp_partic.txt", O_RDONLY ) ) == -1 )
	{ 
		perror( "open from" );
	 
		return -1 ; 
	}

	if( ( rbytes = read( fd_from, buff, MAX ) ) == -1 ) //reading from camp partics file
	{ 
		perror( "read 1" ); 

		return -1 ; 
	}

	while( rbytes > 0 )
	{
		line=strtok(buff,"\n");
		while(line)
		{
			line=strtok(NULL,"\n");
			word++;

		}
		if( ( rbytes = read( fd_from, buff, MAX ) ) == -1 ) // keep reading from the file
		{ 
			perror( "read 2" ); 
	
			return -1; 
		}
	}

	printf("%d companies take part in the Campaign\n", word);
	close(fd_from);
    return 0;
}