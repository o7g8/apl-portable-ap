#include <machine.h>
#if WINDOWS3
#include <windows.h>
#endif
#include <stdlib.h>
#include <string.h>

#include <apl.h>
#include <support.h>
#include <data.h>

#if UNIX
	#include <wctype.h>
#endif

int DLLEXPORT __cdecl add(void *lvp,void *rvp, void *rsltvp)
	{
	ARRAY default_rslt  =	{		
				3,		// LENGTH + ZONES + BOUND
				SIMPLE,
				1,		// VECTOR
				APLLONG,
				0		// no elements
				};

	ARRAY *larg;
	ARRAY *rarg;
	ARRAY *rslt;
	BOUND b;

	aplint32  *lp,*rp,*rsltp;

	larg =  getA(lvp);
	rarg =  getA(rvp);

	putA(rsltvp,&default_rslt);

	if (larg->eltype != rarg->eltype)
		return EDOMAIN;
	if (larg->eltype != APLLONG)
		return EDOMAIN;
	if (larg->rank != rarg->rank)
		return RANK;
	if (larg->rank != 1)
		return RANK;
	if (larg->shape[0] != rarg->shape[0])
		return EDOMAIN;

	rslt = mkarray(APLLONG,1,larg->shape);

	if (rslt==0) return LIMIT;

	lp    = (aplint32 *)&larg->shape[1];
	rp    = (aplint32 *)&rarg->shape[1];
	rsltp = (aplint32 *)&rslt->shape[1];

	for (b=0;b<larg->shape[0];b++)
		*rsltp++=*lp+++*rp++;

	putA(rsltvp,rslt);

	return 0;
	}

int DLLEXPORT __cdecl a_toupper(void *rvp, void *rsltvp)
	{
	ARRAY *rarg;
	ARRAY *rslt;
	BOUND b;

	aplWchar16 *rp,*rsltp;

	rarg =  getA(rvp);

	if (rarg->eltype != APLWCHAR16)
		return EDOMAIN;

	rslt = mkarray(APLWCHAR16,1,rarg->shape);

	if (rslt==0) return LIMIT;

	rp    = (aplWchar16 *)&rarg->shape[1];
	rsltp = (aplWchar16 *)&rslt->shape[1];

	for (b=0;b<rarg->shape[0];b++)
		*rsltp++=towupper(*rp++);

	putA(rsltvp,rslt);

	return 0;
	}


