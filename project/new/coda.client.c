
/* DO NOT EDIT: generated by rp2gen from coda.rpc2 */
#ifdef __cplusplus
extern "C" {
#endif
#include <sys/time.h>
#ifdef __cplusplus
}
#endif

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

long coda_ElapseSwitch = 0;

long coda_EnqueueRequest = 1;

CallCountEntry coda_CallCount[] = {

};