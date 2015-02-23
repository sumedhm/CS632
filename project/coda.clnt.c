#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <lwp/lwp.h>
#include <rpc2/rpc2.h>
#include <string.h>
#include <rpc2/se.h>
#include "coda.h"

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

long Bind(char *host, short port, long subsys, RPC2_Handle *cid)
{
    RPC2_HostIdent   hostid;
    RPC2_PortIdent   portid;
    RPC2_SubsysIdent subsysid;
    RPC2_BindParms   bindparms;

    /* Initialize connection stuff */
    hostid.Tag = RPC2_HOSTBYNAME;
    strcpy(hostid.Value.Name, host);

    portid.Tag = RPC2_PORTBYINETNUMBER;
    portid.Value.InetPortNumber = htons(port);

    subsysid.Tag = RPC2_SUBSYSBYID;
    subsysid.Value.SubsysId= subsys;

    bindparms.SideEffectType = 0;
    bindparms.SecurityLevel = RPC2_OPENKIMONO;
    bindparms.ClientIdent = NULL;

    printf("Trying to bind...");
    return RPC2_NewBinding(&hostid, &portid, &subsysid, &bindparms, cid);
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

    rc = Bind(host, port, subsys, &cid);

    if (rc != RPC2_SUCCESS) {
        printf("RPC2 connection to %s:%d failed with %s.\n",
               host, port, RPC2_ErrorMsg(rc));
        exit(2);
    }
    RPC2_Integer arg;
    rc = VENUS_GetMsg(cid, &arg);

    if(rc!=RPC2_SUCCESS){
	printf("Cannot get message from remote server..\n%s\n", RPC2_ErrorMsg(rc));
    } else {
	printf("Message : %d\n",arg);
    }
    
    RPC2_Unbind(cid);
    printf("RPC2 connection to %s:%d successful.\n", host, port);
    exit(0);

badargs:
    printf("Usage %s hostname port\n", argv[0]);
    exit(-1);
}

