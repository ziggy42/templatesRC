#include <stdio.h>
#include <rpc/rpc.h>
#include "RPC_xFile.h"

int * p1_1_svc(MyStruct * in, struct svc_req * rp)
{	
	static int res;

	printf("Input: %d, %d\n", in->field1, in->field2);
	res = (in->field1) + (in->field2);
	printf("Result: %d\n", res);
	return &res;
}

int * p2_1_svc(MyStruct * in, struct svc_req * rp)
{	
	static int res;

	printf("Input: %d, %d\n", in->field1, in->field2);
	res = (in->field1) * (in->field2);
	printf("Result: %d\n", res);
	return &res;
}