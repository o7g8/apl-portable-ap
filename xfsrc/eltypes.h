#ifndef _ELTYPES_H
#define _ELTYPES_H

// factored out from pocket.h so that 
// p_cmp can use the types in the include file

typedef enum
	{
	APLNCHAR	=  0	// C APL "Narrow" CHAR
,	APLBOOL		=  1	// B
,	APLSINT		=  2	// S
,	APLINTG		=  3	// I
,	APLLONG		=  4	// L
,	APLDOUB		=  5	// D
,	APLPNTR		=  6	// P
,	APLWCHAR8	=  7	// U APL "Wide" CHAR (squeezed to 8 )
,	APLWCHAR16	=  8 	// V APL "Wide" CHAR (squeezed to 16)
,	APLWCHAR32	=  9 	// W APL "Wide" CHAR (squeezed to 32)

,	APLCMPX		= 10	// Z
,	APLRATS		= 11	// R
,	APLDECF_DPD	= 12	// F
,	APLQUAD		= 13	// Q
,	APLDECF_BID	= 14	// G
				// |
				// `--- Character codes used in acc.c.

,	NELTYPES	= 15	// number of ELTYPEs.

#if	HAS_UNICODE
,	APLCHAR8	= APLWCHAR8
,	APLCHAR		= APLWCHAR32
,	APLGUICHAR	= APLWCHAR	// in port.h, eltype for wchar_t
#else //HAS_UNICODE
,	APLCHAR8	= APLNCHAR
,	APLCHAR		= APLNCHAR
,	APLGUICHAR	= APLNCHAR
#endif//HAS_UNICODE

#if	64 == xxBIT
,	APLIPTR		= APLQUAD
#elif	32 == xxBIT
,	APLIPTR		= APLLONG
#endif//xxBIT

#if __STDC_WANT_DEC_FP__
,	APLDECF		= APLDECF_DPD
#else // __STDC_WANT_DEC_FP__
,	APLDECF		= APLDECF_BID
#endif // __STDC_WANT_DEC_FP__
	} APL_ELTYPE;


#define	CHARlog		3
#define	BOOLlog		0
#define	SINTlog		3
#define	INTGlog		4
#define	LONGlog		5
#define	QUADlog		6
#define	DOUBlog		6
#if	64 == xxBIT
#define PNTRlog		6	// 64 bit pointers		
#elif	32 == xxBIT
#define PNTRlog		5	// 32 bit pointers		
#endif
#define	WCHAR8log	3
#define	WCHAR16log	4
#define	WCHAR32log	5
#define	CMPXlog		7
#define	RATSlog		(1 + PNTRlog)
#define DECFlog		7

#define ELTYPE_CHAR	0x01
#define ELTYPE_NUMB	0x02
#define ELTYPE_PNTR	0x04
#define ELTYPE_FLOT	0x10

#define ELTYPE_TYPE_MASK 	0x0f

typedef struct

	{
	ubyte type;
	ubyte size;
	ubyte shift;
	ubyte shfti;
	ubyte utf8x;		// if this type is encoded to utf8 then this is the max expansion
	ubyte align;		// in MEMORYUNS units
	ubyte arith_level;
	ubyte dummy;		// round size up to a power of two for fast access
	} ELTYPE_ATTRIBS;

typedef enum
	{
	LEVEL_BOOL		// arithmetic levels
,	LEVEL_SINT
,	LEVEL_INTG
,	LEVEL_LONG
,	LEVEL_QUAD
,	LEVEL_RATS
,	LEVEL_FLOAT
,	LEVEL_CMPX		// CMPX has to be larger than DECF. This is why I need this information

,	LEVEL_CLASSIC	
,	LEVEL_CHAR8
,	LEVEL_CHAR16
,	LEVEL_CHAR32

,	LEVEL_PNTR
,	NLEVELS
	} ARITH_LEVEL;

#if	defined(__clang__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wconversion"
#endif

#if defined (DEFINE_ELTYPE_ATTRIBS) || defined(AP_BUILD) || defined(BRIDGE_BUILD)

#if defined(AP_BUILD) || defined(BRIDGE_BUILD)
static
#endif

const ELTYPE_ATTRIBS ELTYPE_PROPS[NELTYPES]=
	{	
/*                   TYPE	              SIZE	                 SHIFT	     SHFTI         UTF8X ALIGN                    LEVEL       */
/*                   |                        |                          |           |		   |	 |                        |           */
/*                   |                        |                          |           |		   |	 |                        |           */
/* APLNCHAR	*/ { ELTYPE_CHAR            , (ubyte)sizeof(aplchar8)  , CHARlog   , CHARlog   -3, 3	,1 << 0                  ,LEVEL_CLASSIC},
/* APLBOOL	*/ { ELTYPE_NUMB            , 0                        , BOOLlog   , 0           , 0	,1 << 0                  ,LEVEL_BOOL   },
/* APLSINT	*/ { ELTYPE_NUMB            , (ubyte)sizeof(aplint8 )  , SINTlog   , SINTlog   -3, 0	,1 << 0                  ,LEVEL_SINT   },
/* APLINTG	*/ { ELTYPE_NUMB            , (ubyte)sizeof(aplint16)  , INTGlog   , INTGlog   -3, 0	,1 << 0                  ,LEVEL_INTG   },
/* APLLONG	*/ { ELTYPE_NUMB            , (ubyte)sizeof(aplint32)  , LONGlog   , LONGlog   -3, 0	,1 << 0                  ,LEVEL_LONG   },
/* APLDOUB	*/ { ELTYPE_NUMB|ELTYPE_FLOT, (ubyte)sizeof(aplfloat64), DOUBlog   , DOUBlog   -3, 0	,1 << (DOUBlog - PNTRlog),LEVEL_FLOAT  },
/* APLPNTR	*/ { ELTYPE_PNTR            , (ubyte)sizeof(void   *)  , PNTRlog   , PNTRlog   -3, 0	,1 << 0                  ,LEVEL_PNTR   },
/* APLWCHAR8	*/ { ELTYPE_CHAR            , (ubyte)sizeof(aplWchar8) , WCHAR8log , WCHAR8log -3, 2	,1 << 0                  ,LEVEL_CHAR8  },
/* APLWCHAR16	*/ { ELTYPE_CHAR            , (ubyte)sizeof(aplWchar16), WCHAR16log, WCHAR16log-3, 3	,1 << 0                  ,LEVEL_CHAR16 },
/* APLWCHAR32	*/ { ELTYPE_CHAR            , (ubyte)sizeof(aplWchar32), WCHAR32log, WCHAR32log-3, 4	,1 << 0                  ,LEVEL_CHAR32 },
/* APLCMPX	*/ { ELTYPE_NUMB|ELTYPE_FLOT, (ubyte)sizeof(aplcmpx128), CMPXlog   , CMPXlog   -3, 0	,1 << (DOUBlog - PNTRlog),LEVEL_CMPX   },
/* APLRATS	*/ { ELTYPE_NUMB|ELTYPE_PNTR, (ubyte)sizeof(void   *)*2, RATSlog   , RATSlog   -3, 0	,1 << 0                  ,LEVEL_RATS   },
/* APLDECF	*/ { ELTYPE_NUMB|ELTYPE_FLOT, (ubyte)sizeof(apldecf128), DECFlog   , DECFlog   -3, 0	,1 << (DECFlog - PNTRlog),LEVEL_FLOAT  },
/* APLQUAD	*/ { ELTYPE_NUMB            , (ubyte)sizeof(aplint64)  , QUADlog   , QUADlog   -3, 0	,1 << (QUADlog - PNTRlog),LEVEL_QUAD   },
/* APLDECF	*/ { ELTYPE_NUMB|ELTYPE_FLOT, (ubyte)sizeof(apldecf128), DECFlog   , DECFlog   -3, 0	,1 << (DECFlog - PNTRlog),LEVEL_FLOAT  },
	};


#if defined(AP_BUILD) || defined(BRIDGE_BUILD)
static
#endif
const unsigned short ELTYPE_SUBSETS[NELTYPES] =
{
// C(e): is e a character eltype?
#define C(e) (e==APLNCHAR || e==APLWCHAR8 || e==APLWCHAR16 || e==APLWCHAR32)
// N(e): is e a numeric eltype?
#define N(e) (e==APLBOOL || e== APLSINT || e==APLINTG || e==APLLONG || e==APLDOUB || e==APLCMPX || e==APLDECF_DPD || e==APLDECF_BID)
// S(e,f): is e a subtype of f? As a convenience for T(f) below, the result is
// returned shifted left by e bits.
#define S(e,f) ((f==APLPNTR || (C(e) && C(f) && e<=f) || (N(e) && N(f) && e<=f && !(e==APLCMPX && (f==APLDECF_DPD||f==APLDECF_BID))))<<e)
// T(f): return a bitmap where bit e says whether e is a subtype of f.
#define T(f) (S(0,f)|S(1,f)|S(2,f)|S(3,f)|S(4,f)|S(5,f)|S(6,f)|S(7,f)|S(8,f)|S(9,f)|S(10,f)|S(11,f)|S(12,f)|S(13,f)|S(14,f))
	T(0),T(1),T(2),T(3),T(4),T(5),T(6),T(7),T(8),T(9),T(10),T(11),T(12),T(13),T(14),
#undef T
#undef S
#undef N
#undef C
};

#else
extern const ELTYPE_ATTRIBS ELTYPE_PROPS[NELTYPES];
extern const unsigned short ELTYPE_SUBSETS[NELTYPES];
#endif // defined (DEFINE_ELTYPE_ATTRIBS) || defined(AP_BUILD) || defined(BRIDGE_BUILD)

#if	defined(__clang__)
	#pragma GCC diagnostic pop
#endif

#define ELTYPE_TYPES(e)		(                   ELTYPE_PROPS[e].type )
#define ELTYPE_SIZE(e)		(                   ELTYPE_PROPS[e].size )
#define ELTYPE_SHIFT(e)		(                   ELTYPE_PROPS[e].shift)
#define ELTYPE_SHFTI(e)		(                   ELTYPE_PROPS[e].shfti)
#define ELTYPE_UTF8X(e)		(                   ELTYPE_PROPS[e].utf8x)
#define ELTYPE_ALIGN(e)		(                   ELTYPE_PROPS[e].align)
#define ELTYPE_LEVEL(e)		(                   ELTYPE_PROPS[e].arith_level)

#define ELTYPE_TYPE(e)		(ELTYPE_TYPE_MASK & ELTYPE_TYPES(e) )
#define ELTYPE_ISCHAR(e)	(ELTYPE_CHAR      & ELTYPE_TYPES(e))
#define ELTYPE_ISNUMB(e)	(ELTYPE_NUMB      & ELTYPE_TYPES(e))
#define ELTYPE_ISFLOT(e)	(ELTYPE_FLOT      & ELTYPE_TYPES(e))
#define ELTYPE_ISFLOAT(e)	 ELTYPE_ISFLOT(e)
#define ELTYPE_ISPNTR(e)	(ELTYPE_PNTR      & ELTYPE_TYPES(e))
// #define ELTYPE_ISPNTR(e)	(APLPNTR == e)
#define ELTYPE_ISINTEGER(e)	(ELTYPE_TYPES(e) == ELTYPE_NUMB)

#if AP_BUILD

#define ARRAY_ISNUMB(p)         ((p)->type==SIMPLE && ELTYPE_ISNUMB((p)->eltype))
#define ARRAY_ISCHAR(p)         ((p)->type==SIMPLE && ELTYPE_ISCHAR((p)->eltype))
#define ARRAY_ISPNTR(p)         ((p)->type==NESTED                              )

#else

#define ARRAY_ISNUMB(p)		((p)->TYPE==TYPESIMPLE && ELTYPE_ISNUMB((p)->ELTYPE))
#define ARRAY_ISINTEGER(p)	((p)->TYPE==TYPESIMPLE && ELTYPE_ISINTEGER((p)->ELTYPE))
#define ARRAY_ISFLOAT(p)	((p)->TYPE==TYPESIMPLE && ELTYPE_ISFLOAT((p)->ELTYPE))
#define ARRAY_ISCHAR(p)		((p)->TYPE==TYPESIMPLE && ELTYPE_ISCHAR((p)->ELTYPE))
#define ARRAY_ISPNTR(p)		((p)->TYPE==TYPEGEN                                 )

#endif

// Is e a subset of f? Can every value of eltype e also be represented exactly
// in eltype f?
#define ELTYPE_SUBSET_OF(e,f) ((ELTYPE_SUBSETS[f] >> (e)) & 1)

#endif // _ELTYPES_H
