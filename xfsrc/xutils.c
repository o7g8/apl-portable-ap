//	Copyright (c) 1982 Dyadic Systems Limited	

#include <machine.h>
#undef	getenv		// use library version

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#undef	BUFSIZ
#include <apl.h>
#include <support.h>
#include <assert.h>
#include <unicode.h>
#include <trans.h>
#include <quadav.h>
#include <gfns.h>
#include <charfns.h>
#include <decf.h>

gchar taskid[] = GSTR("xutils");

#define	VERSION	"11.1"
#define	UNS	unsigned

static	void initxutils(int argc, gchar *argv[]);

static void traverse
	(
	ARRAY *array,
	void (*func)(ARRAY *, UNS),
	UNS depth
	);
static void ckchv(ARRAY *array);
static void toupperav(gchar* cptr,UNS num);
static void zero(achar* cptr1,achar* cptr2);
static void regerr(ubyte c);
static void syserr(UCH *msg);
static ubyte* zmalloc(UNS size);
static ubyte* zrealloc(ubyte* ptr, UNS size);
static achar* doreplace(achar* cptr1, achar* cptr2, BOUND *unsptr);
static ARRAY *cbox(ARRAY *larg, ARRAY *rarg, UNS bound);
static int sizeel(UNS eltype);
static void hexcount(ARRAY *array, UNS depth);
static void hexrslt(ARRAY *array, UNS depth);

typedef ARRAY *APLFN(ARRAY *, ARRAY *);

static APLFN
	avx,
	box,
#ifdef	DOS
	copyseg,
	interrupt,
	port,
#endif	// DOS 
#if	defined(DOSWIN)
	gtenv,
#endif//defined(DOSWIN)
	hex,
	ltom,
	ltov,
	mtol,
	dbr,
	ss,
	vtol;

static struct FUNCTION	
	{
	gchar	*f_name;
	UNS	f_syntax;
	APLFN	*f_pointer;
	} functions[] =
		{
		GSTR("avx"),		RSLT|MONA,	avx,
		GSTR("box"),		RSLT|OPTL,	box,
#ifdef	DOS
		GSTR("copyseg"),	SHYR|OPTL,	copyseg,
		GSTR("interrupt"),	SHYR|OPTL,	interrupt,
		GSTR("port"),		SHYR|OPTL,	port,
#endif	// DOS 
#if	defined(DOSWIN)
		GSTR("getenv"),	RSLT|MONA,	gtenv,
#endif//defined(DOSWIN)
		GSTR("hex"),		RSLT|MONA,	hex,
		GSTR("ltom"),		RSLT|OPTL,	ltom,
		GSTR("ltov"),		RSLT|OPTL,	ltov,
		GSTR("mtol"),		RSLT|OPTL,	mtol,
		GSTR("dbr"),		RSLT|MONA,	dbr,
		GSTR("ss"),		RSLT|OPTL,	ss,
		GSTR("vtol"),		RSLT|OPTL,	vtol,
		};

#define		NFUNCTIONS  (sizeof(functions)/sizeof(functions[0]))

ARRAY **Larg;	/* Global pointer so that OPTL larg functions can create
		 * one which will be freed.
		 */

ARRAY **Rarg;	/* Global pointer so that coercions can occur and still be
		 * freed.
		 */

ARRAY **Rslt;	/* Global pointer so that local results can be created and
		 * still freed if an error occurs
		 */

static  achar	*expbuf;	//static and malloc'd.  Allows for remembering
	//last ss search expression

#if	defined(DOS)
	int main(void)
#elif	defined(DOSWIN)
	int ap_main(int argc, gchar *argv[])
#else
	int main(int argc, char *cargv[])
	#define	CONVERT_ARGV	1
#endif	// DOS 
{
#if	CONVERT_ARGV
	int argcount = argc;
	gchar **argv = alloca(argc * sizeof(gchar *));

	for
		(
	;	argcount--
	;	
		)
		{
		UCH *cp = cargv[argcount];
		size_t len = 1 + strlen(cp);
		gchar *gp;

		gp = argv[argcount] = alloca(len * sizeof(gchar));
		
		for
			(
		;	len--
		;	*gp++ = *cp++
			);
		}
	
#endif//CONVERT_ARGV
	{
	ARRAY	*(*f)(ARRAY *, ARRAY *);
	ARRAY	*larg, *rarg, *rslt;
	BOUND	s, i, fromapl();
	int	fncode;

#if	defined(DOS)
	initxutils();
#else
	initxutils(argc, argv);

#endif	// DOS 
	larg = rarg = rslt = 0;

	Larg = &larg; Rarg = &rarg; Rslt = &rslt;

	sigsetjmp(errbuff, 1);		// come here after error 
	for(;;)
		{
		larg = freearray(*Larg);
		if(rarg != rslt)	// dbr() can return its argument
			rarg = freearray(*Rarg);
		rslt = freearray(*Rslt);

		fncode = i = fromapl();
		if(-1 == fncode)	// APL request termination 
			{
			toapl(0);	// return control to APL 
			if (expbuf)	// ss' buffer
				free(expbuf);
			fromapl();	// on DOS we dont get this back 
			exit(0);	// on UNIX we do and we exit 
			}
		if(i >= NFUNCTIONS) syserr("main");
		s = functions[i].f_syntax;
		f = functions[i].f_pointer;
		switch(s & DYAD)
			{
			case DYAD:	// dyadic and optional	
					larg = getarray();
			case MONA:	// monadic 
					rarg = getarray();
			default:	// niladic 
					rslt = (*f)(larg,rarg);
			}

		if(s & RSLT)
			{
			putarray(rslt);
			}
		}
	return 0;
	}
}

/*	s y s e r r
	print syserr with (optional) message and die
*/
static void syserr(UCH* msg)
{
	printf("xutils sys err%s %s\n", *msg ? ": " : "", msg);
	exit(1);
}

/*	i n i t x u t i l s
	define named functions or ALL if none named
*/
#if	defined(DOS)
static	initxutils()
#else
static	void initxutils(int argc, gchar *argv[])
#endif	// DOS 
{
	int 	i;
	int	nfns;
	UCH	wanted[NFUNCTIONS];

	// preset functions requested flags	
	for(i=0; i<NFUNCTIONS; i++) wanted[i]=0;

	// count and identify each function named in arguments	
	nfns = 0;
#if	UNIX	// I dont support args to APs under DOS or WINDOWS
	while(argc--)
		{
		gchar *arg = *(argv++);

		for(i=0; i<NFUNCTIONS; i++)
			{if(wanted[i]) continue;
			if(!gstrcmp(arg,functions[i].f_name)) break;
			}
		if(i<NFUNCTIONS)
			{wanted[i] = 1;
			nfns++;
			}
		else if(gstrcmp(arg,GSTR("xutils")) && gstrcmp(arg,GSTR("")))
			printf
				(
#if	HAS_UNICODE
				"xutils %s not found %ls\n",
				VERSION, (wchar_t *)arg
#else //HAS_UNICODE
				"xutils %s not found %s\n",
				VERSION, (char *)arg
#endif//HAS_UNICODE
				);
		}
		
#endif//UNIX
	// define requested functions, or ALL if none requested	
	startup(nfns?nfns:NFUNCTIONS);
	for(i=0; i<NFUNCTIONS; i++)
		if(nfns==0 || wanted[i])
			define(functions[i].f_name, functions[i].f_syntax, i);
}

/*	b l o c k m o v e
	character block move
*/
static void blockmove(gchar* from, gchar* to, UNS nchars)
{
	while(nchars--) *to++ = *from++;
}

/*	a v x
	return 0-origin index in quad av of simple text
*/

static ARRAY	*avx(ARRAY *ignore, ARRAY *rarg)
{
	ARRAY	*rslt;

	switch (rarg->eltype)
		{
		case APLNCHAR:
		case APLWCHAR8:
			{
			ubyte	 	*cp,*cpend,*cpstart;
			aplint16 	*ip;

			cpstart = arraystart(rarg);
			cpend = cpstart + arraybound(rarg);

			// if all characters are 127 or less can return APLSINT	

			for(cp=cpstart; cp<cpend; cp++) if( *cp & 128 ) break;
			if(cp==cpend)
				{
				rarg->eltype = APLSINT;
				return rarg;
				}
			rslt = mkarray(APLINTG,rarg->rank,rarg->shape);

			ip =  arraystart(rslt);
			for(cp=cpstart; cp<cpend; cp++) *ip++ = *cp & 0377;

			break;
			}
		case APLWCHAR16:
			{
			aplWchar16 	*cp,*cpend,*cpstart;
			aplint32 	*ip;

			cpstart = arraystart(rarg);
			cpend = cpstart + arraybound(rarg);

			rslt = mkarray(APLLONG,rarg->rank,rarg->shape);

			ip =  arraystart(rslt);
			for(cp=cpstart; cp<cpend; cp++) 
				*ip++ = *cp;

			break;
			}
		case APLWCHAR32:
			{
			aplWchar32	*cp,*cpend,*cpstart;
			aplfloat64 	*ip;

			cpstart = arraystart(rarg);
			cpend = cpstart + arraybound(rarg);

			rslt = mkarray(APLDOUB,rarg->rank,rarg->shape);

			ip =  arraystart(rslt);
			for(cp=cpstart; cp<cpend; cp++) 
				*ip++ = *cp;

			break;
		default:
			error(EDOMAIN);
			}
		}
	return rslt;
}

/*	h e x
	display object in hex
*/

static gchar	hexdigs[] = GSTR("0123456789ABCDEF");
static gchar	*hexdest;
static UNS	counter;

static ARRAY *hex(ARRAY *ignore, ARRAY *rarg)
{
	ARRAY	*rslt;
	BOUND	shape;

	counter = 0;
	traverse(rarg,hexcount,0);
	shape = counter - 1;		// dont need trailing delimiter 
	rslt = mkarray(APLGCHAR,1,&shape);
	hexdest = (gchar*)(rslt->shape + 1);
	traverse(rarg,hexrslt,0);
	return rslt;
}

static void hexcount(ARRAY *array, UNS depth)
{
	counter += array->length * (
		2			// two hex digits per byte   
		* sizeof(MEMORYINT)	// number of bytes per word  
		+ 1			// delimiter after each word 
	);
}

static ubyte *endian_fix(ubyte *src)
{
#if	BYTE3210			// little endian
	MEMORYUNS usrc = (MEMORYUNS)src;
	MEMORYUNS mask = sizeof(MEMORYUNS) - 1;
	MEMORYUNS trail = usrc & mask;
	MEMORYUNS new = mask - trail;
	MEMORYUNS nsrc = (usrc & ~mask) + new;
	
	src = (ubyte *)nsrc;
#endif//BYTE3210
	return src;
}
	

static void hexrslt(ARRAY *array, UNS depth)
{
	BOUND	bound, indx;
	ubyte*	hexsrce;
	ubyte	byte;

	bound = array->length * sizeof(MEMORYINT);	// number of chars in array 
	hexsrce = (ubyte*)array;		// first word of array      
	if (depth) *hexdest++ = AV_NEWLINE;	// newline before sub-array 
	for (indx = 0; indx<bound; indx++){
		if (
			indx &&			// ... its not the first one 
			0 == indx%sizeof(MEMORYINT)	// and its a word boundary   
		) *hexdest++ = AV_SP;		// 	leave a gap	     
		byte = *endian_fix(hexsrce++);	// get target byte	     
		*hexdest++ = hexdigs[byte>>4];	// format first hex digit    
		*hexdest++ = hexdigs[byte&0xf];	// format second hex digit   
	}
}

static void traverse  // tramp (possibly nested) array and apply func 
	(
	ARRAY *array,
	void (*func)(ARRAY *, UNS),
	UNS depth
	)
{
	UNS indx, bound;

	(*func)(array,depth);
	switch (array->type){
	case SIMPLE:
		break;

	case NESTED:
		bound = array->length - 2;
		for (indx = array->rank; indx < bound ; indx++)
			traverse((ARRAY *)array->shape[indx],func,depth+1);
		break;
	default:
		error(NONCE);
	}
}

#define SUB_DBR(type)						\
static size_t SUB_DBR_##type(ARRAY *rarg)			\
{								\
	type	*iptr, *optr, *base;				\
								\
	int last = AV_SP;					\
	int begun = 0;						\
	size_t nelts;						\
								\
	base = optr = iptr = arraystart(rarg);			\
	for(nelts = rarg->shape[0]; nelts--;)			\
		{						\
		int c = *iptr++;				\
		if(begun)					\
			{					\
			if(c != AV_SP || last != AV_SP)		\
				{*optr++ = last;		\
				last = c;			\
				}				\
			}					\
		else if(c != AV_SP)				\
			{					\
			begun = 1;				\
			last = c;				\
			}					\
		}						\
	if(last != AV_SP) *optr++ = last;			\
								\
	nelts = rarg->shape[0] = optr-base;			\
								\
	return nelts * sizeof(type);				\
}

SUB_DBR(aplWchar8)
SUB_DBR(aplWchar16)
SUB_DBR(aplWchar32)


/*	d b r
	remove leading, trailing, and multiple blanks
*/
static ARRAY *dbr(ARRAY *ignore, ARRAY *rarg)	
{
	size_t nbytes;

	if(rarg->type   != SIMPLE) error(EDOMAIN);
	if(rarg->rank   == 0 	 ) return rarg;
	if(rarg->rank   != 1	 ) error(RANK);

	switch (rarg->eltype)
		{
		default:
			error(EDOMAIN);
		case APLWCHAR8:
		case APLNCHAR:
			nbytes=SUB_DBR_aplWchar8(rarg);
			break;
		case APLWCHAR16:
			nbytes=SUB_DBR_aplWchar16(rarg);
			break;
		case APLWCHAR32:
			nbytes=SUB_DBR_aplWchar32(rarg);
			break;
		}
			
	rarg->length = (nbytes+sizeof(ARRAY)+sizeof(MEMORYINT)-1)/sizeof(MEMORYINT);

	return rarg;
}

/*
 *	s s
 *	
 *	regular expression matching string search and substitution function.
 *
 */

#define		INIT		achar* sp = (achar* )instring;
#define		GETC()		(*sp++)
#define		PEEKC()		(*sp)
#define		UNGETC(c)	(--sp)
#define		RETURN(c)	return;
#define		ERROR(c)	regerr(c)

#include	<regexpav.h>
#define BUFSIZ (32*EPSIZE)	

static void normalize_rarg(ARRAY *rarg)
{
BOUND n;
ARRAY **argptr = arraystart(rarg);

for (n=0;n<rarg->shape[0];n++,argptr++)
	{
	uns et = (*argptr)->eltype;

	if (0==ELTYPE_ISCHAR(et))
		domain();

	if ((*argptr)->rank > 1)
		error(RANK);

	if (et >= APLWCHAR8 && et != APLWCHAR32)	// unicode item
		{
		ARRAY *new = mkarray(APLACHAR, (*argptr)->rank, (*argptr)->shape);
	
		et_memcpy_et(arraystart(new),APLACHAR,arraystart(*argptr),et,arraybound(*argptr));
		freearray(*argptr);
		*argptr=new;
		}
	}
}

static ARRAY	*ss(ARRAY *larg, ARRAY *rarg)
{

	achar	*ptr1, *ptr2;
	ARRAY	*arg;
	achar	*cbase, *rbase;
	achar	replace, caseless, boolrslt;
	BOUND	num, num2, matches = 0;
	ARRAY	**argptr, *rslt;

	//	check args	
	if(rarg->eltype != APLPNTR) error(EDOMAIN);
	if(rarg->rank != 1)	 error(RANK);
	if(rarg->shape[0] <2 || rarg->shape[0] >3) error(LENGTH);
	replace = rarg->shape[0] == 3;
	caseless = boolrslt = 0;
	if(larg)
		{
		ubyte*	ucp = arraystart(larg);

		if(larg->rank > 1) error(RANK);
		if(arraybound(larg) <1 || arraybound(larg) >2) error(LENGTH);
		if(!larg->rank)	// scalar could be APLBOOL or APLSINT	
			{
			switch(larg->eltype)
				{
			case APLBOOL:
				caseless = 0 != (0x80 & *ucp);
				break;
			case APLSINT: // treat as unsigned char
				if(1 < *ucp) error(EDOMAIN);
				caseless = *ucp;
				break;
			default  :
				error(EDOMAIN);
				}
			}
		else		// vectors always squeezed	
			{
			if(larg->eltype != APLBOOL) error(EDOMAIN);
			caseless = 0 != (0x80 & *ucp);
			if(larg->shape[0] == 2)
				boolrslt = 0 != (0x40 & *ucp);
			}
		}

	normalize_rarg(rarg);

	// check rarg[1] 
	argptr = arraystart(rarg);
	rslt = *argptr++;

	cbase = ptr1 = arraystart(rslt);
	num = arraybound(rslt);
	if(!replace)	// copy the text so can use original array as bool 
		{
		rbase = ptr1;
		cbase = ptr1 = (achar*)zmalloc(num*sizeof(achar));
		amemcpy(cbase, rbase, num);
		}

	// check rarg[2] (the regular expression) 
	arg = *argptr++;
	ptr2 = arraystart(arg);
	num2 = arraybound(arg);

	if(replace)
		{
		arg = *argptr;
		doreplace((achar* )0, (achar* )arg, (BOUND*)0);
		}

	if (expbuf == 0)
		{
		expbuf = (achar *) zmalloc(BUFSIZ*sizeof(achar));
		memset(expbuf, 0, BUFSIZ*sizeof(achar));
		}

	compile(ptr2, expbuf, expbuf+BUFSIZ, num2, caseless);

	if(!replace)
		{
		zero(rbase, rbase+num);
		matches = 0;
		}

	arg = (ARRAY *)(ptr1+num-1);
	lpstart = ptr1;  /* put back in to fix bug 8084 */

	if(step(ptr1, (achar* )arg, expbuf)) for(;;)
		{
		achar*	last_ptr1 = ptr1, *last_loc1 = loc1;
		unsigned again =
			step(ptr1 = loc2, (achar* )arg, expbuf) && !circf;

		if(again && loc1 == last_loc1) break;	// bug 1781 
		if(!replace)
			{
			(*(rbase+(last_loc1-cbase)))++;
			matches++;
			}
		else
			doreplace(last_ptr1, last_loc1, (BOUND *)0);
		if(!again)
			break;
		}

	if (num > INT32_MAX)
		error(LIMIT);	 //If over 2 Gig 

	if(!replace)
		{
		if(boolrslt) 
			{	//  At this point, rbase contains bools
			rslt = mkarray(APLSINT, 1, &num);
			et_memcpy_et(arraystart(rslt),APLWCHAR8
					,rbase,APLACHAR,num);
			}
		else
			{
			if(!matches)
				rslt = mkarray(APLSINT, 1, &matches);
			else
				{
				UNS		indx;
				aplint32	*lptr;

				ptr2 = rbase;
				rslt=mkarray(APLLONG,1,&matches);
				lptr= arraystart(rslt);
				indx = 1;
				while(matches--)
					{
					while(!*ptr2++) indx++;
					*lptr++ = indx++;
					}
				}
			}
		}
	else
		{				// copy the tail	
		UNS tail = num - (ptr1-cbase);

		if( (int)tail < 0 )
			tail = 0;
		ptr2 = doreplace(cbase, (achar* )0, &matches); // query	
		num = matches + tail;
		rbase = arraystart(rslt = mkarray(APLACHAR, 1, &num));
		memcpy(rbase, ptr2, matches*sizeof(achar));
		memcpy(rbase+matches, ptr1, tail*sizeof(achar));
		free(ptr2);	// Bugfix 8/7/87 - free up working space. 
		}
	if(!replace)
		free(cbase);
	return rslt;
}

static void zero(achar* cptr1,achar* cptr2)
{
	for( ;cptr1<cptr2;*cptr1++ = 0);
}

static achar* doreplace(achar* cptr1, achar* cptr2, BOUND *unsptr)
{
	static	achar	*dataptr, *fromptr, *cptr;
	static	UNS	numels;
	static	UNS	bufsize = BUFSIZ;

	if(!cptr1)	// initialise - cptr2 is ptr to rarg[3]	
		{
		fromptr = arraystart((ARRAY *)cptr2);
		numels  = arraybound((ARRAY *)cptr2);
		dataptr = cptr = (achar *)zmalloc(bufsize *sizeof(achar));
		return 0;
		}
	else if(!cptr2)	// query - return dataptr, numused through unsptr 
		{
		*unsptr = cptr - dataptr;
		return dataptr;
		}
	while(bufsize < cptr - dataptr + numels + cptr2 - cptr1)
	//  Bugfix 20/11/87 - repeat until bufsize large enough  
		{
		UNS ptrpos;

		ptrpos = cptr - dataptr;
		bufsize += BUFSIZ;
		dataptr = (achar*)zrealloc((ubyte*)dataptr, bufsize  * sizeof(achar));
		cptr = dataptr + ptrpos;
		}
	while(cptr1 < cptr2) *cptr++ = *cptr1++;
	for	(cptr1 = fromptr, cptr2 = fromptr+numels;
		cptr1 < cptr2;
		*cptr++ = *cptr1++
		);
	return 0;
}

/*	r e g e r r 
	return regular expression error to apl
*/

static void regerr(ubyte c)
{
	int err;

	switch(c)
		{
	case 11:			// Range endpoint too large	
	case 43:			// Too many \(			
	case 50: error(LIMIT);		// Regular expression overflow	
	case 16: err = 0; break;	// Bad number			
	case 25: err = 1; break;	// "\digit" out of range	
	case 36: err = 2; break;	// Illegal or missing delimiter	
	case 41: err = 3; break;	// No remembered search string	
	case 42: err = 4; break;	// \( \) imbalance		
	case 44: err = 5; break;	// More than 2 numbers in \{ \}	
	case 45: err = 6; break;	// } expected after \.
	case 46: err = 7; break;	// 1st no. > 2nd in \{ \}	
	case 49: err = 8; break;	// [ ] imbalance		
	case 51: err = 9; break;	// Character not inside valid range
		}
	error(XUT_ERRBASE + err);
}

/*	d e l i m i t e r
	return the character in an optional singleton array
*/
static achar delimiter(ARRAY *array)
{
	achar delim = AV_NEWLINE;

	if (array)
		{
		if(1!=arraybound(array)) error(LENGTH);
	
		switch (array->eltype)
			{
			default:
				error(EDOMAIN);
			case APLNCHAR:
			case APLWCHAR8:
				delim = *(aplWchar8 *)arraystart(array);
				break;
			case APLWCHAR16:
				delim = *(aplWchar16*)arraystart(array);
				break;
			case APLWCHAR32:
				delim = *(aplWchar32*)arraystart(array);
				break;
			}
		}
	return delim;
}

/*	m k v e c t o r
	return pointer to a text vector with specified text in it
*/
static ARRAY	*mkvector(void* from, int eltype, BOUND n)
{
	ARRAY	*rslt;

	rslt = mkarray(eltype, 1, &n);
	et_memcpy_et(arraystart(rslt),eltype,from,eltype,n);
	return rslt;
}

/*	u m a x
	UNS max
*/
static UNS	umax(UNS a,UNS b)
{
	return a>b ? a : b;
}

/*	l t o v
	linelist to vector of vectors
*/

#define SUB_LTOV(etype,type)							\
										\
static ARRAY	*ltov_##etype(ARRAY *larg, ARRAY *rarg)			\
{										\
	type	*cp, *cpstart, *cpend;						\
	achar	delim;								\
	ARRAY	*rslt, **vptr;							\
	BOUND	ndelims;							\
										\
	delim = delimiter(larg);						\
	if(0==ELTYPE_ISCHAR(rarg->eltype)) error(EDOMAIN);			\
	cpstart = arraystart(rarg);						\
	cpend = cpstart + arraybound(rarg);					\
										\
	/* count delimiters	*/						\
	ndelims = 0;								\
	if(cpend != cpstart)							\
		{for(cp=cpstart; cp<cpend; cp++) if(delim == *cp) ndelims++;	\
		if(delim != *--cp) ndelims++; /* supply trailing delim */	\
		}								\
	rslt = mkarray(APLPNTR, 1, &ndelims);					\
										\
	vptr =  arraystart(rslt);						\
	if(ndelims)	/* install each vector in turn*/			\
		{for(cp=cpstart; cp<cpend; cp++)				\
			{if(delim != *cp) continue;				\
			*vptr++ = mkvector(cpstart, etype, cp-cpstart);	\
			cpstart = 1+cp;						\
			}							\
		if(delim != *--cp)						\
			*vptr = mkvector(cpstart,etype, 1+(cp-cpstart)); ++cp;	\
		}								\
	else *vptr = mkvector(cpstart,etype,0);	/* prototype */		\
	return rslt;								\
}			

SUB_LTOV(APLNCHAR, aplchar8)
SUB_LTOV(APLWCHAR8, aplWchar8)
SUB_LTOV(APLWCHAR16, aplWchar16)
SUB_LTOV(APLWCHAR32, aplWchar32)

static ARRAY	*ltov(ARRAY *larg, ARRAY *rarg)			
{										
switch (rarg->eltype)
	{
	case APLNCHAR:
		return ltov_APLNCHAR(larg,rarg);
	case APLWCHAR8:
		return ltov_APLWCHAR8(larg,rarg);
	case APLWCHAR16:
		return ltov_APLWCHAR16(larg,rarg);
	case APLWCHAR32:
		return ltov_APLWCHAR32(larg,rarg);
	}
domain();
return 0; // keep compiler happy
}

/*	v t o l
	vector of vector to linelist
*/
static ARRAY *vtol(ARRAY *larg, ARRAY *rarg)
{
	ARRAY	**vp, **vstart, **vend, *rslt;
	achar	delim;
	achar*	cp;
	BOUND	nchars;

	switch (ELTYPE_TYPE(rarg->eltype))
		{
		case ELTYPE_CHAR:	return rarg;
		case ELTYPE_PNTR:	break;
		default:	error(EDOMAIN);
		}

	delim = delimiter(larg);
	vstart =  arraystart(rarg);
	vend = vstart + arraybound(rarg);

	// count characters & check single level nested text	
	nchars = vend-vstart;
	for(vp=vstart; vp<vend; vp++)
		{
		if(0==ELTYPE_ISCHAR((*vp)->eltype)) error(EDOMAIN);
		nchars += arraybound(*vp);
		}
	rslt = mkarray(APLACHAR, 1, &nchars);

	// copy in text, with trailing delimiter	
	cp = arraystart(rslt);
	for(vp=vstart; vp<vend; vp++)
		{
		nchars = arraybound(*vp);
		et_memcpy_et(cp,APLACHAR,arraystart(*vp), (*vp)->eltype, nchars);
		cp+=nchars;
		*cp++ = delim;
		}

	return rslt;
}

#define SUB_MTOL(type)								\
static ARRAY *SUB_MTOL_##type(ARRAY *rarg, int delim, BOUND nrows, BOUND ncols)	\
{										\
	type    *to, *rowstart,*rp, *cp,*rsltbase;				\
	BOUND n = nrows*(1+ncols);						\
	BOUND i;								\
	ARRAY * rslt = mkarray(rarg->eltype, 1, &n);				\
										\
	cp = rsltbase = arraystart(rslt);					\
										\
	for(rowstart=arraystart(rarg); nrows--; rowstart+=ncols)		\
		{								\
		to = rowstart+ncols;						\
		while(--to >= rowstart && AV_SP== *to);				\
		n = (1+to)-rowstart;						\
										\
		for(i=n,rp=rowstart;i--;) *cp++ = *rp++;			\
										\
		*cp++ = delim;							\
		}								\
	rslt->shape[0] = n = cp - rsltbase;					\
	rslt->length = ((n*sizeof(type))+sizeof(ARRAY)+sizeof(MEMORYINT)-1)/sizeof(MEMORYINT);	\
										\
	return rslt;								\
}

SUB_MTOL(aplWchar8)
SUB_MTOL(aplWchar16)
SUB_MTOL(aplWchar32)

/*	m t o l
	matrix to linelist with removal of trailing blanks
*/
static ARRAY *mtol(ARRAY *larg, ARRAY *rarg)
{
	achar	delim;

	ARRAY	*rslt;
	BOUND	ncols, nrows;

	delim = delimiter(larg);
	nrows=ncols=1;
	if(rarg->rank)
		{
		BOUND n = rarg->rank;
		ncols = rarg->shape[--n];
		while(n)
			nrows *= rarg->shape[--n];
		}

	switch (rarg->eltype)
		{
		default:
			error(EDOMAIN);
			break;
		case APLNCHAR:
		case APLWCHAR8:
			rslt = SUB_MTOL_aplWchar8(rarg,delim,nrows,ncols);
			break;
		case APLWCHAR16:
			rslt = SUB_MTOL_aplWchar16(rarg,delim,nrows,ncols);
			break;
		case APLWCHAR32:
			rslt = SUB_MTOL_aplWchar32(rarg,delim,nrows,ncols);
			break;
		}

	return rslt;
}

/*	m a k e r o w 
	copy n chars padding to length with spaces
*/
static void makerow(void* from,void*  to,int eltype, BOUND length,BOUND n)
{
	et_memcpy_et(to,eltype,from,eltype,n);
	to=(n * ELTYPE_SIZE(eltype)) + (unsigned char *)to;
	et_memset(to,eltype,AV_SP,length-n);
}

/*	l t o m
	linelist to matrix
*/
#define SUB_LTOM(etype, type)					\
								\
static ARRAY *ltom_##etype(ARRAY *larg, ARRAY *rarg)		\
{								\
	achar	delim;						\
	type	*cp, *rend, *rstart, *last, *to;		\
	BOUND	shape[2], n;					\
	ARRAY	*rslt;						\
								\
	delim = delimiter(larg);				\
	if(0==ELTYPE_ISCHAR(rarg->eltype)) error(EDOMAIN);	\
	rstart = arraystart(rarg);				\
	rend = rstart + arraybound(rarg);			\
								\
	/* count rows & longest line	*/			\
	shape[0]=shape[1]=0;					\
	for(last=cp=rstart; cp<rend; cp++)			\
		{if(delim != *cp) continue;			\
		shape[0]++;					\
		shape[1] = umax(shape[1], cp-last);		\
		last = 1+cp;					\
		}						\
	if(rstart!=rend && delim != *(cp-1))			\
		{shape[0]++;					\
		shape[1] = umax(shape[1], cp-last);		\
		}						\
	rslt = mkarray(etype, 2, shape);			\
								\
	/* copy in rows, with trailing spaces	*/		\
	to = arraystart(rslt);					\
	for(last=cp=rstart; cp<rend; cp++)			\
		{if(delim != *cp) continue;			\
		n = cp-last;					\
		makerow(last,to,etype,shape[1],n);		\
		to += shape[1];					\
		last = 1+cp;					\
		}						\
	if(rstart!=rend && delim != *(cp-1)) 			\
		makerow(last,to,etype,shape[1],cp-last);	\
								\
	return rslt;						\
}					
					

SUB_LTOM(APLNCHAR, aplchar8)
SUB_LTOM(APLWCHAR8, aplWchar8)
SUB_LTOM(APLWCHAR16, aplWchar16)
SUB_LTOM(APLWCHAR32, aplWchar32)

static ARRAY	*ltom(ARRAY *larg, ARRAY *rarg)			
{										
switch (rarg->eltype)
	{
	case APLNCHAR:
		return ltom_APLNCHAR(larg,rarg);
	case APLWCHAR8:
		return ltom_APLWCHAR8(larg,rarg);
	case APLWCHAR16:
		return ltom_APLWCHAR16(larg,rarg);
	case APLWCHAR32:
		return ltom_APLWCHAR32(larg,rarg);
	}
domain();
return 0; // keep compiler happy
}

/*	b o x
	simulates quadbox function.
*/
	union	els
			{
			aplint16	i;
			aplint32	l;
			aplfloat64	d;
			aplcmpx128	z;
			apldecf128	D;
			} fill, delim;
	union	points
			{
			aplint8		*s;
			aplint16	*i;
			aplint32	*l;
			aplfloat64	*d;
			aplcmpx128	*z;
			apldecf128	*D;
			} from, to;

	typedef void	SETFN(ARRAY *, UNS);
	typedef SETFN	*SETFNP;
	static	SETFN	setintg, setlong, setdoub, setwch16,setwch32,setcmpx,setdecf;
	typedef void	ADDFN(short);
	typedef ADDFN	*ADDFNP;
	static	ADDFN	addintg, addlong, adddoub, addwch16,addwch32,addcmpx,adddecf;
	typedef UNS	CMPFN(short, void *);
	typedef CMPFN	*CMPFNP;
	static	CMPFN	cmpintg, cmplong, cmpdoub, cmpwch16,cmpwch32,cmpcmpx,cmpdecf;

	SETFNP	setfns[] =
			{
			setintg, // not used 
			setintg, // not used 
			setintg, // not used 
			setintg, // APLINTG
			setlong, // APLLONG
			setdoub, // APLDOUB
			setintg, // not used 
			setintg, // not used
			setwch16,// APLWCHAR16
			setwch32,// APLWCHAR32
			setcmpx, // APLCMPX
			setintg, // not used
			setdecf, // APLDECF
			};
	ADDFNP	addfns[] =
			{
			addintg, // not used 
			addintg, // not used 
			addintg, // not used 
			addintg,
			addlong,
			adddoub,
			addintg, // not used 
			addintg, // not used
			addintg,
			addlong,
			addcmpx, // APLCMPX
			addintg, // not used
			adddecf, // APLDECF
			};
	CMPFNP	cmpfns[] = 
			{
			cmpintg, // not used 
			cmpintg, // not used 
			cmpintg, // not used 
			cmpintg,
			cmplong,
			cmpdoub,
			cmpintg, // not used 
			cmpintg, // not used 
			cmpintg,
			cmplong,
			cmpcmpx, // APLCMPX
			cmpintg, // not used
			cmpdecf, // APLDECF
			};

static ARRAY *box(ARRAY *larg, ARRAY *rarg)
{
	UNS	i, rowels;
	aplint8 *begrow;
	CMPFN	*cmp;
	ADDFN	*add;
	aplint8 *k;
	aplint8 *ptr;
	BOUND	shape[2], rowsize, arrayend, elsize,
		sh0, sh1, rsh0, rel, rra, lel;
	ARRAY	*rslt, *templ, *tempr;

		//	check rank and data type	

	templ = tempr = 0;		// local temp arrayed must be freed  

	if((rel=rarg->eltype) == APLPNTR)
		error(EDOMAIN);
	else if((rra=rarg->rank) >2)
		error(RANK);
	if(larg) 
		if((lel=larg->eltype) == APLPNTR)
			error(EDOMAIN);
		else if(larg->rank > 1)
			error(RANK);
		else if( (i=arraybound(larg)) < 1 || i > 2)
			error(LENGTH);

	shape[0] = shape[1] = 0;

		//	special-case empty arrays	
	if(!arraybound(rarg))
		return mkarray(rel, (rra == 2 ? 1 : 2), shape);
		//	process left arg		
	if(larg)
		{
		if(lel > rel)	// coerce rarg to largtype 
			{
			tempr = (*convfns[rel][lel])(rarg);
			freearray(rarg); *Rarg = rarg = tempr;
			tempr = 0;
			rel = lel;
			}
		else if(rel > lel)	// or vice versa	
			{
			templ = (*convfns[lel][rel])(larg);
			freearray(larg); *Larg = larg = templ;
			templ = 0;
			lel = rel;
			}
		else if(rel == APLBOOL)	// if both APLBOOL change to SINTS	
			{
			templ = (*convfns[lel][APLSINT])(larg);
			freearray(larg);  *Larg = larg = templ;
			templ = 0;
			lel = APLSINT;

			tempr = (*convfns[APLBOOL][APLSINT])(rarg);
			freearray(rarg); *Rarg = rarg = tempr;
			tempr = 0;
			rel = APLSINT;
			}
		}
	else	// if no larg but rarg is APLBOOL change it to APLSINT	
		if(rel == APLBOOL)
			{
			tempr = (*convfns[APLBOOL][APLSINT])(rarg);
			freearray(rarg); *Rarg = rarg = tempr;
			tempr = 0;
			rel = APLSINT;
			}

	if(rel == APLNCHAR || rel==APLSINT || rel==APLWCHAR8) // fast version for onebytes
	{
		if(larg && lel != rel)
			error(EDOMAIN);	// Num/Char clash 
		else
		{
			rslt = cbox(larg, rarg, i);

			if (templ)
				freearray(templ);

			if (tempr)
				freearray(tempr);

			return rslt;
		}
	}

	(*setfns[rel])(larg, i);
	add = addfns[rel];
	cmp = cmpfns[rel];
	rsh0 = rarg->shape[0];
	ptr = arraystart(rarg);
		
	
	switch(rra)
		{
	case 0:
	case 1:	//	rarg is vector - result is matrix	
		sh0 = 1; sh1 = rowels = 0;
		arrayend = (rra ? rsh0 : 1);
		from.s = ptr;
		for(i = 0;i<arrayend;(*add)(0),++i)
			if((*cmp)(1, 0))
				{
				++sh0;
				if(rowels > sh1) sh1 = rowels;
				rowels = 0;
				}
			else ++rowels;
		if(rowels > sh1) sh1 = rowels;
		shape[0] = sh0; shape[1] = sh1;
		to.s = arraystart(rslt= mkarray(rel, 2, shape));
		from.s = ptr;
		for(rowels = 1,i= 0; i<arrayend; ++i,(*add)(0), ++rowels)
			if((*cmp)(1, 0))
				{
				while(rowels++ <= sh1)
					(*add)(1);
				rowels = 0;
				}
			else	(*add)(2);
		while(rowels++ <= sh1)
			(*add)(1);
		break;
	case 2:	//	rarg is a matrix - result is a vector	
		rowsize = (elsize = sizeel(rel)) * rarg->shape[1];

		begrow = ptr - elsize;		// 1. Get size of result 
		for(i = sh0 = 0;i < rsh0;++i)
		{
			k = begrow + rowsize;
			while (k > begrow)
			{
				if(!(*cmp)(2, k)) break;
				k -= elsize;
			}
			sh0 += (k - begrow) / elsize;
			if(i+1 != rsh0) sh0++;
			begrow += rowsize;
		}

		begrow = ptr - elsize;		// 2. Build result 
		to.s = arraystart(rslt = mkarray(rel, 1, &sh0));
		for(i = 0; i < rsh0; ++i)
		{
			k = begrow + rowsize;
			while (k > begrow)
			{
				if(!(*cmp)(2, k)) break;
				k -= elsize;
			}
			for(ptr=begrow + elsize ; ptr < k+elsize;)
				*to.s++ = *ptr++;
			if(i+1 != rsh0)
				(*add)(3);
			begrow += rowsize;
		}	
		}

	if (templ)
		freearray(templ);

	if (tempr)
		freearray(tempr);

	return rslt;
}

/*	c b o x
	faster version of box - just for chars and sints.
	If delimiter and fill are both AV_SP, then box must suppress multiple
	occurences.  So box box 'andy  andy   andy' becomes 'andy andy andy'
*/
static ARRAY *cbox(ARRAY *larg, ARRAY *rarg, UNS bound)
{
	ubyte	delim, fill, csflg, special_case;
	ubyte	*to, *from, *begrow, *k, *ptr, *arrayend;
	UNS	i;
	BOUND	shape[2], rowels, rowsize, sh0, sh1, rsh0;
	ARRAY	*rslt;
	uns eltype = rarg->eltype;
	static	ubyte prot[] = {'\0',0x04,0x20};	// '0', av space, unicode space

	csflg = (APLSINT  == eltype) ? 0:
		(APLNCHAR == eltype) ? 1:2;

	delim = fill = prot[csflg];

	if(larg)
		{
		ptr = arraystart(larg);
		switch(bound)
			{
		case 2:
			fill  = ptr[1];
		case 1:
			delim = ptr[0];
			}
		}
	
	special_case = ((fill == 0x04) && (delim == 0x04)
		||	(fill == 0x20) && (delim == 0x20));
	
	rsh0 = rarg->shape[0];
	ptr = arraystart(rarg);
	switch(rarg->rank)
		{
	case 0:
	case 1:	//	rarg is vector - result is matrix	
		sh0 = 1; sh1 = rowels = 0;
		from = ptr;
		arrayend = from + (rarg->rank ? rsh0 : 1);
		for(; from < arrayend; ++from)
			if(*from == delim)
				{
				++sh0;
				if(rowels > sh1) sh1 = rowels;
				rowels = 0;
				}
			else
				++rowels;
		if(rowels > sh1) sh1 = rowels;
		shape[0] = sh0; shape[1] = sh1;
		to= arraystart(rslt= mkarray(eltype, 2, shape));
		from = ptr;
		for(rowels= 1; from < arrayend; ++from, ++rowels)
			if(*from == delim)
				{
				while(rowels++ <= sh1)
					*to++ = fill;
				rowels = 0;
				}
			else
				*to++ = *from;
		while(rowels++ <= sh1)
			*to++=fill;
		break;
	case 2:	//	rarg is a matrix - result is a vector	
		rowsize = rarg->shape[1];
		begrow = ptr - 1;
		for(i = sh0 = 0 ;i < rsh0 ;++i)
		{
			k = begrow + rowsize;
			while (k > begrow)
			{
				if (*k != fill) break;
				k--;
			}
			sh0 += k - begrow;
			/* Don't have delimiter if last row, or
			   if special case and at start of row */
			if (i+1 != rsh0)  
				if(!(k==begrow && special_case))
					sh0++;
			begrow += rowsize;
		}

		to = arraystart(rslt = mkarray(eltype, 1, &sh0));
		begrow = ptr - 1;
		for(i = 0; i < rsh0; ++i)
		{
			k = begrow + rowsize;
			while (k > begrow)
			{
				if (*k != fill) break;
				k--;
			}
			for(ptr=begrow+1; ptr <=k;)
				*to++ = *ptr++;
			if(i+1 !=rsh0)
				if(!(k==begrow && special_case))
						*to++ = delim;
			begrow += rowsize;
		}
		}	// switch 
	return rslt;
}

/*	s e t   f u n c t i o n s
	initialise the fill and or delimiter elements depending
	on the type of box's larg.
*/

static void setwch16(ARRAY *larg, UNS lbnd)
{
	aplint16	*ptr;

	delim.i = fill.i = (aplint16)0x20;

	if(larg)
		{
		ptr = arraystart(larg);
		switch(lbnd)
			{
		case 2: fill.i = ptr[1];
		case 1: delim.i = ptr[0];
			}
		}
}

static void setwch32(ARRAY *larg, UNS lbnd)
{
	aplint32	*ptr;

	delim.l = fill.l = (aplint32)0x20;
	if(larg)
		{
		ptr = arraystart(larg);
		switch(lbnd)
			{
		case 2: fill.l = ptr[1];
		case 1: delim.l = ptr[0];
			}
		}
}

static void setintg(ARRAY *larg, UNS lbnd)
{
	aplint16	*ptr;

	delim.i = fill.i = (aplint16)0;
	if(larg)
		{
		ptr = arraystart(larg);
		switch(lbnd)
			{
		case 2: fill.i = ptr[1];
		case 1: delim.i = ptr[0];
			}
		}
}

static void setlong(ARRAY *larg, UNS lbnd)
{
	aplint32	*ptr;

	delim.l = fill.l = (aplint32)0;
	if(larg)
		{
		ptr = arraystart(larg);
		switch(lbnd)
			{
		case 2: fill.l = ptr[1];
		case 1: delim.l = ptr[0];
			}
		}
}

static void setdoub(ARRAY *larg, UNS lbnd)
{
	aplfloat64		*ptr;

	delim.d = fill.d = 0.0;
	if(larg)
		{
		ptr = arraystart(larg);
		switch(lbnd)
			{
		case 2: fill.d = ptr[1];
		case 1: delim.d = ptr[0];
			}
		}
}

static void setcmpx(ARRAY *larg, UNS lbnd)
{
	aplcmpx128		*ptr;
	aplcmpx128		zero = {0.0, 0.0};

	delim.z = fill.z = zero;
	if(larg)
		{
		ptr = arraystart(larg);
		switch(lbnd)
			{
		case 2: fill.z = ptr[1];
		case 1: delim.z = ptr[0];
			}
		}
}

static void setdecf(ARRAY *larg, UNS lbnd)
{
	apldecf128		*ptr;
	apldecf128		zero = {0};	// all zero bits is a valid
						// representation of DECF_ZERO 

	delim.D = fill.D = zero;
	if(larg)
		{
		ptr = arraystart(larg);
		switch(lbnd)
			{
		case 2: fill.D = ptr[1];
		case 1: delim.D = ptr[0];
			}
		}
}

/*	a d d   f u n c t i o n s 
	add an element of one type to the results array.
	entry codes:
	0 = increment relevant member of the `from' union.
	1 = add fill element from the `fill' union.
	2 = add data from the `from' union.
	3 = add delimiter from the `delim' union.
*/


static void addintg(short int code)
{
	if(code)
		*to.i++ = (code==1)? fill.i :(code==2)?*from.i :delim.i;
	else
		++from.i;
}

static void addlong(short int code)
{
	if(code)
		*to.l++ = (code==1)? fill.l :(code==2)?*from.l :delim.l;
	else
		++from.l;
}

static void adddoub(short int code)
{
	if(code)
		*to.d++ = (code==1)? fill.d :(code==2)?*from.d :delim.d;
	else
		++from.d;
}

static void addcmpx(short int code)
{
	if(code)
		*to.z++ = (code==1)? fill.z :(code==2)?*from.z :delim.z;
	else
		++from.d;
}


static void adddecf(short int code)
{
	if(code)
		*to.D++ = (code==1)? fill.D :(code==2)?*from.D :delim.D;
	else
		++from.d;
}

/*	c m p   f u n c t i o n s
	compare element of data to either fill element or delimiter.
	arguments:
	1 = compare data in `from' union to element in `delim' union.
	2, pointer = compare the *ptr to element in `fill' union.
*/

static UNS cmpintg(short int code, void *vptr)
{
	return 1 == code ? delim.i == *from.i : fill.i == *(aplint16 *)vptr;
}

static UNS cmplong(short int code, void *vptr)
{
	return 1 == code ? delim.l == *from.l : fill.l == *(aplint32 *)vptr;
}

static UNS cmpdoub(short int code, void *vptr)
{
	return 1 == code ? delim.d == *from.d : fill.d == *(aplfloat64 *)vptr;
}

static UNS cmpcmpx(short int code, void *vptr)
{
	return 1 == code
	?	0 == memcmp(&delim.z, from.z, sizeof(aplcmpx128))
	:	0 == memcmp(&fill.z, vptr, sizeof(aplcmpx128))
	;
}

static UNS cmpdecf(short int code, void *vptr)
{
	return 1 == code
	?	0 == decf_eq(delim.D, *from.D)
	:	decf_eq(fill.D, *(apldecf128 *)vptr)
	;
}

/*	s i z e e l
	returns size in bytes of apl variables
*/
	
static int sizeel(UNS eltype)
{
	static int elsize[] =
		{
		/* 0 APLNCHAR */	sizeof(aplchar8),
		/* 1 APLBOOL */		-1,
		/* 2 APLSINT */		sizeof(aplint8),
		/* 3 APLINTG */		sizeof(aplint16),
		/* 4 APLLONG */		sizeof(aplint32),
		/* 5 APLDOUB */		sizeof(aplfloat64),
		/* 4 APLLONG */		sizeof(aplint32),
		/* 1 APLWCHAR8 */	sizeof(aplWchar8),
		/* 2 APLWCHAR16 */	sizeof(aplWchar16),
		/* 4 APLWCHAR32 */	sizeof(aplWchar32),
		/* 8 APLCMPX */		sizeof(aplcmpx128),
		/* not used  */		0,
		/* 8 APLDECF */		sizeof(apldecf128)
		};

	return elsize[eltype];
}

static ubyte* zmalloc(UNS size)
{
	ubyte* ret = malloc(size + !size); // malloc(0) is undefined

	if(0 == ret)
		error(LIMIT);
	return	ret;
}

static ubyte* zrealloc(ubyte* ptr, UNS size)
{
	ptr = realloc(ptr, size);
	if(0 == ptr)
		error(LIMIT);
	return	ptr;
}

#ifdef	DOS

#if	0

	{R} <- {X} copyseg Y

	Y specifies attributes of X and R
	Y is a 4 element numeric vector
	Y[0] <-> size of each item in bytes 1 or 2 or 4
	Y[1] <-> number of items
	Y[2] <-> protected mode segment number, some useful values are
		28 <-> maps the screen
		44 <-> maps the enviroment block
		64 <-> maps all of physical memory
		96 <-> maps the first 1 Meg of memory ( MS-DOS )
	Y[3] <-> offset of first item in segment

	X if present specifies the values to be written to memory
	X is a numeric scalar or vector of signed values which fit in width Y[0]

	R is the values which were in those memory locations before the write.

	To find out the contents of the first row of the screen
		+copyseg 1 160 28 0
	gives a vector of length 160 containg char,attr,char,attr,...

	To write to the screen a row of white 'D's on a blue background
		(160 reshape 68 23)copyseg 1 160 28 0

	Note:
	This code works with protected mode addresses. If you want to access
	a real mode address seg:offset then the protected mode address is
		protected segment 96
		protected offset 16 1+.x seg offset
#endif	// 0 

#include	<hardware.h>

static ARRAY *copyseg(ARRAY *larg, ARRAY *rarg)
{
	unsigned nitems, item_width, nbytes, restype;
	ARRAY *rslt;

	switch(rarg->eltype)
		{
	default:domain();		// rarg must be numeric simple 
	case APLBOOL: case APLSINT: case APLINTG: case APLDOUB:	// convert it to APLLONG 
		{
		ARRAY *tmp = (*convfns[rarg->eltype][APLLONG])(rarg);
	
		freearray(rarg); *Rarg = rarg = tmp;
		}
	case APLLONG:
		break;
		}
	switch(rarg->rank)
		{
	default:error(RANK);		// rarg must be a vector 
	case 1: break;
		}
	switch(rarg->shape[0])
		{
	default:error(LENGTH);
	case 4:	break;
		}
	if(rarg->shape[2] < 0) domain();
	nitems = rarg->shape[2];
	restype = APLSINT;
	switch(item_width = rarg->shape[1])
		{
	default:domain();	// only some data widths are supported 
	case 4:	++restype;
	case 2:	++restype;
	case 1:	break;
		}
	if(larg)
		{
		switch(larg->eltype)
			{
		default:domain();	// must be a numeric type 
		case APLBOOL: case APLSINT: case APLINTG: case APLLONG: case APLDOUB:
			if(larg->eltype != restype)
				{
				ARRAY *tmp = (*convfns[larg->eltype][restype])(larg);

				freearray(larg);
				*Larg = larg = tmp;
				}
			break;
			}
		switch(larg->rank)
			{
		default:error(RANK);
		case 0:
			{
			unsigned one[1] = { 1 };
			ARRAY *tmp = mkarray(restype, 1, one);
		
			tmp->shape[1] = larg->shape[0];
			freearray(larg);
			*Larg = larg = tmp;
			}
		case 1:
			if(larg->shape[0] != nitems) error(LENGTH);
			break;
			}
		}

	nbytes = item_width * nitems;
	*Rslt = rslt = mkarray(restype, 1, &nitems);
		{
		FARP remote = { rarg->shape[4], rarg->shape[3] };
		FARP remtop = { rarg->shape[4] + nbytes - 1, rarg->shape[3] };
		FARP to     = { (unsigned)&rslt->shape[1], MYDATA() };

		switch(TESTACCESS(&remote))
			{
		default:domain();		// no access 
		case 1: if(larg) domain();	// readable  
		case 3:	break;			// read and write 
			}
		if(nbytes > 1)switch(TESTACCESS(&remtop))
			{
		default:domain();
		case 1: if(larg) domain();
		case 3:	break;
			}
		COPYSEG(&to, &remote, nbytes);
		if(larg)
			{
			FARP from = { (unsigned)&larg->shape[1], MYDATA() };

			COPYSEG(&remote, &from, nbytes);
			}
		}
	return rslt;
}


#if	0

	{R} <- {X} interrupt Y

	Issue Phar Lap interrupt.

	Y is a numeric integer scalar between 0 and 255.

	X if present must be a nine element numeric integer vector
	  which specifies the contents of the registers in the order
	  EAX, EBX, ECX, EDX, ESI, EDI, DS, ES, EFLAGS

	  If X is not present it is treated as 9 reshape 0.

	R is the content of the registers after the interrupt has returned.

	eg. To find out the total and unused portions of the C: drive you might
	use
		EDX <- 3
		EAX <- 54x256
		REG <- EAX 0 0 EDX 0 0 0 0 0
		+REG interrupt 33

	notes:
	For calls that indicate failure by setting the carry flag the EFLAGS
	register in the result will be odd.

	The DS and ES registers are for protected mode segments. If given
	values they must specify writeable segments.

	To use the DS or ES registers a previous call must have allocated some
	memory eg

	QuadIO <- 0
	EBX <- 1
	EAX <- 72x256					function 48 hex
	REG <- EAX EBX 0 0 0 0 0 0 0 interrupt 33	request 1 page seg
	QuadSIGNAL (2|REG[8])/10			limit error if failed
	DS <- (2*16)|REG[0]				new seg in AX
	EAX <- 71x256					function 47 hex
	EDX <- 3					drive C:
	ESI <- 0					DS:0
	REG <- EAX 0 0 EDX ESI 0 DS 0 0 interrupt 33	get current directory
	QuadSIGNAL (2|REG[8])/11			domain error if failed
	NAME <- copyseg 1 64 DS 0			get the 64 byte buffer
	NAME <- (NAME iota 0) take NAME			ASCIIZ string
	ASCIIAV[NAME]					current C: directory
	ES <- DS
	EAX <- 73x256					function 49 hex
	REG <- EAX 0 0 0 0 0 0 ES 0 interrupt 33	free segment
	QuadSIGNAL (2|REG[8])/11			domain error if failed

#endif	// 0 


#undef	L		// quadav.h macro clashes with msdos.cf 
#undef	H		// quadav.h macro clashes with msdos.cf 
#undef	R		// quadav.h macro clashes with msdos.cf 
#undef	W		// quadav.h macro clashes with msdos.cf 
#undef data		// support.h macro clashes with msdos.cf 
#include <msdos.cf>	// metaware HIGHC include file 
#define NREGS	9	// number of registers to specify 

static ARRAY *interrupt(ARRAY *larg, ARRAY *rarg)
{
	ARRAY *rslt;

	if(!larg)
		{
		unsigned len = NREGS;

		*Larg = larg = mkarray(APLLONG, 1, &len);
		memset(arraystart(larg), 0, len*sizeof(aplint32));
		}
	switch(larg->eltype)
		{
	default: domain();	// must be numeric type 
	case APLBOOL: case APLSINT: case APLINTG: case APLDOUB:
		{
		ARRAY *tmp = (*convfns[larg->eltype][APLLONG])(larg);
	
		freearray(larg); *Larg = larg = tmp;
		}
	case APLLONG:
		break;
		}
	switch(larg->rank)
		{
	default:error(RANK);
	case 1:
		break;
		}
	if(larg->shape[7])	// DS register specified 
		{
		FARP ds = { 0, larg->shape[7] };

		switch(TESTACCESS(&ds))
			{
		default:domain();	// insist on a writable seg 
		case 3: break;
			}
		}
	if(larg->shape[8])	// ES register specified 
		{
		FARP es = { 0, larg->shape[8] };

		switch(TESTACCESS(&es))
			{
		default:domain();	// insist on a writable seg 
		case 3: break;
			}
		}
	switch(larg->shape[0])
		{
	default:error(LENGTH);
	case NREGS:
		break;
		}
	switch(rarg->eltype)
		{
	default: domain();	// must be numeric type 
	case APLBOOL: case APLSINT: case APLINTG: case APLDOUB:
		{
		ARRAY *tmp = (*convfns[rarg->eltype][APLLONG])(rarg);
	
		freearray(rarg); *Rarg = rarg = tmp;
		}
	case APLLONG:
		break;
		}
	switch(rarg->rank)
		{
	default:error(RANK);
	case 0:
		break;
		}
	if(255 < rarg->shape[0] || 0 > rarg->shape[0]) domain();

	{
	unsigned len = NREGS;

	*Rslt = rslt = mkarray(APLLONG, 1, &len);
	}

	memcpy(&Registers, &larg->shape[1], sizeof(Registers));
	callint(rarg->shape[0]);
	memcpy(&rslt->shape[1], &Registers, sizeof(Registers));
	return rslt;
}



#if	0
	{R} <- {X} port Y

	Y is a 3 column matrix of port specifiers

	Y[;0] <-> port number in range 0 to 65535
	Y[;1] <-> port width 1 or 2 or 4 ( note 0=Y[;1]|Y[;0] )
	Y[;2] <-> read or write  0 <-> read, 1 <-> write

	X is a vector of values to be written to the ports
	  the values must be representable as signed integers
	  in the width given in Y[;2].
	
	The length of X is +/Y[;2]

	X may only be absent if 0=+/Y[;2]

	R is a vector of values read from the ports

	The length of R is +/~Y[;2]

	eg. To find the scan code of the last key pressed

		SCAN<-port 1 3 reshape 96 1 0

	eg. To save the font information from an EGA/VGA card
	    Warning DONT try this in desk calculator mode.
	    Nothing must try to write to the screen between the 2 calls on port.

		SET<-4 6 5 0 6 12 4 2
		RESET<-4 2 5 16 6 14 4 0
		PORTS<-960 + 4 5 14 15 14 15 14 15		0x3c0 is 960
		PORTS<-PORTS jot.* 1 0 0	3 col matrix extended with 1s
		SET port PORTS			write only ports on EGA
		FONT<-copyseg 1 (32x1024) 28 0
		RESET port PORTS

	The restriction of supplying correct width signed integers can be
	less onerous if the following function is used.

		[0] NUMS<-BASE unsigned NUMS;SHAPE
		[1] NUMS top commute <- BASE
		[2] SHAPE<- rho NUMS
		[3] (first SHAPE)<- 1
		[4] NUMS x<- 1+(rho NUMS) take SHAPE reshape negative 2
		[5] NUMS base commute <- BASE

	note that BASE must be a vector.

		2 2 2 2 2 2 2 2 unsigned 1 2 254 255
	1 2 -2 -1
		
#endif	// 0 



static ARRAY *port(ARRAY *larg, ARRAY *rarg)
{
	ARRAY *rslt;
	int *resp, *lp, *rp, *lplim;
	unsigned i, nreads;

	if(!larg)
		{
		int len = 0;

		*Larg = larg = mkarray(APLLONG, 1, &len);
		}
	switch(larg->eltype)
		{
	default: domain();	// must be numeric type 
	case APLBOOL: case APLSINT: case APLINTG: case APLDOUB:
		{
		ARRAY *tmp = (*convfns[larg->eltype][APLLONG])(larg);
	
		freearray(larg); *Larg = larg = tmp;
		}
	case APLLONG:
		break;
		}
	switch(larg->rank)
		{
	default:error(RANK);
	case 1:	break;
		}
	switch(rarg->eltype)
		{
	default: domain();	// must be numeric type 
	case APLBOOL: case APLSINT: case APLINTG: case APLDOUB:
		{
		ARRAY *tmp = (*convfns[rarg->eltype][APLLONG])(rarg);
	
		freearray(rarg); *Rarg = rarg = tmp;
		}
	case APLLONG:
		break;
		}
	switch(rarg->rank)
		{
	default:error(RANK);
	case 2: break;
		}
	switch(rarg->shape[1])
		{
	default:error(LENGTH);
	case 3: break;
		}
	rp = (int *)&rarg->shape[2];
	lp = (int *)&larg->shape[1];
	lplim = lp + larg->shape[0];
	for(i = rarg->shape[0] ; i--; rp += 3)
		{
		if(lp == lplim && rp[2]) error(LENGTH);
		lp += testport(lp, rp);
		if(lp > lplim) error(LENGTH);
		}
	if(lp < lplim)error(LENGTH);
	nreads = rarg->shape[0] - larg->shape[0];
	*Rslt = rslt = mkarray(APLLONG, 1, &nreads);
	rp = (int *)&rarg->shape[2];
	lp = (int *)&larg->shape[1];
	resp = (int *)&rslt->shape[1];
	for(i = rarg->shape[0] ; i--; rp += 3)
		{
		if(doport(resp, lp, rp)) lp++;
		else resp++;
		}
	return rslt;
}

static int testport(int *lp, int rvec[3])
{
	if(rvec[0] < 0 || rvec[0] > 65535) domain();
	switch(rvec[1])
		{
	default:domain();
	case 4: if(rvec[0] & 3) domain();
	case 2: if(rvec[0] & 1) domain();
	case 1: break;
		}
	switch(rvec[2])
		{
	default:domain();
	case 1:
		{
		switch(rvec[1])
			{
		case 1:
			{
			volatile ubyte tmp;

			tmp = *lp;
			if(tmp != *lp) domain();
			break;
			}
		case 2:
			{
			volatile short tmp;

			tmp = *lp;
			if(tmp != *lp) domain();
			break;
			}
		case 4:
			break;
			}
		}
	case 0: break;
		}
	return rvec[2];

}

static int doport(int *resp, int *lp, int rvec[3])
{
	static int (*in[5])(int) =
		{ 0, INP,  INPW,  0, INPD };
	static void (*out[5])(int, int) =
		{ 0, OUTP, OUTPW, 0, OUTPD};

	switch(rvec[2])
		{
	case 0:
		*resp = (*in[rvec[1]])(rvec[0]);
		break;
	case 1:
		(*out[rvec[1]])(rvec[0], *lp);
		break;
		}
	
	return rvec[2];
}

#endif	// DOS 

#if	defined(DOSWIN)
/*
	Crude getenv()
	restricted to Upper Case Alpha
	and vectors (does not cope with scalar arguments)
	name must be less than 256 characters long
	no leading or trailing spaces
*/
static ARRAY* gtenv(ARRAY *ignore, ARRAY *rarg)
	{
	gchar name[256];
	BOUND len;
	BOUND *up;
	gchar *envp, *ap;
	ARRAY * rslt;
	int code;

	up = rarg->shape;
	len = *up++;

	if (0==ARRAY_ISCHAR(rarg))
		error(EDOMAIN);

	if (1 != rarg->rank)
		error(RANK);

	if (len > 255)
		error(LENGTH);

	et_memcpy_et(name,APLGCHAR, up, rarg->eltype, len);
	name[len] = 0;
	envp = (void *)ggetenv(name);
	switch(code = 0 != envp)
		{
	case 0:			// the enviroment var does not exist
		len = 0;
		rslt = mkarray(APLLONG, 1, &len);
		break;		// return Zilde
	default:
		len = gstrlen(envp);
		rslt = mkarray(APLGCHAR, 1, &len);
		for
			(
			ap = (void *)&rslt->shape[1]
		;	len--
		;	*ap++ = unixtoav(*envp++)
			);
		break;
		}
	return rslt;
	}

#endif//defined(DOSWIN)

//	Copyright (c) 1982 Dyadic Systems Limited	
