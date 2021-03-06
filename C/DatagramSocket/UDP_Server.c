#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <ctype.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <string.h>

#define MAX_LENGTH 256
#define USAGE "Usage: server serverPort"


void handler(int signo)
{
    printf("SIGCHLD handler\n");
    int state;
    wait(&state);
}


int isNumber(const char *s)
{
    while(*s)
        if (isdigit(*s++) == 0)
            return 0;
    return 1;
}


int main(int argc, char * argv[])
{
    struct sockaddr_in cliaddr, servaddr;
    struct hostent * clienthost;
    int port, sd, len;
    char request[MAX_LENGTH];
    const int on = 1;

    if(argc!=2)
    {
        printf("%s\n", USAGE);
        exit(1);
    }

    if (!isNumber(argv[1]))
    {
        printf("%s\n", USAGE);
        printf("%s is not a number\n", argv[1]);
        exit(3);
    }

    port = atoi(argv[1]);
    if (port < 1024 || port > 65535) 
    {
        printf("Port must be in [1024, 65535]\n");
        exit(4);
    }

    signal(SIGCHLD, handler);

    memset ((char *)&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;  
    servaddr.sin_port = htons(port);

    sd=socket(AF_INET, SOCK_DGRAM, 0);
    if(sd <0)
    {
        perror("creazione socket "); 
        exit(1);
    }
    printf("Server: socket created: sd=%d\n", sd);

    if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))<0)
    {
        perror("set socket options "); 
        exit(1);
    }
    printf("Server: set socket options ok\n");

    if(bind(sd,(struct sockaddr *) &servaddr, sizeof(servaddr))<0)
    {
        perror("bind socket "); 
        exit(1);
    }
    printf("Server: bind socket ok\n");

    for (;;)
    {
        len = sizeof(cliaddr);
        if (recvfrom(sd, request, sizeof(request), 0, (struct sockaddr *) &cliaddr, &len)<0)
        {
            perror("recvfrom "); 
            continue;
        }

        if (fork() == 0)
        {
            printf("\nChild %d created\n", getpid());
            clienthost = gethostbyaddr( (char *) &cliaddr.sin_addr, sizeof(cliaddr.sin_addr), AF_INET);
            if (clienthost == NULL) 
                printf("client host information not found\n");
            else 
                printf("Request received from: %s %i\n", clienthost->h_name, (unsigned)ntohs(cliaddr.sin_port));

            if (sendto(sd, &request, sizeof(request), 0, (struct sockaddr *)&cliaddr, len)<0)
            {
                perror("sendto "); 
                continue;
            } else {
                printf("%d: Response sent\n", getpid());
            }

            exit(0);
        }
    }

    return 0;
}