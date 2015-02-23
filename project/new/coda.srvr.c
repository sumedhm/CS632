#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <assert.h>
#include <sys/time.h>
#include <lwp/lwp.h>
#include <rpc2/rpc2.h>
#include <rpc2/se.h>
#include "coda.h"

long GetMsg(RPC2_Handle _cid,int *tv_sec){
	*tv_sec = 19;
	return RPC2_SUCCESS;
}

void main(){

	RPC2_Handle cid;
	RPC2_RequestFilter reqfilter;
	RPC2_PacketBuffer *reqbuffer;
	int rc;

	PROCESS mylpid;
	RPC2_PortIdent pid, *pids;
	RPC2_SubsysIdent sid;

	/* Initialize LWP package */
	if (LWP_Init(LWP_VERSION,LWP_NORMAL_PRIORITY,&mylpid) != LWP_SUCCESS)
		error_report("Can't Initialize LWP");
	/* Initialize RPC2 package */
	pids = &pid;
	pid.Tag = RPC2_PORTALBYINETNUMBER;
	pid.Value.InetPortNumber = htons(CODAPORTNO);
	rc = RPC2_Init(RPC2_VERSION, NULL,&pids, 1, -1, NULL);
	if(rc != RPC2_SUCCESS){
		printf("Can't Initialize RPC2..\n");
	}

	sid.Tag = RPC2_SUBSYSBYID;
	sid.Value.SubsysId = CODASUBSYSID;
	rc = RPC2_Export(&sid) != RPC2_SUCCESS;
	if(rc!=RPC2_SUCCESS){
      		printf("Can't export the coda subsystem..\n");
	}

	reqfilter.FromWhom = ANY;
	reqfilter.OldOrNew = OLDORNEW;
	reqfilter.ConnOrSubsys.SubsysId = CODASUBSYSID;

	/* loop forever, wait for the client to call for service */
	for(;;){
		rc = RPC2_GetRequest(&reqfilter,&cid,&reqbuffer, NULL, NULL, NULL);
		if(rc!= RPC2_SUCCESS){
			printf("Cannot get request from client..\n");
		}
      		rc = rtime_ExecuteRequest(cid, reqbuffer);
      		if(rc!=RPC2_SUCCESS)
			fprintf(stderr, RPC2_ErrorMsg(rc));
  	}
}

