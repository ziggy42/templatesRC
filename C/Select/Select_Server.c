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
#include <ctype.h>

#define MAX_LENGTH 256
#define max(a,b) ((a) > (b) ? (a) : (b))
#define USAGE "Usage: server serverPort"


void handler(int signo)
{
    int stato;
    wait(&stato);
}


int isNumber(const char *s)
{
    while(*s)
        if (isdigit(*s++) == 0)
            return 0;
    return 1;
}


int main(int argc, char const *argv[])
{
    struct sockaddr_in cliaddr, servaddr;
    int port, listen_sd, connsd, datagramsd, maxfdp, nready, len, readRes;
    fd_set rset;
    char request[MAX_LENGTH], endChar = '\0';
    const int on = 1;


    if(argc!=2)
    {
        printf("%s\n", USAGE);
        exit(1);
    }

    if (!isNumber(argv[1]))
    {
        printf("%s\n", USAGE);
        printf("%s is not a number\n", argv[2]);
        exit(3);
    }

    port = atoi(argv[1]);
    if (port < 1024 || port > 65535) 
    {
        printf("Port must be in [1024, 65535]\n");
        exit(3);
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
        exit(4);
    }
    printf("Server: listen socket created: sd=%d\n", listen_sd);

    if(setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))<0)
    {
        perror("set socket TCP options "); 
        exit(5);
    }
    printf("Server: set socket TCP options ok\n");

    if(bind(listen_sd,(struct sockaddr *) &servaddr, sizeof(servaddr))<0)
    {
        perror("bind socket TCP "); 
        exit(6);
    }
    printf("Server: bind socket TCP ok\n");

    if (listen(listen_sd, 5) < 0)
    {
        perror("listen"); 
        exit(7);
    }
    printf("Server: listen ok\n");

    datagramsd = socket(AF_INET, SOCK_DGRAM, 0);
    if(datagramsd <0)
    {
        perror("socket UDP creation "); 
        exit(8);
    }

    if(setsockopt(datagramsd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on))<0)
    {
        perror("set socket UDP options "); 
        exit(9);
    }
    printf("Server: set socket UDP options ok\n");

    if(bind(datagramsd,(struct sockaddr *) &servaddr, sizeof(servaddr))<0)
    {
        perror("bind socket UDP"); 
        exit(10);
    }
    printf("Server: bind socket UDP ok\n");

    FD_ZERO(&rset);
    maxfdp = max(listen_sd, datagramsd) + 1;

    for (;;)
    {
        FD_SET(listen_sd, &rset);
        FD_SET(datagramsd, &rset);

        if ((nready = select(maxfdp, &rset, NULL, NULL, NULL)) < 0)
        {
            if (errno == EINTR)
                continue;
            else
            {
                perror("Select");
                exit(11);
            }

        }

        if (FD_ISSET(datagramsd, &rset))
        {
            printf("\nUDP request\n");
            len = sizeof(struct sockaddr_in);
            if (recvfrom(datagramsd, request, sizeof(request), 0, 
                (struct sockaddr *) &cliaddr, &len) < 0)
            {
                perror("recvfrom "); 
                continue;
            }

            printf("Request: %s\n", request);

            if (sendto(datagramsd, request, sizeof(request), 0, 
                (struct sockaddr *) &cliaddr, len) < 0)
            {
                perror("sendto "); 
                continue;
            } else {
                printf("%d: Response sent\n\n", getpid());
            }
        }

        if (FD_ISSET(listen_sd, &rset))
        {
            len = sizeof(struct sockaddr_in);
            if((connsd = accept(listen_sd,(struct sockaddr *)&cliaddr,&len))<0)
            {
                if (errno==EINTR)
                {
                    perror("Forzo la continuazione della accept");
                    continue;
                }
                else exit(11);
            }

            if (fork() == 0)
            {
                close(listen_sd);
                printf("\nTCP request: Child %d created\n", getpid());

                for(;;)
                {
                    if ((readRes = read(connsd, &request, sizeof(request))) < 0)
                    { 
                        perror("read"); 
                        break; 
                    } else if( readRes==0)
                    { 
                        printf("EOF\n"); 
                        break; 
                    }

                    printf("Request: %s\n", request);
                    write(connsd, request, strlen(request) + 1);
                    write(connsd, &endChar, sizeof(char));
                    printf("Response sent...\n");
                }

                close(connsd);
                exit(0);
            }
        }
    }

    return 0;
}