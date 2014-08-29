/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "tellmeday.h"
#include <stdio.h>
#include <stdlib.h>
#include <rpc/pmap_clnt.h>
#include <string.h>
#include <memory.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef SIG_PF
#define SIG_PF void(*)(int)
#endif

static void
tell_me_day_1(struct svc_req *rqstp, register SVCXPRT *transp)
{
	union {
		date no_leap_years_1_arg;
		dates no_of_days_1_arg;
		dates get_day_1_arg;
		args get_no_of_weekday_1_arg;
	} argument;
	char *result;
	xdrproc_t _xdr_argument, _xdr_result;
	char *(*local)(char *, struct svc_req *);

	switch (rqstp->rq_proc) {
	case NULLPROC:
		(void) svc_sendreply (transp, (xdrproc_t) xdr_void, (char *)NULL);
		return;

	case no_leap_years:
		_xdr_argument = (xdrproc_t) xdr_date;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) no_leap_years_1_svc;
		break;

	case no_of_days:
		_xdr_argument = (xdrproc_t) xdr_dates;
		_xdr_result = (xdrproc_t) xdr_long;
		local = (char *(*)(char *, struct svc_req *)) no_of_days_1_svc;
		break;

	case get_day:
		_xdr_argument = (xdrproc_t) xdr_dates;
		_xdr_result = (xdrproc_t) xdr_int;
		local = (char *(*)(char *, struct svc_req *)) get_day_1_svc;
		break;

	case get_no_of_weekday:
		_xdr_argument = (xdrproc_t) xdr_args;
		_xdr_result = (xdrproc_t) xdr_long;
		local = (char *(*)(char *, struct svc_req *)) get_no_of_weekday_1_svc;
		break;

	default:
		svcerr_noproc (transp);
		return;
	}
	memset ((char *)&argument, 0, sizeof (argument));
	if (!svc_getargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		svcerr_decode (transp);
		return;
	}
	result = (*local)((char *)&argument, rqstp);
	if (result != NULL && !svc_sendreply(transp, (xdrproc_t) _xdr_result, result)) {
		svcerr_systemerr (transp);
	}
	if (!svc_freeargs (transp, (xdrproc_t) _xdr_argument, (caddr_t) &argument)) {
		fprintf (stderr, "%s", "unable to free arguments");
		exit (1);
	}
	return;
}

int
main (int argc, char **argv)
{
	register SVCXPRT *transp;

	pmap_unset (TELL_ME_DAY, TELL_ME_DAY_VERSION);

	transp = svcudp_create(RPC_ANYSOCK);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create udp service.");
		exit(1);
	}
	if (!svc_register(transp, TELL_ME_DAY, TELL_ME_DAY_VERSION, tell_me_day_1, IPPROTO_UDP)) {
		fprintf (stderr, "%s", "unable to register (TELL_ME_DAY, TELL_ME_DAY_VERSION, udp).");
		exit(1);
	}

	transp = svctcp_create(RPC_ANYSOCK, 0, 0);
	if (transp == NULL) {
		fprintf (stderr, "%s", "cannot create tcp service.");
		exit(1);
	}
	if (!svc_register(transp, TELL_ME_DAY, TELL_ME_DAY_VERSION, tell_me_day_1, IPPROTO_TCP)) {
		fprintf (stderr, "%s", "unable to register (TELL_ME_DAY, TELL_ME_DAY_VERSION, tcp).");
		exit(1);
	}

	svc_run ();
	fprintf (stderr, "%s", "svc_run returned");
	exit (1);
	/* NOTREACHED */
}
