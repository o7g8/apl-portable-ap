#ifndef _APLTYPES_H
#define _APLTYPES_H

#include <sys/types.h>

#if	MAC	// GRS checked
	typedef off_t off64_t;	// MAC file system is 64 bit by default
#endif//MAC

#include <stddef.h>


#define __STDC_CONSTANT_MACROS	// get UINT64_C defined even in C++
#include <stdint.h>		// ISO C 99 standard

#if	defined(_MSC_VER)
	typedef	intptr_t	ssize_t;
	typedef int64_t		off64_t;

	#define PRIu32 "I32u"
	#define PRIu64 "I64u"
	#define PRId32 "I32d"
	#define PRId64 "I64d"

#endif

typedef UNSIGNED_CHAR_TYPE	UCH;
typedef UCH	 	*UCP;
typedef UCH		uch;
typedef unsigned 	UNS;
typedef unsigned	uns;
typedef wchar_t	unicode_t;	// Bill's unicode type

typedef	signed char	sbyte;	
typedef	UNSIGNED_CHAR_TYPE	ubyte;
typedef ubyte	 	*UBP;

typedef ubyte		TOKEN;

typedef uns		TID;


#if	__cplusplus	// has built in bool type
#else //__cplusplus
	#define bool	_Bool
#endif//__cplusplus

typedef	void *VP;

typedef ubyte		aplchar8;

typedef ubyte		aplWchar8;
typedef uint16_t	aplWchar16;
typedef unsigned	aplWchar32;

typedef signed char	aplint8;
typedef int16_t		aplint16;
typedef int32_t		aplint32;
typedef int64_t		aplint64;

typedef	intptr_t	apliptr;
typedef	uintptr_t	apluptr;

typedef uint8_t		apluns8;
typedef uint16_t	apluns16;
typedef uint32_t	apluns32;
typedef uint64_t 	apluns64;

typedef double		aplfloat64;

typedef aplWchar16	symchar_unicode;
typedef aplchar8	symchar_classic;

#if	HAS_UNICODE
	typedef  wchar_t	gchar;
	typedef  aplWchar32	achar;
	typedef  symchar_unicode symchar;	// see p_symbol.h for symchar fns and macros
	#define APLSYMCHAR	APLWCHAR16
#else //HAS_UNICODE
	typedef  aplchar8	achar;
	typedef  symchar_classic symchar;	// see p_symbol.h for symchar fns and macros
	#define APLSYMCHAR	APLNCHAR

	typedef  char		gchar;
#endif//HAS_UNICODE

#define	APLCLASSICSYMCHAR	APLNCHAR
#define	APLUNISYMCHAR		APLWCHAR16

typedef  gchar *	gstr;
typedef  gchar *	GCP;
typedef  gchar 		GCH;

#define gsizeof(buf)		(sizeof(buf)/sizeof(gchar))

typedef	aplint32	aplintmax;	// max encountered

typedef  int64_t 	OFFSET;		// offset (signed bound) (for take, etc)


#if	64 == xxBIT
#elif	32 == xxBIT
#endif//xxBIT
#if	64 == xxBIT

	#define	UINTPTR_C(c)	UINT64_C(c)
	#define	INTPTR_C(c)	INT64_C(c)

	typedef uint64_t 	BOUND;	// ie SHAPETC[]
	typedef	 int64_t	PLENGTH;
	typedef uint64_t	RCOUNT;	// ie REFCOUNT

	#define PLENGTH_MAX	(UINT64_MAX >> 3)
	#define RCOUNT_MAX	UINT64_MAX

	typedef	 int64_t	MEMORYINT;
	typedef	 uint64_t	MEMORYUNS;

	#define INT64_INRANGE_OFFSET(v)	1
	// Note we compare with INT64_MAX+1 (not INT64_MAX) because it is
	// exactly representable in floating point.
	#define DOUB_INRANGE_OFFSET(v)	((v)>=INT64_MIN && (v)<INT64_MAX+1.0)

#elif	32 == xxBIT

	#if	defined _MSC_VER || defined __ANDROID__
		// avoid redefinition warning
		#undef	UINTPTR_C
		#undef	INTPTR_C
		// avoid compiler warning on truncation of 64 bit constant
		#define	UINTPTR_C(c)	(uintptr_t)(UINT64_C(c) & (~(UINT32_C(0))))
		#define	INTPTR_C(c)		(intptr_t)(INT64_C(c) & (~(UINT32_C(0))))
	#else	//	defined_MSC_VER || defined __ANDROID__
		#define	UINTPTR_C(c)	((uintptr_t)UINT64_C(c))
		#define	INTPTR_C(c)		((intptr_t)INT64_C(c))
	#endif	//	defined_MSC_VER || defined __ANDROID__

	typedef uint32_t WARN_W64 	BOUND;	// ie SHAPETC[]
	typedef	 int32_t WARN_W64	PLENGTH;
	typedef uint32_t WARN_W64 	RCOUNT;	// ie REFCOUNT

	#define PLENGTH_MAX		(UINT32_MAX >> 2)
	#define RCOUNT_MAX		UINT32_MAX

	typedef	 int32_t  WARN_W64	MEMORYINT;
	typedef	 uint32_t WARN_W64	MEMORYUNS;

	#define INT64_INRANGE_OFFSET(v)	((v)>=-(OFFSET)UINT32_MAX && (v)<=(OFFSET)UINT32_MAX)
	#define DOUB_INRANGE_OFFSET(v)	INT64_INRANGE_OFFSET(v)

#else //xxBIT
	#error	How many bits?
#endif//xxBIT

#define BOUND_MAX	(~(BOUND)0)
#define UPTR_MAX	(~(UPTR )0)

typedef unsigned		ZONESBITFIELD;

typedef MEMORYINT WARN_W64	IPTR;	// signed integer type that holds ptr
typedef MEMORYUNS WARN_W64 	UPTR;	// unsigned integer type that holds ptr
typedef MEMORYUNS		HEADERTYPE;
typedef	ptrdiff_t		BIAS;

typedef UPTR 			WEAK_REFERENCE;

typedef MEMORYUNS		muns;
typedef MEMORYINT		mint;

typedef int 			OBJINDEX;

#define BITSPERHEADERWORD (8*sizeof(HEADERTYPE))

typedef UPTR	     	XID;		// window IDs

	typedef struct
		{
		double	re;
		double	im;
		} aplcmpx128;	// I hated doing this. However, the gcc lib was
				// of such poor quality compared with
				// Roger Hui's code that I conceded. (GRS)

	#if	0		// code for proper C complex numbers
				// since MSC doesn't support them I found
				// it hard to argue their case
		typedef double _Complex	aplcmpx128;
	#endif//0



// use intel's decf type alignment (16 bytes) so that apldecf128s can be
// passed to intel lib functions.

#if	MSC
	#if	defined _M_IX86 || defined BRIDGE_BUILD
		// MSC on Win IA-32, according to libbid
		#define DECF_ALIGN(n)		// nothing
	#else //defined _M_IX86 || defined BRIDGE_BUILD
		#define DECF_ALIGN(n)		__declspec(align(n))
	#endif//defined _M_IX86 || defined BRIDGE_BUILD
#else //MSC
	#define DECF_ALIGN(n)		__attribute__ ((aligned(n)))
#endif//MSC

typedef DECF_ALIGN(16) struct { uint64_t w[2]; } BID_UINT128;

#if	__STDC_WANT_DEC_FP__
	typedef _Decimal128 apldecf128;
#else //__STDC_WANT_DEC_FP__
	typedef BID_UINT128 apldecf128;
#endif// __STDC_WANT_DEC_FP__


#endif//_APLTYPES_H
