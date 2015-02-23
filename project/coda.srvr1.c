#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <lwp/lwp.h>
#include <rpc2/rpc2.h>
#include <string.h>
#include <rpc2/se.h>
#include "coda.h"

long VICE_GetMsg(RPC2_Handle cid, int *arg){
	*arg = 9;
	return RPC2_SUCCESS;
}

void Initialize(void)
{
    RPC2_Options options;
    PROCESS pid;
    struct timeval tv;
    long rc;

    /* initialize the subsystems LWP/RPC */
    rc = LWP_Init(LWP_VERSION, LWP_NORMAL_PRIORITY, &pid);
    if (rc != LWP_SUCCESS) {
	printf("LWP_Init() failed\n");
	exit(-1);
    }

    tv.tv_sec = 15;
    tv.tv_usec = 0;

    memset(&options, 0, sizeof(options));
    options.Flags = RPC2_OPTION_IPV6;

    rc = RPC2_Init(RPC2_VERSION, &options, NULL, -1, &tv);
    if (rc != LWP_SUCCESS) {
	printf("RPC_Init() failed\n");
	exit(-1);
    }
}

long Handle(char *host, short port, long subsys, RPC2_Handle *cid)
{
    RPC2_RequestFilter reqfilter;
    RPC2_PacketBuffer *reqbuffer;
    int rc;

    RPC2_PortIdent   portid;
    RPC2_SubsysIdent subsysid;
    /* Initialize connection stuff */

    portid.Tag = RPC2_PORTBYINETNUMBER;
    portid.Value.InetPortNumber = htons(port);

    subsysid.Tag = RPC2_SUBSYSBYID;
    subsysid.Value.SubsysId= subsys;

    rc = RPC2_Export(&subsysid) != RPC2_SUCCESS;
    if(rc!=RPC2_SUCCESS){
	printf("Can't export the coda subsystem..\n");
    }

    reqfilter.FromWhom = ANY;
    reqfilter.OldOrNew = OLDORNEW;
    reqfilter.ConnOrSubsys.SubsysId = RPC2_SUBSYSID;
    /* loop forever, wait for the client to call for service */
    printf("Waiting for requests from client..\n");
    while(1){
	    rc = RPC2_GetRequest(&reqfilter,cid,&reqbuffer, NULL, NULL, 0, NULL);
	    if(rc!= RPC2_SUCCESS){
		    printf("Cannot get request from client..\n");
	    }
	    printf("Executing request from client..\n");
	    rc = coda_ExecuteRequest(*cid, reqbuffer, NULL);
	    if(rc!=RPC2_SUCCESS)
		    printf("Error in executing request..\n");
    }

    printf("Shouldn't have reached here.. Error..\n");
    exit(1);
    return rc;
}

int main(int argc, char *argv[])
{
    RPC2_Handle cid;
    long	subsys = 1001;
    long        rc;
    char       *host;
    short       port;

    if (argc < 3)
        goto badargs;

    host = argv[1];
    port = atoi(argv[2]);

    Initialize();

    printf("RPC2 connection to %s:%d successful.\n", host, port);
    rc = Handle(host, port, subsys, &cid);


    if (rc != RPC2_SUCCESS) {
        printf("RPC2 connection to %s:%d failed with %s.\n",
               host, port, RPC2_ErrorMsg(rc));
        exit(2);
    }
    
    RPC2_Unbind(cid);
    exit(0);

badargs:
    printf("Usage %s hostname port\n", argv[0]);
    exit(-1);
}

