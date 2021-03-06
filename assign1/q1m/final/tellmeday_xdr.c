/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "tellmeday.h"

bool_t
xdr_date (XDR *xdrs, date *objp)
{
	register int32_t *buf;

	int i;

	if (xdrs->x_op == XDR_ENCODE) {
		buf = XDR_INLINE (xdrs, (2 +  20 )* BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_int (xdrs, &objp->dd))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->mm))
				 return FALSE;
			 if (!xdr_vector (xdrs, (char *)objp->yy, 20,
				sizeof (int), (xdrproc_t) xdr_int))
				 return FALSE;
		} else {
			IXDR_PUT_LONG(buf, objp->dd);
			IXDR_PUT_LONG(buf, objp->mm);
			{
				register int *genp;

				for (i = 0, genp = objp->yy;
					i < 20; ++i) {
					IXDR_PUT_LONG(buf, *genp++);
				}
			}
		}
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		buf = XDR_INLINE (xdrs, (2 +  20 )* BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_int (xdrs, &objp->dd))
				 return FALSE;
			 if (!xdr_int (xdrs, &objp->mm))
				 return FALSE;
			 if (!xdr_vector (xdrs, (char *)objp->yy, 20,
				sizeof (int), (xdrproc_t) xdr_int))
				 return FALSE;
		} else {
			objp->dd = IXDR_GET_LONG(buf);
			objp->mm = IXDR_GET_LONG(buf);
			{
				register int *genp;

				for (i = 0, genp = objp->yy;
					i < 20; ++i) {
					*genp++ = IXDR_GET_LONG(buf);
				}
			}
		}
	 return TRUE;
	}

	 if (!xdr_int (xdrs, &objp->dd))
		 return FALSE;
	 if (!xdr_int (xdrs, &objp->mm))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->yy, 20,
		sizeof (int), (xdrproc_t) xdr_int))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_dates (XDR *xdrs, dates *objp)
{
	register int32_t *buf;

	int i;

	if (xdrs->x_op == XDR_ENCODE) {
		 if (!xdr_date (xdrs, &objp->d1))
			 return FALSE;
		buf = XDR_INLINE (xdrs, ( 20 ) * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_vector (xdrs, (char *)objp->res, 20,
				sizeof (int), (xdrproc_t) xdr_int))
				 return FALSE;
		} else {
			{
				register int *genp;

				for (i = 0, genp = objp->res;
					i < 20; ++i) {
					IXDR_PUT_LONG(buf, *genp++);
				}
			}
		}
		return TRUE;
	} else if (xdrs->x_op == XDR_DECODE) {
		 if (!xdr_date (xdrs, &objp->d1))
			 return FALSE;
		buf = XDR_INLINE (xdrs, ( 20 ) * BYTES_PER_XDR_UNIT);
		if (buf == NULL) {
			 if (!xdr_vector (xdrs, (char *)objp->res, 20,
				sizeof (int), (xdrproc_t) xdr_int))
				 return FALSE;
		} else {
			{
				register int *genp;

				for (i = 0, genp = objp->res;
					i < 20; ++i) {
					*genp++ = IXDR_GET_LONG(buf);
				}
			}
		}
	 return TRUE;
	}

	 if (!xdr_date (xdrs, &objp->d1))
		 return FALSE;
	 if (!xdr_vector (xdrs, (char *)objp->res, 20,
		sizeof (int), (xdrproc_t) xdr_int))
		 return FALSE;
	return TRUE;
}
