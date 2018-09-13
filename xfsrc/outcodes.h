#ifndef _OUTCODES_H
#define _OUTCODES_H


typedef enum
	{
//	non moded string codes 

	INIT	= -1
,	RESET	= -2

,	V0	= -768		// videos 
,	C0	= -1024		// foreground colours 
#undef	B0			// clashes with <sys/ttydev.h> on aix
,	B0	= -1280		// background colours 

//	moded string codes	

#if	HAS_UNICODE
,	CC	= 257	
,	MODE0	= 258	// start of mode switching area 
#else //HAS_UNICODE
,	CC	= 256	// leave classic where it was, just in case...
,	MODE0	= 257	// start of mode switching area 
#endif//HAS_UNICODE
			// user label is M0	
	} OUTPUT_SPECIAL_CODES;


#endif // _OUTCODES_H
