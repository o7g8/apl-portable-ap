#ifndef _PORT_H
#define _PORT_H


//	Copyright (c) 1982 Dyadic Systems Limited	

#define	MSC	1		// using MSC
#define	FEXTEND	1		// append file extensions
#define	WINDOWS	1		// WINDOWS
#define	WINDOWS3	1	// specific WINDOWS system
#define	DOSWIN	1		// on dos platforms
#define	GUI	1		// generic enviroment

#define WINVER          0x600
#define _WIN32_WINNT    0x600
#define _WIN32_IE 	0x700



#if _MSC_VER < 1900
#define snprintf	_snprintf	// VS <2015 not ISO compliant
#endif

#define _CRT_SECURE_NO_DEPRECATE 
#define _CRT_NONSTDC_DEPRECATE(_NewName)
#define _CRT_NON_CONFORMING_SWPRINTFS

#define DLLEXPORT __declspec( dllexport )

#if xxBIT==32

	#if !defined(_MANAGED)

	extern int __fastcall parse_setjmp(void *);
	extern void __fastcall parse_longjmp(void *, int);

	#define PARSE_JMP_BUF_SIZE 7


	#endif // _MANAGED

						// calling syntax must be matched by PP/DLL build
	#define LIBCALL __cdecl
	#define STACKCALL __cdecl

	#define strtoimax	strtol	// not provided by lib. In contravention of C 99
	#define strtoumax	strtoul	// ditto

	#define wcstoumax	wcstoul
	#define wcstoimax	wcstol

	#define NEEDS_FPU_RESET 1	// Turn on protection against FPU corruption by rogue DLLs

#else

	extern int parse_setjmp(void *);
	extern __declspec(noreturn) void parse_longjmp(void *, int);

	#define PARSE_JMP_BUF_SIZE 11

	#define strtoimax	_strtoi64	// not supplied by compiler. ISO C 99
	#define strtoumax	_strtoui64	// ditto

	#define LIBCALL
	#define STACKCALL

#endif // xxBIT

#if !defined(CALLED_BY_DWA_DLL)

#define CALLED_BY_DWA_DLL

#if !defined(BRIDGE_BUILD)
	#if xxBIT==32
		#undef  CALLED_BY_DWA_DLL
		#define CALLED_BY_DWA_DLL __fastcall	// function is called by "Direct Workspace Access" PP/DLL 
	#endif
#endif	
#endif	


#define strncasecmp	strnicmp
#define strcasecmp	stricmp

#define GSTAT struct _stat64 


#define HAS_TRANS		1	// override ENGINE definition
#define HAS_QUADSM		1	// override ENGINE definition

#define HAS_DDE			1
#define HAS_DOTNET		1
#define HAS_DOUBLEBUF		1
#define HAS_SEMAPHORES		1
#define HAS_OLE			1
#define HAS_SPY			1
#define HAS_VALUETIPS		1
#define HAS_POPUPHELP		1
#define HAS_HOTKEYS		1
#define HAS_THEMES		1
#define HAS_BOUNDEXE		1
#define HAS_XPLOOK		1
#define HAS_SETPROPCOVERS	1
#define HAS_ONLINEHELP		1
#define HAS_LANGUAGEBAR		1
#define HAS_SKEYS		1
#define HAS_DIRLIST     	1
#define HAS_EMAIL		1
#define HAS_RIDE		1
#define HAS_SV			1
#define HAS_DATABINDING		1

#if HAS_UNICODE

	#if !APL_IME
		#define USES_IME	1
	#endif
#endif

#define	BANNER1	""

#if xxBIT==32

#define DALIGN	1

#define WARN_W64 __w64

// P4ALIGN:
// From: Intel Pentium 4 and Intel Xeon Processor Optimization
//     : Chapter 2
//  "A 64-byte or greater data structure or array should be aligned so that its base address is
//   a multiple of 64."
// Reckon the MS compiler should do this for us (we should have the "optimize for P4" option
// enabled) but it seems not to.

#define P4ALIGN __declspec(align(64))

#endif // xxBIT==32

#define BYTE3210	1

#define	LOOP_UNROLL	1
#define HAS_FASTASM	1

typedef unsigned short ushort;	// not supplied in types.h

#define	SIGQUIT	SIGBREAK	// this is what we want
#define	SIGHUP	SIGILL		// MSC doesn't define SIGHUP or honour it if we
				// define it as 1. Pick one of theirs that we
				// don't use. Critical for clean exit of APs.
#define	SIGWINCH SIGILL		// ditto, this used to resize tty window
#define	SIGALRM	SIGTERM		// this is not a good map

#define PATH_MAX	259	// MSC don't define this (except for POSIX)

#define	ftruncate(h,s) chsize_64(h,s)

#define	STRUCT_MAX_SIZE	(-4+64*1024)

#define FORCEINLINE __forceinline

#define	sigsetjmp(buf, save)	setjmp(buf)
#define	siglongjmp(buf, ret)	longjmp(buf, ret)
#define sigjmp_buf		jmp_buf

typedef unsigned char UCHAR;
#define APLWCHAR APLWCHAR16	// eltype for native wchar_t


#if !APL_IME && !defined(_MANAGED)

#define read 	win_read
#define write 	win_write
#define close 	win_close
#define lseek 	win_lseek

extern int LIBCALL win_read(int,void *,unsigned);
extern int LIBCALL win_write(int,const void *,unsigned);
extern int LIBCALL win_close(int);
extern long LIBCALL win_lseek(int,long,int);

#endif

#define MSC_BODGE	1

#if defined(_MANAGED)
typedef struct _TREEITEM  { int dummy; } _TREEITEM;
typedef struct _IMAGELIST { int dummy; } _IMAGELIST;
typedef struct _PSP 	{ int dummy; } _PSP;
#endif //defined(_MANAGED)

#define WM_MOUSEWHEEL                   0x020A
#define WHEEL_DELTA                     120

#if !RC_INVOKED
#if _MSC_VER<=1700 	// VS 2012 or lower
#define va_copy(a, b) do (a) = (b); while (0)
#endif
#endif

#define __thread __declspec(thread)
/*
 * IS_DRIVE_LETTER is used to check for drive specifiers in non-Unix filenames
 * i.e. A-Z or a-z. Note: *not* (islower(c) || isupper(c)) because some locales
 * have other characters in them.
 */
#define IS_DRIVE_LETTER(c)						      \
	((((c) >= 'A') && ((c) <= 'Z')) || (((c) >= 'a') && ((c) <= 'z')))

//	Copyright (c) 1982 Dyadic Systems Limited

#endif // _PORT_H
