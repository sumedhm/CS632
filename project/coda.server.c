
/* DO NOT EDIT: generated by rp2gen from coda.rpc2 */
#include <sys/time.h>
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


#if (coda_HEAD_VERSION != 1414748928)
; char *NOTE[] = CAUTION_______________________________________!!!VERSION_IS_INCONSISTENT_WITH_HEADER_FILE______!!!PLEASE_CHECK_YOUR_FILE_VERSION________________;
#endif

static RPC2_PacketBuffer *_VICE_GetMsg(RPC2_Handle _cid, RPC2_PacketBuffer *_reqbuffer, SE_Descriptor *_bd)
{
    char *_ptr;
    long _length, _rpc2val, _code;
    RPC2_PacketBuffer *_rspbuffer = NULL;
    /*OUT*/	RPC2_Integer arg;
    char *_EOB;

     /* This avoids compiler warnings */
    _ptr = NULL;
    _reqbuffer = _reqbuffer;
    _bd = _bd;

    _code = VICE_GetMsg(_cid, &arg);

    _length = 4;
    _rpc2val = RPC2_AllocBuffer(_length, &_rspbuffer);
    if (_rpc2val != RPC2_SUCCESS) return NULL;
    _rspbuffer->Header.ReturnCode = _code;

    /* Pack return parameters */
    _ptr = (char *)_rspbuffer->Body;
    _EOB = (char *)_rspbuffer + _rspbuffer->Prefix.BufferSize;
    *(RPC2_Integer *) _ptr = htonl(arg);
    _ptr += 4;
    return _rspbuffer;
}

long coda_ExecuteRequest(RPC2_Handle _cid, RPC2_PacketBuffer *_reqbuffer, SE_Descriptor *_bd)
{
    RPC2_PacketBuffer *_rspbuffer;
    long _rpc2val, _rpc2tmpval;

    switch (_reqbuffer->Header.Opcode) {
	case GetMsg_OP:
		_rspbuffer = _VICE_GetMsg(_cid, _reqbuffer, _bd);
				break;
	case RPC2_NEWCONNECTION:
		RPC2_FreeBuffer(&_reqbuffer);
		return RPC2_Enable(_cid);
	default:
		if (RPC2_AllocBuffer(0, &_rspbuffer) != RPC2_SUCCESS) return(RPC2_FAIL);
		_rspbuffer->Header.ReturnCode = RPC2_INVALIDOPCODE;
    }
    _rpc2tmpval = RPC2_FreeBuffer(&_reqbuffer);
    _rpc2val = RPC2_SendResponse(_cid, _rspbuffer);
    if (_rpc2val == RPC2_SUCCESS)
	_rpc2val = _rpc2tmpval;
    _rpc2tmpval = RPC2_FreeBuffer(&_rspbuffer);
    if (_rpc2val == RPC2_SUCCESS)
	_rpc2val = _rpc2tmpval;
    return _rpc2val;
}