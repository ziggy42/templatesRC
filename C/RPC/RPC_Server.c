#include <stdio.h>
#include <rpc/rpc.h>
#include <string.h>
#include "echo.h"

char ** procedure_1_svc(char ** msg, struct svc_req * rp)
{
	static char * echo_msg;

	printf("\nRequest: %s\n", *msg);

	free(echo_msg);
	echo_msg = (char *) malloc(strlen(* msg) + 1);
	strcpy(echo_msg, * msg);

	printf("Response: %s\n", echo_msg);

	return &echo_msg;
}
