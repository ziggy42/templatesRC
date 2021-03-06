#include <stdio.h>
#include <rpc/rpc.h>
#include "echo.h"

#define MAX_LENGTH 100
#define INPUT_MESSAGE "Insert input: "

int main(int argc, char *argv[])
{

	CLIENT *cl;
	char **echo_msg;
	char *server;
	char *msg;
	char ok[5];

	if (argc < 2) 
	{
		printf("Usage: %s host\n", argv[0]);
		exit(1);
	}

	server = argv[1];

	cl = clnt_create(server, MYPROG, MYVERS, "udp");
	if (cl == NULL) 
	{
		clnt_pcreateerror(server);
		exit(1);
	}

	msg= (char*) malloc(MAX_LENGTH + 1);

	printf("%s", INPUT_MESSAGE);
	while (fgets(msg, MAX_LENGTH, stdin))
	{
		if (msg[strlen(msg) - 1] == '\n')
            		msg[strlen(msg) - 1] = '\0';

		echo_msg = procedure_1(&msg, cl);
		if (echo_msg == NULL) {
			fprintf(stderr, "%s: %s failed rpc\n", argv[0], server);
			clnt_perror(cl, server);
			exit(1);
		}

		if (*echo_msg == NULL) {
			fprintf(stderr, "%s: %s empty string\n", argv[0], server);
		}
		else{
			printf("Result from %s: %s\n", server, *echo_msg);
		}

		printf("\n%s", INPUT_MESSAGE);
	} 

	free(msg);
	clnt_destroy(cl);
	return 0;
}
