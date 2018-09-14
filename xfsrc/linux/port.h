#ifndef _PORT_H
#define _PORT_H

//	Copyright (c) 1982 Dyadic Systems Limited	

#define	__stdcall
#define	__cdecl		
#define DLLEXPORT  __attribute__ ((visibility("default")))

#define HAS_TRANS		1	// override ENGINE definition
#define HAS_QUADSM		1	// override ENGINE definition

#define HAS_DOTNET		1
#define HAS_RIDE		1
#define HAS_DATABINDING		0
#define HAS_WORDEXPAND		0

#define	LINUX			1
#define	HAS_SEMAPHORES		1
#define	SYSTEM_V		1
#define	SIGHANDSHAKE		SIGUSR2	// SIGUSR1 used by wine
#define BYTE3210		1

#define	_ISOC99_SOURCE		1
#define	_POSIX_C_SOURCE		199506L
#define	_XOPEN_SOURCE		1
#define	_XOPEN_SOURCE_EXTENDED	1
#define _GNU_SOURCE		1
#define	_LARGEFILE64_SOURCE	1
#define	_FILE_OFFSET_BITS	64

#if 64 == xxBIT
#	define	BANNER1	"i86_64"
#elif 32 == xxBIT
#	define	BANNER1	"i86"
#	define	DALIGN		1
#endif // xxBIT

#define HAS_FASTASM		1

#define	O_BINARY		0	// not supplied

#define	PACKED(something)	something __attribute__ ((__packed__))
#define	PACKED_STRUCT		__attribute__ ((__aligned__ (1), __packed__))

#define APLWCHAR		APLWCHAR32	// eltype for native wchar_t

#if 64 == xxBIT
#       define PARSE_JMP_BUF_SIZE       8
#elif   32 == xxBIT
#       define PARSE_JMP_BUF_SIZE       6
#else
#       error compiler has not passed a value for xxBIT
#endif//xxBIT

extern int parse_setjmp(void*) __attribute__((returns_twice));
extern void parse_longjmp(void*, int) __attribute__((noreturn));

//	Copyright (c) 1982 Dyadic Systems Limited	

#endif // _PORT_H
