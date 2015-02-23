
/* DO NOT EDIT: generated by rp2gen from coda.rpc2 */

#include "coda.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>


#ifdef __cplusplus
}
#endif

#define _PAD(n)	((((n)-1) | 3) + 1)


#if (coda_HEAD_VERSION != 1413805299)
; char *NOTE[] = CAUTION_______________________________________!!!VERSION_IS_INCONSISTENT_WITH_HEADER_FILE______!!!PLEASE_CHECK_YOUR_FILE_VERSION________________;
#endif

MultiCallEntry coda_MultiCall[] = {

};

MultiStubWork coda_MultiStubWork[] = {

};

void coda_startlog(long op)
{
    struct timeval timestart;

    ++coda_MultiCall[op].countent;
    if ( coda_MultiStubWork[0].opengate ) {
        gettimeofday(&timestart, NULL);
        coda_MultiStubWork[op].tsec = timestart.tv_sec;
        coda_MultiStubWork[op].tusec = timestart.tv_usec;
        coda_MultiStubWork[op].opengate = 1;
    } else coda_MultiStubWork[op].opengate = 0;
}

void coda_endlog(long op, RPC2_Integer many, RPC2_Handle *cidlist, RPC2_Integer *rclist)
{
    struct timeval timeend;
    long i, timework, istimeouted, hosts;

    istimeouted = hosts = 0;
    if ( rclist == 0 ) return;
    for ( i = 0; i < many ; i++) {
        if ( cidlist[i] != 0 && rclist[i] == RPC2_TIMEOUT ) istimeouted = 1;
        if ( cidlist[i] != 0 && (rclist[i] >= 0) ) hosts++;
    }
    if ( istimeouted == 0 ) {
        ++coda_MultiCall[op].countexit;
        if ( coda_MultiStubWork[op].opengate ) {
            gettimeofday(&timeend, NULL);
            timework = (coda_MultiCall[op].tusec += (timeend.tv_sec-coda_MultiStubWork[op].tsec)*1000000+(timeend.tv_usec-coda_MultiStubWork[op].tusec))/1000000;
            coda_MultiCall[op].tusec -= timework*1000000;
            coda_MultiCall[op].tsec += timework;
            ++coda_MultiCall[op].counttime;
            coda_MultiCall[op].counthost += hosts;
        }
    }
}
