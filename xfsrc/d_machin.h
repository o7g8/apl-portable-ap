#ifndef _D_MACHIN_H
#define _D_MACHIN_H

#if defined(__cplusplus)
#define C_EXTERN "C"
#else
#define C_EXTERN 
#endif

#define APLBUSY (AplBusy())

#define APLVERSION	"don't use APLVERSION ,svn should handle that for us"

//	Macros which may be overridden by machine.h	


#ifndef HAS_TRANS
	#if ENGINE
		#define HAS_TRANS 0
	#else
		#define HAS_TRANS 1
	#endif
#endif

#ifndef HAS_QUADSM
	#if ENGINE
		#define HAS_QUADSM 0
	#else
		#define HAS_QUADSM 1
	#endif
#endif

#ifndef RIDE_LOCAL
	#if ENGINE
		#define RIDE_LOCAL 1
	#else
		#define RIDE_LOCAL 0
	#endif
#endif

#if _DEBUG
	#define DEBUG_FUNCTIONS 1
#else
	#define DEBUG_FUNCTIONS 0
#endif

#ifndef HAS_SOCKETS
#define HAS_SOCKETS	1
#endif

#ifndef HAS_VALUETIPS
#define HAS_VALUETIPS	1
#endif

#ifndef HAS_MM_GUI
#define HAS_MM_GUI	1
#endif

#ifndef HAS_ASM
#define HAS_ASM	1
#endif

#ifndef HAS_WEAK_REFERENCES
#define HAS_WEAK_REFERENCES	1
#endif

#ifndef HAS_ASM_ADDSUB
#define HAS_ASM_ADDSUB HAS_ASM
#endif

#ifndef HAS_ASM_TIM
#define HAS_ASM_TIM HAS_ASM
#endif

#ifdef	SYSTEM_V
#define	BELL	1
#endif	// SYSTEM_V 

#ifdef	XAPL
#define	GUI 1
#endif	// XAPL 

#ifdef	WINDOWS3
#define	WINDOWS 1
#else
#define	WINDOWS 0
#endif	// WINDOWS3 

#ifndef	GUI
#define	GUI 0
#endif	// GUI 

#if HAS_UNICODE && GUI
#define HAS_COLORED_URLS	1
#endif

#if !defined(UNSIGNED_CHAR_TYPE)
#define UNSIGNED_CHAR_TYPE unsigned char
#endif

#ifndef HAS_APLMON
#define HAS_APLMON 1
#endif//HAS_APLMON
#ifndef HAS_APLMON2
#define HAS_APLMON2 0
#endif//HAS_APLMON2

#ifndef HAS_OUTLINING
#define HAS_OUTLINING 1
#endif//HAS_OUTLINING

#ifndef HAS_DOUBLEBUF
#define HAS_DOUBLEBUF 0
#endif//HAS_DOUBLEBUF

#ifndef HAS_JADE
#define HAS_JADE 1
#endif//HAS_JADE

#ifndef HAS_SYNCOL
#define HAS_SYNCOL 1
#endif//HAS_SYNCOL

#ifndef HAS_RIDE
#define HAS_RIDE 1
#endif//HAS_RIDE

#if	AP_BUILD || APL_IME
	#undef	HAS_RIDE
	#define	HAS_RIDE	0
#endif//AP_BUILD || APL_IME

#ifndef HAS_PARALLEL
#define HAS_PARALLEL 1
#endif//HAS_PARALLEL

#ifndef WANT_PCRE
#define WANT_PCRE 1
#endif // ndef WANT_PCRE

#if HAS_SYNCOL && !HAS_OUTLINING
#error HAS_SYNCOL requires HAS_OUTLINING (to do the syntax analysis)
#endif

#if HAS_UNICODE

#define  UNICODE 	// for Windows
#define _UNICODE	// for Windows

#define GSTR(name) 	L##name

#else

#define GSTR(name) 	name

#endif

#ifndef GPA	
#define GPA(a)	(a)		// even UNICODE desktop windows has narrow GetProcAddress
#endif

#ifndef xxBIT
#define xxBIT		32	// if not set, 32 bit build
#endif

#ifndef	SIGHANDSHAKE
#define SIGHANDSHAKE	SIGTERM
#endif

#ifndef IALIGN
#define IALIGN		0
#endif

#ifdef	DALIGN			// If the machine requires double 	
#define	WSDALIGN	1	// alignment, define WorkSpace		
#endif//DALIGN			// alignment also.

#ifndef P4ALIGN			// No Pentium 4 alignment needed
#define P4ALIGN
#endif //P4ALIGN

#ifndef MAXUNSIGNED		// some machines don't support unsigned	
#define MAXUNSIGNED	~0	// well - if so set to the maximum	
#endif	/* MAXUNSIGNED		 *//* possible default (signed) integer	*/

#ifndef MAXFCHARS		// max characters in full path name	
#define MAXFCHARS	FILENAME_MAX	// get from stdio.h
#endif	// MAXFCHARS 

#ifndef	SMLOGSIZ		// characters in session manager's log	
#define	SMLOGSIZ	81920	// 10240 until 1994-07-08
				// 40960 until 1998-09-15
#endif	// SMLOGSIZ 

#ifndef	PFKBUFSIZ		// characters in pfkey buffer		
#if HAS_UNICODE
#define	PFKBUFSIZ	10240
#else
#define	PFKBUFSIZ	2048
#endif
#endif	// PFKBUFSIZ 

#ifndef	GREENBUFSIZ		// characters in input line buffer	
#if HAS_UNICODE
#define	GREENBUFSIZ	10240
#else
#define	GREENBUFSIZ	4096
#endif
#endif	// GREENBUFSIZ 

#ifndef	HISTBUFSIZ		// characters in input history buffer	
#if HAS_UNICODE
#define	HISTBUFSIZ	10240
#else
#define	HISTBUFSIZ	2048
#endif
#endif	// HISTBUFSIZ 

#ifndef	CUTBUFSIZ		// characters in cut / paste buffer	
#define	CUTBUFSIZ	256
#endif	// CUTBUFSIZ 

#if 	!defined(CALLED_BY_DWA_DLL)
#define CALLED_BY_DWA_DLL
#endif

#if 	!defined(LIBCALL)
#define LIBCALL
#endif

#if 	!defined(STACKCALL)
#define STACKCALL
#endif

#if 	!defined(CRTIMP)
#define CRTIMP
#endif

#if 	!defined(FORCEINLINE)
# if defined(__GNUC__)
#  define FORCEINLINE __attribute__((always_inline)) inline
# else
#  define FORCEINLINE INLINE
# endif
#endif

#if	defined(_APL_IME) && !AP_BUILD

#define ggetenv	_wgetenv

#else

#define ggetenv	w_shad_getenv

#if !defined(RC_INVOKED)
#	ifndef	getenv	// enable environment defaults and overrides 
#		include <stdlib.h>	// get real definition before ...
#		define getenv	shad_getenv
		char * LIBCALL shad_getenv(const char *);
#	endif //getenv 
#endif

#endif//defined(APL_IME)

#define APLDINT		APLLONG	// 4 byte default integers		
#define dint		l	// access via hold.l			
#define dintp		lp	// ptr via ptr.lp			
#define dint_t		aplint32// use long type

#if	BYTE3210		// little-endian (ARM, Intel etc)
#elif	BYTE0123		// big-endian (SPARC, POWER etc)
#else
#	error
#endif

#if defined (BYTE3210)
#define IS_BIG_ENDIAN 0
#define IS_LITTLE_ENDIAN 1
#else
#define IS_BIG_ENDIAN    1
#define IS_LITTLE_ENDIAN 0
#endif


// derive standard value for OLD_DEPRECATED_MACHINETYPE 
// 	// cvc 20-02-2002, i'm going to hijack machinetype e for the WIN CE
// 		so that we can load CE workspaces in apl/w 9
//

#ifdef	BYTE3210
#	if defined(UNSINT)
#		ifdef	DALIGN
#		define	OLD_DEPRECATED_MACHINETYPE	0xf	// non known 
#		else//  
#		define	OLD_DEPRECATED_MACHINETYPE	0xe	// non known 
#		endif	// DALIGN 
#	else//  
#		ifdef	DALIGN
#		define	OLD_DEPRECATED_MACHINETYPE	0xd	// non known 
#		else//  
#		define	OLD_DEPRECATED_MACHINETYPE	0xc	// Nat Semi, Intel 
#		endif	// DALIGN 
#	endif	// UNSINT 
#else//  
#	ifdef	UNSINT
#		ifdef	DALIGN
#		define	OLD_DEPRECATED_MACHINETYPE	0xb	// sps9 (Ridge) 
#		else//  
#		define	OLD_DEPRECATED_MACHINETYPE	0xa	// 6150		
#		endif	// DALIGN 
#	else//  
#		ifdef	DALIGN
#		define	OLD_DEPRECATED_MACHINETYPE	0x9	// Sun4, HP800	
#		else//  
#		define	OLD_DEPRECATED_MACHINETYPE	0x8	// most common	
#		endif	// DALIGN 
#	endif	// UNSINT 
#endif	// BYTE3210 

// note that all real (still extant) machines have the Ieee bit set
// so I can use it to distinguish ARCH from OLD_DEPRECATED_MACHINETYPE.
// I need to do this because early 64 bit component headers mistakenly
// used OLD_DEPRECATED_MACHINETYPE.

// Also, whilst I think it horrible to do it, using a bit to indicate that
// integer alignment is required means that all architectures will have at least
// one bit set. This is because the only machines that don't have this as a
// requirement are Intel/AMD and they are little endian.

// The thing that requires the distinction is that the GUI structures built into
// pockets have a different alignment on Intel than anything else. The Linux
// port uses the edges of the gcc compiler to support the same alignment as
// Microsoft generates. The ARM port cannot because the hardware itself can't
// access the data on those boundaries.

// derive new architecture type for WS_VER 7 onwards 

#define	ARCH_LITTLE		0x80
#define	ARCH_DEXTEND		0x40	// unused
#define	ARCH_64BIT		0x20
#define	ARCH_DALIGN		0x10
#define	ARCH_MACH		0x08	// not an ARCH this one is MACHINETYPE
#define	ARCH_UNICODE		0x04
#define	ARCH_IALIGN		0x02	//need this to distinguish ARM from INTEL
#define ARCH_SPARE		0x01

#if	defined(BYTE3210)
#	define	ARCH_BIT_7	ARCH_LITTLE
#else
#	define	ARCH_BIT_7	0
#endif

#define		ARCH_BIT_6	0

#if	64 == xxBIT
#	define	ARCH_BIT_5	ARCH_64BIT
#else
#	define	ARCH_BIT_5	0
#endif

#if	defined(DALIGN)
#	define	ARCH_BIT_4	ARCH_DALIGN
#else
#	define	ARCH_BIT_4	0
#endif

#define		ARCH_BIT_3	0

#if	defined(UNICODE)
#	define	ARCH_BIT_2	ARCH_UNICODE
#else
#	define	ARCH_BIT_2	0
#endif

#if	IALIGN
#	define	ARCH_BIT_1	ARCH_IALIGN
#else
#	define	ARCH_BIT_1	0
#endif

#if	0
#	define	ARCH_BIT_0	ARCH_SPARE
#else
#	define	ARCH_BIT_0	0
#endif

#if	defined(PACKED_STRUCT)
#else //defined(PACKED_STRUCT)
#	define PACKED_STRUCT
#endif//defined(PACKED_STRUCT)

#if	defined(PACKED)
#else //defined(PACKED)
#	define PACKED(something) something
#endif//defined(PACKED)

#define ACCRUED_EXCEPTION 1
#define DECF_ACCRUED_EXCEPTION 1


#if defined(PARSE_JMP_BUF_SIZE)
typedef void *parse_jmp_buf[PARSE_JMP_BUF_SIZE];
#else
#define parse_jmp_buf jmp_buf
#endif


#if defined(DOSWIN)
#	define	DIRDELIM	'\\'
#	define	PATHDELIM	';'
#	define 	PATHDELIMSTR 	GSTR(";")
#else
#	define	DIRDELIM	'/'
#	define	PATHDELIM	':'
#	define 	PATHDELIMSTR 	GSTR(":")
#endif


// automatic help file creation

#define	ARCH		\
	(		\
	ARCH_BIT_7	\
|	ARCH_BIT_6	\
|	ARCH_BIT_5	\
|	ARCH_BIT_4	\
|	ARCH_BIT_3	\
|	ARCH_BIT_2	\
|	ARCH_BIT_1	\
|	ARCH_BIT_0	\
	)

#if !defined(WARN_W64)
#define WARN_W64
#endif // WARN_W64


#include <apltypes.h>
#include <apllimits.h>

//extern gstr wstrdup(const gchar *s1);		// "
extern void ODS(const gchar *fmt,...);		// for debugger output

extern gchar * LIBCALL w_shad_getenv(const gchar *envname);

extern unsigned in_syserror;

#if UNIX && HAS_UNICODE

// wide character externals that we will have to implement on most
// unix systems

// functions implemented in gfns.c :
extern int _waccess(const gchar *filename, int mode);
extern gchar *_wgetenv(const gchar *envname);
extern gchar *_wmktemp(gchar *templatename);
extern int _wrename(const gchar *oldname, const gchar *newname);
extern int _wunlink(const gchar *filename);
extern int _wrmdir(const gchar *filename);
extern int _wmkdir(const gchar *filename);

// functions not yet implemented :
extern CRTIMP int LIBCALL _wopen(const wchar_t *filename,int flag, ...);
extern CRTIMP int LIBCALL _wsopen(const wchar_t *filename,int flag,int, ...);
extern CRTIMP int LIBCALL wcsicmp(const wchar_t *s1,const wchar_t *s2);
extern CRTIMP int LIBCALL wcsicmp(const wchar_t *s1,const wchar_t *s2);
extern CRTIMP wchar_t * LIBCALL _wgetcwd(wchar_t *buf,int len);
extern void *_wopendir(wchar_t *);
extern wchar_t * _wcserror(unsigned short err);
extern wchar_t *_wctime(int *mtime);

#endif

#if UNIX

extern int _mkdir(const char *filename);

#endif // UNIX



#define NATIVE(a) (char *)(a)

#if HAS_ONLINEHELP
#define HID(str)	GSTR(str)
#else
#define HID(str)	(0)
#endif //HAS_ONLINEHELP

#define WIDEN(x) GSTR(x)
#define __WDATE__ WIDEN(__DATE__)

#define STRINGIFY(a)	#a
#define TOSTRING(a)	STRINGIFY(a)

#if !defined(GSTAT)
#define GSTAT struct stat
#endif

#ifndef NORETURN
# if defined(__GNUC__) || defined(__xlC__)
#  define NORETURN __attribute__((noreturn))
# elif defined(MSC)
#  define NORETURN __declspec(noreturn)
# else
#  define NORETURN
# endif
#endif // NORETURN

#ifndef NORETURN_PTR
# if defined(__GNUC__) || defined(__xlC__)
#  define NORETURN_PTR __attribute__((noreturn))
# elif defined(MSC) && defined(__clang__)
#  define NORETURN_PTR __declspec(noreturn)
# else
#  define NORETURN_PTR
# endif
#endif // NORETURN_PTR

#ifndef NOINLINE
# if defined(__GNUC__) || defined(__xlC__)
#  define NOINLINE __attribute__((noinline))
# elif defined(MSC)
#  define NOINLINE __declspec(noinline)
# else
#  define NOINLINE
# endif
#endif // NOINLINE

// LIKELY(x) evaluates to x, but hints to the compiler that its value is
// likely to be non-zero. UNLIKELY hints that the value is likely to be zero.
// X should be an integer-valued expression.
#ifndef LIKELY
# if defined(__GNUC__) || defined(__xlC__)
#  define LIKELY(x) __builtin_expect(x, 1)
#  define UNLIKELY(x) __builtin_expect(x, 0)
# else
#  define LIKELY(x) (x)
#  define UNLIKELY(x) (x)
# endif
#endif // LIKELY
#define ifl(x) if (LIKELY (x))
#define ifu(x) if (UNLIKELY (x))

#define	FALSE	0
#define	TRUE	1

#ifndef DLLEXPORT
#	define DLLEXPORT
#endif

#if defined(WINDOWS3) && !_MANAGED && !AP_BUILD

extern void __declspec(noreturn) LIBCALL cover_exit(int);
#define exit	cover_exit	// use our exit function (will call library exit)

#endif

#if MAC && !AP_BUILD

#include <mac_exit.h>
#define exit	mac_exit
#define atexit	mac_atexit

#endif

#ifdef MSC
# define INLINE __inline
# define RESTRICT __restrict
#else
# define INLINE inline
# define RESTRICT restrict
#endif

#if _MSC_VER && !__clang__
// Visual Studio doesn't have C _Static_assert,
// but does allow C++ static_assert in C mode,
// but clang-cl doesn't follow suit (see discussion at
// http://lists.llvm.org/pipermail/cfe-commits/Week-of-Mon-20160215/150681.html).
# define STATIC_ASSERT_MSG(x,s) static_assert(x, s )
#elif __cplusplus
# if __cplusplus >= 201103L // C++11
#  define STATIC_ASSERT_MSG(x,s) static_assert(x, s)
# endif
#else
# if __STDC_VERSION__ >= 201112L || __GNUC__ // C11
#  define STATIC_ASSERT_MSG(x,s) _Static_assert(x, s)
# endif
#endif
#ifndef STATIC_ASSERT_MSG
# define STATIC_ASSERT_MSG(x,s)
#endif

#define STATIC_ASSERT(x) STATIC_ASSERT_MSG(x, "")

//	Copyright (c) 1982 Dyadic Systems Limited	

#endif // _D_MACHIN_H
