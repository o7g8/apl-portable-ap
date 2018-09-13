//	Copyright (c) 1982 Dyadic Systems Limited	

#include <machine.h>

#if APL_H_INCLUDED
#else
#define APL_H_INCLUDED 1

#include <setjmp.h>
#include <signal.h>

#if	defined(DOS)
#include <dos_comms.h>
#elif	defined(DOSWIN)
#	if	defined(WINDOWS3)
		extern void * parentid;			// HANDLE
#	else
		extern unsigned short parentid;		// HANDLE
#	endif
#	define INPIPE	((unsigned)(IPTR)parentid)
#	define OUTPIPE	((unsigned)(IPTR)parentid)
#endif	// DOS 

#ifdef INPIPE                  // eg apollo wants to override these
#define ARGSPIPE INPIPE
#define RSLTPIPE OUTPIPE
#else
#define ARGSPIPE 3             // pipe allocations
#define RSLTPIPE 4
#endif // INPIPE

#if	64 == xxBIT
#define PNTRlog		6	// 64 bit pointers		
#elif	32 == xxBIT
#define PNTRlog		5	// 32 bit pointers		
#endif
				// function syntax			
#define	RSLT	0x020		// returns a result			
#define	SHYR	0x060		// returns a shy result			
#define	NILA	0x000		// takes no arguments			
#define	MONA	0x004		// takes only a right argument		
#define	DYAD	0x00C		// takes a left and right argument	
#define	OPTL	0x01C		// left argument is optional (ambivalent)
#define	INTR	0x080		// handles its own interrupts		
#define	XWID	0x100		// request to return apl's X window ID  

#define	UNSMASK	(sizeof(MEMORYUNS)-1)
#define	UNSEXTRA(bytes)	((1 + ((bytes) ^ UNSMASK)) & UNSMASK)
#define	UNSROUND(bytes)	((bytes) + UNSEXTRA(bytes))
#define UNSUNITS(bytes)	((UNSROUND(bytes))/sizeof(MEMORYUNS))
#define	BITMASK	(BITSPERHEADERWORD-1)
#define	BITEXTRA(bits)	((1 + ((bits) ^ BITMASK)) & BITMASK)

#if	64 == xxBIT
#		define ALIGNWD(A)					\
			ZONESBITFIELD : (BITEXTRA(A)) - 32;		\
			ZONESBITFIELD : 16;				\
			ZONESBITFIELD : 16

#elif	32 == xxBIT
#	define ALIGNWD(A) ZONESBITFIELD         : (BITEXTRA(A))
#endif//xxBIT

#if	defined(DALIGN)
#	define		DOUBWORDS	(sizeof(double)/sizeof(MEMORYINT))
#endif	// DALIGN 

typedef struct 			// an APL ARRAY				
{

	PLENGTH       length;	// total length in words including self	
	ZONESBITFIELD type:4;   // SIMPLE|NESTED
	ZONESBITFIELD rank:4;   // rank of array
	ZONESBITFIELD eltype:4; // type of all the elements (see below)
	ZONESBITFIELD spare:4;  // to make sure that the following REFCOUNT
				// field  is 8 bit aligned
	ZONESBITFIELD refcount:8;// define a refcount field for macros that
				// think they need one
				// note that the refcount field is ignored in an
				// AP. It is only present to allow
				// the use of internal dyalog macros etc on AP ARRAYs
				// in bridge*.dll etc
	ALIGNWD(24);            // word align

	BOUND         shape[1];	
} ARRAY;


#define DEFINE_ELTYPE_ATTRIBS

#include <eltypes.h>

#define SIMPLE 0xf
#define NESTED 0x7

#if HAS_UNICODE
       #define APLCHAR8                APLWCHAR8
       #define APLACHAR                APLWCHAR32
       #if UNIX
               #define APLGCHAR        APLWCHAR32
       #else
               #define APLGCHAR        APLWCHAR16
       #endif
#else
       #define APLCHAR8                APLNCHAR
       #define APLACHAR                APLNCHAR
       #define APLGCHAR                APLNCHAR
#endif

#define	WSFULL		1	// APL error codes			
#define	SYNTAX		2
#define	INDEX		3
#define	RANK		4
#define	LENGTH		5
#define	VALUE		6
#define	LIMIT		10
#define	EDOMAIN		11	// DOMAIN is defined in math.h
#define	NONCE		16
#define	NOPIPES		30
#define	NOPROCS		76
#define AP_EXIT		77
#define	FERR		100
#define	INTERRUPT	1003
#define	EOF_INTERRUPT	1005

				// NB error codes 200-500 are reserved	
				// for distributed auxiliary processors	
#define	PRE_ERRBASE	200	// 		prefect			
#define	VIA_ERRBASE	220	// 		via			
#define	XUT_ERRBASE	240	// 		xutils			
#define	QFS_ERRBASE	260	//		formerly qfsck			
#define CGI_ERRBASE	280	//	cgi - taking next 53 errors	
#define NFL_ERRBASE	360	//	nfiles - taking next 20 errors	

extern void domain(void);
extern void nonce(void);
extern double getd(void *vbase, BOUND indx);
extern void putd(double val, double *vbase, BOUND indx);

extern aplchar8		(*initchar   (ARRAY *))(void *vbase, BOUND indx);
extern aplint16		(*initintg   (ARRAY *))(void *vbase, BOUND indx);
extern aplint32		(*initdint   (ARRAY *))(void *vbase, BOUND indx);
extern aplint32		(*initlong   (ARRAY *))(void *vbase, BOUND indx);
extern aplfloat64	(*initdoub   (ARRAY *))(void *vbase, BOUND indx);
extern ARRAY*		(*initpntr   (ARRAY *))(void *vbase, BOUND indx);
extern aplWchar8	(*initWchar8 (ARRAY *))(void *vbase, BOUND indx);
extern aplWchar16	(*initWchar16(ARRAY *))(void *vbase, BOUND indx);
extern aplWchar32	(*initWchar32(ARRAY *))(void *vbase, BOUND indx);
extern aplcmpx128	(*initcmpx   (ARRAY *))(void *vbase, BOUND indx);
extern apldecf128	(*initdecf   (ARRAY *))(void *vbase, BOUND indx);

extern ARRAY *(*convfns[NELTYPES][NELTYPES])(ARRAY *);	// array conversion functions

#endif //APL_H_INCLUDED 1

//	Copyright (c) 1982 Dyadic Systems Limited	

