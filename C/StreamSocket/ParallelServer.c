#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define MAX_LENGTH 256

void handler(int signo)
{
    int stato;
    wait(&stato);
}

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
	struct sockaddr_in cliaddr, servaddr;
  	struct hostent * clienthost;
	int port, sd, len, listen_sd, nread;
	char request[MAX_LENGTH];
	const int on = 1;

	if(argc!=2)
  	{
    	printf("Error: %s port\n", argv[0]);
    	exit(1);
  	}

  	isNumber(argv[1]);
  	port = atoi(argv[1]);
  	if (port < 1024 || port > 65535) 
  	{
  		printf("Port must be in [1024, 65535]\n");
    	exit(4);
	}

    signal(SIGCHLD, handler);

	memset ((char *) &servaddr, 0, sizeof(servaddr));
  	servaddr.sin_family = AF_INET;
  	servaddr.sin_addr.s_addr = INADDR_ANY;  
  	servaddr.sin_port = htons(port);

  	listen_sd = socket(AF_INET, SOCK_STREAM, 0);
  	if(listen_sd <0)
  	{
  		perror("listen socket creation"); 
  		exit(1);
  	}
  	printf("Server: listen socket created: sd=%d\n", listen_sd);

  	if(setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))<0)
  	{
  		perror("set socket options "); 
  		exit(1);
  	}
  	printf("Server: set socket options ok\n");

  	if(bind(listen_sd,(struct sockaddr *) &servaddr, sizeof(servaddr))<0)
  	{
  		perror("bind socket "); 
  		exit(1);
  	}
 	printf("Server: bind socket ok\n");

    if (listen(listen_sd, 5) < 0)
    {
        perror("listen"); 
        exit(1);
    }
    printf("Server: listen ok\n");

 	for (;;)
 	{
 		len = sizeof(cliaddr);
        if((sd=accept(listen_sd,(struct sockaddr *)&cliaddr,&len))<0)
        {
            if (errno==EINTR)
            {
                perror("Forzo la continuazione della accept");
                continue;
            }
            else exit(1);
        }

        if (fork() == 0)
        {
            close(listen_sd);
            printf("\nChild %d created\n", getpid());
            clienthost = gethostbyaddr( (char *) &cliaddr.sin_addr, sizeof(cliaddr.sin_addr), AF_INET);
            if (clienthost == NULL) 
                printf("client host information not found\n");
            else 
                printf("Request received from: %s %i\n", clienthost->h_name, (unsigned)ntohs(cliaddr.sin_port));

            if(nread = read(sd, request, MAX_LENGTH) < 0)
            {
                printf("ERROR receiving request\n");
            }
            shutdown(sd,0);
            printf("Child: %d Request: %s\n", getpid(), request);
            
            write(sd, request, strlen(request) + 1);
            shutdown(sd,1);

            close(sd);
        }

        close(sd);
    }

	return 0;
}