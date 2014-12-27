#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define MAX_LENGTH 256
#define INSERT_INPUT "\nInsert Input"

int isNumber(const char *s)
{
    while (*s) 
    	if (isdigit(*s++) == 0) 
    	{
    		printf("%s is not a number\n", s);
    		exit(3);
    	}
}


int main(int argc, char * argv[])
{
	struct hostent *host;
	struct sockaddr_in clientaddr, servaddr;
	int port, sd, len;
	char input[MAX_LENGTH], result[MAX_LENGTH];

	if(argc!=3)
  	{
    	printf("Error:%s serverAddress serverPort\n", argv[0]);
    	exit(1);
  	}

  	host = gethostbyname (argv[1]);
  	if (host == NULL)
  	{
  		printf("%s not found in /etc/hosts\n", argv[1]);
    	exit(2);
  	}

  	isNumber(argv[2]);
  	port = atoi(argv[2]);
  	if (port < 1024 || port > 65535) 
  	{
  		printf("Port must be in [1024, 65535]\n");
    	exit(4);
	}

  	memset((char * ) &clientaddr, 0, sizeof(struct sockaddr_in));
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = INADDR_ANY;
	clientaddr.sin_port = 0;

	memset((char * ) &servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr))->s_addr;
	servaddr.sin_port = htons(port);

	sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0)
	{
		perror("Opening Socket"); 
		exit(1);
	}
	printf("Socket created: sd=%d\n", sd);

	if(bind(sd,(struct sockaddr *) &clientaddr, sizeof(clientaddr))<0)
	{
		perror("bind socket "); 
		exit(1);
	}
  	printf("Bind socket ok, port: %i\n", clientaddr.sin_port);

  	printf("%s\n", INSERT_INPUT);
  	while(fgets(input, MAX_LENGTH, stdin) != NULL)
  	{

		if (input[strlen(input) - 1] == '\n')
	    	input[strlen(input) - 1] = '\0';

	    len=sizeof(servaddr);
    	if(sendto(sd, input, sizeof(input), 0, (struct sockaddr * ) &servaddr, len)<0)
    	{
      		perror("sendto");
      		continue;
    	}

    	printf("Waiting for result...\n");
    	if (recvfrom(sd, result, sizeof(result), 0, (struct sockaddr *) &servaddr, &len)<0) 
    	// NB: len is the same
    	{
    		perror("recvfrom"); 
    		continue;
    	}

    	printf("Result: %s\n", result);

  		printf("%s\n", INSERT_INPUT);
  	}

  	close(sd);
	return 0;
}