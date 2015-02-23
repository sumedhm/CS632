
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

int coda_PrintOpcode(int opcode, int subsysid) {
/*    Subsystem : coda */

    printf("coda:");

    switch (opcode) {
	case GetMsg_OP:
		printf("GetMsg_OP");
		break;
	default:
		printf("%d",opcode);
    }
}
