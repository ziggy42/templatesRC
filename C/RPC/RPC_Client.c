#include <stdio.h>
#include <stdlib.h>
#include <rpc/rpc.h>
#include "RPC_xFile.h"


int isAllowed(char operation)
{
	return operation == 'm' || operation == 's';
}


int main(int argc, char *argv[])
{
	if (argc != 5)
	{
		printf("Usage: %s host operation field1 field2\n", argv[0]);
		exit(0);
	}

	CLIENT * cl;
	MyStruct mStruct;
	char operation;
	char * server;
	int * res;

	if (!isAllowed(argv[2][0]))
	{
		printf("Operation not allowed\n");
		exit(0);
	}

	operation = argv[2][0];
	server = argv[1];
	mStruct.field1 = atoi(argv[3]);
	mStruct.field2 = atoi(argv[4]);

	cl = clnt_create(server, MYPROG, MYVERS, "udp");
	if (cl == NULL) {
		clnt_pcreateerror(server);
		exit(1);
	}

	switch(operation) {
		case 's':
			res = p1_1(&mStruct, cl);
			break;
		case 'm':
			res = p2_1(&mStruct, cl);
			break;
		default:
			clnt_perror(cl, server);
			exit(1);
	}


	printf("Risult from %s: %i\n", server, *res);
	
	clnt_destroy(cl);
	return 0;
}
