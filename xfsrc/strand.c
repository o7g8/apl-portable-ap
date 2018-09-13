
//	Copyright (c) 1982 Dyadic Systems Limited		

//	Sample external function which uses support routines	

#include <apl.h>
#include <stdlib.h>
#include <support.h>

#define STRAND	1		// function code 
#define NFNS	1		// number of function 

gchar taskid[] = GSTR("strand");

ARRAY *strand(ARRAY *, ARRAY *);

#if	defined(DOS)
	int main(void)
#elif	defined(DOSWIN)
	int ap_main(int argc, char *argv[])
#else
	int main(int argc, char *argv[])
#endif
{
	ARRAY *larg, *rarg, *rslt;

	startup(NFNS);		// startup protocol with apl 

	define(			// install function 
		GSTR("eg"),		// name 
		RSLT | OPTL,	// result-returning, optional left arg 
		STRAND		// function code 
	);

	larg = rarg = rslt = 0;
	sigsetjmp(errbuff, 1);		// come here after error 

	while (1) {
		larg = freearray(larg);	// free up space after use of error 
		rarg = freearray(rarg);
		rslt = freearray(rslt);

		switch (fromapl()) {	// switch on function code 
		case -1:		// APL pronounces the death sentence 
			toapl(0);	// Accept verdict 
			fromapl();	// await execution 
			exit(0);	// reject afterlife 
			return 0;	// keep the compiler happy
		case STRAND:	
			larg = getarray();
			rarg = getarray();
			rslt = strand(larg,rarg);
			putarray(rslt);
			rslt = freearray(rslt);
			larg = 0;	// AP arrays not refcounted
			rarg = 0;	// these used in the rslt and now free
			break;
		}
	}
}

/*	Dummy function:
		returns its argument if called with only one argument,
		or the 'join' of its arguments if called with two.
*/

static BOUND two = 2;

ARRAY *strand(ARRAY *larg,ARRAY *rarg)
{
	ARRAY *rslt;

	if (larg) {
		rslt = mkarray(APLPNTR,1,&two);
		rslt->shape[1] = (BOUND)larg;
		rslt->shape[2] = (BOUND)rarg;
	}
	else rslt = rarg;
	return rslt;
}

//	Copyright (c) 1982 Dyadic Systems Limited	

